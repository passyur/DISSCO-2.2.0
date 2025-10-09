#include "MarkovModelLibraryWindow.hpp"
#include "../widgets/ProjectViewController.hpp"
#include <QHBoxLayout>
#include <QHeaderView>

#include "../utilities.hpp"

MarkovModelLibraryWindow::MarkovModelLibraryWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Markov Model Library");
    resize(800, 600);

    // Splitter between list and detail panes
    m_paned = new QSplitter(this);
    setCentralWidget(m_paned);

    // Left side: list of models
    m_leftWidget = new QWidget;
    m_listLayout = new QVBoxLayout(m_leftWidget);
    m_treeView = new QTreeView;
    m_listModel = new QStandardItemModel;
    m_listModel->setHorizontalHeaderLabels({ "Id" });
    m_treeView->setModel(m_listModel);
    m_treeView->header()->setStretchLastSection(false);
    m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_listScroll = new QScrollArea;
    m_listScroll->setWidgetResizable(true);
    m_listScroll->setWidget(m_treeView);
    m_listLayout->addWidget(m_listScroll);
    m_paned->addWidget(m_leftWidget);

    // Right side: detail controls
    m_rightWidget = new QWidget;
    m_detailLayout = new QVBoxLayout(m_rightWidget);

    // Size control row
    m_detailBox1 = new QWidget;
    m_detailBox1Layout = new QVBoxLayout(m_detailBox1);
    m_sizeEntry = new QLineEdit;
    m_sizeButton = new QPushButton("Set Size");
    m_detailBox1Layout->addWidget(m_sizeEntry);
    m_detailBox1Layout->addWidget(m_sizeButton);

    // Initial distribution
    m_detailLayout->addWidget(new QLabel("Initial Distribution"));
    m_distScroll = new QScrollArea;
    m_distScroll->setFixedHeight(2 * ENTRY_HEIGHT);
    m_valueScroll = new QScrollArea;
    m_valueScroll->setFixedHeight(2 * ENTRY_HEIGHT);
    m_matrixScroll = new QScrollArea;

    m_detailLayout->addWidget(m_detailBox1);
    m_detailLayout->addWidget(m_distScroll);
    m_detailLayout->addWidget(m_valueScroll);
    m_detailLayout->addWidget(m_matrixScroll);

    m_paned->addWidget(m_rightWidget);

    // Context menu actions
    m_contextMenu = new QMenu(this);
    m_contextMenu->addAction("Create New Model", this, &MarkovModelLibraryWindow::createNewModel);
    m_contextMenu->addAction("Duplicate Model",   this, &MarkovModelLibraryWindow::duplicateModel);
    m_contextMenu->addAction("Delete Model",      this, &MarkovModelLibraryWindow::removeModel);

    // Signals & slots
    connect(m_treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MarkovModelLibraryWindow::onSelectionChanged);
    connect(m_treeView, &QTreeView::customContextMenuRequested,
            this, &MarkovModelLibraryWindow::onRightClick);
    connect(m_sizeButton, &QPushButton::clicked,
            this, &MarkovModelLibraryWindow::onSetSize);
}

MarkovModelLibraryWindow::~MarkovModelLibraryWindow() = default;

void MarkovModelLibraryWindow::setActiveProject(ProjectView* project) {
    activeProject = project;
    m_listModel->removeRows(0, m_listModel->rowCount());
    if (!activeProject) return;
    ProjectManager *pm = Inst::get_project_manager();
    auto& models = pm->markovmodels();
    for (int i = 0; i < models.size(); ++i) {
        auto item = new QStandardItem(QString::number(i));
        m_listModel->appendRow(item);
    }
}

void MarkovModelLibraryWindow::createNewModel() {
    if (!activeProject) return;
    int newIdx = MarkovUtilities::createNewMarkovModel();
    MUtilities::modified();
    auto item = new QStandardItem(QString::number(newIdx));
    m_listModel->appendRow(item);
}

void MarkovModelLibraryWindow::duplicateModel() {
    if (currentSelection < 0 || !activeProject) return;
    int newIdx = MarkovUtilities::duplicateMarkovModel(currentSelection);
    if (newIdx < 0) return;
    MUtilities::modified();
    auto item = new QStandardItem(QString::number(newIdx));
    m_listModel->appendRow(item);
}

void MarkovModelLibraryWindow::removeModel() {
    if (currentSelection < 0 || !activeProject) return;
    MarkovUtilities::removeMarkovModel(currentSelection);
    MUtilities::modified();
    m_listModel->removeRow(currentSelection);
}

void MarkovModelLibraryWindow::onSelectionChanged(const QModelIndex& index) {
    if (!index.isValid()) return;
    update(index.row());
}

