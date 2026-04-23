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
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QPair>
#include <QPushButton>
#include <algorithm>

namespace {

// Associative list entry: key = (owner event type, owner index, layer index)
// within the project; value = package indices in that layer whose reference
// matches the stale name+type.
struct StaleOwnerKey {
    Eventtype ownerType;
    int       ownerIndex;  // ignored when ownerType == top
    int       layerIndex;
};
using StaleRefAssoc = QList<QPair<StaleOwnerKey, QList<int>>>;

// Walk every HEvent's layers and collect references whose Package has
// event_type == typeNumStr and event_name == oldName. Result is an
// associative list grouped by the owning layer so that a single reconciliation
// choice can be applied uniformly below.
StaleRefAssoc collectStaleLayerReferences(const QString& typeNumStr,
                                          const QString& oldName)
{
    StaleRefAssoc refs;
    ProjectManager* pm = Inst::get_project_manager();

    auto scan = [&](HEvent& h, Eventtype ownerType, int ownerIndex) {
        for (int li = 0; li < h.event_layers.size(); ++li) {
            const QList<Package>& pkgs = h.event_layers[li].discrete_packages;
            QList<int> hits;
            for (int pi = 0; pi < pkgs.size(); ++pi) {
                if (pkgs[pi].event_type == typeNumStr &&
                    pkgs[pi].event_name == oldName) {
                    hits.append(pi);
                }
            }
            if (!hits.isEmpty()) {
                refs.append(qMakePair(StaleOwnerKey{ownerType, ownerIndex, li}, hits));
            }
        }
    };

    scan(pm->topevent(), top, -1);
    for (int i = 0; i < pm->highevents().size(); ++i)
        scan(pm->highevents()[i], high, i);
    for (int i = 0; i < pm->midevents().size(); ++i)
        scan(pm->midevents()[i], mid, i);
    for (int i = 0; i < pm->lowevents().size(); ++i)
        scan(pm->lowevents()[i], low, i);
    for (int i = 0; i < pm->bottomevents().size(); ++i)
        scan(pm->bottomevents()[i].event, bottom, i);

    return refs;
}

HEvent* resolveOwner(const StaleOwnerKey& k) {
    ProjectManager* pm = Inst::get_project_manager();
    switch (k.ownerType) {
        case top:    return &pm->topevent();
        case high:   return (k.ownerIndex < pm->highevents().size())   ? &pm->highevents()[k.ownerIndex]        : nullptr;
        case mid:    return (k.ownerIndex < pm->midevents().size())    ? &pm->midevents()[k.ownerIndex]         : nullptr;
        case low:    return (k.ownerIndex < pm->lowevents().size())    ? &pm->lowevents()[k.ownerIndex]         : nullptr;
        case bottom: return (k.ownerIndex < pm->bottomevents().size()) ? &pm->bottomevents()[k.ownerIndex].event : nullptr;
        default:     return nullptr;
    }
}

void applyRenameTo(const StaleRefAssoc& refs, const QString& newName) {
    for (const auto& entry : refs) {
        HEvent* h = resolveOwner(entry.first);
        if (!h || entry.first.layerIndex >= h->event_layers.size()) continue;
        QList<Package>& pkgs = h->event_layers[entry.first.layerIndex].discrete_packages;
        for (int pi : entry.second) {
            if (pi >= 0 && pi < pkgs.size()) pkgs[pi].event_name = newName;
        }
    }
}

void applyDeleteTo(const StaleRefAssoc& refs) {
    for (const auto& entry : refs) {
        HEvent* h = resolveOwner(entry.first);
        if (!h || entry.first.layerIndex >= h->event_layers.size()) continue;
        QList<Package>& pkgs = h->event_layers[entry.first.layerIndex].discrete_packages;
        QList<int> desc = entry.second;
        std::sort(desc.begin(), desc.end(), std::greater<int>());
        for (int pi : desc) {
            if (pi >= 0 && pi < pkgs.size()) pkgs.removeAt(pi);
        }
    }
}

int totalReferenceCount(const StaleRefAssoc& refs) {
    int n = 0;
    for (const auto& entry : refs) n += entry.second.size();
    return n;
}

} // namespace

