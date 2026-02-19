#include "PaletteViewController.hpp"
#include "ProjectViewController.hpp"
#include "../core/event_struct.hpp"
#include "../core/project_struct.hpp"
#include "../inst.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include <QTreeView>
#include <QStandardItem>
#include <QHeaderView>
#include <algorithm>

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
    treeView->header()->setSectionResizeMode(QHeaderView::Fixed);
    layout->addWidget(treeView, 1);
    treeView->setDragEnabled(true);

    // Adds folder events to tree view
    folderTop = new QStandardItem("Folder");
    folderTop->setFlags(folderTop->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    QStandardItem* nameTop = new QStandardItem("Top");
    nameTop->setFlags(nameTop->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderTop, nameTop});

    folderHigh = new QStandardItem("Folder");
    folderHigh->setFlags(folderHigh->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    QStandardItem* nameHigh = new QStandardItem("High");
    nameHigh->setFlags(nameHigh->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderHigh, nameHigh});

    folderMid = new QStandardItem("Folder");
    folderMid->setFlags(folderMid->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    QStandardItem* nameMid = new QStandardItem("Mid");
    nameMid->setFlags(nameMid->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderMid, nameMid});

    folderLow = new QStandardItem("Folder");
    folderLow->setFlags(folderLow->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    QStandardItem* nameLow = new QStandardItem("Low");
    nameLow->setFlags(nameLow->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderLow, nameLow});

    folderBottom = new QStandardItem("Folder");
    folderBottom->setFlags(folderBottom->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    QStandardItem* nameBottom = new QStandardItem("Bottom");
    nameBottom->setFlags(nameBottom->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderBottom, nameBottom});

    folderSpectrum = new QStandardItem("Folder");
    folderSpectrum->setFlags(folderSpectrum->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    QStandardItem* nameSpectrum = new QStandardItem("Spectrum");
    nameSpectrum->setFlags(nameSpectrum->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderSpectrum, nameSpectrum});

    folderNote = new QStandardItem("Folder");
    folderNote->setFlags(folderNote->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    QStandardItem* nameNote = new QStandardItem("Note");
    nameNote->setFlags(nameNote->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderNote, nameNote});

    folderEnv = new QStandardItem("Folder");
    folderEnv->setFlags(folderEnv->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    QStandardItem* nameEnv = new QStandardItem("Envelope");
    nameEnv->setFlags(nameEnv->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderEnv, nameEnv});

    folderSiv = new QStandardItem("Folder");
    folderSiv->setFlags(folderSiv->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    QStandardItem* nameSiv = new QStandardItem("Sieve");
    nameSiv->setFlags(nameSiv->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderSiv, nameSiv});

    folderSpa = new QStandardItem("Folder");
    folderSpa->setFlags(folderSpa->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    QStandardItem* nameSpa = new QStandardItem("Spatialization");
    nameSpa->setFlags(nameSpa->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderSpa, nameSpa});

    folderPat = new QStandardItem("Folder");
    folderPat->setFlags(folderPat->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    QStandardItem* namePat = new QStandardItem("Pattern");
    namePat->setFlags(namePat->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderPat, namePat});

    folderRev = new QStandardItem("Folder");
    folderRev->setFlags(folderRev->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    QStandardItem* nameRev = new QStandardItem("Reverb");
    nameRev->setFlags(nameRev->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderRev, nameRev});

    folderFil = new QStandardItem("Folder");
    folderFil->setFlags(folderFil->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    QStandardItem* nameFil = new QStandardItem("Filter");
    nameFil->setFlags(nameFil->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderFil, nameFil});

    folderMea = new QStandardItem("Folder");
    folderMea->setFlags(folderMea->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    QStandardItem* nameMea = new QStandardItem("Measurement");
    nameMea->setFlags(nameMea->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderMea, nameMea});

    // Calls objectActivated
    connect(treeView, &QTreeView::doubleClicked, this, &PaletteViewController::objectActivated);

    // Right-click context menu on event items
    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeView, &QTreeView::customContextMenuRequested,
            this, &PaletteViewController::onContextMenuRequested);
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

void PaletteViewController::onContextMenuRequested(const QPoint& pos)
{
    QModelIndex index = treeView->indexAt(pos);
    if (!index.isValid()) return;

    QStandardItem* item = model->itemFromIndex(index);
    if (!item || !item->parent()) return; // folder row — ignore

    // Type string lives in column 0 of this row
    QString typeStr = model->itemFromIndex(index.sibling(index.row(), 0))->text();
    if (typeStr == "Top") return; // Top is a singleton — cannot delete/duplicate

    int eventIndex = index.row(); // row within folder == index in PM list

    QMenu menu(this);
    QAction* deleteAct    = menu.addAction("Delete");
    QAction* duplicateAct = menu.addAction("Duplicate");

    QAction* chosen = menu.exec(treeView->viewport()->mapToGlobal(pos));
    if (!chosen) return;

    if (chosen == deleteAct) {
        projectView->deleteEvent(typeStr, eventIndex);
    } else if (chosen == duplicateAct) {
        projectView->duplicateEvent(typeStr, eventIndex);
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