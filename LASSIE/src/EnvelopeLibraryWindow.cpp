#include "EnvelopeLibraryWindow.h"
#include "ui_EnvelopeLibraryWindow.h"
#include "EnvLibDrawingArea.h"
#include "EnvelopeLibraryEntry.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QDomDocument>
#include <QDomElement>
#include <QTextStream>

EnvelopeLibraryWindow::EnvelopeLibraryWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EnvelopeLibraryWindow)
    , treeModel(new QStandardItemModel(this))
    , drawingArea(nullptr)
    , activeEnvelope(nullptr)
{
    ui->setupUi(this);
    setupUi();
    createActions();
    createMenus();
    refreshEnvelopeList();
}

EnvelopeLibraryWindow::~EnvelopeLibraryWindow()
{
    delete ui;
}

void EnvelopeLibraryWindow::setupUi()
{
    // Set up tree view
    ui->envelopeTreeView->setModel(treeModel);
    ui->envelopeTreeView->setHeaderHidden(true);
    ui->envelopeTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->envelopeTreeView->setDragEnabled(true);
    ui->envelopeTreeView->setAcceptDrops(true);
    ui->envelopeTreeView->setDropIndicatorShown(true);

    // Create drawing area
    if (!drawingArea) {
        drawingArea = new EnvLibDrawingArea(this);
        QVBoxLayout* layout = new QVBoxLayout(ui->drawingAreaWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(drawingArea);
    }

    // Connect signals
    connect(ui->envelopeTreeView, &QTreeView::activated,
            this, &EnvelopeLibraryWindow::onTreeItemActivated);
    connect(ui->envelopeTreeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &EnvelopeLibraryWindow::onTreeSelectionChanged);
    connect(ui->xValueEdit, &QLineEdit::textChanged,
            this, &EnvelopeLibraryWindow::onValueEntriesChanged);
    connect(ui->yValueEdit, &QLineEdit::textChanged,
            this, &EnvelopeLibraryWindow::onValueEntriesChanged);
    connect(ui->newButton, &QPushButton::clicked,
            this, &EnvelopeLibraryWindow::onNewEnvelopeTriggered);
    connect(ui->duplicateButton, &QPushButton::clicked,
            this, &EnvelopeLibraryWindow::onDuplicateEnvelopeTriggered);
    connect(ui->actionSave, &QAction::triggered,
            this, &EnvelopeLibraryWindow::onSaveTriggered);
}

void EnvelopeLibraryWindow::createActions()
{
    newEnvelopeAction = new QAction(tr("Create &New Envelope"), this);
    connect(newEnvelopeAction, &QAction::triggered,
            this, &EnvelopeLibraryWindow::onNewEnvelopeTriggered);

    duplicateEnvelopeAction = new QAction(tr("&Duplicate Envelope"), this);
    connect(duplicateEnvelopeAction, &QAction::triggered,
            this, &EnvelopeLibraryWindow::onDuplicateEnvelopeTriggered);

    saveAction = ui->actionSave;
    connect(saveAction, &QAction::triggered,
            this, &EnvelopeLibraryWindow::onSaveTriggered);
}

void EnvelopeLibraryWindow::createMenus()
{
    contextMenu = new QMenu(this);
    contextMenu->addAction(newEnvelopeAction);
    contextMenu->addAction(duplicateEnvelopeAction);
}

void EnvelopeLibraryWindow::contextMenuEvent(QContextMenuEvent *event)
{
    contextMenu->exec(event->globalPos());
}

void EnvelopeLibraryWindow::closeEvent(QCloseEvent *event)
{
    // TODO: Check for unsaved changes
    event->accept();
}

void EnvelopeLibraryWindow::createNewEnvelope()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("New Envelope"),
                                       tr("Enter envelope name:"), QLineEdit::Normal,
                                       QString(), &ok);
    if (ok && !name.isEmpty()) {
        // Create new envelope
        auto envelope = new EnvelopeLibraryEntry(name);
        
        // Add to tree model
        QStandardItem *item = new QStandardItem(name);
        item->setData(QVariant::fromValue(envelope), Qt::UserRole);
        treeModel->appendRow(item);
        
        // Set as active envelope
        activeEnvelope = envelope;
        ui->envelopeTreeView->setCurrentIndex(item->index());
    }
}

