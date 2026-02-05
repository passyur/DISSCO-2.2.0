#include "PaletteViewController.hpp"
#include "ProjectViewController.hpp"
#include "../windows/ObjectWindow.hpp"
#include "../core/event_struct.hpp"
#include "../core/project_struct.hpp"
#include "../inst.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include <QTreeView>
#include <QStandardItem>

PaletteViewController::PaletteViewController(ProjectView* projectView)
    : QWidget(nullptr), projectView(projectView)
{
    // Creates Objects List layout and label
    auto* layout = new QVBoxLayout(this);
    QLabel* label = new QLabel("Objects List");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    // Creates Tree View
    treeView = new QTreeView(this);
    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({"Type", "Name"});
    treeView->setModel(model);
    layout->addWidget(treeView, 1);

    // Adds folder events to tree view
    folderTop = new QStandardItem("Folder");
    QStandardItem* nameTop = new QStandardItem("Top");
    model->appendRow({folderTop, nameTop});

    folderHigh = new QStandardItem("Folder");
    QStandardItem* nameHigh = new QStandardItem("High");
    model->appendRow({folderHigh, nameHigh});

    folderMid = new QStandardItem("Folder");
    QStandardItem* nameMid = new QStandardItem("Mid");
    model->appendRow({folderMid, nameMid});

    folderLow = new QStandardItem("Folder");
    QStandardItem* nameLow = new QStandardItem("Low");
    model->appendRow({folderLow, nameLow});

    folderBottom = new QStandardItem("Folder");
    QStandardItem* nameBottom = new QStandardItem("Bottom");
    model->appendRow({folderBottom, nameBottom});

    folderSpectrum = new QStandardItem("Folder");
    QStandardItem* nameSpectrum = new QStandardItem("Spectrum");
    model->appendRow({folderSpectrum, nameSpectrum});

    folderNote = new QStandardItem("Folder");
    QStandardItem* nameNote = new QStandardItem("Note");
    model->appendRow({folderNote, nameNote});

    folderEnv = new QStandardItem("Folder");
    QStandardItem* nameEnv = new QStandardItem("Envelope");
    model->appendRow({folderEnv, nameEnv});

    folderSiv = new QStandardItem("Folder");
    QStandardItem* nameSiv = new QStandardItem("Sieve");
    model->appendRow({folderSiv, nameSiv});

    folderSpa = new QStandardItem("Folder");
    QStandardItem* nameSpa = new QStandardItem("Spatialization");
    model->appendRow({folderSpa, nameSpa});

    folderPat = new QStandardItem("Folder");
    QStandardItem* namePat = new QStandardItem("Pattern");
    model->appendRow({folderPat, namePat});

    folderRev = new QStandardItem("Folder");
    QStandardItem* nameRev = new QStandardItem("Reverb");
    model->appendRow({folderRev, nameRev});

    folderFil = new QStandardItem("Folder");
    QStandardItem* nameFil = new QStandardItem("Filter");
    model->appendRow({folderFil, nameFil});

    folderMea = new QStandardItem("Folder");
    QStandardItem* nameMea = new QStandardItem("Measurement");
    model->appendRow({folderMea, nameMea});

    // Calls objectActivated
    connect(treeView, &QTreeView::doubleClicked, this, &PaletteViewController::objectActivated);

    // Connect to itemChanged signal to sync palette names with backend
    connect(model, &QStandardItemModel::itemChanged, this, &PaletteViewController::onItemChanged);

    // Connect to rowsAboutToBeRemoved signal to sync deletions with backend
    connect(model, &QStandardItemModel::rowsAboutToBeRemoved, this, &PaletteViewController::onRowsAboutToBeRemoved);

    // Connect to rowsInserted signal to sync additions with backend
    connect(model, &QStandardItemModel::rowsInserted, this, &PaletteViewController::onRowsInserted);

    // // Delete right click Menu
    // this->setContextMenuPolicy(Qt::CustomContextMenu);
    // connect(this,&QWidget::customContextMenuRequested,this,&PaletteViewController::slotCustomMenuRequested);

    // // Delete Action
    // delAct = new QAction("Delete Object", this);
    // connect(delAct, &QAction::triggered, this, &PaletteViewController::deleteObject);

    treeView->expandAll();


    // Initialize empty vectors for each event type
    eventsByType["Top"] = std::vector<IEvent*>();
    eventsByType["High"] = std::vector<IEvent*>();
    eventsByType["Mid"] = std::vector<IEvent*>();
    eventsByType["Low"] = std::vector<IEvent*>();
    eventsByType["Bottom"] = std::vector<IEvent*>();
    eventsByType["Spectrum"] = std::vector<IEvent*>();
    eventsByType["Envelope"] = std::vector<IEvent*>();
    eventsByType["Sieve"] = std::vector<IEvent*>();
    eventsByType["Spatialization"] = std::vector<IEvent*>();
    eventsByType["Pattern"] = std::vector<IEvent*>();
    eventsByType["Reverb"] = std::vector<IEvent*>();
    eventsByType["Note"] = std::vector<IEvent*>();
    eventsByType["Filter"] = std::vector<IEvent*>();
    eventsByType["Measurement"] = std::vector<IEvent*>();
}

