#ifndef ENVELOPELIBRARYWINDOW_H
#define ENVELOPELIBRARYWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QTreeWidget>
#include <QLabel>
#include <QLineEdit>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QStandardItemModel>
#include <QDomDocument>
#include <QDomElement>

class EnvelopeLibraryEntry;
class EnvLibDrawingArea;

namespace Ui {
class EnvelopeLibraryWindow;
}

class EnvelopeLibraryWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EnvelopeLibraryWindow(QWidget *parent = nullptr);
    ~EnvelopeLibraryWindow();

    void createNewEnvelope();
    void duplicateEnvelope();
    void setEntries(const QString& x, const QString& y);
    EnvelopeLibraryEntry* getActiveEnvelope();
    QString folderSelected(); // check if any row is selected

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onTreeItemActivated(const QModelIndex &index);
    void onTreeSelectionChanged();
    void onValueEntriesChanged();
    void onNewEnvelopeTriggered();
    void onDuplicateEnvelopeTriggered();
    void onSaveTriggered();

private:
    void setupUi();
    void createActions();
    void createMenus();
    void refreshEnvelopeList();
    void showEnvelopeLibrary();

    Ui::EnvelopeLibraryWindow *ui;
    QStandardItemModel *treeModel;
    EnvLibDrawingArea *drawingArea;
    EnvelopeLibraryEntry *activeEnvelope;
    
    // Actions
    QAction *newEnvelopeAction;
    QAction *duplicateEnvelopeAction;
    QAction *saveAction;
    
    // Context menu
    QMenu *contextMenu;
};

#endif // ENVELOPELIBRARYWINDOW_H 