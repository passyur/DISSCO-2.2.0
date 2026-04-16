#include "MarkovModelLibraryWindow.hpp"

#include <QAction>
#include <QCloseEvent>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QHideEvent>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QScrollArea>
#include <QSplitter>
#include <QStandardItemModel>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidget>

#include <algorithm>

#include "Markov.h"
#include "../utilities.hpp"
#include "../widgets/ProjectViewController.hpp"

namespace {

QWidget* makeGridHost() {
    auto* host = new QWidget;
    auto* layout = new QGridLayout(host);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setHorizontalSpacing(6);
    layout->setVerticalSpacing(4);
    return host;
}

void clearGrid(QWidget* host) {
    if (auto* old = host->layout()) {
        QLayoutItem* item;
        while ((item = old->takeAt(0)) != nullptr) {
            if (QWidget* w = item->widget()) {
                w->deleteLater();
            }
            delete item;
        }
        delete old;
    }
    auto* layout = new QGridLayout(host);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setHorizontalSpacing(6);
    layout->setVerticalSpacing(4);
}

QLabel* headerLabel(int n) {
    auto* l = new QLabel(QString::number(n));
    l->setAlignment(Qt::AlignCenter);
    return l;
}

}  // namespace

MarkovModelLibraryWindow::MarkovModelLibraryWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Markov Model Library"));
    resize(900, 650);

    auto* splitter = new QSplitter(Qt::Horizontal, this);
    setCentralWidget(splitter);

    // --- Left panel: model list ---------------------------------------------
    m_listModel = new QStandardItemModel(this);
    m_listModel->setHorizontalHeaderLabels({tr("Id")});

    m_treeView = new QTreeView;
    m_treeView->setModel(m_listModel);
    m_treeView->setRootIsDecorated(false);
    m_treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_treeView->header()->setStretchLastSection(true);
    splitter->addWidget(m_treeView);

    // --- Right panel: editor ------------------------------------------------
    auto* rightPanel = new QWidget;
    auto* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(8, 8, 8, 8);
    rightLayout->setSpacing(8);

    // Size row
    auto* sizeRow = new QHBoxLayout;
    sizeRow->setSpacing(6);
    sizeRow->addWidget(new QLabel(tr("Markov chain size:")));
    m_sizeEntry = new QLineEdit;
    m_sizeEntry->setValidator(new QIntValidator(1, 10000, m_sizeEntry));
    m_sizeEntry->setMaximumWidth(80);
    sizeRow->addWidget(m_sizeEntry);
    m_sizeButton = new QPushButton(tr("Set"));
    sizeRow->addWidget(m_sizeButton);
    sizeRow->addStretch(1);
    rightLayout->addLayout(sizeRow);

    // Initial distribution group
    auto* distGroup = new QGroupBox(tr("Initial Distribution"));
    auto* distGroupLayout = new QVBoxLayout(distGroup);
    distGroupLayout->setContentsMargins(6, 6, 6, 6);
    auto* distScroll = new QScrollArea;
    distScroll->setWidgetResizable(true);
    distScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    distScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    distScroll->setFixedHeight(90);
    m_distGridHost = makeGridHost();
    distScroll->setWidget(m_distGridHost);
    distGroupLayout->addWidget(distScroll);
    rightLayout->addWidget(distGroup);

    // Values-for-states group
    auto* valueGroup = new QGroupBox(tr("Values for states"));
    auto* valueGroupLayout = new QVBoxLayout(valueGroup);
    valueGroupLayout->setContentsMargins(6, 6, 6, 6);
    auto* valueScroll = new QScrollArea;
    valueScroll->setWidgetResizable(true);
    valueScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    valueScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    valueScroll->setFixedHeight(90);
    m_valueGridHost = makeGridHost();
    valueScroll->setWidget(m_valueGridHost);
    valueGroupLayout->addWidget(valueScroll);
    rightLayout->addWidget(valueGroup);

    // Transition matrix group
    auto* matrixGroup = new QGroupBox(tr("Transition Matrix"));
    auto* matrixGroupLayout = new QVBoxLayout(matrixGroup);
    matrixGroupLayout->setContentsMargins(6, 6, 6, 6);
    auto* matrixScroll = new QScrollArea;
    matrixScroll->setWidgetResizable(true);
    matrixScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    matrixScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_matrixGridHost = makeGridHost();
    matrixScroll->setWidget(m_matrixGridHost);
    matrixGroupLayout->addWidget(matrixScroll);
    rightLayout->addWidget(matrixGroup, /*stretch=*/1);

    splitter->addWidget(rightPanel);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    splitter->setSizes({160, 740});

    // --- Context menu -------------------------------------------------------
    m_contextMenu = new QMenu(this);
    m_createAction    = m_contextMenu->addAction(tr("Create New Model"),
                                                 this, &MarkovModelLibraryWindow::createNewModel);
    m_duplicateAction = m_contextMenu->addAction(tr("Duplicate Model"),
                                                 this, &MarkovModelLibraryWindow::duplicateModel);
    m_deleteAction    = m_contextMenu->addAction(tr("Delete Model"),
                                                 this, &MarkovModelLibraryWindow::removeModel);
    updateContextMenuEnablement();

    // --- Signal wiring ------------------------------------------------------
    connect(m_treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MarkovModelLibraryWindow::onSelectionChanged);
    connect(m_treeView, &QTreeView::customContextMenuRequested,
            this, &MarkovModelLibraryWindow::onRightClick);
    connect(m_sizeButton, &QPushButton::clicked,
            this, &MarkovModelLibraryWindow::onSetSize);
    connect(m_sizeEntry, &QLineEdit::returnPressed,
            this, &MarkovModelLibraryWindow::onSetSize);
}

