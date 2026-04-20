#include "MarkovModelLibraryWindow.hpp"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QCloseEvent>
#include <QDoubleValidator>
#include <QItemSelectionModel>
#include <QKeySequence>
#include <QModelIndexList>
#include <QShortcut>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QHideEvent>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QMenu>
#include <QPushButton>
#include <QRegularExpression>
#include <QSplitter>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStringList>
#include <QStyledItemDelegate>
#include <QTableView>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidget>

#include <limits>

#include <algorithm>

#include "Markov.h"
#include "../utilities.hpp"
#include "../widgets/ProjectViewController.hpp"

namespace {

// Item delegate restricting input to nonnegative real numbers (ints/doubles).
class NonNegativeRealDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    QWidget* createEditor(QWidget* parent,
                          const QStyleOptionViewItem& /*option*/,
                          const QModelIndex& /*index*/) const override {
        auto* editor = new QLineEdit(parent);
        auto* validator = new QDoubleValidator(
            0.0, std::numeric_limits<double>::max(), 12, editor);
        validator->setNotation(QDoubleValidator::StandardNotation);
        // Use C locale so '.' is always the decimal separator.
        QLocale c(QLocale::C);
        c.setNumberOptions(QLocale::RejectGroupSeparator);
        validator->setLocale(c);
        editor->setValidator(validator);

        // Commit on every keystroke (not just focus-out / Enter) so that a
        // partially-typed value like ".1" is pushed into the model — and
        // therefore into the underlying MarkovModel — even if the user
        // triggers a project save without leaving the cell.
        connect(editor, &QLineEdit::textEdited, this,
                [this, editor](const QString&) {
                    const_cast<NonNegativeRealDelegate*>(this)
                        ->emitCommitData(editor);
                });
        return editor;
    }

    void emitCommitData(QWidget* editor) { emit commitData(editor); }

    void setModelData(QWidget* editor,
                      QAbstractItemModel* model,
                      const QModelIndex& index) const override {
        auto* le = qobject_cast<QLineEdit*>(editor);
        if (!le) return;
        const QString text = le->text().trimmed();
        if (text.isEmpty()) {
            model->setData(index, QStringLiteral("0"), Qt::EditRole);
            return;
        }
        bool ok = false;
        const double v = QLocale::c().toDouble(text, &ok);
        if (!ok || v < 0.0) {
            // Reject: leave existing model value unchanged.
            return;
        }
        model->setData(index, text, Qt::EditRole);
    }
};

QStringList numberedHeaders(int n) {
    QStringList out;
    out.reserve(n);
    for (int i = 0; i < n; ++i) out << QString::number(i + 1);
    return out;
}

int rowHeightFor(const QTableView* v) {
    return v->verticalHeader()->defaultSectionSize()
         + v->horizontalHeader()->height()
         + 2 * v->frameWidth();
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

    auto* delegate = new NonNegativeRealDelegate(this);

    // Initial distribution
    m_distModel = new QStandardItemModel(this);
    m_distView = new QTableView;
    m_distView->setModel(m_distModel);
    m_distView->setItemDelegate(delegate);
    m_distView->verticalHeader()->setVisible(false);
    m_distView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    m_distView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_distView->setFixedHeight(rowHeightFor(m_distView) + m_distView->verticalHeader()->defaultSectionSize());

    auto* distGroup = new QGroupBox(tr("Initial Distribution"));
    auto* distGroupLayout = new QVBoxLayout(distGroup);
    distGroupLayout->setContentsMargins(6, 6, 6, 6);
    distGroupLayout->addWidget(m_distView);
    rightLayout->addWidget(distGroup);

    // Values for states
    m_valueModel = new QStandardItemModel(this);
    m_valueView = new QTableView;
    m_valueView->setModel(m_valueModel);
    m_valueView->setItemDelegate(delegate);
    m_valueView->verticalHeader()->setVisible(false);
    m_valueView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    m_valueView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_valueView->setFixedHeight(rowHeightFor(m_valueView) + m_valueView->verticalHeader()->defaultSectionSize());

    auto* valueGroup = new QGroupBox(tr("Values for states"));
    auto* valueGroupLayout = new QVBoxLayout(valueGroup);
    valueGroupLayout->setContentsMargins(6, 6, 6, 6);
    valueGroupLayout->addWidget(m_valueView);
    rightLayout->addWidget(valueGroup);

    // Transition matrix
    m_matrixModel = new QStandardItemModel(this);
    m_matrixView = new QTableView;
    m_matrixView->setModel(m_matrixModel);
    m_matrixView->setItemDelegate(delegate);
    m_matrixView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    m_matrixView->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    m_matrixView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    auto* matrixGroup = new QGroupBox(tr("Transition Matrix"));
    auto* matrixGroupLayout = new QVBoxLayout(matrixGroup);
    matrixGroupLayout->setContentsMargins(6, 6, 6, 6);
    matrixGroupLayout->addWidget(m_matrixView);
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
    connect(m_distModel, &QStandardItemModel::itemChanged,
            this, &MarkovModelLibraryWindow::onItemChanged);
    connect(m_valueModel, &QStandardItemModel::itemChanged,
            this, &MarkovModelLibraryWindow::onItemChanged);
    connect(m_matrixModel, &QStandardItemModel::itemChanged,
            this, &MarkovModelLibraryWindow::onItemChanged);

    installCopyPasteShortcuts(m_distView);
    installCopyPasteShortcuts(m_valueView);
    installCopyPasteShortcuts(m_matrixView);
}

