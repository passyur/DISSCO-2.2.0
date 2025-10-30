#ifndef EVENTATTRIBUTESVIEWCONTROLLER_HPP
#define EVENTATTRIBUTESVIEWCONTROLLER_HPP

#include "../lassie.hpp"
#include <QFrame>
#include <QList>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeView>
#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDebug>
#include <QHeaderView>

#include "../core/event_struct.hpp"

class SharedPointers;
class ProjectView;
class EventBottomModifier;
class SpectrumPartial;
class EventLayer;
class QKeyEvent;

namespace Ui {
class EventAttributesViewController;
}

typedef enum {
    maxChildDurFunButton,
    unitsPerSecondFunButton,
    unitsPerBarFunButton,
    numOfChildFunButton1,
    numOfChildFunButton2,
    numOfChildFunButton3,
    childEventDefStartTimeFunButton,
    childEventDefChildTypeFunButton,
    childEventDefDurationFunButton,
    childEventDefAttackSieveFunButton,
    childEventDefDurationSieveFunButton,
    spectrumDeviationFunButton,
    spectrumGenerateFunButton,
    attributesSpaFunButton,
    attributesRevFunButton,
    attributesFilFunButton,
    BSLoudnessFunButton,
    BSModGroupFunButton,
    BSWellTemperedFunButton,
    BSFunFreq1FunButton,
    BSFunFreq2FunButton,
    BSContinuumFunButton
} FunctionButton;

// forward enums from original GTK version - now defined in LASSIE.h
// enum TempoPrefix;
// enum TempoNoteValue;
// enum ModifierType;

class EventAttributesViewController : public QFrame {
    Q_OBJECT

public:
    explicit EventAttributesViewController(ProjectView* projectView);
    // explicit EventAttributesViewController(SharedPointers* sharedPointers,
    //                                        QWidget* parent = nullptr);                                       
    ~EventAttributesViewController() override;
    ProjectView* e_projectView;

    // /*! \brief shows the attributes of the event
    //  *  @param event The event to be shown
    //  */
    void showAttributesOfEvent(Eventtype event); //IEvent* event

    // /*! \brief get the currently shown event */
    // IEvent* getCurrentEvent() const;

    // /*! \brief checks whether attack sieve name refers to existing sieve */
    // bool checkAttackSieve() const;

    // /*! \brief checks whether duration sieve name refers to existing sieve */
    // bool checkDurationSieve() const;

protected:
    // void keyPressEvent(QKeyEvent* event) override;

private slots:
    // // mode buttons
    void fixedButtonClicked();
    void densityButtonClicked();
    void byLayerButtonClicked();

    // // child-event-def buttons
    void continuumButtonClicked();
    void sweepButtonClicked();
    void discreteButtonClicked();

    // // insert-function buttons
    void maxChildDurFunButtonClicked();
    void unitsPerSecondFunButtonClicked();
    void numOfChildFunButton1Clicked();
    void numOfChildFunButton2Clicked();
    void numOfChildFunButton3Clicked();
    void childEventDefStartTimeFunButtonClicked();
    void childEventDefChildTypeFunButtonClicked();
    void childEventDefDurationFunButtonClicked();
    void childEventAttackSieveButtonClicked();
    void childEventDurationSieveButtonClicked();
    // void deviationFunButtonClicked();
    // void generatespectrumFunButtonClicked();

    // // bottom-sub attribute function buttons
    void attributesStandardRevButtonClicked();
    void attributesStandardFilButtonClicked();
    void attributesStandardSpaButtonClicked();
    void BSLoudnessButtonClicked();
    // void BSSpatializationButtonClicked();
    // void BSReverbButtonClicked();
    // void BSFilterButtonClicked();
    void BSModifierGroupButtonClicked();

    void wellTemperedRadioButtonClicked();
    void fundamentalRadioButtonClicked();
    void continuumRadioButtonClicked();
    void BSWellTemperedButtonClicked();
    void BSFunFreqButton1Clicked();
    void BSFunFreqButton2Clicked();
    void BSContinuumButtonClicked();

    // // main actions
    // void addNewLayerButtonClicked();
    // void addModifierButtonClicked();
    // void addPartialButtonClicked();

    // // tempo controls
    void tempoAsNoteValueButtonClicked();
    void tempoAsFractionButtonClicked();
    // void tempoAsNoteValueEntryChanged(const QString&);

private:
    // UI loader
    Ui::EventAttributesViewController* ui;

