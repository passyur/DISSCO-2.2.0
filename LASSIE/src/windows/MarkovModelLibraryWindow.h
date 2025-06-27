#ifndef MARKOVMODELLIBRARYWINDOW_H
#define MARKOVMODELLIBRARYWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QTreeView>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMenu>
#include <QVector>
#include <QString>
#include "ProjectViewController.h"
#include "../../CMOD/src/Markov.h"

// Define missing constant for minimal build
#define ENTRY_HEIGHT 25

class MarkovModelLibraryWindow : public QMainWindow {
    Q_OBJECT

public:
    // Constructor: set up UI
    explicit MarkovModelLibraryWindow(QWidget* parent = nullptr);
    // Destructor
    ~MarkovModelLibraryWindow() override;

    // Set which project this window is editing
    void setActiveProject(ProjectView* project);
    // Create a brand-new Markov model
    void createNewModel();
    // Duplicate the currently selected model
    void duplicateModel();
    // Remove the currently selected model
    void removeModel();
    // Refresh the view for a given model index
    void update(int selection);

protected:
    // Save state back to model when hidden
    void hideEvent(QHideEvent* event) override;

private slots:
    // Called when the tree selection changes
    void onSelectionChanged(const QModelIndex& index);
    // (Re)build all the entry widgets based on current size
    void buildTable();
    // Handle editing of any entry cell
    void onEntryChange();
    // Show context menu on right-click
    void onRightClick(const QPoint& pos);
    // Apply new chain size from sizeEntry
    void onSetSize();

private:
    // Serialize current entries to a string
    QString toString() const;

    ProjectView* activeProject = nullptr;

    QSplitter* m_paned;
    QWidget* m_leftWidget;
    QWidget* m_rightWidget;

    // Left pane: list of models
    QVBoxLayout* m_listLayout;
    QScrollArea* m_listScroll;
    QTreeView* m_treeView;
    QStandardItemModel* m_listModel;

    // Right pane: details
    QVBoxLayout* m_detailLayout;
    QWidget* m_detailBox1;
    QVBoxLayout* m_detailBox1Layout;
    QLineEdit* m_sizeEntry;
    QPushButton* m_sizeButton;
    QScrollArea* m_distScroll;
    QScrollArea* m_valueScroll;
    QScrollArea* m_matrixScroll;

    // Context menu
    QMenu* m_contextMenu;

    // Dynamic entry widgets
    QVector<QLineEdit*> m_distEntries;
    QVector<QLineEdit*> m_valueEntries;
    QVector<QLineEdit*> m_matrixEntries;

    int size = 0;
    int currentSelection = -1;
};

#endif // MARKOVMODELLIBRARYWINDOW_H
