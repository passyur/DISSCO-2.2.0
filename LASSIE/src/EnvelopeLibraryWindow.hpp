#ifndef ENVELOPE_LIBRARY_WINDOW_H
#define ENVELOPE_LIBRARY_WINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStandardItemModel>
#include "EnvelopeLibraryEntry.hpp"

namespace Ui {
class EnvelopeLibraryWindow;
}

class EnvLibDrawingArea;

class EnvelopeLibraryWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit EnvelopeLibraryWindow(QWidget *parent = nullptr);
    ~EnvelopeLibraryWindow();

    EnvelopeLibraryEntry* getActiveEnvelope() const;
    void setEntries(const QString& x, const QString& y);
    QString folderSelected();

private slots:
    void onTreeItemActivated(const QModelIndex &index);
    void onTreeSelectionChanged();
    void onValueEntriesChanged();
    void onNewEnvelopeTriggered();
    void onDuplicateEnvelopeTriggered();
    void onSaveTriggered();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    void setupUi();
    void createActions();
    void createMenus();
    void createNewEnvelope();
    void duplicateEnvelope();
    void refreshEnvelopeList();
    void showEnvelopeLibrary();
    void updateEnvelopeView(EnvelopeLibraryEntry* envelope);
    void clearEnvelopeView();
    void addEnvelopeToTree(EnvelopeLibraryEntry* envelope);
    void removeEnvelopeFromTree(const QString& name);
    void filterEnvelopes(const QString& searchText);

    Ui::EnvelopeLibraryWindow *ui;
    QStandardItemModel *treeModel;
    EnvLibDrawingArea *drawingArea;
    EnvelopeLibraryEntry *activeEnvelope;
    QMenu *contextMenu;
    QAction *newEnvelopeAction;
    QAction *duplicateEnvelopeAction;
    QAction *saveAction;
};

#endif // ENVELOPE_LIBRARY_WINDOW_H 