PaletteViewController::~PaletteViewController() = default;

void PaletteViewController::objectActivated(const QModelIndex &index){
    if (!index.isValid()) { return; }

    QModelIndex selectedIndex = index;
    QStandardItem* item = model->itemFromIndex(selectedIndex);
    QStandardItem* parent = item->parent();

    QString eventType;
    QString eventName;

    /* if it is a Folder, it will have no parent */
    if (!parent) {
        eventType = model->itemFromIndex(index.sibling(index.row(), 1))->text();
        eventName = NULL;
    } else {
        eventType = model->itemFromIndex(index.sibling(index.row(), 0))->text();
        eventName = model->itemFromIndex(index.sibling(index.row(), 1))->text();
        projectView->showAttributes(eventType, index.row());
    }
}

ObjectWindowObjectPackage* PaletteViewController::getObjectsLinkedList(const QString& type)
{
    if (!eventsByType.contains(type) || eventsByType[type].empty()) {
        return nullptr;
    }

    // Create the first package
    ObjectWindowObjectPackage* head = new ObjectWindowObjectPackage(projectView);
    head->ievent = eventsByType[type][0];
    head->prev = nullptr;
    head->next = nullptr;

    // Create packages for remaining events
    ObjectWindowObjectPackage* current = head;
    for (size_t i = 1; i < eventsByType[type].size(); ++i) {
        ObjectWindowObjectPackage* next = new ObjectWindowObjectPackage(projectView);
        next->ievent = eventsByType[type][i];
        next->prev = current;
        next->next = nullptr;
        
        current->next = next;
        current = next;
    }

    return head;
}

void PaletteViewController::insertEvent(IEvent* event, const QString& type)
{
    if (eventsByType.contains(type)) {
        eventsByType[type].push_back(event);
    }

    // Adds items to tree view to be continued here later
    // QStandardItem* folder = new QStandardItem("Folder");
    // QStandardItem* name = new QStandardItem("Top");
    // model->appendRow({folder, name});
}

void PaletteViewController::removeEvent(IEvent* event, const QString& type)
{
    if (eventsByType.contains(type)) {
        auto& events = eventsByType[type];
        events.erase(std::remove(events.begin(), events.end(), event), events.end());
    }
}

