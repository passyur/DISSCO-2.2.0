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

/**
 * @brief Main window for managing envelope library entries
 */
class EnvelopeLibraryWindow : public QMainWindow {
    Q_OBJECT

    friend class EnvLibDrawingArea; ///< Allow EnvLibDrawingArea to access private members

public:
    /**
     * @brief Constructor: builds the UI
     * @param parent  parent widget
     */
    EnvelopeLibraryWindow(QWidget* parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~EnvelopeLibraryWindow();

    /**
     * @brief Set the active project
     * @param project  project to set as active
     */
    void setActiveProject(ProjectView* project);
    
    /**
     * @brief Create a brand‐new envelope
     */
    void createNewEnvelope();
    
    /**
     * @brief Duplicate the selected envelope
     */
    void duplicateEnvelope();
    
    /**
     * @brief Delete the selected envelope
     */
    void deleteEnvelope();
    
    /**
     * @brief Programmatically update the X/Y entries
     * @param x  x-coordinate value to set
     * @param y  y-coordinate value to set
     */
    void setEntries(const QString& x, const QString& y);
    
    /**
     * @brief Return the currently active envelope
     * @return pointer to the currently active envelope
     */
    EnvelopeLibraryEntry* getActiveEnvelope() const;

    QLineEdit*              xEntry;           ///< X‐coordinate editor
    QLineEdit*              yEntry;           ///< Y‐coordinate editor

protected slots:
    /**
     * @brief Called when a tree item is activated (double‐click or Enter)
     * @param index  index of the activated item
     */
    void objectActivated(const QModelIndex& index);
    
    /**
     * @brief Called when the selection changes in the envelope list
     * @param current   current selection index
     * @param previous  previous selection index
     */
    void onCursorChanged(const QModelIndex& current, const QModelIndex& previous);
    
    /**
     * @brief Show context menu on right‐click
     * @param pos  position where right-click occurred
     */
    void onRightClick(const QPoint& pos);
    
    /**
     * @brief Called when either X or Y QLineEdit text changes
     */
    void valueEntriesChanged();

protected:
    /**
     * @brief Rebuild the entire envelope list from the project
     */
    void refreshEnvelopeList();
    
    /**
     * @brief Save via project controller (e.g. Ctrl+S)
     */
    void fileSave();
    
    /**
     * @brief Catch key presses for shortcuts
     * @param event  key event to handle
     */
    void keyPressEvent(QKeyEvent* event) override;

private:
    QTreeView*              envelopeLibrary;  ///< lists envelopes
    QStandardItemModel*     refModel;         ///< model backing the tree
    EnvLibDrawingArea*      drawingArea;      ///< custom canvas widget

    ProjectView*  activeProject{nullptr};     ///< currently active project
    EnvelopeLibraryEntry*   activeEnvelope{nullptr}; ///< currently active envelope

    QMenu*                  popupMenu;        ///< right‐click menu
    QAction*                actionAdd;        ///< "Create New Envelope"
    QAction*                actionDuplicate;  ///< "Duplicate Envelope"
    QAction*                actionSave;       ///< "Save" (Ctrl+S)
};

#endif // ENVELOPE_LIBRARY_WINDOW_HPP