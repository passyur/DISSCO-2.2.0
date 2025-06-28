#include "EnvelopeLibraryWindow.hpp"
#include "EnvLibDrawingArea.hpp"
#include "ProjectViewController.hpp"
#include "EnvelopeLibraryEntry.hpp"

#include <QTreeView>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QAction>
#include <QKeyEvent>
#include <QHeaderView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSizePolicy>

EnvelopeLibraryWindow::EnvelopeLibraryWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // Window setup
    setWindowTitle("Envelope Library");
    resize(600, 220); // Compact initial size

    // Central widget and layout
    QWidget* central = new QWidget(this);
    auto* layout = new QVBoxLayout(central);

    // Add button row for create/duplicate/delete
    auto* buttonRow = new QWidget(this);
    auto* buttonLayout = new QHBoxLayout(buttonRow);
    QPushButton* btnCreate = new QPushButton("Create New Envelope", this);
    QPushButton* btnDuplicate = new QPushButton("Duplicate Envelope", this);
    QPushButton* btnDelete = new QPushButton("Delete Envelope", this);
    buttonLayout->addWidget(btnCreate);
    buttonLayout->addWidget(btnDuplicate);
    buttonLayout->addWidget(btnDelete);
    buttonRow->setLayout(buttonLayout);
    layout->addWidget(buttonRow);
    connect(btnCreate, &QPushButton::clicked, this, &EnvelopeLibraryWindow::createNewEnvelope);
    connect(btnDuplicate, &QPushButton::clicked, this, &EnvelopeLibraryWindow::duplicateEnvelope);
    connect(btnDelete, &QPushButton::clicked, this, &EnvelopeLibraryWindow::deleteEnvelope);

    // Envelope list view (moved above drawing area)
    envelopeLibrary = new QTreeView(this);
    refModel = new QStandardItemModel(this);
    refModel->setHorizontalHeaderLabels({ "Envelope Library Number" });
    envelopeLibrary->setModel(refModel);
    envelopeLibrary->header()->setStretchLastSection(true);
    envelopeLibrary->setMinimumWidth(120); // Make the envelope list narrower
    layout->addWidget(envelopeLibrary);

    // Drawing area (moved below envelope list)
    drawingArea = new EnvLibDrawingArea(this);
    layout->addWidget(drawingArea);

    // X/Y entry fields
    xEntry = new QLineEdit(this);
    yEntry = new QLineEdit(this);
    xEntry->setPlaceholderText("X value");
    yEntry->setPlaceholderText("Y value");

    // Instead, create a horizontal layout for X/Y fields and legend:
    QHBoxLayout* xyLegendLayout = new QHBoxLayout();
    QWidget* xyWidget = new QWidget(this);
    QFormLayout* xyForm = new QFormLayout();
    xyForm->addRow("X value:", xEntry);
    xyForm->addRow("Y value:", yEntry);
    xyWidget->setLayout(xyForm);
    xyLegendLayout->addWidget(xyWidget, 0, Qt::AlignLeft);
    QLabel* legend = new QLabel("Right click the graph to see available actions or click-and-drag a node to adjust the envelope.\nThick segment = Flexible; Thin segment = Fixed.\nBlue = Linear; Green = Spline; Red = Exponential.", this);
    legend->setWordWrap(true);
    legend->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    xyLegendLayout->addWidget(legend, 1);
    layout->addLayout(xyLegendLayout);

    // Actions for context menu & save
    actionAdd = new QAction("Create New Envelope", this);
    connect(actionAdd, &QAction::triggered,
            this, &EnvelopeLibraryWindow::createNewEnvelope);

    actionDuplicate = new QAction("Duplicate Envelope", this);
    connect(actionDuplicate, &QAction::triggered,
            this, &EnvelopeLibraryWindow::duplicateEnvelope);

    actionSave = new QAction("Save", this);
    actionSave->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    connect(actionSave, &QAction::triggered,
            this, &EnvelopeLibraryWindow::fileSave);
    addAction(actionSave);

    // Build popup menu
    popupMenu = new QMenu(this);
    popupMenu->addAction(actionAdd);
    popupMenu->addAction(actionDuplicate);

    // Connect tree signals
    connect(envelopeLibrary, &QTreeView::activated,
            this, &EnvelopeLibraryWindow::objectActivated);
    connect(envelopeLibrary->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            &EnvelopeLibraryWindow::onCursorChanged);

    envelopeLibrary->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(envelopeLibrary, &QWidget::customContextMenuRequested,
            this, &EnvelopeLibraryWindow::onRightClick);

    setCentralWidget(central);
    drawingArea->setMinimumSize(200, 200); // Ensure it has a visible size
    drawingArea->show(); // Explicitly show the widget
    drawingArea->raise(); // Bring it to the front
    drawingArea->repaint(); // Force repaint after window is shown
}

EnvelopeLibraryWindow::~EnvelopeLibraryWindow() = default;