void PaletteViewController::onItemChanged(QStandardItem* item)
{
    // Only handle changes to name items (column 1)
    if (!item || item->column() != 1) {
        return;
    }

    // Get the parent item to determine the folder/event type
    QStandardItem* parent = item->parent();
    if (!parent) {
        return; // This is a folder name, not an event name
    }

    // Get the new name from the item
    QString newName = item->text();

    // Get the row index within the parent folder
    int index = item->row();

    // Get the event type from the type column (column 0)
    QStandardItem* typeItem = parent->child(item->row(), 0);
    if (!typeItem) {
        return;
    }
    QString eventType = typeItem->text();

    // Get project manager
    ProjectManager* pm = Inst::get_project_manager();

    // Update the corresponding event name in the backend
    if (eventType == "Top") {
        // Top event is singular, index should be 0
        pm->topevent().name = newName;
    }
    else if (eventType == "High") {
        if (index < pm->highevents().size()) {
            pm->highevents()[index].name = newName;
        }
    }
    else if (eventType == "Mid") {
        if (index < pm->midevents().size()) {
            pm->midevents()[index].name = newName;
        }
    }
    else if (eventType == "Low") {
        if (index < pm->lowevents().size()) {
            pm->lowevents()[index].name = newName;
        }
    }
    else if (eventType == "Bottom") {
        if (index < pm->bottomevents().size()) {
            pm->bottomevents()[index].event.name = newName;
        }
    }
    else if (eventType == "Spectrum") {
        if (index < pm->spectrumevents().size()) {
            pm->spectrumevents()[index].name = newName;
        }
    }
    else if (eventType == "Note") {
        if (index < pm->noteevents().size()) {
            pm->noteevents()[index].name = newName;
        }
    }
    else if (eventType == "Envelope") {
        if (index < pm->envelopeevents().size()) {
            pm->envelopeevents()[index].name = newName;
        }
    }
    else if (eventType == "Sieve") {
        if (index < pm->sieveevents().size()) {
            pm->sieveevents()[index].name = newName;
        }
    }
    else if (eventType == "Spatialization") {
        if (index < pm->spaevents().size()) {
            pm->spaevents()[index].name = newName;
        }
    }
    else if (eventType == "Pattern") {
        if (index < pm->patternevents().size()) {
            pm->patternevents()[index].name = newName;
        }
    }
    else if (eventType == "Reverb") {
        if (index < pm->reverbevents().size()) {
            pm->reverbevents()[index].name = newName;
        }
    }
    else if (eventType == "Filter") {
        if (index < pm->filterevents().size()) {
            pm->filterevents()[index].name = newName;
        }
    }
}

void PaletteViewController::onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    // Only handle removals from folder items (not root level)
    if (!parent.isValid()) {
        return; // This would be removing a folder itself, not an event
    }

    // Get the parent item to determine the event type
    QStandardItem* parentItem = model->itemFromIndex(parent);
    if (!parentItem) {
        return;
    }

    // Get the folder name from the sibling column (column 1)
    QModelIndex parentSibling = parent.sibling(parent.row(), 1);
    QStandardItem* folderNameItem = model->itemFromIndex(parentSibling);
    if (!folderNameItem) {
        return;
    }

    QString folderName = folderNameItem->text();

    // Get project manager
    ProjectManager* pm = Inst::get_project_manager();

    // Remove events from backend in reverse order (to maintain correct indices)
    for (int row = last; row >= first; --row) {
        if (folderName == "Top") {
            // Top event is singular - shouldn't normally be deleted
            // but if it is, we could reset it to a default state
            // For now, we'll skip this case
        }
        else if (folderName == "High") {
            if (row < pm->highevents().size()) {
                pm->highevents().removeAt(row);
            }
        }
        else if (folderName == "Mid") {
            if (row < pm->midevents().size()) {
                pm->midevents().removeAt(row);
            }
        }
        else if (folderName == "Low") {
            if (row < pm->lowevents().size()) {
                pm->lowevents().removeAt(row);
            }
        }
        else if (folderName == "Bottom") {
            if (row < pm->bottomevents().size()) {
                pm->bottomevents().removeAt(row);
            }
        }
        else if (folderName == "Spectrum") {
            if (row < pm->spectrumevents().size()) {
                pm->spectrumevents().removeAt(row);
            }
        }
        else if (folderName == "Note") {
            if (row < pm->noteevents().size()) {
                pm->noteevents().removeAt(row);
            }
        }
        else if (folderName == "Envelope") {
            if (row < pm->envelopeevents().size()) {
                pm->envelopeevents().removeAt(row);
            }
        }
        else if (folderName == "Sieve") {
            if (row < pm->sieveevents().size()) {
                pm->sieveevents().removeAt(row);
            }
        }
        else if (folderName == "Spatialization") {
            if (row < pm->spaevents().size()) {
                pm->spaevents().removeAt(row);
            }
        }
        else if (folderName == "Pattern") {
            if (row < pm->patternevents().size()) {
                pm->patternevents().removeAt(row);
            }
        }
        else if (folderName == "Reverb") {
            if (row < pm->reverbevents().size()) {
                pm->reverbevents().removeAt(row);
            }
        }
        else if (folderName == "Filter") {
            if (row < pm->filterevents().size()) {
                pm->filterevents().removeAt(row);
            }
        }
    }
}