MarkovModelLibraryWindow::~MarkovModelLibraryWindow() = default;

void MarkovModelLibraryWindow::setActiveProject(ProjectView* project) {
    activeProject = project;
    currentSelection = -1;
    currentSize = 0;
    m_sizeEntry->clear();
    resizeTables(0);
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

void MarkovModelLibraryWindow::resizeTables(int size) {
    suppressItemChanged = true;

    const QStringList headers = numberedHeaders(size);

    m_distModel->clear();
    m_distModel->setRowCount(size > 0 ? 1 : 0);
    m_distModel->setColumnCount(size);
    m_distModel->setHorizontalHeaderLabels(headers);

    m_valueModel->clear();
    m_valueModel->setRowCount(size > 0 ? 1 : 0);
    m_valueModel->setColumnCount(size);
    m_valueModel->setHorizontalHeaderLabels(headers);

    m_matrixModel->clear();
    m_matrixModel->setRowCount(size);
    m_matrixModel->setColumnCount(size);
    m_matrixModel->setHorizontalHeaderLabels(headers);
    m_matrixModel->setVerticalHeaderLabels(headers);

    currentSize = size;
    suppressItemChanged = false;
}

void MarkovModelLibraryWindow::loadModelIntoEditor(int modelIdx) {
    if (!activeProject) return;
    ProjectManager* pm = Inst::get_project_manager();
    if (!pm || !pm->get_curr_project()) return;
    auto& models = pm->markovmodels();
    if (modelIdx < 0 || modelIdx >= models.size()) return;

    auto& model = models[modelIdx];
    const int size = model.getStateSize();

    resizeTables(size);
    m_sizeEntry->setText(QString::number(size));

    suppressItemChanged = true;
    for (int j = 0; j < size; ++j) {
        m_distModel->setItem(0, j,
            new QStandardItem(QString::number(model.getInitialProbability(j))));
        m_valueModel->setItem(0, j,
            new QStandardItem(QString::number(model.getStateValue(j))));
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            m_matrixModel->setItem(i, j,
                new QStandardItem(QString::number(model.getTransitionProbability(i, j))));
        }
    }
    suppressItemChanged = false;
}

QString MarkovModelLibraryWindow::serializeEditor() const {

    auto cell = [](QStandardItem* it) -> QString {
        if (!it || it->text().isEmpty()) return QStringLiteral("0");
        return it->text();
    };

    QString s = QString::number(currentSize) + "\n";
    for (int j = 0; j < currentSize; ++j) {
        s += cell(m_valueModel->item(0, j)) + " ";
    }
    s += "\n";
    for (int j = 0; j < currentSize; ++j) {
        s += cell(m_distModel->item(0, j)) + " ";
    }
    s += "\n";
    for (int i = 0; i < currentSize; ++i) {
        for (int j = 0; j < currentSize; ++j) {
            s += cell(m_matrixModel->item(i, j)) + " ";
        }
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
        resizeTables(0);
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

    const int oldSize = currentSize;
    QVector<QString> oldVals(oldSize);
    QVector<QString> oldDists(oldSize);
    QVector<QVector<QString>> oldMat(oldSize, QVector<QString>(oldSize));
    for (int j = 0; j < oldSize; ++j) {
        if (auto* it = m_valueModel->item(0, j)) oldVals[j] = it->text();
        if (auto* it = m_distModel->item(0, j)) oldDists[j] = it->text();
    }
    for (int i = 0; i < oldSize; ++i) {
        for (int j = 0; j < oldSize; ++j) {
            if (auto* it = m_matrixModel->item(i, j)) oldMat[i][j] = it->text();
        }
    }

    resizeTables(newSize);

    suppressItemChanged = true;
    const int overlap = std::min(oldSize, newSize);
    for (int j = 0; j < overlap; ++j) {
        m_valueModel->setItem(0, j, new QStandardItem(oldVals[j]));
        m_distModel->setItem(0, j, new QStandardItem(oldDists[j]));
    }
    for (int i = 0; i < overlap; ++i) {
        for (int j = 0; j < overlap; ++j) {
            m_matrixModel->setItem(i, j, new QStandardItem(oldMat[i][j]));
        }
    }
    suppressItemChanged = false;

    if (currentSelection >= 0) {
        saveEditorIntoModel(currentSelection);
    }
}

void MarkovModelLibraryWindow::onItemChanged(QStandardItem* /*item*/) {
    if (suppressItemChanged) return;
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
        resizeTables(0);
        m_sizeEntry->clear();
        updateContextMenuEnablement();
    }
}

