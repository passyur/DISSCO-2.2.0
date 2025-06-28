// ObjectWindow.h
#ifndef OBJECTWINDOW_H
#define OBJECTWINDOW_H

#include "../../CMOD/src/Libraries.h"
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QGridLayout>
#include <QWidget>
#include <QString>

class MainWindow;
class ProjectView;
class IEvent;

// EventType is already defined in CMOD/src/Libraries.h

// -----------------------------------------------------------------------------
// A button representing one object in the list; wraps an IEvent.
// -----------------------------------------------------------------------------
class ObjectWindowObjectPackage : public QPushButton {
    Q_OBJECT
public:
    // Construct with a pointer to the project view controller
    explicit ObjectWindowObjectPackage(ProjectView* projectView);
    // Destructor
    ~ObjectWindowObjectPackage();

    // Count how many packages from here to end of the linked list
    int count();
    // Recursively delete this package and all that follow
    void clear();
    // Slot: show this package's content in the project view
    void showContent();

    ObjectWindowObjectPackage* prev;  ///< previous in list
    ObjectWindowObjectPackage* next;  ///< next in list
    IEvent* ievent;                   ///< associated event

private:
    ProjectView* projectView; ///< to call showAttributes(...)
};

// -----------------------------------------------------------------------------
// A window that displays a scrollable grid of ObjectWindowObjectPackage.
// -----------------------------------------------------------------------------
class ObjectWindow : public QMainWindow {
    Q_OBJECT
public:
    // Create for a given event type and controller
    ObjectWindow(EventType type, ProjectView* projectView);
    ~ObjectWindow();

    // Change which project we're looking at
    void setActiveProject(ProjectView* project);
    // Print event names to stdout for debugging
    void print();
    // Reload the list and rebuild the UI
    void refresh();
    // (Placeholder) return the currently selected folder name
    QString folderSelected();

protected:
    // Re‐layout on resize
    void resizeEvent(QResizeEvent* event) override;

private slots:
    // Called when number of columns should change
    void sizeChange(int newNumOfColumns);
    // (Re)populate the grid from head of linked list
    void numOfObjectsChange();

private:
    EventType type;                         ///< which list to show
    ProjectView* activeProject;   ///< application controller
    ObjectWindowObjectPackage* objects;     ///< head of the linked list
    int currentNumOfColumns;                ///< track layout

    QScrollArea* scrollArea;                ///< scroll container
    QWidget* containerWidget;               ///< holds gridLayout
    QGridLayout* gridLayout;                ///< arranges the buttons
};

#endif // OBJECTWINDOW_H