MarkovModelLibraryWindow::~MarkovModelLibraryWindow() = default;

void MarkovModelLibraryWindow::setActiveProject(ProjectView* project) {
    activeProject = project;
    currentSelection = -1;
    currentSize = 0;
    m_sizeEntry->clear();
    rebuildEditorGrid(0);
    rebuildModelList();
    updateContextMenuEnablement();
}

void MarkovModelLibraryWindow::hideEvent(QHideEvent* event) {
    if (currentSelection >= 0) {
        saveEditorIntoModel(currentSelection);
    }
    QMainWindow::hideEvent(event);
}

void MarkovModelLibraryWindow::closeEvent(QCloseEvent* event) {
    if (currentSelection >= 0) {
        saveEditorIntoModel(currentSelection);
    }
    QMainWindow::closeEvent(event);
}

void MarkovModelLibraryWindow::rebuildModelList() {
    m_listModel->removeRows(0, m_listModel->rowCount());
    if (!activeProject) return;
    ProjectManager* pm = Inst::get_project_manager();
    if (!pm || !pm->get_curr_project()) return;
    const auto& models = pm->markovmodels();
    for (int i = 0; i < models.size(); ++i) {
        auto* item = new QStandardItem(QString::number(i));
        item->setEditable(false);
        m_listModel->appendRow(item);
    }
}