void MarkovModelLibraryWindow::updateContextMenuEnablement() {
    const bool hasSelection = currentSelection >= 0;
    if (m_duplicateAction) m_duplicateAction->setEnabled(hasSelection);
    if (m_deleteAction)    m_deleteAction->setEnabled(hasSelection);
}

// ---------------------------------------------------------------------------
// Copy / paste
// ---------------------------------------------------------------------------

void MarkovModelLibraryWindow::installCopyPasteShortcuts(QTableView* view) {
    // Scoped to the view so that typing Ctrl+C/V inside an open cell editor
    // goes to the QLineEdit's normal clipboard handling, not here.
    auto* copy = new QShortcut(QKeySequence::Copy, view);
    copy->setContext(Qt::WidgetShortcut);
    connect(copy, &QShortcut::activated, this,
            [this, view]{ copySelection(view); });

    auto* paste = new QShortcut(QKeySequence::Paste, view);
    paste->setContext(Qt::WidgetShortcut);
    connect(paste, &QShortcut::activated, this,
            [this, view]{ pasteSelection(view); });
}

void MarkovModelLibraryWindow::copySelection(QTableView* view) const {
    auto* model = qobject_cast<QStandardItemModel*>(view->model());
    if (!model) return;
    const QModelIndexList sel = view->selectionModel()->selectedIndexes();
    if (sel.isEmpty()) return;

    // Bounding rect of the selection.
    int minRow = sel.first().row(), maxRow = minRow;
    int minCol = sel.first().column(), maxCol = minCol;
    for (const QModelIndex& idx : sel) {
        minRow = std::min(minRow, idx.row());
        maxRow = std::max(maxRow, idx.row());
        minCol = std::min(minCol, idx.column());
        maxCol = std::max(maxCol, idx.column());
    }

    // Fill a dense grid; unselected cells in the bounding rect become empty
    // so downstream paste sees a regular shape.
    const int rows = maxRow - minRow + 1;
    const int cols = maxCol - minCol + 1;
    QVector<QVector<QString>> grid(rows, QVector<QString>(cols));
    for (const QModelIndex& idx : sel) {
        if (auto* it = model->item(idx.row(), idx.column())) {
            grid[idx.row() - minRow][idx.column() - minCol] = it->text();
        }
    }

    QString out;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (j > 0) out += QLatin1Char('\t');
            out += grid[i][j];
        }
        if (i + 1 < rows) out += QLatin1Char('\n');
    }
    QApplication::clipboard()->setText(out);
}

void MarkovModelLibraryWindow::pasteSelection(QTableView* view) {
    auto* model = qobject_cast<QStandardItemModel*>(view->model());
    if (!model) return;
    const QModelIndex anchor = view->selectionModel()->currentIndex();
    if (!anchor.isValid()) return;

    const QString text = QApplication::clipboard()->text();
    if (text.isEmpty()) return;

    // Strip a single trailing newline (common from Excel / our own copy), but
    // leave internal blank rows alone.
    QString body = text;
    if (body.endsWith(QLatin1Char('\n'))) body.chop(1);
    if (body.endsWith(QLatin1Char('\r'))) body.chop(1);

    // Row split accepts \r\n, \n, \r (tolerant of platform differences).
    const QStringList rows = body.split(QRegularExpression(QStringLiteral("\r\n|\n|\r")));

    const int r0 = anchor.row();
    const int c0 = anchor.column();
    const int rowCount = model->rowCount();
    const int colCount = model->columnCount();

    suppressItemChanged = true;
    bool anyChange = false;
    for (int i = 0; i < rows.size(); ++i) {
        const int targetRow = r0 + i;
        if (targetRow >= rowCount) break;  // overflow: drop remaining rows
        const QStringList cells = rows[i].split(QLatin1Char('\t'));
        for (int j = 0; j < cells.size(); ++j) {
            const int targetCol = c0 + j;
            if (targetCol >= colCount) break;  // overflow: drop remaining cols
            const QString raw = cells[j].trimmed();
            if (raw.isEmpty()) continue;  // leave empty cells alone
            bool ok = false;
            const double v = QLocale::c().toDouble(raw, &ok);
            if (!ok || v < 0.0) continue;  // same validation as the delegate
            auto* it = model->item(targetRow, targetCol);
            if (!it) {
                it = new QStandardItem(raw);
                model->setItem(targetRow, targetCol, it);
            } else {
                it->setText(raw);
            }
            anyChange = true;
        }
    }
    suppressItemChanged = false;

    // Single save for the whole paste, not one per cell.
    if (anyChange && currentSelection >= 0) {
        saveEditorIntoModel(currentSelection);
    }
}