// Sorts children within folders alphabetically, but keeps folders in insertion order.
class PaletteSortProxy : public QSortFilterProxyModel {
public:
    using QSortFilterProxyModel::QSortFilterProxyModel;

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override {
        // Root-level items (folders): preserve insertion order
        if (!left.parent().isValid())
            return left.row() < right.row();
        // Children: sort alphabetically by display text
        return QSortFilterProxyModel::lessThan(left, right);
    }
};

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

    proxyModel = new PaletteSortProxy(this);
    proxyModel->setSourceModel(model);
    proxyModel->setSortRole(Qt::DisplayRole);
    proxyModel->setDynamicSortFilter(true);

    treeView->setModel(proxyModel);
    proxyModel->sort(1, Qt::AscendingOrder);
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
}

PaletteViewController::~PaletteViewController() = default;

void PaletteViewController::objectActivated(const QModelIndex &index){
    if (!index.isValid()) { return; }

    QModelIndex sourceIndex = proxyModel->mapToSource(index);
    QStandardItem* item = model->itemFromIndex(sourceIndex);
    QStandardItem* parent = item->parent();

    QString eventType;
    QString eventName;

    /* if it is a Folder, it will have no parent */
    if (!parent) {
        eventType = model->itemFromIndex(sourceIndex.sibling(sourceIndex.row(), 1))->text();
        eventName = NULL;
    } else {
        eventType = model->itemFromIndex(sourceIndex.sibling(sourceIndex.row(), 0))->text();
        eventName = model->itemFromIndex(sourceIndex.sibling(sourceIndex.row(), 1))->text();
        projectView->showAttributes(eventType, sourceIndex.row());
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

QString PaletteViewController::selectedType() const {
    QModelIndex proxyIndex = treeView->currentIndex();
    if (!proxyIndex.isValid())
        return {};

    QModelIndex sourceIndex = proxyModel->mapToSource(proxyIndex);
    QStandardItem* item = model->itemFromIndex(sourceIndex);
    if (!item)
        return {};

    if (item->parent()) {
        // Child item: column 0 holds the type string
        return model->itemFromIndex(sourceIndex.sibling(sourceIndex.row(), 0))->text();
    } else {
        // Folder item: column 1 holds the folder name, which is the type
        return model->itemFromIndex(sourceIndex.sibling(sourceIndex.row(), 1))->text();
    }
}

QStandardItem* PaletteViewController::folderForType(const QString& typeStr) const {
    if (typeStr == "Top")            return folderTop;
    if (typeStr == "High")           return folderHigh;
    if (typeStr == "Mid")            return folderMid;
    if (typeStr == "Low")            return folderLow;
    if (typeStr == "Bottom")         return folderBottom;
    if (typeStr == "Spectrum")       return folderSpectrum;
    if (typeStr == "Note")           return folderNote;
    if (typeStr == "Envelope")       return folderEnv;
    if (typeStr == "Sieve")          return folderSiv;
    if (typeStr == "Spatialization") return folderSpa;
    if (typeStr == "Pattern")        return folderPat;
    if (typeStr == "Reverb")         return folderRev;
    if (typeStr == "Filter")         return folderFil;
    return nullptr;
}

void PaletteViewController::updateItemName(const QString& typeStr, int index, const QString& name)
{
    QStandardItem* folder = folderForType(typeStr);

    QStandardItem* nameItem = folder->child(index, 1);
    if (!nameItem || nameItem->text() == name) return;

    model->blockSignals(true);
    nameItem->setText(name);
    model->blockSignals(false);
}

void PaletteViewController::onContextMenuRequested(const QPoint& pos)
{
    QModelIndex proxyIndex = treeView->indexAt(pos);
    if (!proxyIndex.isValid()) return;

    QModelIndex index = proxyModel->mapToSource(proxyIndex);
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

    // Top event name is fixed — ignore any attempted change
    if (eventType == "Top") return;

    // Get project manager
    ProjectManager* pm = Inst::get_project_manager();

    // Retrieve old name from backend and reject the rename if newName is a duplicate.
    QString oldName;
    {
        bool isDuplicate = false;

        auto checkHList = [&](QList<HEvent>& list) {
            if (index >= list.size()) return;
            oldName = list[index].name;
            for (int i = 0; i < list.size(); ++i)
                if (i != index && list[i].name == newName) isDuplicate = true;
        };
        auto checkSList = [&](const auto& list) {
            if (index >= list.size()) return;
            oldName = list[index].name;
            for (int i = 0; i < list.size(); ++i)
                if (i != index && list[i].name == newName) isDuplicate = true;
        };

        if      (eventType == "High")           checkHList(pm->highevents());
        else if (eventType == "Mid")            checkHList(pm->midevents());
        else if (eventType == "Low")            checkHList(pm->lowevents());
        else if (eventType == "Bottom") {
            const auto& list = pm->bottomevents();
            if (index < list.size()) {
                oldName = list[index].event.name;
                for (int i = 0; i < list.size(); ++i)
                    if (i != index && list[i].event.name == newName) isDuplicate = true;
            }
        }
        else if (eventType == "Spectrum")       checkSList(pm->spectrumevents());
        else if (eventType == "Note")           checkSList(pm->noteevents());
        else if (eventType == "Envelope")       checkSList(pm->envelopeevents());
        else if (eventType == "Sieve")          checkSList(pm->sieveevents());
        else if (eventType == "Spatialization") checkSList(pm->spaevents());
        else if (eventType == "Pattern")        checkSList(pm->patternevents());
        else if (eventType == "Reverb")         checkSList(pm->reverbevents());
        else if (eventType == "Filter")         checkSList(pm->filterevents());

        if (isDuplicate) {
            model->blockSignals(true);
            item->setText(oldName);
            model->blockSignals(false);
            QMessageBox::warning(nullptr, "Duplicate Name",
                QString("A %1 event named \"%2\" already exists. Please choose a different name.")
                    .arg(eventType, newName));
            return;
        }

        if (eventType == "Bottom" && !newName.isEmpty() &&
            newName[0] != QLatin1Char('s') && newName[0] != QLatin1Char('n')) {
            model->blockSignals(true);
            item->setText(oldName);
            model->blockSignals(false);
            QMessageBox::warning(nullptr, "Invalid Name",
                "Bottom Event names must start with 's' or 'n'.");
            return;
        }
    }

    // Update the corresponding event name in the backend
    if (eventType == "High") {
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

    // Sync name change to the attributes view if it's currently showing this event
    projectView->updateAttributesNameEntry(eventType, index, newName);

    // Reconcile any layerbox references to the old name. Per issue #63 option 3,
    // if stale references exist the user must choose rename-all or delete-all;
    // we loop on the dialog until one of those actions is taken.
    if (!oldName.isEmpty() && oldName != newName) {
        const QString typeNumStr = displayStringToEventtypeString(eventType);
        StaleRefAssoc stale = collectStaleLayerReferences(typeNumStr, oldName);
        if (!stale.isEmpty()) {
            QMessageBox mb;
            mb.setIcon(QMessageBox::Question);
            mb.setWindowTitle("Update layerbox references");
            mb.setText(QString("Renaming \"%1\" to \"%2\" leaves %3 layerbox reference(s) to the old name.\n\nChoose how to handle them:")
                .arg(oldName, newName).arg(totalReferenceCount(stale)));
            QPushButton* renameBtn = mb.addButton("Rename references",
                                                  QMessageBox::AcceptRole);
            QPushButton* deleteBtn = mb.addButton("Delete references",
                                                  QMessageBox::DestructiveRole);
            mb.setDefaultButton(renameBtn);

            QAbstractButton* clicked = nullptr;
            while (clicked != renameBtn && clicked != deleteBtn) {
                mb.exec();
                clicked = mb.clickedButton();
            }

            if (clicked == renameBtn) applyRenameTo(stale, newName);
            else                      applyDeleteTo(stale);

            projectView->reloadAllLayerBoxes();
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
