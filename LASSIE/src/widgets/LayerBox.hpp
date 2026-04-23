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
#include <QMenu>
#include <QKeyEvent>
#include <QShortcut>

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
    LayerBox(Eventtype eventType, int eventIndex, int layerIndex, QWidget* parent);
    ~LayerBox() override;

    QStandardItem* extractItemFromDrop(QDropEvent* event);
    bool eventFilter(QObject* obj, QEvent* event) override;

    // Called by EventAttributesViewController when layers above this one are deleted
    void setLayerIndex(int layerIndex) { m_layerIndex = layerIndex; }
    // Called when a preceding sibling event is deleted (index shifts down by 1)
    void setEventIndex(int eventIndex) { m_eventIndex = eventIndex; }

    // Rebuild the tree-view model from the backend Layer. Use after the
    // backend's discrete_packages have been mutated externally (e.g. after
    // reconciling references to a renamed event).
    void reloadFromBackend();

    // Flush the weight entry text to the backend Layer::by_layer.
    // \todo this is tremendously chud and should be somehow delegated to ProjectManager, but for now is fine
    void saveWeightToBackend() {
        getBackendLayer().by_layer = m_weightEntry->text();
    }

    // Show or hide the per-package editing columns (Weight, envelopes, scalers).
    // Call with true when Discrete child-event-definition is selected.
    void setPackageFieldsVisible(bool visible) {
        for (int col = 3; col <= 7; ++col)
            m_treeView->setColumnHidden(col, !visible);
    }

    // Enable or disable the weight entry and Insert Function button.
    // Text is preserved when disabling.
    void setWeightEnabled(bool enabled) {
        m_weightEntry->setEnabled(enabled);
        m_weightFuncButton->setEnabled(enabled);
    }

signals:
    void deleteRequested(LayerBox* self);

private slots:
    void onWeightFunctionClicked() {};
    void onDeleteLayerClicked();
    void onWeightChanged(const QString& text);
    void onContextMenu(const QPoint& pos);
    void onCopySelected();
    void onPasteClipboard();
    void onDeleteSelected();

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private:
    // Returns a reference to the actual Layer in the backend.
    // Valid as long as the parent HEvent's event_layers list is not resized.
    Layer& getBackendLayer();

    QList<int> selectedRows() const;

    // Identity of the layer in the backend
    Eventtype m_eventType;
    int       m_eventIndex;
    int       m_layerIndex;

    QList<Package> m_clipboard;

    // UI elements
    QVBoxLayout*        m_mainLayout = nullptr;
    QTreeView*          m_treeView = nullptr;
    QStandardItemModel* m_model = nullptr;
    QLineEdit*          m_weightEntry = nullptr;
    QPushButton*        m_weightFuncButton = nullptr;
    QPushButton*        m_deleteLayerButton = nullptr;
};

#endif
