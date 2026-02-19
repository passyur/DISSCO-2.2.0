#ifndef LAYERBOX_HPP
#define LAYERBOX_HPP

#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeView>
#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QLabel>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDebug>
#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QPoint>

#include "../core/event_struct.hpp"

class EventAttributesViewController;
class ProjectView;

// LayerBox: represents a single layer's UI and logic.
// Instead of holding a copy of the Layer struct (which would be disconnected from
// the backend), LayerBox stores the event type/index and layer index so it can
// always reach the authoritative Layer object via Inst::get_project_manager().
class LayerBox : public QFrame {
    Q_OBJECT

public:
    // eventType / eventIndex identify the parent HEvent in the ProjectManager.
    // layerIndex is the position of this layer within HEvent::event_layers.
    LayerBox(Eventtype eventType, unsigned eventIndex, int layerIndex, QWidget* parent);
    ~LayerBox() override;

    QStandardItem* extractItemFromDrop(QDropEvent* event);
    bool eventFilter(QObject* obj, QEvent* event) override;

    // Called by EventAttributesViewController when layers above this one are deleted
    void setLayerIndex(int layerIndex) { m_layerIndex = layerIndex; }
    // Called when a preceding sibling event is deleted (index shifts down by 1)
    void setEventIndex(unsigned eventIndex) { m_eventIndex = eventIndex; }

signals:
    void deleteRequested(LayerBox* self);

private slots:
    void onWeightFunctionClicked() {};
    void onDeleteLayerClicked();
    void onWeightChanged(const QString& text);
    void onPackageContextMenu(const QPoint& pos);

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private:
    // Returns a reference to the actual Layer in the backend.
    // Valid as long as the parent HEvent's event_layers list is not resized.
    Layer& getBackendLayer();

    // Identity of the layer in the backend
    Eventtype m_eventType;
    unsigned  m_eventIndex;
    int       m_layerIndex;

    // UI elements
    QVBoxLayout*        m_mainLayout;
    QTreeView*          m_treeView;
    QStandardItemModel* m_model;
    QLineEdit*          m_weightEntry;
    QPushButton*        m_weightFuncButton;
    QPushButton*        m_deleteLayerButton;
};

#endif