// Set the active project and populate the list
void EnvelopeLibraryWindow::setActiveProject(ProjectView* project)
{
    drawingArea->clearGraph();
    activeProject = project;
    refreshEnvelopeList();

    EnvelopeLibraryEntry* entry = activeProject->getEnvelopeLibraryEntries();
    while (entry) {
        // append each entry to the tree
        QStandardItem* item = new QStandardItem(entry->getNumberString());
        // store the pointer in UserRole
        item->setData(QVariant::fromValue<void*>(entry), Qt::UserRole);
        refModel->appendRow(item);
        entry = entry->next;
    }
    activeEnvelope = nullptr;
}

// Create a new envelope via the project controller
void EnvelopeLibraryWindow::createNewEnvelope()
{
    if (!activeProject) return;
    EnvelopeLibraryEntry* newEnv = activeProject->createNewEnvelope();
    activeProject->modified();

    QStandardItem* item = new QStandardItem(newEnv->getNumberString());
    item->setData(QVariant::fromValue<void*>(newEnv), Qt::UserRole);
    refModel->appendRow(item);

    // Automatically select the new envelope
    QModelIndex newIndex = refModel->index(refModel->rowCount() - 1, 0);
    envelopeLibrary->setCurrentIndex(newIndex);
    // This will trigger onCursorChanged and update the graph
}

// Duplicate the selected envelope
void EnvelopeLibraryWindow::duplicateEnvelope()
{
    QModelIndex idx = envelopeLibrary->currentIndex();
    if (!idx.isValid()) return;

    QStandardItem* origItem = refModel->itemFromIndex(idx);
    auto ptr = static_cast<EnvelopeLibraryEntry*>(origItem->data(Qt::UserRole).value<void*>());
    if (!ptr || !activeProject) return;

    EnvelopeLibraryEntry* dupEnv = activeProject->duplicateEnvelope(ptr);
    activeProject->modified();

    QStandardItem* newItem = new QStandardItem(dupEnv->getNumberString());
    newItem->setData(QVariant::fromValue<void*>(dupEnv), Qt::UserRole);
    refModel->appendRow(newItem);
}

// Delete the selected envelope
void EnvelopeLibraryWindow::deleteEnvelope()
{
    QModelIndex idx = envelopeLibrary->currentIndex();
    if (!idx.isValid()) return;

    QStandardItem* item = refModel->itemFromIndex(idx);
    auto ptr = static_cast<EnvelopeLibraryEntry*>(item->data(Qt::UserRole).value<void*>());
    if (!ptr || !activeProject) return;

    activeProject->deleteEnvelope(ptr);
    activeProject->modified();

    refModel->removeRow(idx.row());
}

// Return the active envelope pointer
EnvelopeLibraryEntry* EnvelopeLibraryWindow::getActiveEnvelope() const
{
    return activeEnvelope;
}

// Handle double‐click/Enter on a tree item
void EnvelopeLibraryWindow::objectActivated(const QModelIndex& index)
{
    if (!index.isValid()) return;
    QStandardItem* item = refModel->itemFromIndex(index);
    activeEnvelope = static_cast<EnvelopeLibraryEntry*>(item->data(Qt::UserRole).value<void*>());

    drawingArea->resetFields();
    drawingArea->adjustBoundary(activeEnvelope);
    drawingArea->showGraph(activeEnvelope);
    drawingArea->update();
}

// Handle selection changes
void EnvelopeLibraryWindow::onCursorChanged(const QModelIndex& current,
                                            const QModelIndex&)
{
    if (!current.isValid()) return;
    QStandardItem* item = refModel->itemFromIndex(current);
    activeEnvelope = static_cast<EnvelopeLibraryEntry*>(item->data(Qt::UserRole).value<void*>());

    drawingArea->resetFields();
    drawingArea->adjustBoundary(activeEnvelope);
    drawingArea->showGraph(activeEnvelope);
    drawingArea->update();
}

// Show right‐click context menu
void EnvelopeLibraryWindow::onRightClick(const QPoint& pos)
{
    QModelIndex idx = envelopeLibrary->indexAt(pos);
    if (!idx.isValid()) return;
    popupMenu->exec(envelopeLibrary->viewport()->mapToGlobal(pos));
}

// Clear and rebuild the model
void EnvelopeLibraryWindow::refreshEnvelopeList()
{
    refModel->removeRows(0, refModel->rowCount());
}

// Save via project controller
void EnvelopeLibraryWindow::fileSave()
{
    if (activeProject) activeProject->save();
}

// Capture Ctrl+S
void EnvelopeLibraryWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_S) {
        fileSave();
        return;
    }
    QMainWindow::keyPressEvent(event);
}

// Called whenever X or Y entry changes
void EnvelopeLibraryWindow::valueEntriesChanged()
{
    if (!drawingArea) return;
    drawingArea->setActiveNodeCoordinate(
        xEntry->text(),
        yEntry->text());
}

// Update the line edits from code
void EnvelopeLibraryWindow::setEntries(const QString& x, const QString& y)
{
    xEntry->setText(x);
    yEntry->setText(y);
} 