void PaletteViewController::onRowsInserted(const QModelIndex &parent, int first, int last)
{
    // Only handle insertions into folder items (not root level)
    if (!parent.isValid()) {
        return; // This would be inserting a folder itself
    }

    // Get the parent item to determine the event type
    QStandardItem* parentItem = model->itemFromIndex(parent);
    if (!parentItem) {
        return;
    }

    // Get the folder name from the sibling column (column 1)
    QModelIndex parentSibling = parent.sibling(parent.row(), 1);
    QStandardItem* folderNameItem = model->itemFromIndex(parentSibling);
    if (!folderNameItem) {
        return;
    }

    QString folderName = folderNameItem->text();

    // Get project manager
    ProjectManager* pm = Inst::get_project_manager();

    // Map folder names to event types
    Eventtype eventType;
    if (folderName == "Top") {
        eventType = Eventtype::top;
    }
    else if (folderName == "High") {
        eventType = Eventtype::high;
    }
    else if (folderName == "Mid") {
        eventType = Eventtype::mid;
    }
    else if (folderName == "Low") {
        eventType = Eventtype::low;
    }
    else if (folderName == "Bottom") {
        eventType = Eventtype::bottom;
    }
    else if (folderName == "Spectrum") {
        eventType = Eventtype::sound;
    }
    else if (folderName == "Note") {
        eventType = Eventtype::note;
    }
    else if (folderName == "Envelope") {
        eventType = Eventtype::env;
    }
    else if (folderName == "Sieve") {
        eventType = Eventtype::sieve;
    }
    else if (folderName == "Spatialization") {
        eventType = Eventtype::spa;
    }
    else if (folderName == "Pattern") {
        eventType = Eventtype::pattern;
    }
    else if (folderName == "Reverb") {
        eventType = Eventtype::reverb;
    }
    else if (folderName == "Filter") {
        eventType = Eventtype::filter;
    }
    else if (folderName == "Measurement") {
        eventType = Eventtype::mea;
    }
    else {
        return; // Unknown folder type
    }

    // Add events to backend for each inserted row
    for (int row = first; row <= last; ++row) {
        // Get the name from the inserted row
        QStandardItem* nameItem = parentItem->child(row, 1);
        QString eventName = nameItem ? nameItem->text() : "New Event";

        // Check if we need to add a new event to the backend
        // This handles cases where rows are inserted directly into the model
        // (not through the normal insertObject flow)
        bool needsBackendEvent = false;

        if (folderName == "Top") {
            // Top is singular, shouldn't insert multiple
            needsBackendEvent = false;
        }
        else if (folderName == "High") {
            needsBackendEvent = (row >= pm->highevents().size());
        }
        else if (folderName == "Mid") {
            needsBackendEvent = (row >= pm->midevents().size());
        }
        else if (folderName == "Low") {
            needsBackendEvent = (row >= pm->lowevents().size());
        }
        else if (folderName == "Bottom") {
            needsBackendEvent = (row >= pm->bottomevents().size());
        }
        else if (folderName == "Spectrum") {
            needsBackendEvent = (row >= pm->spectrumevents().size());
        }
        else if (folderName == "Note") {
            needsBackendEvent = (row >= pm->noteevents().size());
        }
        else if (folderName == "Envelope") {
            needsBackendEvent = (row >= pm->envelopeevents().size());
        }
        else if (folderName == "Sieve") {
            needsBackendEvent = (row >= pm->sieveevents().size());
        }
        else if (folderName == "Spatialization") {
            needsBackendEvent = (row >= pm->spaevents().size());
        }
        else if (folderName == "Pattern") {
            needsBackendEvent = (row >= pm->patternevents().size());
        }
        else if (folderName == "Reverb") {
            needsBackendEvent = (row >= pm->reverbevents().size());
        }
        else if (folderName == "Filter") {
            needsBackendEvent = (row >= pm->filterevents().size());
        }

        // Add to backend if needed
        if (needsBackendEvent) {
            pm->addEvent(eventType, eventName);
        }
    }
}