void MarkovModelLibraryWindow::rebuildEditorGrid(int size) {
    m_distEntries.clear();
    m_valueEntries.clear();
    m_matrixEntries.clear();

    clearGrid(m_distGridHost);
    clearGrid(m_valueGridHost);
    clearGrid(m_matrixGridHost);

    currentSize = size;
    if (size <= 0) return;

    auto* distLayout = static_cast<QGridLayout*>(m_distGridHost->layout());
    for (int j = 0; j < size; ++j) {
        distLayout->addWidget(headerLabel(j + 1), 0, j);
    }
    for (int j = 0; j < size; ++j) {
        auto* le = new QLineEdit;
        le->setMinimumWidth(60);
        connect(le, &QLineEdit::editingFinished,
                this, &MarkovModelLibraryWindow::onEntryEdited);
        distLayout->addWidget(le, 1, j);
        m_distEntries.push_back(le);
    }

    auto* valueLayout = static_cast<QGridLayout*>(m_valueGridHost->layout());
    for (int j = 0; j < size; ++j) {
        valueLayout->addWidget(headerLabel(j + 1), 0, j);
    }
    for (int j = 0; j < size; ++j) {
        auto* le = new QLineEdit;
        le->setMinimumWidth(60);
        connect(le, &QLineEdit::editingFinished,
                this, &MarkovModelLibraryWindow::onEntryEdited);
        valueLayout->addWidget(le, 1, j);
        m_valueEntries.push_back(le);
    }

    auto* matrixLayout = static_cast<QGridLayout*>(m_matrixGridHost->layout());
    for (int j = 0; j < size; ++j) {
        matrixLayout->addWidget(headerLabel(j + 1), 0, j + 1);
    }
    for (int i = 0; i < size; ++i) {
        matrixLayout->addWidget(headerLabel(i + 1), i + 1, 0);
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            auto* le = new QLineEdit;
            le->setMinimumWidth(60);
            connect(le, &QLineEdit::editingFinished,
                    this, &MarkovModelLibraryWindow::onEntryEdited);
            matrixLayout->addWidget(le, i + 1, j + 1);
            m_matrixEntries.push_back(le);
        }
    }
}

void MarkovModelLibraryWindow::loadModelIntoEditor(int modelIdx) {
    if (!activeProject) return;
    ProjectManager* pm = Inst::get_project_manager();
    if (!pm || !pm->get_curr_project()) return;
    auto& models = pm->markovmodels();
    if (modelIdx < 0 || modelIdx >= models.size()) return;

    auto& model = models[modelIdx];
    const int size = model.getStateSize();

    if (size != currentSize) {
        rebuildEditorGrid(size);
    }
    m_sizeEntry->setText(QString::number(size));

    for (int i = 0; i < size; ++i) {
        m_distEntries[i]->setText(QString::number(model.getInitialProbability(i)));
        m_valueEntries[i]->setText(QString::number(model.getStateValue(i)));
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            m_matrixEntries[i * size + j]
                ->setText(QString::number(model.getTransitionProbability(i, j)));
        }
    }
}

QString MarkovModelLibraryWindow::serializeEditor() const {
    QString s = QString::number(currentSize) + "\n";
    for (auto* e : m_valueEntries) {
        s += (e->text().isEmpty() ? QStringLiteral("0") : e->text()) + " ";
    }
    s += "\n";
    for (auto* e : m_distEntries) {
        s += (e->text().isEmpty() ? QStringLiteral("0") : e->text()) + " ";
    }
    s += "\n";
    for (auto* e : m_matrixEntries) {
        s += (e->text().isEmpty() ? QStringLiteral("0") : e->text()) + " ";
    }
    return s;
}

void MarkovModelLibraryWindow::saveEditorIntoModel(int modelIdx) {
    if (!activeProject) return;
    ProjectManager* pm = Inst::get_project_manager();
    if (!pm || !pm->get_curr_project()) return;
    auto& models = pm->markovmodels();
    if (modelIdx < 0 || modelIdx >= models.size()) return;
    if (currentSize <= 0) return;

    models[modelIdx].from_str(serializeEditor().toStdString());
    MUtilities::modified();
}

void MarkovModelLibraryWindow::onSelectionChanged(const QModelIndex& current,
                                                  const QModelIndex& previous) {
    if (previous.isValid()) {
        saveEditorIntoModel(previous.row());
    }
    if (current.isValid()) {
        currentSelection = current.row();
        loadModelIntoEditor(currentSelection);
    } else {
        currentSelection = -1;
        rebuildEditorGrid(0);
        m_sizeEntry->clear();
    }
    updateContextMenuEnablement();
}

void MarkovModelLibraryWindow::onRightClick(const QPoint& pos) {
    updateContextMenuEnablement();
    m_contextMenu->exec(m_treeView->viewport()->mapToGlobal(pos));
}

