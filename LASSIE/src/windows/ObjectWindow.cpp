// ObjectWindow.cpp
#include "ObjectWindow.hpp"
#include "PaletteViewController.hpp"
#include "../../CMOD/src/Libraries.h"
#include "ProjectViewController.hpp"
#include "MainWindow.hpp"
#include <QResizeEvent>
#include <QDebug>

#define COLUMN_WIDTH 200

//-----------------------------------------------------------------------------
// ObjectWindow Implementation
//-----------------------------------------------------------------------------

ObjectWindow::ObjectWindow(EventType _type, ProjectView* _projectView)
    : QMainWindow(nullptr),
      type(_type),
      activeProject(_projectView),
      objects(nullptr),
      currentNumOfColumns(1)
{
    // Initial window size
    resize(500, COLUMN_WIDTH);

    // Choose title & load linked list head based on type
    QString title;
    switch (type) {
        case 0:
            title = "Top Events";
            objects = activeProject->getPalette()->getObjectsLinkedList("Top");
            break;
        case 1:
            title = "High Events";
            objects = activeProject->getPalette()->getObjectsLinkedList("High");
            break;
        case 2:
            title = "Mid Events";
            objects = activeProject->getPalette()->getObjectsLinkedList("Mid");
            break;
        case 3:
            title = "Low Events";
            objects = activeProject->getPalette()->getObjectsLinkedList("Low");
            break;
        case 4:
            title = "Bottom Events";
            objects = activeProject->getPalette()->getObjectsLinkedList("Bottom");
            break;
        case 5:
            title = "Spectra";
            objects = activeProject->getPalette()->getObjectsLinkedList("Spectrum");
            break;
        case 6:
            title = "Envelopes";
            objects = activeProject->getPalette()->getObjectsLinkedList("Envelope");
            break;
        case 7:
            title = "Sieves";
            objects = activeProject->getPalette()->getObjectsLinkedList("Sieve");
            break;
        case 8:
            title = "Spatializations";
            objects = activeProject->getPalette()->getObjectsLinkedList("Spatialization");
            break;
        case 9:
            title = "Patterns";
            objects = activeProject->getPalette()->getObjectsLinkedList("Pattern");
            break;
        case 10:
            title = "Reverberations";
            objects = activeProject->getPalette()->getObjectsLinkedList("Reverb");
            break;
        case 12:
            title = "Notes";
            objects = activeProject->getPalette()->getObjectsLinkedList("Note");
            break;
        case 13:
            title = "Filters";
            objects = activeProject->getPalette()->getObjectsLinkedList("Filter");
            break;
        case 14:
            title = "Measurements";
            objects = activeProject->getPalette()->getObjectsLinkedList("Measurement");
            break;
        default:
            title = "Objects";
            break;
    }

    setWindowTitle(title);
    setContentsMargins(3, 3, 3, 3);

    // Build UI: a scroll area containing a widget with a grid layout
    scrollArea = new QScrollArea(this);
    containerWidget = new QWidget();
    gridLayout = new QGridLayout(containerWidget);
    gridLayout->setSpacing(5);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(containerWidget);
    setCentralWidget(scrollArea);

    // Debug output
    qDebug() << "Created ObjectWindow:" << title << "with" << (objects ? "some" : "no") << "objects";

    // Populate initial content
    numOfObjectsChange();
}

ObjectWindow::~ObjectWindow() = default;

void ObjectWindow::setActiveProject(ProjectView* _project) {
    activeProject = _project;
}

