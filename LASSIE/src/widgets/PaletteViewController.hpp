#ifndef PALETTEVIEWCONTROLLER_HPP
#define PALETTEVIEWCONTROLLER_HPP

#include <QTreeView>
#include <QStandardItemModel>
#include <QMenu>

#include "../core/event_struct.hpp"

class ProjectView;

class PaletteViewController : public QWidget {
    Q_OBJECT

public:
    explicit PaletteViewController(ProjectView* projectView);
    ~PaletteViewController() override;
    
    // Populate all palette folders from the current project's event lists
    void populate() const;

    // Append a {type, name} row to the folder corresponding to the given Eventtype
    void appendEventRow(Eventtype type, const QString& name) const;

    // Handles activation of an object in the palette
    void objectActivated(const QModelIndex &index) const;

    // Handle name changes in palette items
    void onItemChanged(const QStandardItem* item) const;

    // Update a specific palette item's name (signals blocked to avoid re-entry)
    void updateItemName(const QString& typeStr, int index, const QString& name) const;

    // Return the folder item for the given type string, or nullptr if unknown
    QStandardItem* string_to_folder(const QString& s) const;

    // Handle row removals from palette
    void onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last) const;

    // Handle row insertions to palette
    void onRowsInserted(const QModelIndex &parent, int first, int last) const;
    
    // Build a two-column {type, name} row suitable for appendRow()
    static QList<QStandardItem*> makeChildPaletteTuple(const QString& type, const QString& name);

private slots:
    void onContextMenuRequested(const QPoint& pos);

private:
    ProjectView* projectView = nullptr;
    
    // Object Folders under tree view
    QStandardItem* folderTop = nullptr;
    QStandardItem* folderHigh = nullptr;
    QStandardItem* folderMid = nullptr;
    QStandardItem* folderLow = nullptr;
    QStandardItem* folderBottom = nullptr;
    QStandardItem* folderSpectrum = nullptr;
    QStandardItem* folderNote = nullptr;
    QStandardItem* folderEnv = nullptr;
    QStandardItem* folderSiv = nullptr;
    QStandardItem* folderSpa = nullptr;
    QStandardItem* folderPat = nullptr;
    QStandardItem* folderRev = nullptr;
    QStandardItem* folderFil = nullptr;
    QStandardItem* folderMea = nullptr;
    
    // Tree View
    QTreeView* treeView = nullptr;
    QStandardItemModel* model = nullptr;

};

#endif // PALETTEVIEWCONTROLLER_HPP