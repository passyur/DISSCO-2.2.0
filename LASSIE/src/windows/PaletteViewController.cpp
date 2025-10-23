#include "PaletteViewController.hpp"
#include "ProjectViewController.hpp"
#include "ObjectWindow.hpp"
#include "../core/event_struct.hpp"

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
    treeView->setDragEnabled(true);

    // Adds folder events to tree view
    folderTop = new QStandardItem("Folder");
    folderTop->setFlags(folderTop->flags() & ~Qt::ItemIsDragEnabled);
    QStandardItem* nameTop = new QStandardItem("Top");
    nameTop->setFlags(nameTop->flags() & ~Qt::ItemIsDragEnabled);
    model->appendRow({folderTop, nameTop});

    folderHigh = new QStandardItem("Folder");
    folderHigh->setFlags(folderHigh->flags() & ~Qt::ItemIsDragEnabled);
    QStandardItem* nameHigh = new QStandardItem("High");
    nameHigh->setFlags(nameHigh->flags() & ~Qt::ItemIsDragEnabled);
    model->appendRow({folderHigh, nameHigh});

    folderMid = new QStandardItem("Folder");
    folderMid->setFlags(folderMid->flags() & ~Qt::ItemIsDragEnabled);
    QStandardItem* nameMid = new QStandardItem("Mid");
    nameMid->setFlags(nameMid->flags() & ~Qt::ItemIsDragEnabled);
    model->appendRow({folderMid, nameMid});

    folderLow = new QStandardItem("Folder");
    folderLow->setFlags(folderLow->flags() & ~Qt::ItemIsDragEnabled);
    QStandardItem* nameLow = new QStandardItem("Low");
    nameLow->setFlags(nameLow->flags() & ~Qt::ItemIsDragEnabled);
    model->appendRow({folderLow, nameLow});

    folderBottom = new QStandardItem("Folder");
    folderBottom->setFlags(folderBottom->flags() & ~Qt::ItemIsDragEnabled);
    QStandardItem* nameBottom = new QStandardItem("Bottom");
    nameBottom->setFlags(nameBottom->flags() & ~Qt::ItemIsDragEnabled);
    model->appendRow({folderBottom, nameBottom});

    folderSpectrum = new QStandardItem("Folder");
    folderSpectrum->setFlags(folderSpectrum->flags() & ~Qt::ItemIsDragEnabled);
    QStandardItem* nameSpectrum = new QStandardItem("Spectrum");
    nameSpectrum->setFlags(nameSpectrum->flags() & ~Qt::ItemIsDragEnabled);
    model->appendRow({folderSpectrum, nameSpectrum});

    folderNote = new QStandardItem("Folder");
    folderNote->setFlags(folderNote->flags() & ~Qt::ItemIsDragEnabled);
    QStandardItem* nameNote = new QStandardItem("Note");
    nameNote->setFlags(nameNote->flags() & ~Qt::ItemIsDragEnabled);
    model->appendRow({folderNote, nameNote});

    folderEnv = new QStandardItem("Folder");
    folderEnv->setFlags(folderEnv->flags() & ~Qt::ItemIsDragEnabled);
    QStandardItem* nameEnv = new QStandardItem("Envelope");
    nameEnv->setFlags(nameEnv->flags() & ~Qt::ItemIsDragEnabled);
    model->appendRow({folderEnv, nameEnv});

    folderSiv = new QStandardItem("Folder");
    folderSiv->setFlags(folderSiv->flags() & ~Qt::ItemIsDragEnabled);
    QStandardItem* nameSiv = new QStandardItem("Sieve");
    nameSiv->setFlags(nameSiv->flags() & ~Qt::ItemIsDragEnabled);
    model->appendRow({folderSiv, nameSiv});

    folderSpa = new QStandardItem("Folder");
    folderSpa->setFlags(folderSpa->flags() & ~Qt::ItemIsDragEnabled);
    QStandardItem* nameSpa = new QStandardItem("Spatialization");
    nameSpa->setFlags(nameSpa->flags() & ~Qt::ItemIsDragEnabled);
    model->appendRow({folderSpa, nameSpa});

    folderPat = new QStandardItem("Folder");
    folderPat->setFlags(folderPat->flags() & ~Qt::ItemIsDragEnabled);
    QStandardItem* namePat = new QStandardItem("Pattern");
    namePat->setFlags(namePat->flags() & ~Qt::ItemIsDragEnabled);
    model->appendRow({folderPat, namePat});

    folderRev = new QStandardItem("Folder");
    folderRev->setFlags(folderRev->flags() & ~Qt::ItemIsDragEnabled);
    QStandardItem* nameRev = new QStandardItem("Reverb");
    nameRev->setFlags(nameRev->flags() & ~Qt::ItemIsDragEnabled);
    model->appendRow({folderRev, nameRev});

    folderFil = new QStandardItem("Folder");
    folderFil->setFlags(folderFil->flags() & ~Qt::ItemIsDragEnabled);
    QStandardItem* nameFil = new QStandardItem("Filter");
    nameFil->setFlags(nameFil->flags() & ~Qt::ItemIsDragEnabled);
    model->appendRow({folderFil, nameFil});

    folderMea = new QStandardItem("Folder");
    folderMea->setFlags(folderMea->flags() & ~Qt::ItemIsDragEnabled);
    QStandardItem* nameMea = new QStandardItem("Measurement");
    nameMea->setFlags(nameMea->flags() & ~Qt::ItemIsDragEnabled);
    model->appendRow({folderMea, nameMea});

    // Calls objectActivated
    connect(treeView, &QTreeView::doubleClicked, this, &PaletteViewController::objectActivated);

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

    if (!parent) {
        eventType = model->itemFromIndex(index.sibling(index.row(), 1))->text();
        eventName = NULL;
    } else {
        eventType = model->itemFromIndex(index.sibling(index.row(), 0))->text();;
        eventName = model->itemFromIndex(index.sibling(index.row(), 1))->text();
    }
    
    projectView->showAttributes(eventType, eventName);
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

int PaletteViewController::getEventTypeFromString(const QString& type)
{
    if (type == "Top") return 0;
    if (type == "High") return 1;
    if (type == "Mid") return 2;
    if (type == "Low") return 3;
    if (type == "Bottom") return 4;
    if (type == "Spectrum") return 5;
    if (type == "Envelope") return 6;
    if (type == "Sieve") return 7;
    if (type == "Spatialization") return 8;
    if (type == "Pattern") return 9;
    if (type == "Reverb") return 10;
    if (type == "Note") return 12;
    if (type == "Filter") return 13;
    if (type == "Measurement") return 14;
    return -1;
} 