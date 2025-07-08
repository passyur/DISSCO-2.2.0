#ifndef PALETTEVIEWCONTROLLER_HPP
#define PALETTEVIEWCONTROLLER_HPP

#include <QWidget>
#include <QMap>
#include <QString>
#include <vector>
#include <QTreeView>
#include <QStandardItemModel>

class ProjectView;
class IEvent;
class ObjectWindowObjectPackage;

class PaletteViewController : public QWidget {
    Q_OBJECT

public:
    explicit PaletteViewController(ProjectView* projectView);
    ~PaletteViewController();

    // Get the linked list of objects for a specific type
    ObjectWindowObjectPackage* getObjectsLinkedList(const QString& type);
    
    // Insert a new event into the palette
    void insertEvent(IEvent* event, const QString& type);
    
    // Remove an event from the palette
    void removeEvent(IEvent* event, const QString& type);

    // Opens Object Window
    void objectActivated(const QModelIndex &index);

    // Object Folders under tree view
    QStandardItem* folderTop;
    QStandardItem* folderHigh;
    QStandardItem* folderMid;
    QStandardItem* folderLow;
    QStandardItem* folderBottom;
    QStandardItem* folderSpectrum;
    QStandardItem* folderNote;
    QStandardItem* folderEnv;
    QStandardItem* folderSiv;
    QStandardItem* folderSpa;
    QStandardItem* folderPat;
    QStandardItem* folderRev;
    QStandardItem* folderFil;
    QStandardItem* folderMea;

private:
    ProjectView* projectView;
    
    // Store events by type
    QMap<QString, std::vector<IEvent*>> eventsByType;
    
    // Convert event type string to EventType enum
    int getEventTypeFromString(const QString& type);

    // Tree View
    QTreeView* treeView;
    QStandardItemModel* model;

};

#endif // PALETTEVIEWCONTROLLER_HPP