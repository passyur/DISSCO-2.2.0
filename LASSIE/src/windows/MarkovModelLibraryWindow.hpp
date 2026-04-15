#ifndef MARKOVMODELLIBRARYWINDOW_HPP
#define MARKOVMODELLIBRARYWINDOW_HPP

#include <QMainWindow>
#include <QVector>
#include <QString>

class QAction;
class QCloseEvent;
class QHideEvent;
class QLineEdit;
class QMenu;
class QPushButton;
class QStandardItemModel;
class QTreeView;
class QWidget;
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
    void onEntryEdited();
    void createNewModel();
    void duplicateModel();
    void removeModel();

private:
    void rebuildModelList();
    void rebuildEditorGrid(int size);
    void loadModelIntoEditor(int modelIdx);
    void saveEditorIntoModel(int modelIdx);
    void updateContextMenuEnablement();
    QString serializeEditor() const;

    ProjectView* activeProject = nullptr;
    int currentSelection = -1;
    int currentSize = 0;

    QTreeView* m_treeView = nullptr;
    QStandardItemModel* m_listModel = nullptr;

    QLineEdit* m_sizeEntry = nullptr;
    QPushButton* m_sizeButton = nullptr;

    QWidget* m_distGridHost = nullptr;
    QWidget* m_valueGridHost = nullptr;
    QWidget* m_matrixGridHost = nullptr;

    QVector<QLineEdit*> m_distEntries;
    QVector<QLineEdit*> m_valueEntries;
    QVector<QLineEdit*> m_matrixEntries;

    QMenu* m_contextMenu = nullptr;
    QAction* m_createAction = nullptr;
    QAction* m_duplicateAction = nullptr;
    QAction* m_deleteAction = nullptr;
};

#endif // MARKOVMODELLIBRARYWINDOW_HPP
