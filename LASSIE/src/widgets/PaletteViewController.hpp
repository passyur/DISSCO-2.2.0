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

class PaletteViewController : public QWidget {
    Q_OBJECT

public:
    explicit PaletteViewController(ProjectView* projectView);
    ~PaletteViewController();
    
    // Insert a new event into the palette
    void insertEvent(IEvent* event, const QString& type);
    
    // Remove an event from the palette
    void removeEvent(IEvent* event, const QString& type);

    // Opens Object Window
    void objectActivated(const QModelIndex &index);

    // Handle name changes in palette items
    void onItemChanged(QStandardItem* item);

    // Handle row removals from palette
    void onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);

    // Handle row insertions to palette
    void onRowsInserted(const QModelIndex &parent, int first, int last);

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
    
    // Tree View
    QTreeView* treeView;
    QStandardItemModel* model;
    // void slotCustomMenuRequested(QPoint pos);
    // QAction *delAct;
    // void deleteObject();

};

#endif // PALETTEVIEWCONTROLLER_HPP