void MarkovModelLibraryWindow::onSetSize() {
    bool ok = false;
    const int newSize = m_sizeEntry->text().toInt(&ok);
    if (!ok || newSize <= 0) return;
    if (newSize == currentSize) return;

    QVector<QString> oldVals = [this]{
        QVector<QString> v;
        for (auto* e : m_valueEntries) v.push_back(e->text());
        return v;
    }();
    QVector<QString> oldDists = [this]{
        QVector<QString> v;
        for (auto* e : m_distEntries) v.push_back(e->text());
        return v;
    }();
    const int oldSize = currentSize;
    QVector<QVector<QString>> oldMat(oldSize, QVector<QString>(oldSize));
    for (int i = 0; i < oldSize; ++i) {
        for (int j = 0; j < oldSize; ++j) {
            oldMat[i][j] = m_matrixEntries[i * oldSize + j]->text();
        }
    }

    rebuildEditorGrid(newSize);

    const int overlap = std::min(oldSize, newSize);
    for (int i = 0; i < overlap; ++i) {
        m_valueEntries[i]->setText(oldVals[i]);
        m_distEntries[i]->setText(oldDists[i]);
    }
    for (int i = 0; i < overlap; ++i) {
        for (int j = 0; j < overlap; ++j) {
            m_matrixEntries[i * newSize + j]->setText(oldMat[i][j]);
        }
    }

    if (currentSelection >= 0) {
        saveEditorIntoModel(currentSelection);
    }
}

void MarkovModelLibraryWindow::onEntryEdited() {
    if (currentSelection >= 0) {
        saveEditorIntoModel(currentSelection);
    }
}

void MarkovModelLibraryWindow::createNewModel() {
    if (!activeProject) return;
    ProjectManager* pm = Inst::get_project_manager();
    if (!pm || !pm->get_curr_project()) return;

    if (currentSelection >= 0) {
        saveEditorIntoModel(currentSelection);
    }

    pm->markovmodels().append(MarkovModel<float>()); 
    MUtilities::modified();

    rebuildModelList();
    const QModelIndex idx = m_listModel->index(pm->markovmodels().size() - 1, 0);
    m_treeView->setCurrentIndex(idx);
}

void MarkovModelLibraryWindow::duplicateModel() {
    if (!activeProject || currentSelection < 0) return;
    ProjectManager* pm = Inst::get_project_manager();
    if (!pm || !pm->get_curr_project()) return;

    saveEditorIntoModel(currentSelection);

    if (currentSelection < pm->markovmodels().size())
        pm->markovmodels().append(pm->markovmodels()[currentSelection]); 
    MUtilities::modified();

    rebuildModelList();
    const QModelIndex idx = m_listModel->index(pm->markovmodels().size() - 1, 0);
    m_treeView->setCurrentIndex(idx);
}

void MarkovModelLibraryWindow::removeModel() {
    if (!activeProject || currentSelection < 0) return;
    ProjectManager* pm = Inst::get_project_manager();
    if (!pm || !pm->get_curr_project()) return;

    if (currentSelection < pm->markovmodels().size())
        pm->markovmodels().erase(pm->markovmodels().begin() + currentSelection);
    MUtilities::modified();

    const int removed = currentSelection;
    currentSelection = -1;
    rebuildModelList();

    const int remaining = m_listModel->rowCount();
    if (remaining > 0) {
        const int nextRow = std::min(removed, remaining - 1);
        m_treeView->setCurrentIndex(m_listModel->index(nextRow, 0));
    } else {
        rebuildEditorGrid(0);
        m_sizeEntry->clear();
        updateContextMenuEnablement();
    }
}

void MarkovModelLibraryWindow::updateContextMenuEnablement() {
    const bool hasSelection = currentSelection >= 0;
    if (m_duplicateAction) m_duplicateAction->setEnabled(hasSelection);
    if (m_deleteAction)    m_deleteAction->setEnabled(hasSelection);
}
