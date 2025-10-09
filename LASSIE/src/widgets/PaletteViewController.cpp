#include "PaletteViewController.hpp"
#include "ProjectViewController.hpp"
#include "../windows/ObjectWindow.hpp"
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