    // // data/model
    // SharedPointers*              m_sharedPointers;
    // IEvent*                      m_currentlyShownEvent;
    Eventtype                       m_currentlyShownEvent;
    // class LayerBox*              m_modifiers;             // head of doubly-linked modifiers
    // class SoundPartialHBox*      m_soundPartialHboxes;    // head of doubly-linked partials

    // QList<class LayerBox*>           m_layerBoxesStorage;
    // QList<class QCheckBox*>          m_noteModifierCheckBoxes;

    // // internal helpers
    // void saveCurrentShownEventData();
    void showCurrentEventData();
    void insertFunctionString(FunctionButton fn);
    // void refreshChildTypeInLayer();
    // bool deleteLayer(class LayerBox* box);
    // void buildNoteModifiersList();
    // QString generateTempoStringByNoteValue() const;
    // QString generateTempoStringByFraction() const;
    void fixStackedWidgetLayout(QWidget* currPage);
};

// LayerBox: represents a single layer's UI and logic
class LayerBox : public QFrame {
    Q_OBJECT

public:
    LayerBox(EventAttributesViewController* parentController,
             ProjectView*         projectView
            //  EventLayer*                    layerModel,
            //  bool                           showDiscreteColumns
             );
    ~LayerBox() override;
    
    QStandardItem* extractItemFromDrop(QDropEvent* event);
    bool eventFilter(QObject* obj, QEvent* event) override;

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

// public slots:
//     packageAdded();
//     packageRemoved();

private slots:
    void onWeightFunctionClicked() {};
    void onDeleteLayerClicked() {};

protected:
    // bool event(QEvent* e) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private:
    EventAttributesViewController* m_attributesView;
    ProjectView*         m_projectView;
    // EventLayer*                    m_layerModel;

    // UI elements
    QVBoxLayout*                   m_mainLayout;
    QTreeView*                     m_treeView;
    QStandardItemModel*            m_model;
    QLineEdit*                     m_weightEntry;
    QPushButton*                   m_weightFuncButton;
    QPushButton*                   m_deleteLayerButton;

    void renumberChildColumn();
};

// // BottomEventModifierAlignment: UI + logic for a single modifier
// class BottomEventModifierAlignment : public QWidget {
//     Q_OBJECT

// public:
//     BottomEventModifierAlignment(EventBottomModifier* modifier,
//                                  EventAttributesViewController* parentView);
//     ~BottomEventModifierAlignment() override;

//     /*! \brief write all fields back into the EventBottomModifier and its successors */
//     void saveToEvent();

//     BottomEventModifierAlignment* prev { nullptr };
//     BottomEventModifierAlignment* next { nullptr };

// private slots:
//     void onApplyHowChanged(int index);
//     void onTypeChanged(int index);
//     void onRemoveClicked();
//     void probablityEnvelopeButtonClicked();
//     void ampValueEnvelopeButtonClicked();
//     void rateValueEnvelopeButtonClicked();
//     void widthEnvelopeButtonClicked();
//     void partialButtonClicked();
//     void modified();

// private:
//     void grayOutModifierFields(ModifierType type);

//     EventAttributesViewController* m_attributesView;
//     EventBottomModifier*           m_modifier;
//     // class Ui::ModifierWidget*      ui;  // holds pointers to comboboxes, entries, etc.
// };

// // SoundPartialHBox: UI + logic for a single spectrum partial
// class SoundPartialHBox : public QWidget {
//     Q_OBJECT

// public:
//     SoundPartialHBox(SpectrumPartial* partial,
//                      EventAttributesViewController* parentView);
//     ~SoundPartialHBox() override;

//     /*! \brief remove all successors and delete this */
//     void clear();

//     /*! \brief write back envString into SpectrumPartial chain */
//     void saveString();

//     /*! \brief set the displayed "Partial N" label, recursing to next */
//     void setPartialNumber(int number);

//     SoundPartialHBox* prev;
//     SoundPartialHBox* next;

// private slots:
//     void removeButtonClicked();
//     void functionButtonClicked();
//     void modified();

// private:
//     SpectrumPartial*                     m_partial;
//     EventAttributesViewController*       m_attributesView;

//     // UI elements
//     QHBoxLayout*                         m_layout;
//     QLabel*                              m_label;
//     QLineEdit*                           m_envelopeEntry;
//     QPushButton*                         m_functionButton;
//     QPushButton*                         m_removeButton;
// };

#endif // EVENTATTRIBUTESVIEWCONTROLLER_HPP
