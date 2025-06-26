#ifndef ENVELOPELIBRARYWINDOW_H
#define ENVELOPELIBRARYWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QUndoStack>

class EnvLibDrawingArea;
class EnvelopeLibraryEntry;

namespace Ui {
class EnvelopeLibraryWindow;
}

class EnvelopeLibraryWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EnvelopeLibraryWindow(QWidget *parent = nullptr);
    ~EnvelopeLibraryWindow();

    void showEnvelopeLibrary();
    EnvelopeLibraryEntry* getActiveEnvelope();
    QString folderSelected();
    QUndoStack* getUndoStack() { return undoStack; }

public slots:
    void setEntries(const QString& x, const QString& y);

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
    void onUndoTriggered();
    void onRedoTriggered();

private:
    void setupUi();
    void createActions();
    void createMenus();
    void createNewEnvelope();
    void duplicateEnvelope();
    void refreshEnvelopeList();

    Ui::EnvelopeLibraryWindow *ui;
    QStandardItemModel *treeModel;
    EnvLibDrawingArea *drawingArea;
    EnvelopeLibraryEntry *activeEnvelope;
    QUndoStack *undoStack;
    QMenu *contextMenu;

    // Actions
    QAction *newEnvelopeAction;
    QAction *duplicateEnvelopeAction;
    QAction *saveAction;
    QAction *undoAction;
    QAction *redoAction;
};

#endif // ENVELOPELIBRARYWINDOW_H 