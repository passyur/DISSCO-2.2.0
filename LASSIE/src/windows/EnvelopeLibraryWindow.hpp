#ifndef ENVELOPE_LIBRARY_WINDOW_HPP
#define ENVELOPE_LIBRARY_WINDOW_HPP

#include <QMainWindow>
#include "ProjectViewController.hpp"

class EnvelopeLibraryEntry;
class ProjectView;
class EnvLibDrawingArea;
class QTreeView;
class QStandardItemModel;
class QLineEdit;
class QMenu;
class QAction;

class EnvelopeLibraryWindow : public QMainWindow {
    Q_OBJECT

    friend class EnvLibDrawingArea; // Allow EnvLibDrawingArea to access private members

public:
    // Constructor: builds the UI
    EnvelopeLibraryWindow(QWidget* parent = nullptr);
    // Destructor
    ~EnvelopeLibraryWindow();

    // Set the active project
    void setActiveProject(ProjectView* project);
    // Create a brand‐new envelope
    void createNewEnvelope();
    // Duplicate the selected envelope
    void duplicateEnvelope();
    // Delete the selected envelope
    void deleteEnvelope();
    // Programmatically update the X/Y entries
    void setEntries(const QString& x, const QString& y);
    // Return the currently active envelope
    EnvelopeLibraryEntry* getActiveEnvelope() const;

    QLineEdit*              xEntry;           // X‐coordinate editor
    QLineEdit*              yEntry;           // Y‐coordinate editor

protected slots:
    // Called when a tree item is activated (double‐click or Enter)
    void objectActivated(const QModelIndex& index);
    // Called when the selection changes in the envelope list
    void onCursorChanged(const QModelIndex& current, const QModelIndex& previous);
    // Show context menu on right‐click
    void onRightClick(const QPoint& pos);
    // Called when either X or Y QLineEdit text changes
    void valueEntriesChanged();

protected:
    // Rebuild the entire envelope list from the project
    void refreshEnvelopeList();
    // Save via project controller (e.g. Ctrl+S)
    void fileSave();
    // Catch key presses for shortcuts
    void keyPressEvent(QKeyEvent* event) override;

private:
    QTreeView*              envelopeLibrary;  // lists envelopes
    QStandardItemModel*     refModel;         // model backing the tree
    EnvLibDrawingArea*      drawingArea;      // custom canvas widget

    ProjectView*  activeProject{nullptr};
    EnvelopeLibraryEntry*   activeEnvelope{nullptr};

    QMenu*                  popupMenu;        // right‐click menu
    QAction*                actionAdd;        // "Create New Envelope"
    QAction*                actionDuplicate;  // "Duplicate Envelope"
    QAction*                actionSave;       // "Save" (Ctrl+S)
};

#endif // ENVELOPE_LIBRARY_WINDOW_HPP