void MarkovModelLibraryWindow::update(int selection) {
    if (!activeProject) return;
    ProjectManager *pm = Inst::get_project_manager();
    auto& models = pm->markovmodels();
    if (models.empty()) return;

    // Save previous
    if (currentSelection >= 0) {
        models[currentSelection].from_str(toString().toStdString());
        MUtilities::modified();
    }
    currentSelection = selection;
    auto& model = models[selection];

    // Rebuild table if size changed
    if (size != model.getStateSize()) {
        size = model.getStateSize();
        buildTable();
    }

    // Populate entries
    for (int i = 0; i < size; ++i)
        m_distEntries[i]->setText(QString::number(model.getInitialProbability(i)));
    for (int i = 0; i < size; ++i)
        m_valueEntries[i]->setText(QString::number(model.getStateValue(i)));

    int k = 0;
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j, ++k)
            m_matrixEntries[k]->setText(QString::number(model.getTransitionProbability(i, j)));
}

void MarkovModelLibraryWindow::buildTable() {
    // Clear old widgets
    auto clearArea = [&](QScrollArea* area, QVector<QLineEdit*>& vec){
        QWidget* w = new QWidget;
        area->setWidget(w);
        vec.clear();
    };
    clearArea(m_distScroll,  m_distEntries);
    clearArea(m_valueScroll, m_valueEntries);
    clearArea(m_matrixScroll,m_matrixEntries);

    // Dist & values share similar layout logic...
    auto populateLinear = [&](QScrollArea* area, QVector<QLineEdit*>& vec){
        QWidget* container = new QWidget;
        auto* layout = new QHBoxLayout(container);
        for (int i = 0; i < size; ++i) {
            auto* le = new QLineEdit;
            layout->addWidget(le);
            connect(le, &QLineEdit::editingFinished, this, &MarkovModelLibraryWindow::onEntryChange);
            vec.push_back(le);
        }
        area->setWidget(container);
    };

    populateLinear(m_distScroll,  m_distEntries);
    populateLinear(m_valueScroll, m_valueEntries);

    // Matrix as grid
    QWidget* matrixContainer = new QWidget;
    auto* grid = new QGridLayout(matrixContainer);
    for (int i = 0; i < size+1; ++i) {
        for (int j = 0; j < size+1; ++j) {
            if (i==0 && j>0)       grid->addWidget(new QLabel(QString::number(j)), i, j);
            else if (j==0 && i>0)  grid->addWidget(new QLabel(QString::number(i)), i, j);
            else if (i>0 && j>0) {
                auto* le = new QLineEdit;
                connect(le, &QLineEdit::editingFinished, this, &MarkovModelLibraryWindow::onEntryChange);
                grid->addWidget(le, i, j);
                m_matrixEntries.push_back(le);
            }
        }
    }
    m_matrixScroll->setWidget(matrixContainer);
}

void MarkovModelLibraryWindow::onEntryChange() {
    // placeholder: validation or flagging modification
}

QString MarkovModelLibraryWindow::toString() const {
    std::ostringstream ss;
    ss << size << "\n";
    for (auto* e : m_valueEntries) {
        ss << e->text().toFloat() << " ";
    }
    ss << "\n";
    for (auto* e : m_distEntries) {
        ss << e->text().toFloat() << " ";
    }
    ss << "\n";
    for (auto* e : m_matrixEntries) {
        ss << e->text().toFloat() << " ";
    }
    return QString::fromStdString(ss.str());
}

void MarkovModelLibraryWindow::onSetSize() {
    // Cache old
    QVector<QString> oldVals, oldDists; 
    QVector<QVector<QString>> oldMat(size, QVector<QString>(size));
    for (auto* e : m_valueEntries) oldVals.push_back(e->text());
    for (auto* e : m_distEntries)  oldDists.push_back(e->text());
    int k = 0;
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j, ++k)
            oldMat[i][j] = m_matrixEntries[k]->text();

    // New size
    bool ok;
    int newSize = m_sizeEntry->text().toInt(&ok);
    if (ok && newSize > 0) {
        size = newSize;
        buildTable();
        // Restore overlaps
        for (int i = 0; i < std::min(size, static_cast<int>(oldVals.size())); ++i)
            m_valueEntries[i]->setText(oldVals[i]);
        for (int i = 0; i < std::min(size, static_cast<int>(oldDists.size())); ++i)
            m_distEntries[i]->setText(oldDists[i]);
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                m_matrixEntries[i*size+j]->setText(oldMat[i][j]);
    }
}

void MarkovModelLibraryWindow::onRightClick(const QPoint& pos) {
    QPoint global = m_treeView->viewport()->mapToGlobal(pos);
    m_contextMenu->exec(global);
}

// void MarkovModelLibraryWindow::hideEvent(QHideEvent* event) {
//     QMainWindow::hideEvent(event);
//     if (currentSelection >= 0 && activeProject) {
//         auto& models = activeProject->getMarkovModels();
//         models[currentSelection].from_str(toString().toStdString());
//         activeProject->modified();
//     }
// }