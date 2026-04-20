#ifndef MARKOVMODELLIBRARYWINDOW_HPP
#define MARKOVMODELLIBRARYWINDOW_HPP

#include <QMainWindow>
#include <QString>

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
class ProjectView;

class MarkovModelLibraryWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MarkovModelLibraryWindow(QWidget* parent = nullptr);
    ~MarkovModelLibraryWindow() override;

    void setActiveProject(ProjectView* project);

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

private:
    void rebuildModelList();
    void resizeTables(int size);
    void loadModelIntoEditor(int modelIdx);
    void saveEditorIntoModel(int modelIdx);
    void updateContextMenuEnablement();
    void installCopyPasteShortcuts(QTableView* view);
    void copySelection(QTableView* view) const;
    void pasteSelection(QTableView* view);
    QString serializeEditor() const;

    ProjectView* activeProject = nullptr;
    int currentSelection = -1;
    int currentSize = 0;
    bool suppressItemChanged = false;

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
};

#endif // MARKOVMODELLIBRARYWINDOW_HPP
