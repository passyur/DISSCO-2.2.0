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

class EventAttributesViewController;
class ProjectView;

// LayerBox: represents a single layer's UI and logic
// Note: LayerBoxes correspond to a Layer in event_struct.hpp, whereas the contents of the QTreeView will be the Packages
class LayerBox : public QFrame {
    Q_OBJECT

public:
    LayerBox(EventAttributesViewController* parentController,
             ProjectView*         projectView
            //  EventLayer*                    layerModel,
            //  bool                           showDiscreteColumns
             );
    ~LayerBox() override;

    // /*! \brief refresh child‐type index numbers, returns next index */
    // int refreshChildTypeIndex(int start);

    // /*! \brief enable or disable the "by‐layer" weight field */
    // void setWeightEnabled(bool enabled);

    // /*! \brief write back the weight field into the underlying EventLayer */
    // void applyWeightToModel();

    // /*! \brief delete this layer from UI and model */
    // void deleteObject();

signals:
    // void requestRefresh();

private slots:
    void onWeightFunctionClicked() {};
    void onDeleteLayerClicked() {};

protected:
    // bool event(QEvent* e) override;

private:
    EventAttributesViewController* m_attributesView;
    ProjectView*                   m_projectView;

    // UI elements
    QVBoxLayout*                   m_mainLayout;
    QTreeView*                     m_treeView;
    QStandardItemModel*            m_model;
    QLineEdit*                     m_weightEntry;
    QPushButton*                   m_weightFuncButton;
    QPushButton*                   m_deleteLayerButton;
};

#endif