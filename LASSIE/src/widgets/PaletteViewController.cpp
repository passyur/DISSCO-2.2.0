#include "PaletteViewController.hpp"
#include "ProjectViewController.hpp"
#include "../core/event_struct.hpp"
#include "../core/project_struct.hpp"
#include "../inst.hpp"

#include "../utilities.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include <QStandardItem>
#include <QHeaderView>

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
    auto* nameTop = new QStandardItem("Top");
    nameTop->setFlags(nameTop->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderTop, nameTop});

    folderHigh = new QStandardItem("Folder");
    folderHigh->setFlags(folderHigh->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    auto* nameHigh = new QStandardItem("High");
    nameHigh->setFlags(nameHigh->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderHigh, nameHigh});

    folderMid = new QStandardItem("Folder");
    folderMid->setFlags(folderMid->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    auto* nameMid = new QStandardItem("Mid");
    nameMid->setFlags(nameMid->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderMid, nameMid});

    folderLow = new QStandardItem("Folder");
    folderLow->setFlags(folderLow->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    auto* nameLow = new QStandardItem("Low");
    nameLow->setFlags(nameLow->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderLow, nameLow});

    folderBottom = new QStandardItem("Folder");
    folderBottom->setFlags(folderBottom->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    auto* nameBottom = new QStandardItem("Bottom");
    nameBottom->setFlags(nameBottom->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderBottom, nameBottom});

    folderSpectrum = new QStandardItem("Folder");
    folderSpectrum->setFlags(folderSpectrum->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    auto* nameSpectrum = new QStandardItem("Spectrum");
    nameSpectrum->setFlags(nameSpectrum->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderSpectrum, nameSpectrum});

    folderNote = new QStandardItem("Folder");
    folderNote->setFlags(folderNote->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    auto* nameNote = new QStandardItem("Note");
    nameNote->setFlags(nameNote->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderNote, nameNote});

    folderEnv = new QStandardItem("Folder");
    folderEnv->setFlags(folderEnv->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    auto* nameEnv = new QStandardItem("Envelope");
    nameEnv->setFlags(nameEnv->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderEnv, nameEnv});

    folderSiv = new QStandardItem("Folder");
    folderSiv->setFlags(folderSiv->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    auto* nameSiv = new QStandardItem("Sieve");
    nameSiv->setFlags(nameSiv->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderSiv, nameSiv});

    folderSpa = new QStandardItem("Folder");
    folderSpa->setFlags(folderSpa->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    auto* nameSpa = new QStandardItem("Spatialization");
    nameSpa->setFlags(nameSpa->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderSpa, nameSpa});

    folderPat = new QStandardItem("Folder");
    folderPat->setFlags(folderPat->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    auto* namePat = new QStandardItem("Pattern");
    namePat->setFlags(namePat->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderPat, namePat});

    folderRev = new QStandardItem("Folder");
    folderRev->setFlags(folderRev->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    auto* nameRev = new QStandardItem("Reverb");
    nameRev->setFlags(nameRev->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderRev, nameRev});

    folderFil = new QStandardItem("Folder");
    folderFil->setFlags(folderFil->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    auto* nameFil = new QStandardItem("Filter");
    nameFil->setFlags(nameFil->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    model->appendRow({folderFil, nameFil});

    folderMea = new QStandardItem("Folder");
    folderMea->setFlags(folderMea->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
    auto* nameMea = new QStandardItem("Measurement");
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
}

PaletteViewController::~PaletteViewController() = default;

QList<QStandardItem*> PaletteViewController::makeChildPaletteTuple(const QString& type, const QString& name) {
    auto* typeItem = new QStandardItem(type);
    auto* nameItem = new QStandardItem(name);
    typeItem->setFlags(typeItem->flags() & ~Qt::ItemIsEditable);
    typeItem->setData(type, Qt::UserRole + 1);
    typeItem->setData(name, Qt::UserRole + 2);
    nameItem->setData(type, Qt::UserRole + 1);
    nameItem->setData(name, Qt::UserRole + 2);
    return {typeItem, nameItem};
}

void PaletteViewController::appendEventRow(Eventtype type, const QString& name) const {
    QStandardItem* folder;
    QString typeStr;
    switch (type) {
        case high:    folder = folderHigh;     typeStr = "High";           break;
        case mid:     folder = folderMid;      typeStr = "Mid";            break;
        case low:     folder = folderLow;      typeStr = "Low";            break;
        case bottom:  folder = folderBottom;   typeStr = "Bottom";         break;
        case sound:   folder = folderSpectrum; typeStr = "Spectrum";       break;
        case note:    folder = folderNote;     typeStr = "Note";           break;
        case env:     folder = folderEnv;      typeStr = "Envelope";       break;
        case sieve:   folder = folderSiv;      typeStr = "Sieve";          break;
        case spa:     folder = folderSpa;      typeStr = "Spatialization"; break;
        case pattern: folder = folderPat;      typeStr = "Pattern";        break;
        case reverb:  folder = folderRev;      typeStr = "Reverb";         break;
        case filter:  folder = folderFil;      typeStr = "Filter";         break;
        default: return;
    }
    folder->appendRow(makeChildPaletteTuple(typeStr, name));
}

void PaletteViewController::populate() const {
    ProjectManager *pm = Inst::get_project_manager();

    // Top event is singular — add directly without a loop
    {
        QList<QStandardItem*> tuple = makeChildPaletteTuple("Top", pm->topevent().name);
        tuple[0]->setFlags(tuple[0]->flags() & ~Qt::ItemIsDragEnabled);
        tuple[1]->setFlags(tuple[1]->flags() & ~Qt::ItemIsDragEnabled & ~Qt::ItemIsEditable);
        folderTop->appendRow(tuple);
    }

    for (const HEvent& item : pm->highevents())
        folderHigh->appendRow(makeChildPaletteTuple("High", item.name));

    for (const HEvent& item : pm->midevents())
        folderMid->appendRow(makeChildPaletteTuple("Mid", item.name));

    for (const HEvent& item : pm->lowevents())
        folderLow->appendRow(makeChildPaletteTuple("Low", item.name));

    for (const BottomEvent& item : pm->bottomevents())
        folderBottom->appendRow(makeChildPaletteTuple("Bottom", item.event.name));

    for (const SpectrumEvent& item : pm->spectrumevents())
        folderSpectrum->appendRow(makeChildPaletteTuple("Spectrum", item.name));

    for (const NoteEvent& item : pm->noteevents())
        folderNote->appendRow(makeChildPaletteTuple("Note", item.name));

    for (const EnvelopeEvent& item : pm->envelopeevents())
        folderEnv->appendRow(makeChildPaletteTuple("Envelope", item.name));

    for (const SieveEvent& item : pm->sieveevents())
        folderSiv->appendRow(makeChildPaletteTuple("Sieve", item.name));

    for (const SpaEvent& item : pm->spaevents())
        folderSpa->appendRow(makeChildPaletteTuple("Spatialization", item.name));

    for (const PatternEvent& item : pm->patternevents())
        folderPat->appendRow(makeChildPaletteTuple("Pattern", item.name));

    for (const ReverbEvent& item : pm->reverbevents())
        folderRev->appendRow(makeChildPaletteTuple("Reverb", item.name));

    for (const FilterEvent& item : pm->filterevents())
        folderFil->appendRow(makeChildPaletteTuple("Filter", item.name));
}

void PaletteViewController::objectActivated(const QModelIndex &index) const {
    if (!index.isValid()) return;
    
    /* check if it has a parent; if it is a Folder, it will have no parent */
    if (const QStandardItem* item = model->itemFromIndex(index); item->parent()) {
        const QString eventType = model->itemFromIndex(index.sibling(index.row(), 0))->text();
        projectView->showAttributes(eventType, index.row());
    }
}

// Takes formatted QStrings and returns the folder associated with its contents.
// NOTE: callers should do a nullptr check before using the result if they haven't
// sanitized input.
QStandardItem* PaletteViewController::string_to_folder(const QString& s) const {
    if (s == "Top")            return folderTop;
    if (s == "High")           return folderHigh;
    if (s == "Mid")            return folderMid;
    if (s == "Low")            return folderLow;
    if (s == "Bottom")         return folderBottom;
    if (s == "Spectrum")       return folderSpectrum;
    if (s == "Note")           return folderNote;
    if (s == "Envelope")       return folderEnv;
    if (s == "Sieve")          return folderSiv;
    if (s == "Spatialization") return folderSpa;
    if (s == "Pattern")        return folderPat;
    if (s == "Reverb")         return folderRev;
    if (s == "Filter")         return folderFil;
    if (s == "Measurement")    return folderMea;
    qDebug() << "[WARNING] Malformed string passed to string_to_folder:" << s;
    return nullptr;
}

void PaletteViewController::updateItemName(const QString& typeStr, const int index, const QString& name) const {
    if (const QStandardItem* folder = string_to_folder(typeStr); folder != nullptr) {
        QStandardItem* nameItem = folder->child(index, 1);
        if (!nameItem || nameItem->text() == name) return;

        model->blockSignals(true);
        nameItem->setText(name);
        model->blockSignals(false);
    }
}

void PaletteViewController::onContextMenuRequested(const QPoint& pos)
{
    const QModelIndex index = treeView->indexAt(pos);
    if (!index.isValid()) return;

    if (const QStandardItem* item = model->itemFromIndex(index); !item || !item->parent()) 
        return; // folder row

    // Type string lives in column 0 of this row
    const QString typeStr = model->itemFromIndex(index.sibling(index.row(), 0))->text();
    if (typeStr == "Top") return; // Top is a singleton — cannot delete/duplicate

    const int eventIndex = index.row(); // row within folder == index in PM list

    QMenu menu(this);
    const QAction* deleteAct    = menu.addAction("Delete");
    const QAction* duplicateAct = menu.addAction("Duplicate");

    const QAction* chosen = menu.exec(treeView->viewport()->mapToGlobal(pos));
    if (!chosen) return;

    if (chosen == deleteAct) {
        projectView->deleteEvent(typeStr, eventIndex);
    } else if (chosen == duplicateAct) {
        projectView->duplicateEvent(typeStr, eventIndex);
    }
}

void PaletteViewController::onItemChanged(const QStandardItem* item) const {
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
    const QString newName = item->text();

    // Get the row index within the parent folder
    const int index = item->row();

    // Get the event type from the type column (column 0)
    const QStandardItem* typeItem = parent->child(item->row(), 0);
    if (!typeItem) return;
    const QString eventType = typeItem->text();

    // Top event name is fixed — ignore any attempted change
    if (eventType == "Top") return;

    // Get project manager
    ProjectManager* pm = Inst::get_project_manager();

    // Update the corresponding event name in the backend
    if (eventType == "High") {
        if (index < pm->highevents().size())
            pm->highevents()[index].name = newName;
    }
    else if (eventType == "Mid") {
        if (index < pm->midevents().size())
            pm->midevents()[index].name = newName;
    }
    else if (eventType == "Low") {
        if (index < pm->lowevents().size())
            pm->lowevents()[index].name = newName;
    }
    else if (eventType == "Bottom") {
        if (index < pm->bottomevents().size())
            pm->bottomevents()[index].event.name = newName;
    }
    else if (eventType == "Spectrum") {
        if (index < pm->spectrumevents().size())
            pm->spectrumevents()[index].name = newName;
    }
    else if (eventType == "Note") {
        if (index < pm->noteevents().size())
            pm->noteevents()[index].name = newName;
    }
    else if (eventType == "Envelope") {
        if (index < pm->envelopeevents().size())
            pm->envelopeevents()[index].name = newName;
    }
    else if (eventType == "Sieve") {
        if (index < pm->sieveevents().size())
            pm->sieveevents()[index].name = newName;
    }
    else if (eventType == "Spatialization") {
        if (index < pm->spaevents().size())
            pm->spaevents()[index].name = newName;
    }
    else if (eventType == "Pattern") {
        if (index < pm->patternevents().size())
            pm->patternevents()[index].name = newName;
    }
    else if (eventType == "Reverb") {
        if (index < pm->reverbevents().size())
            pm->reverbevents()[index].name = newName;
    }
    else if (eventType == "Filter") {
        if (index < pm->filterevents().size())
            pm->filterevents()[index].name = newName;
    }

    // Sync name change to the attributes view if it's currently showing this event
    projectView->updateAttributesNameEntry(eventType, index, newName);
}

void PaletteViewController::onRowsAboutToBeRemoved(const QModelIndex &parent, const int first, const int last) const {
    // Only handle removals from folder items (not root level)
    if (!parent.isValid())
        return; // This would be removing a folder itself, not an event

    // Get the parent item to determine the event type
    if (const QStandardItem* parentItem = model->itemFromIndex(parent); !parentItem)
        return;

    // Get the folder name from the sibling column (column 1)
    const QModelIndex parentSibling = parent.sibling(parent.row(), 1);
    const QStandardItem* folderNameItem = model->itemFromIndex(parentSibling);
    if (!folderNameItem) return;

    const QString folderName = folderNameItem->text();

    // Get project manager
    ProjectManager* pm = Inst::get_project_manager();

    if (folderName == "Top")
        return; // we shouldn't be able to delete the Top event
    // Remove events from backend in reverse order (to maintain correct indices)
    for (int row = last; row >= first; --row) {
        if (folderName == "High") {
            if (row < pm->highevents().size())
                pm->highevents().removeAt(row);
        }
        else if (folderName == "Mid") {
            if (row < pm->midevents().size())
                pm->midevents().removeAt(row);
        }
        else if (folderName == "Low") {
            if (row < pm->lowevents().size())
                pm->lowevents().removeAt(row);
        }
        else if (folderName == "Bottom") {
            if (row < pm->bottomevents().size())
                pm->bottomevents().removeAt(row);
        }
        else if (folderName == "Spectrum") {
            if (row < pm->spectrumevents().size())
                pm->spectrumevents().removeAt(row);
        }
        else if (folderName == "Note") {
            if (row < pm->noteevents().size())
                pm->noteevents().removeAt(row);
        }
        else if (folderName == "Envelope") {
            if (row < pm->envelopeevents().size())
                pm->envelopeevents().removeAt(row);
        }
        else if (folderName == "Sieve") {
            if (row < pm->sieveevents().size())
                pm->sieveevents().removeAt(row);
        }
        else if (folderName == "Spatialization") {
            if (row < pm->spaevents().size())
                pm->spaevents().removeAt(row);
        }
        else if (folderName == "Pattern") {
            if (row < pm->patternevents().size())
                pm->patternevents().removeAt(row);
        }
        else if (folderName == "Reverb") {
            if (row < pm->reverbevents().size())
                pm->reverbevents().removeAt(row);
        }
        else if (folderName == "Filter") {
            if (row < pm->filterevents().size())
                pm->filterevents().removeAt(row);
        }
    }
}

void PaletteViewController::onRowsInserted(const QModelIndex &parent, const int first, const int last) const {
    // Only handle insertions into folder items (not root level)
    if (!parent.isValid())
        return; // This would be inserting a folder itself

    // Get the parent item to determine the event type
    const QStandardItem* parentItem = model->itemFromIndex(parent);
    if (!parentItem) return;

    // Get the folder name from the sibling column (column 1)
    const QModelIndex parentSibling = parent.sibling(parent.row(), 1);
    const QStandardItem* folderNameItem = model->itemFromIndex(parentSibling);
    if (!folderNameItem) return;

    const QString folderName = folderNameItem->text();

    ProjectManager* pm = Inst::get_project_manager();

    // Add events to backend for each inserted row
    for (int row = first; row <= last; ++row) {
        // Check if we need to add a new event to the backend
        // This handles cases where rows are inserted directly into the model
        // (not through the normal insertObject flow)
        bool needsBackendEvent = false;

        if (folderName == "Top")
            needsBackendEvent = false;
        else if (folderName == "High")
            needsBackendEvent = (row >= pm->highevents().size());
        else if (folderName == "Mid")
            needsBackendEvent = (row >= pm->midevents().size());
        else if (folderName == "Low")
            needsBackendEvent = (row >= pm->lowevents().size());
        else if (folderName == "Bottom")
            needsBackendEvent = (row >= pm->bottomevents().size());
        else if (folderName == "Spectrum")
            needsBackendEvent = (row >= pm->spectrumevents().size());
        else if (folderName == "Note")
            needsBackendEvent = (row >= pm->noteevents().size());
        else if (folderName == "Envelope")
            needsBackendEvent = (row >= pm->envelopeevents().size());
        else if (folderName == "Sieve")
            needsBackendEvent = (row >= pm->sieveevents().size());
        else if (folderName == "Spatialization")
            needsBackendEvent = (row >= pm->spaevents().size());
        else if (folderName == "Pattern")
            needsBackendEvent = (row >= pm->patternevents().size());
        else if (folderName == "Reverb")
            needsBackendEvent = (row >= pm->reverbevents().size());
        else if (folderName == "Filter")
            needsBackendEvent = (row >= pm->filterevents().size());

        if (needsBackendEvent) {
            // Get the name from the inserted row
            const QStandardItem* nameItem = parentItem->child(row, 1);
            const QString eventName = nameItem ? nameItem->text() : "New Event";
            pm->addEvent(string_to_eventtype[folderName], eventName);
        }
    }
}