void EnvelopeLibraryWindow::duplicateEnvelope()
{
    if (!activeEnvelope) {
        QMessageBox::warning(this, tr("Duplicate Envelope"),
                           tr("Please select an envelope to duplicate."));
        return;
    }

    bool ok;
    QString name = QInputDialog::getText(this, tr("Duplicate Envelope"),
                                       tr("Enter new envelope name:"), QLineEdit::Normal,
                                       activeEnvelope->getName() + tr("_copy"), &ok);
    if (ok && !name.isEmpty()) {
        // Create new envelope as a copy
        auto envelope = new EnvelopeLibraryEntry(*activeEnvelope);
        envelope->setName(name);
        
        // Add to tree model
        QStandardItem *item = new QStandardItem(name);
        item->setData(QVariant::fromValue(envelope), Qt::UserRole);
        treeModel->appendRow(item);
        
        // Set as active envelope
        activeEnvelope = envelope;
        ui->envelopeTreeView->setCurrentIndex(item->index());
    }
}

void EnvelopeLibraryWindow::setEntries(const QString& x, const QString& y)
{
    ui->xValueEdit->setText(x);
    ui->yValueEdit->setText(y);
}

EnvelopeLibraryEntry* EnvelopeLibraryWindow::getActiveEnvelope()
{
    return activeEnvelope;
}

QString EnvelopeLibraryWindow::folderSelected()
{
    QModelIndex index = ui->envelopeTreeView->currentIndex();
    if (!index.isValid())
        return QString();
    return treeModel->data(index, Qt::DisplayRole).toString();
}

void EnvelopeLibraryWindow::onTreeItemActivated(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    QStandardItem *item = treeModel->itemFromIndex(index);
    if (!item)
        return;

    // Get envelope from item data
    activeEnvelope = item->data(Qt::UserRole).value<EnvelopeLibraryEntry*>();
    if (activeEnvelope) {
        drawingArea->setEnvelope(activeEnvelope);
    }
}

void EnvelopeLibraryWindow::onTreeSelectionChanged()
{
    QModelIndex index = ui->envelopeTreeView->currentIndex();
    if (!index.isValid()) {
        activeEnvelope = nullptr;
        drawingArea->setEnvelope(nullptr);
        return;
    }

    QStandardItem *item = treeModel->itemFromIndex(index);
    if (!item)
        return;

    // Get envelope from item data
    activeEnvelope = item->data(Qt::UserRole).value<EnvelopeLibraryEntry*>();
    if (activeEnvelope) {
        drawingArea->setEnvelope(activeEnvelope);
    }
}

void EnvelopeLibraryWindow::onValueEntriesChanged()
{
    if (!activeEnvelope)
        return;

    bool xOk, yOk;
    double x = ui->xValueEdit->text().toDouble(&xOk);
    double y = ui->yValueEdit->text().toDouble(&yOk);

    if (xOk && yOk) {
        // Update envelope values
        if (drawingArea) {
            drawingArea->updateSelectedNode(x, y);
        }
    }
}

void EnvelopeLibraryWindow::onNewEnvelopeTriggered()
{
    createNewEnvelope();
}

void EnvelopeLibraryWindow::onDuplicateEnvelopeTriggered()
{
    duplicateEnvelope();
}

void EnvelopeLibraryWindow::onSaveTriggered()
{
    if (!activeEnvelope) {
        QMessageBox::warning(this, tr("Save Envelope"),
                           tr("Please select an envelope to save."));
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Envelope"),
                                                  QString(),
                                                  tr("XML Files (*.xml);;All Files (*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("Save Envelope"),
                           tr("Cannot write file %1:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName),
                                file.errorString()));
        return;
    }

    QDomDocument doc("envelope");
    QDomElement root = doc.createElement("envelope");
    doc.appendChild(root);
    activeEnvelope->saveToXml(doc, root);

    QTextStream out(&file);
    doc.save(out, 4);
}

void EnvelopeLibraryWindow::refreshEnvelopeList()
{
    treeModel->clear();
    treeModel->setHorizontalHeaderLabels(QStringList() << tr("Envelopes"));
}

void EnvelopeLibraryWindow::showEnvelopeLibrary()
{
    show();
    raise();
    activateWindow();
} 