void ObjectWindow::refresh() {
    // Clear out existing widgets
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Delete old list
    if (objects) {
        objects->clear();
        objects = nullptr;
    }

    // Reload based on type
    switch (type) {
        case 0:
            objects = activeProject->getPalette()->getObjectsLinkedList("Top");
            break;
        case 1:
            objects = activeProject->getPalette()->getObjectsLinkedList("High");
            break;
        case 2:
            objects = activeProject->getPalette()->getObjectsLinkedList("Mid");
            break;
        case 3:
            objects = activeProject->getPalette()->getObjectsLinkedList("Low");
            break;
        case 4:
            objects = activeProject->getPalette()->getObjectsLinkedList("Bottom");
            break;
        case 5:
            objects = activeProject->getPalette()->getObjectsLinkedList("Spectrum");
            break;
        case 6:
            objects = activeProject->getPalette()->getObjectsLinkedList("Envelope");
            break;
        case 7:
            objects = activeProject->getPalette()->getObjectsLinkedList("Sieve");
            break;
        case 8:
            objects = activeProject->getPalette()->getObjectsLinkedList("Spatialization");
            break;
        case 9:
            objects = activeProject->getPalette()->getObjectsLinkedList("Pattern");
            break;
        case 10:
            objects = activeProject->getPalette()->getObjectsLinkedList("Reverb");
            break;
        case 12:
            objects = activeProject->getPalette()->getObjectsLinkedList("Note");
            break;
        case 13:
            objects = activeProject->getPalette()->getObjectsLinkedList("Filter");
            break;
        case 14:
            objects = activeProject->getPalette()->getObjectsLinkedList("Measurement");
            break;
        default:
            break;
    }

    numOfObjectsChange();
}

void ObjectWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    int newCols = width() / COLUMN_WIDTH;
    if (newCols > 0 && newCols != currentNumOfColumns) {
        sizeChange(newCols);
    }
}

void ObjectWindow::print() {
    qDebug() << windowTitle() << ":";
    for (auto pkg = objects; pkg; pkg = pkg->next) {
        // PLACEHOLDER BC IEVENT getEventName NOT MADE YET
        qDebug() << "obj window print";
        //qDebug() << pkg->ievent->getEventName().c_str();
    }
}

QString ObjectWindow::folderSelected() {
    // TODO: implement actual selection logic
    return QString();
}

void ObjectWindow::sizeChange(int newNumOfColumns) {
    currentNumOfColumns = newNumOfColumns;
    // Remove all widgets
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    if (!objects) return;

    // Re‐attach in new grid
    int row = 0, col = 0;
    for (auto pkg = objects; pkg; pkg = pkg->next) {
        // Set button text to event name
        if (pkg->ievent) {
            // PLACEHOLDER BC IEVENT getEventName NOT MADE YET
            pkg->setText("size change event name");
            //pkg->setText(QString::fromStdString(pkg->ievent->getEventName()));
        }
        gridLayout->addWidget(pkg, row, col);
        if (++col >= currentNumOfColumns) {
            col = 0;
            ++row;
        }
    }
    containerWidget->setLayout(gridLayout);
}

void ObjectWindow::numOfObjectsChange() {
    // Clear existing
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    if (!objects) return;

    // Compute columns
    currentNumOfColumns = width() / COLUMN_WIDTH;
    if (currentNumOfColumns == 0) currentNumOfColumns = 1;

    // Attach all packages
    int row = 0, col = 0;
    for (auto pkg = objects; pkg; pkg = pkg->next) {
        // Set button text to event name
        if (pkg->ievent) {
            // PLACEHOLDER BC IEVENT getEventName NOT MADE YET
            pkg->setText("num of obj changed event name");
            //pkg->setText(QString::fromStdString(pkg->ievent->getEventName()));
        }
        gridLayout->addWidget(pkg, row, col);
        if (++col >= currentNumOfColumns) {
            col = 0;
            ++row;
        }
    }
    containerWidget->setLayout(gridLayout);
}

//-----------------------------------------------------------------------------
// ObjectWindowObjectPackage Implementation
//-----------------------------------------------------------------------------

ObjectWindowObjectPackage::ObjectWindowObjectPackage(ProjectView* _projectView)
    : QPushButton(nullptr),
      projectView(_projectView),
      prev(nullptr),
      next(nullptr),
      ievent(nullptr)
{
    // When pressed, show this event's attributes
    connect(this, &QPushButton::pressed, this, &ObjectWindowObjectPackage::showContent);
}

ObjectWindowObjectPackage::~ObjectWindowObjectPackage() = default;

int ObjectWindowObjectPackage::count() {
    return next ? 1 + next->count() : 1;
}

void ObjectWindowObjectPackage::clear() {
    if (next) next->clear();
    delete this;
}

void ObjectWindowObjectPackage::showContent() {
    //projectView->showAttributes(ievent);
}
