#ifndef MARKOVMODELLIBRARYWINDOW_HPP
#define MARKOVMODELLIBRARYWINDOW_HPP

#include <QMainWindow>
#include <QString>
#include <QVector>

class QAction;
class QCloseEvent;
class QHideEvent;
class QLineEdit;
class QMenu;
class QPushButton;
class QStandardItem;
class QStandardItemModel;
class QTableView;
class QTreeView;
class QUndoStack;
class ProjectView;

class MarkovModelLibraryWindow : public QMainWindow {
    Q_OBJECT

public:
    // Full state of the three tables; used as the undo/redo unit.
    struct EditorSnapshot {
        int size = 0;
        QVector<QString> dist;    // length `size`
        QVector<QString> values;  // length `size`
        QVector<QString> matrix;  // row-major, length `size * size`
    };

    explicit MarkovModelLibraryWindow(QWidget* parent = nullptr);
    ~MarkovModelLibraryWindow() override;

    void setActiveProject(ProjectView* project);

    // Exposed so QUndoCommand subclasses can capture/restore editor state.
    EditorSnapshot snapshotEditor() const;
    void applySnapshot(const EditorSnapshot& s);

protected:
    void hideEvent(QHideEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onSelectionChanged(const QModelIndex& current, const QModelIndex& previous);
    void onRightClick(const QPoint& pos);
    void onSetSize();
    void onItemChanged(QStandardItem* item);
    void createNewModel();
    void duplicateModel();
    void removeModel();
    void onEditorClosed();

private:
    void rebuildModelList();
    void resizeTables(int size);
    void loadModelIntoEditor(int modelIdx);
    void saveEditorIntoModel(int modelIdx);
    void updateContextMenuEnablement();
    void installCopyPasteShortcuts(QTableView* view);
    void copySelection(QTableView* view) const;
    void pasteSelection(QTableView* view);
    void beginEditCapture();
    void pushSnapshotCommand(const EditorSnapshot& before,
                             const EditorSnapshot& after,
                             const QString& label);
    QString serializeEditor() const;

    ProjectView* activeProject = nullptr;
    int currentSelection = -1;
    int currentSize = 0;
    bool suppressItemChanged = false;
    bool applyingSnapshot = false;

    QTreeView* m_treeView = nullptr;
    QStandardItemModel* m_listModel = nullptr;

    QLineEdit* m_sizeEntry = nullptr;
    QPushButton* m_sizeButton = nullptr;

    QTableView* m_distView = nullptr;
    QTableView* m_valueView = nullptr;
    QTableView* m_matrixView = nullptr;

    QStandardItemModel* m_distModel = nullptr;
    QStandardItemModel* m_valueModel = nullptr;
    QStandardItemModel* m_matrixModel = nullptr;

    QMenu* m_contextMenu = nullptr;
    QAction* m_createAction = nullptr;
    QAction* m_duplicateAction = nullptr;
    QAction* m_deleteAction = nullptr;

    QUndoStack* m_undoStack = nullptr;
    EditorSnapshot m_editStartSnapshot;
    bool m_editInProgress = false;
};

#endif // MARKOVMODELLIBRARYWINDOW_HPP
