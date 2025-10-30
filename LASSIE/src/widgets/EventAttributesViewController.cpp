#include "EventAttributesViewController.hpp"
#include "../ui/ui_Attributes.h"

#include "../inst.hpp"
#include "../core/event_struct.hpp"
#include "../dialogs/FunctionGenerator.hpp"
#include "ProjectViewController.hpp"
//#include "PaletteViewController.h"
//#include "PartialWindow.h"
#include "../widgets/LayerBox.hpp"

#include "Modifiers.hpp"
#include "../ui/ui_Modifiers.h"

#include <QMessageBox>
#include <QKeyEvent>
#include <QScrollBar>

// EventAttributesViewController::EventAttributesViewController(SharedPointers* sharedPointers,
//                                                              QWidget* parent)
EventAttributesViewController::EventAttributesViewController(ProjectView* projectView)
  : QFrame(nullptr),
//   : QFrame(parent),
    // m_sharedPointers(sharedPointers),
    // m_currentlyShownEvent(nullptr),
    // m_modifiers(nullptr),
    // m_soundPartialHboxes(nullptr),
    ui(new Ui::EventAttributesViewController)
{
    ui->setupUi(this);
    ui->stackedWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    e_projectView = projectView;

    // --- connect child‐count mode buttons ---
    connect(ui->fixedButton, &QRadioButton::clicked,
            this, &EventAttributesViewController::fixedButtonClicked);
    connect(ui->densityButton, &QRadioButton::clicked,
            this, &EventAttributesViewController::densityButtonClicked);
    connect(ui->byLayerButton, &QRadioButton::clicked,
            this, &EventAttributesViewController::byLayerButtonClicked);

    // --- connect child‐event‐definition buttons ---
    ui->childEventDefContSweepPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->childEventDefDiscretePage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    connect(ui->continuumButton, &QRadioButton::clicked,
            this, &EventAttributesViewController::continuumButtonClicked);
    connect(ui->sweepButton, &QRadioButton::clicked,
            this, &EventAttributesViewController::sweepButtonClicked);
    connect(ui->discreteButton, &QRadioButton::clicked,
            this, &EventAttributesViewController::discreteButtonClicked);

    // --- connect "insert function" buttons ---
    connect(ui->maxChildDurFunButton, &QPushButton::clicked,
            this, &EventAttributesViewController::maxChildDurFunButtonClicked);
    connect(ui->unitsPerSecondFunButton, &QPushButton::clicked,
            this, &EventAttributesViewController::unitsPerSecondFunButtonClicked);
/*    connect(ui->unitsPerBarFunButton, &QPushButton::clicked,
            this, [=]{ insertFunctionString(unitsPerBarFunButton); });
*/   connect(ui->numOfChildFunButton1, &QPushButton::clicked,
            this, &EventAttributesViewController::numOfChildFunButton1Clicked);
    connect(ui->numOfChildFunButton2, &QPushButton::clicked,
            this, &EventAttributesViewController::numOfChildFunButton2Clicked);
    connect(ui->numOfChildFunButton3, &QPushButton::clicked,
            this, &EventAttributesViewController::numOfChildFunButton3Clicked);
    connect(ui->childEventDefStartTimeFunButton, &QPushButton::clicked,
            this, &EventAttributesViewController::childEventDefStartTimeFunButtonClicked);
    connect(ui->childEventDefChildTypeFunButton, &QPushButton::clicked,
            this, &EventAttributesViewController::childEventDefChildTypeFunButtonClicked);
    connect(ui->childEventDefDurationFunButton, &QPushButton::clicked,
            this, &EventAttributesViewController::childEventDefDurationFunButtonClicked);
    connect(ui->childEventAttackSieveFunButton, &QPushButton::clicked,
            this, &EventAttributesViewController::childEventAttackSieveButtonClicked);
    connect(ui->childEventDurationSieveFunButton, &QPushButton::clicked,
            this, &EventAttributesViewController::childEventDurationSieveButtonClicked);
/*    connect(ui->spectrumDeviationFunButton, &QPushButton::clicked,
            this, &EventAttributesViewController::deviationFunButtonClicked);
    connect(ui->spectrumGenerateFunButton, &QPushButton::clicked,
            this, &EventAttributesViewController::generatespectrumFunButtonClicked);

*/    // --- bottom‐sub‐attribute function buttons ---
    connect(ui->attributesStandardRevButton, &QPushButton::clicked,
            this, &EventAttributesViewController::attributesStandardRevButtonClicked);
    connect(ui->attributesStandardFilButton, &QPushButton::clicked,
            this, &EventAttributesViewController::attributesStandardFilButtonClicked);
    connect(ui->attributesStandardSpaButton, &QPushButton::clicked,
            this, &EventAttributesViewController::attributesStandardSpaButtonClicked);
    connect(ui->BSLoudnessButton, &QPushButton::clicked,
            this, &EventAttributesViewController::BSLoudnessButtonClicked);
/*    connect(ui->BSSpatializationButton, &QPushButton::clicked,
            this, &EventAttributesViewController::BSSpatializationButtonClicked);
    connect(ui->BSReverbButton, &QPushButton::clicked,
            this, &EventAttributesViewController::BSReverbButtonClicked);
    connect(ui->BSFilterButton, &QPushButton::clicked,
            this, &EventAttributesViewController::BSFilterButtonClicked);
*/    connect(ui->BSModifierGroupButton, &QPushButton::clicked,
            this, &EventAttributesViewController::BSModifierGroupButtonClicked);

    ui->BSWellTemperedPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->BSFundamentalPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->BSContinuumPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    connect(ui->wellTemperedRadio, &QRadioButton::clicked,
            this, &EventAttributesViewController::wellTemperedRadioButtonClicked);
    connect(ui->fundamentalRadio, &QRadioButton::clicked,
            this, &EventAttributesViewController::fundamentalRadioButtonClicked);
    connect(ui->continuumRadio, &QRadioButton::clicked,
            this, &EventAttributesViewController::continuumRadioButtonClicked);
          
    connect(ui->BSWellTemperedButton, &QPushButton::clicked,
            this, &EventAttributesViewController::BSWellTemperedButtonClicked);
    connect(ui->BSFunFreqButton1, &QPushButton::clicked,
            this, &EventAttributesViewController::BSFunFreqButton1Clicked);
    connect(ui->BSFunFreqButton2, &QPushButton::clicked,
            this, &EventAttributesViewController::BSFunFreqButton2Clicked);
    connect(ui->BSContinuumButton, &QPushButton::clicked,
            this, &EventAttributesViewController::BSContinuumButtonClicked);

/*
    // --- additional controls ---
    connect(ui->addNewLayerButton, &QPushButton::clicked,
            this, &EventAttributesViewController::addNewLayerButtonClicked);*/
    connect(ui->addModifierButton, &QPushButton::clicked,
            this, &EventAttributesViewController::addModifierButtonClicked);
/*    connect(ui->addPartialButton, &QPushButton::clicked,
            this, &EventAttributesViewController::addPartialButtonClicked);

 */   // --- tempo controls ---
    ui->tempoValuePage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->tempoFractionPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    connect(ui->tempoAsNoteValueRadio, &QRadioButton::clicked,
            this, &EventAttributesViewController::tempoAsNoteValueButtonClicked);
    connect(ui->tempoAsFractionRadio, &QRadioButton::clicked,
            this, &EventAttributesViewController::tempoAsFractionButtonClicked);
/*    connect(ui->tempoValueEntry, &QLineEdit::textChanged,
            this, &EventAttributesViewController::tempoAsNoteValueEntryChanged);
    connect(ui->tempoFractionEntry1, &QLineEdit::textChanged,
            this, &EventAttributesViewController::tempoAsNoteValueEntryChanged);
*/
    setFocusPolicy(Qt::StrongFocus);
    ui->stackedWidget->setCurrentWidget(ui->emptyPage);
    fixStackedWidgetLayout(ui->emptyPage);

    LayerBox* box = new LayerBox(this, e_projectView);
    ui->layersLayout->addWidget(box);
}

EventAttributesViewController::~EventAttributesViewController() {
    // Qt will delete child widgets automatically
}

void EventAttributesViewController::fixStackedWidgetLayout(QWidget* currPage) {
    QList<QWidget*> pages = {
        ui->emptyPage,
        ui->soundPage,
        ui->envPage,
        ui->sievePage,
        ui->spaPage,
        ui->patPage,
        ui->revPage,
        ui->filPage,
        ui->meaPage,
        ui->notePage
    };
    for (QWidget* page : pages) {
        page->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    }
    if (currPage) {
        currPage->adjustSize();
        ui->stackedWidget->setFixedSize(600, currPage->sizeHint().height());
    }
    ui->stackedWidget->adjustSize();
}

void EventAttributesViewController::showAttributesOfEvent(Eventtype type, unsigned index) {


    /// \todo implemenet saveCurrentShownEventData();
    // saveCurrentShownEventData();
    if (m_curreventtype != type || m_curreventindex != index) {
        saveCurrentShownEventData();
    }
    m_curreventtype = type;
    m_curreventindex = index;
    
    showCurrentEventData();
}

void EventAttributesViewController::saveCurrentShownEventData() {

    qDebug() << "saveCurrentShownEventData called";

    Eventtype type = m_curreventtype;
    ProjectManager *pm = Inst::get_project_manager();
    HEvent& event = pm->topevent();
    qDebug() << "event: " << event;
    qDebug() << "old name: " << event.name;


    if (type <= bottom) {
        if (type == bottom) {
    
            BottomEvent& bottom_event = pm->bottomevents()[m_curreventindex];
            ExtraInfo& extra_info = bottom_event.extra_info;

            FreqInfo& freq_info = extra_info.freq_info;
            if (ui->wellTemperedRadio->isChecked()) { freq_info.freq_flag = welltempered; }
            if (ui->fundamentalRadio->isChecked()) { freq_info.freq_flag = fundamental; }
            if (ui->continuumRadio->isChecked()) { freq_info.freq_flag = continuum; }
            freq_info.entry_1 = ui->wellTemperedEntry->text();
            freq_info.entry_1 = ui->funFreqEntry1->text();
            freq_info.entry_2 = ui->funFreqEntry2->text();
            if (ui->hertzRadio->isChecked()) { freq_info.continuum_flag = hertz; }
            if (ui->powerOfTwoRadio->isChecked()) { freq_info.continuum_flag = power_of_two; }

            extra_info.loudness = ui->loudnessEntry->text();
            extra_info.spa = ui->spaEntry->text();
            extra_info.reverb = ui->revEntry->text();
            extra_info.filter = ui->filEntry->text();
            extra_info.modifier_group = ui->modifierGroupEntry->text();

            event = bottom_event.event;
        }else if(type == top){
            event = pm->topevent();
        }else if(type == high){
            event = pm->highevents()[m_curreventindex];
        }else if(type == mid){
            event = pm->midevents()[m_curreventindex];
        }else{
            event = pm->lowevents()[m_curreventindex];
        }
        event.name = ui->nameEntry->text();

        qDebug() << "event.name: " << event.name;

        event.max_child_duration = ui->maxChildDurEntry->text();
        event.timesig.bar_value = ui->timeSig1Entry->text();
        event.timesig.note_value = ui->timeSig2Entry->text();
        event.edu_perbeat = ui->unitsPerSecondEntry->text();


        Tempo& temp = event.tempo;
        if (ui->tempoAsNoteValueRadio->isChecked()) { /* as note */
            temp.method_flag = 0;
            temp.prefix = ui->tempoNotePrefixCombo1->currentText();

            temp.note_value = ui->tempoNoteCombo1->currentText();
            temp.valentry = ui->tempoValueEntry->text();
        } else { /* as note */
            temp.frentry_1 = ui->tempoFractionEntry1->text();
            temp.frentry_2 = ui->tempoFractionEntry2->text();
            temp.prefix = ui->tempoNotePrefixCombo2->currentText();
            temp.note_value = ui->tempoNoteCombo2->currentText();
        }  

        NumChildren& num_children = event.numchildren;
        if (ui->fixedButton->isChecked()) {
            num_children.method_flag = Numchildrenflag::fixed;
        } else if (ui->densityButton->isChecked()) {
            num_children.method_flag = density;
        } else if (ui->byLayerButton->isChecked()) {
            num_children.method_flag = bylayer;
        }  
        num_children.entry_1 = ui->childCountEntry1->text();
        num_children.entry_2 = ui->childCountEntry2->text();
        num_children.entry_3 = ui->childCountEntry3->text();

        // Save layers
        
        ChildDef& childeventdef = event.child_event_def;
        if (ui->discreteButton->isChecked()) {
            childeventdef.definition_flag = discrete;
        } else if (ui->continuumButton->isChecked()) {
            childeventdef.definition_flag = continuumdef;
        } else if (ui->sweepButton->isChecked()) {
            childeventdef.definition_flag = sweep;
        }
        childeventdef.entry_1 = ui->childDefEntry1->text();
        childeventdef.entry_2 = ui->childDefEntry2->text();
        childeventdef.entry_3 = ui->childDefEntry3->text();
        childeventdef.attack_sieve = ui->attackSieveEntry->text();
        childeventdef.duration_sieve = ui->durationSieveEntry->text();

        if (ui->startTypePercentRadio->isChecked()) {
            childeventdef.starttype_flag = percentage;
        } else if (ui->startTypeUnitsRadio->isChecked()) {
            childeventdef.starttype_flag = units;
        } else if (ui->startTypeSecondsRadio->isChecked()) {
            childeventdef.starttype_flag = seconds;
        }

        if (ui->durationTypePercentRadio->isChecked()) {
            childeventdef.durationtype_flag = percentage;
        } else if (ui->durationTypeUnitsRadio->isChecked()) {
            childeventdef.durationtype_flag = units;
        } else if (ui->durationTypeSecondsRadio->isChecked()) {
            childeventdef.durationtype_flag = seconds;
        }

        if (type != bottom) {
            event.spa = ui->spaEntry->text();
            event.reverb = ui->revEntry->text();
            event.filter = ui->filEntry->text();
        }
    }
        

    /*if (!m_currentlyShownEvent) return;

    // Name
    m_currentlyShownEvent->setEventName(ui->nameEntry->text().toStdString());

    // Only if not a folder
    if (m_currentlyShownEvent->getEventType() <= eventBottom) {
        m_currentlyShownEvent->setMaxChildDur(ui->maxChildDurEntry->text().toStdString());
        m_currentlyShownEvent->setTimeSignatureEntry1(ui->timeSig1Entry->text().toStdString());
        m_currentlyShownEvent->setTimeSignatureEntry2(ui->timeSig2Entry->text().toStdString());
        m_currentlyShownEvent->setUnitsPerSecond(ui->unitsPerSecondEntry->text().toStdString());

        // Tempo
        bool asNote = ui.tempoAsNoteValueRadio->isChecked();
        m_currentlyShownEvent->setTempoMethodFlag(asNote ? 0 : 1);
        m_currentlyShownEvent->setTempoPrefix(
            static_cast<TempoPrefix>(ui->tempoNotePrefixCombo->currentData().toInt()));
        m_currentlyShownEvent->setTempoNoteValue(
            static_cast<TempoNoteValue>(ui->tempoNoteValueCombo->currentData().toInt()));
        m_currentlyShownEvent->setTempoFractionEntry1(ui->tempoFractionEntry1->text().toStdString());
        m_currentlyShownEvent->setTempoFractionEntry2(ui->tempoValueEntry->text().toStdString());
        m_currentlyShownEvent->setTempoValueEntry(ui->tempoValueEntry->text().toStdString());

        // Num children
        if (ui->fixedButton->isChecked()) {
            m_currentlyShownEvent->setFlagNumChildren(0);
            m_currentlyShownEvent->setNumChildrenEntry1(ui->childCountEntry1->text().toStdString());
            m_currentlyShownEvent->setNumChildrenEntry2("0");
            m_currentlyShownEvent->setNumChildrenEntry3("0");
        } else if (ui.densityButton->isChecked()) {
            m_currentlyShownEvent->setFlagNumChildren(1);
            m_currentlyShownEvent->setNumChildrenEntry1(ui->childCountEntry1->text().toStdString());
            m_currentlyShownEvent->setNumChildrenEntry2(ui->childCountEntry2->text().toStdString());
            m_currentlyShownEvent->setNumChildrenEntry3(ui->childCountEntry3->text().toStdString());
        } else {
            m_currentlyShownEvent->setFlagNumChildren(2);
        }

        // Layers weights:
        for (auto* layerWidget : m_layerBoxesStorage) {
            layerWidget->applyWeightToModel();
        }

        // Child‐event‐definition
        if (ui->continuumButton->isChecked()) {
            m_currentlyShownEvent->setFlagChildEventDef(0);
            m_currentlyShownEvent->setChildEventDefEntry1(ui->childDefEntry1->text().toStdString());
            m_currentlyShownEvent->setChildEventDefEntry2(ui->childDefEntry2->text().toStdString());
            m_currentlyShownEvent->setChildEventDefEntry3(ui->childDefEntry3->text().toStdString());
            if (ui->startTypePercentRadio->isChecked())       m_currentlyShownEvent->setFlagChildEventDefStartType(0);
            else if (ui->startTypeUnitsRadio->isChecked())    m_currentlyShownEvent->setFlagChildEventDefStartType(1);
            else if (ui->startTypeSecondsRadio->isChecked())  m_currentlyShownEvent->setFlagChildEventDefStartType(2);
            if (ui->durationTypePercentRadio->isChecked())    m_currentlyShownEvent->setFlagChildEventDefDurationType(0);
            else if (ui->durationTypeUnitsRadio->isChecked()) m_currentlyShownEvent->setFlagChildEventDefDurationType(1);
            else if (ui->durationTypeSecondsRadio->isChecked()) m_currentlyShownEvent->setFlagChildEventDefDurationType(2);
        }
        else if (ui->sweepButton->isChecked()) {
            m_currentlyShownEvent->setFlagChildEventDef(1);
            m_currentlyShownEvent->setChildEventDefEntry1(ui->childDefEntry1->text().toStdString());
            m_currentlyShownEvent->setChildEventDefEntry2(ui->childDefEntry2->text().toStdString());
            m_currentlyShownEvent->setChildEventDefEntry3(ui->childDefEntry3->text().toStdString());
            if (ui->startTypePercentRadio->isChecked())       m_currentlyShownEvent->setFlagChildEventDefStartType(0);
            else if (ui->startTypeUnitsRadio->isChecked())    m_currentlyShownEvent->setFlagChildEventDefStartType(1);
            else if (ui->startTypeSecondsRadio->isChecked())  m_currentlyShownEvent->setFlagChildEventDefStartType(2);
            if (ui->durationTypePercentRadio->isChecked())    m_currentlyShownEvent->setFlagChildEventDefDurationType(0);
            else if (ui->durationTypeUnitsRadio->isChecked()) m_currentlyShownEvent->setFlagChildEventDefDurationType(1);
            else if (ui->durationTypeSecondsRadio->isChecked()) m_currentlyShownEvent->setFlagChildEventDefDurationType(2);
        }
        else { // discrete
            m_currentlyShownEvent->setFlagChildEventDef(2);
            m_currentlyShownEvent->setChildEventDefAttackSieve(ui->attackSieveEntry->text().toStdString());
            m_currentlyShownEvent->setChildEventDefDurationSieve(ui->durationSieveEntry->text().toStdString());
        }

        // Environment (top/high/mid/low)
        if (m_currentlyShownEvent->getEventType() >= eventTop
         && m_currentlyShownEvent->getEventType() <= eventLow)
        {
            m_currentlyShownEvent->setSpa(ui->spaEntry->text().toStdString());
            m_currentlyShownEvent->setRev(ui->revEntry->text().toStdString());
            m_currentlyShownEvent->setFil(ui->filEntry->text().toStdString());
        }

        // Bottom‐extra
        if (m_currentlyShownEvent->getEventType() == eventBottom) {
            auto* extra = m_currentlyShownEvent->getEventExtraInfo();
            if (ui->wellTemperedRadio->isChecked()) {
                extra->setFrequencyFlag(0);
                extra->setFrequencyEntry1(ui->wellTemperedEntry->text().toStdString());
            }
            else if (ui->fundamentalRadio->isChecked()) {
                extra->setFrequencyFlag(1);
                extra->setFrequencyEntry1(ui->funFreqEntry1->text().toStdString());
                extra->setFrequencyEntry2(ui->funFreqEntry2->text().toStdString());
            }
            else {
                extra->setFrequencyFlag(2);
                extra->setFrequencyEntry1(ui->continuumFreqEntry->text().toStdString());
                extra->setFrequencyContinuumFlag(ui->hertzRadio->isChecked() ? 0 : 1);
            }
            extra->setLoudness(ui->loudnessEntry->text().toStdString());
            extra->setSpatialization(ui->spatializationEntry->text().toStdString());
            extra->setReverb(ui->reverbEntry->text().toStdString());
            extra->setFilter(ui->filterEntry->text().toStdString());
            extra->setModifierGroup(ui->modifierGroupEntry->text().toStdString());

            if (ui->childTypeSoundRadio->isChecked()) {
                // modifiers
                if (m_modifiers) m_modifiers->saveToEvent();
            }
        }
    }
    // Sound event
    else if (m_currentlyShownEvent->getEventType() == eventSound) {
        auto* extra = m_currentlyShownEvent->getEventExtraInfo();
        extra->setDeviation(ui->spectrumDeviationEntry->text().toStdString());
        extra->setSpectrumGenBuilder(ui->spectrumGenEntry->text().toStdString());
        // partials
        if (m_soundPartialHboxes) m_soundPartialHboxes->saveString();
    }
    // Envelope, Sieve, Spa, Pat, Rev, Fil, Mea, Note – analogous:
    else {
        auto* extra = m_currentlyShownEvent->getEventExtraInfo();
        if (m_currentlyShownEvent->getEventType() == eventEnv)
            extra->setEnvelopeBuilder(ui->envBuilderEntry->text().toStdString());
        else if (m_currentlyShownEvent->getEventType() == eventSiv)
            extra->setSieveBuilder(ui->sieveBuilderEntry->text().toStdString());
        else if (m_currentlyShownEvent->getEventType() == eventSpa)
            extra->setSpatializationBuilder(ui->spaBuilderEntry->text().toStdString());
        else if (m_currentlyShownEvent->getEventType() == eventPat)
            extra->setPatternBuilder(ui->patBuilderEntry->text().toStdString());
        else if (m_currentlyShownEvent->getEventType() == eventRev)
            extra->setReverbBuilder(ui->revBuilderEntry->text().toStdString());
        else if (m_currentlyShownEvent->getEventType() == eventFil)
            extra->setFilterBuilder(ui->filBuilderEntry->text().toStdString());
        else if (m_currentlyShownEvent->getEventType() == eventMea)
            extra->setMeasureBuilder(ui->meaBuilderEntry->text().toStdString());
        else if (m_currentlyShownEvent->getEventType() == eventNote) {
            extra->clearNoteModifiers();
            extra->setStaffNum(ui.staffNumberEntry->text().toStdString());
            for (auto* cb : m_noteModifierCheckBoxes) {
                if (cb->isChecked())
                    extra->addNoteModifiers(cb->text().toStdString());
            }
        }
    }*/
}


void EventAttributesViewController::showCurrentEventData() {
    // clear dynamic widgets
    // qDeleteAll(m_layerBoxesStorage);
    // m_layerBoxesStorage.clear();
    // if (m_modifiers) {
    //     delete m_modifiers;
    //     m_modifiers = nullptr;
    // }
    // if (m_soundPartialHboxes) {
    //     m_soundPartialHboxes->clear();
    //     m_soundPartialHboxes = nullptr;
    // }

    // Choose page based on type of currently shown event
    Eventtype type = m_curreventtype;
    switch(type) {
        case top: 
        case high: 
        case mid: 
        case low: 
        case bottom:
            ui->stackedWidget->setCurrentWidget(ui->standardPage);
            if (type == bottom) {  
                ui->frequencyContainer->setVisible(true);
                ui->loudnessContainer->setVisible(true);
                ui->modGroupContainer->setVisible(true);
            } else {
                ui->frequencyContainer->setVisible(false);
                ui->loudnessContainer->setVisible(false);
                ui->modGroupContainer->setVisible(false);
            }
            fixStackedWidgetLayout(ui->standardPage);
            break;
        case sound:
            ui->stackedWidget->setCurrentWidget(ui->soundPage);
            fixStackedWidgetLayout(ui->soundPage);
            break;
        case env:
            ui->stackedWidget->setCurrentWidget(ui->envPage);
            fixStackedWidgetLayout(ui->envPage);
            break;
        case sieve:
            ui->stackedWidget->setCurrentWidget(ui->sievePage);
            fixStackedWidgetLayout(ui->sievePage);
            break;
        case spa:
            ui->stackedWidget->setCurrentWidget(ui->spaPage);
            fixStackedWidgetLayout(ui->spaPage);
            break;
        case pattern:
            ui->stackedWidget->setCurrentWidget(ui->patPage);
            fixStackedWidgetLayout(ui->patPage);
            break;
        case reverb:
            ui->stackedWidget->setCurrentWidget(ui->revPage);     
            fixStackedWidgetLayout(ui->revPage);  
            break;
        case filter:
            ui->stackedWidget->setCurrentWidget(ui->filPage);
            fixStackedWidgetLayout(ui->filPage);
            break;
        case mea:
            ui->stackedWidget->setCurrentWidget(ui->meaPage);
            fixStackedWidgetLayout(ui->meaPage);
            break;
        case note:
            ui->stackedWidget->setCurrentWidget(ui->notePage);
            fixStackedWidgetLayout(ui->notePage);
            break;
        default:
            ui->stackedWidget->setCurrentWidget(ui->emptyPage);
            fixStackedWidgetLayout(ui->emptyPage);
    }

    // populate fields
    ProjectManager *pm = Inst::get_project_manager();
    // ui->nameEntry->setText(QString::fromStdString(m_currentlyShownEvent->getEventName()));
    HEvent event;
    if(type <= bottom){
        if(type == bottom){
            const BottomEvent& bottom_event = pm->bottomevents()[m_curreventindex];
            ExtraInfo extra_info = bottom_event.extra_info;

            FreqInfo freq_info = extra_info.freq_info;
            ui->wellTemperedRadio->setChecked(freq_info.freq_flag == 0);
            ui->fundamentalRadio->setChecked(freq_info.freq_flag == 1);
            ui->continuumRadio->setChecked(freq_info.freq_flag == 2);
            ui->wellTemperedEntry->setText(freq_info.entry_1);
            ui->funFreqEntry1->setText(freq_info.entry_1);
            ui->funFreqEntry2->setText(freq_info.entry_2);
            // ui->contFreqEntry->setText(freq_info.entry_1);
            ui->hertzRadio->setChecked(freq_info.continuum_flag == 0);
            ui->powerOfTwoRadio->setChecked(freq_info.continuum_flag == 1);

            ui->loudnessEntry->setText(extra_info.loudness);
            ui->spaEntry->setText(extra_info.spa);
            ui->revEntry->setText(extra_info.reverb);
            ui->filEntry->setText(extra_info.filter);
            ui->modifierGroupEntry->setText(extra_info.modifier_group);

            /// \todo modifiers
            // if (ui->childTypeSoundRadio->isChecked()) {
            //     auto* em = extra->getModifiers();
            //     while (em) {
            //         auto* align = new EventBottomModifierAlignment(em, this);
            //         if (!m_modifiers) m_modifiers = align;
            //         else {
            //             auto* tail = m_modifiers;
            //             while (tail->next) tail = tail->next;
            //             tail->next = align;
            //             align->prev = tail;
            //         }
            //         ui.modifiersLayout->addWidget(align);
            //         em = em->next;
            //     }
            //     for(auto iter : )
            // }

            event = bottom_event.event;
        }else if(type == top){
            event = pm->topevent();
        }else if(type == high){
            event = pm->highevents()[m_curreventindex];
            qDebug() << "got high";
        }else if(type == mid){
            event = pm->midevents()[m_curreventindex];
        }else{
            event = pm->lowevents()[m_curreventindex];
        }
        ui->nameEntry->setText(event.name);

        ui->maxChildDurEntry->setText(event.max_child_duration);
        ui->timeSig1Entry->setText(event.timesig.bar_value);
        ui->timeSig2Entry->setText(event.timesig.note_value);
        ui->unitsPerSecondEntry->setText(event.edu_perbeat);

        // tempo
        Tempo tempo = event.tempo;
        /// \todo sort this out
        // ui->tempoNotePrefixCombo->setCurrentIndex(static_cast<int>(tempo.prefix));
        // ui->tempoNoteValueCombo->setCurrentIndex(static_cast<int>(tempo.note_value));
        
        if(tempo.method_flag == 0){  /* as note */
            ui->tempoAsNoteValueRadio->click();
            ui->tempoNotePrefixCombo1->setCurrentIndex(tempo.prefix.toUInt());
            ui->tempoNoteCombo1->setCurrentIndex(tempo.note_value.toUInt());
            ui->tempoValueEntry->setText(tempo.valentry);
        }else{ /* as note */
            ui->tempoAsFractionRadio->click();
            ui->tempoFractionEntry1->setText(tempo.frentry_1);
            ui->tempoFractionEntry2->setText(tempo.frentry_2);
            ui->tempoNotePrefixCombo2->setCurrentIndex(tempo.prefix.toUInt());
            ui->tempoNoteCombo2->setCurrentIndex(tempo.note_value.toUInt());
        }

        // num children
        NumChildren num_children = event.numchildren;
        switch(num_children.method_flag){
            case 0:
                ui->fixedButton->click();
                break;
            case 1:
                ui->densityButton->click();
                break;
            case 2:
                ui->byLayerButton->click();
        }
        ui->childCountEntry1->setText(num_children.entry_1);
        ui->childCountEntry2->setText(num_children.entry_2);
        ui->childCountEntry3->setText(num_children.entry_3);

        // build layers
        // for (auto* layer : m_currentlyShownEvent->layers) {
        //     LayerBox* box = new LayerBox(this, m_sharedPointers->projectView, layer,
        //                                 (event.numchildren.method_flag == bylayer));
        //     ui->layersLayout->addWidget(box);
        //     m_layerBoxesStorage.push_back(box);
        // }
        // refreshChildTypeInLayer();

        // child‐event‐def
        ChildDef childeventdef = event.child_event_def;
        switch(childeventdef.definition_flag){
            case 2:
                ui->discreteButton->click();
                break;
            case 0:
                ui->continuumButton->click();
                break;
            case 1:
                ui->sweepButton->click();
                break;

        }
        ui->childDefEntry1->setText(childeventdef.entry_1);
        ui->childDefEntry2->setText(childeventdef.entry_2);
        ui->childDefEntry3->setText(childeventdef.entry_3);
        ui->attackSieveEntry->setText(childeventdef.attack_sieve);
        ui->durationSieveEntry->setText(childeventdef.duration_sieve);

        unsigned st_flag = childeventdef.starttype_flag;
        ui->startTypePercentRadio->setChecked(st_flag == 0);
        ui->startTypeUnitsRadio->setChecked(st_flag == 1);
        ui->startTypeSecondsRadio->setChecked(st_flag == 2);

        unsigned dt_flag = childeventdef.durationtype_flag;
        ui->durationTypePercentRadio->setChecked(dt_flag == 0);
        ui->durationTypeUnitsRadio->setChecked(dt_flag == 1);
        ui->durationTypeSecondsRadio->setChecked(dt_flag == 2);

        // environment
        if (type != bottom) {
            ui->spaEntry->setText(event.spa);
            ui->revEntry->setText(event.reverb);
            ui->filEntry->setText(event.filter);
        }
    }else{
        // if(type == sound){
            /// \todo implement partials info display
            // auto* extra = m_currentlyShownEvent->getEventExtraInfo();
            // ui->spectrumNumPartialEntry->setText(QString::fromStdString(extra->getNumPartials()));
            // ui->spectrumDeviationEntry->setText(QString::fromStdString(extra->getDeviation()));
            // ui->spectrumGenEntry->setText(QString::fromStdString(extra->getSpectrumGenBuilder()));
            /// \todo implement partials display
            // auto* sp = extra->getSpectrumPartials();
            // if (sp) {
            //     m_soundPartialHboxes = new SoundPartialHBox(sp, this);
            //     ui.partialsLayout->addWidget(m_soundPartialHboxes);
            //     sp = sp->next;
            //     while (sp) {
            //         auto* nextBox = new SoundPartialHBox(sp, this);
            //         m_soundPartialHboxes->next = nextBox;
            //         nextBox->prev = m_soundPartialHboxes;
            //         ui->partialsLayout->addWidget(nextBox);
            //         sp = sp->next;
            //     }
            //     m_soundPartialHboxes->setPartialNumber(1);
            // }

        // }else
        if(type == env){
            const EnvelopeEvent& event = pm->envelopeevents()[m_curreventindex];
            ui->envNameEntry->setText(event.name);
            ui->envBuilderEntry->setText(event.envelope_builder);
        }else if(type == sieve){
            const SieveEvent& event = pm->sieveevents()[m_curreventindex];
            ui->sieveNameEntry->setText(event.name);
            ui->sieveBuilderEntry->setText(event.sieve_builder);
        }else if(type == spa){
            const SpaEvent& event = pm->spaevents()[m_curreventindex];
            ui->spaNameEntry->setText(event.name);
            ui->spaBuilderEntry->setText(event.spatialization);
        }else if(type == pattern){
            const PatternEvent& event = pm->patternevents()[m_curreventindex];
            ui->patNameEntry->setText(event.name);
            ui->patBuilderEntry->setText(event.pattern_builder);
        }else if(type == reverb){
            const ReverbEvent& event = pm->reverbevents()[m_curreventindex];
            ui->revNameEntry->setText(event.name);
            ui->revBuilderEntry->setText(event.reverberation);
        }else if(type == note){
            const NoteEvent& event = pm->noteevents()[m_curreventindex];
            ui->noteNameEntry->setText(event.name);
            ui->staffNumberEntry->setText(event.note_info.staffs);
        }else if(type == filter){
            const FilterEvent& event = pm->filterevents()[m_curreventindex];
            ui->filNameEntry->setText(event.name);
            ui->filBuilderEntry->setText(event.filter_builder);
        }else{
            qDebug() << "Cannot show event data: type of event not valid";
        }
        // Sound event
        // if (type == eventSound) {
            
        // }

        // // envelope, sieve, spa, pat, rev, fil, mea, note
        // else {
        //     auto* extra = m_currentlyShownEvent->getEventExtraInfo();
        //     if (type == eventEnv) {
        //         ui->envNameEntry->setText(QString::fromStdString(m_currentlyShownEvent->getEventName()));
        //         ui->envBuilderEntry->setText(QString::fromStdString(extra->getEnvelopeBuilder()));
        //     }
        //     else if (type == eventSiv) {
        //         ui->sieveNameEntry->setText(QString::fromStdString(m_currentlyShownEvent->getEventName()));
        //         ui->sieveBuilderEntry->setText(QString::fromStdString(extra->getSieveBuilder()));
        //     }
        //     else if (type == eventSpa) {
        //         ui->spaNameEntry->setText(QString::fromStdString(m_currentlyShownEvent->getEventName()));
        //         ui->spaBuilderEntry->setText(QString::fromStdString(extra->getSpatializationBuilder()));
        //     }
        //     else if (type == eventPat) {
        //         ui->patNameEntry->setText(QString::fromStdString(m_currentlyShownEvent->getEventName()));
        //         ui->patBuilderEntry->setText(QString::fromStdString(extra->getPatternBuilder()));
        //     }
        //     else if (type == eventRev) {
        //         ui->revNameEntry->setText(QString::fromStdString(m_currentlyShownEvent->getEventName()));
        //         ui->revBuilderEntry->setText(QString::fromStdString(extra->getReverbBuilder()));
        //     }
        //     else if (type == eventFil) {
        //         ui->filNameEntry->setText(QString::fromStdString(m_currentlyShownEvent->getEventName()));
        //         ui->filBuilderEntry->setText(QString::fromStdString(extra->getFilterBuilder()));
        //     }
        //     else if (type == eventMea) {
        //         ui->meaNameEntry->setText(QString::fromStdString(m_currentlyShownEvent->getEventName()));
        //         ui->meaBuilderEntry->setText(QString::fromStdString(extra->getMeasureBuilder()));
        //     }
        //     else if (type == eventNote) {
        //         ui->noteNameEntry->setText(QString::fromStdString(m_currentlyShownEvent->getEventName()));
        //         ui->staffNumberEntry->setText(QString::fromStdString(extra->getStaffNum()));
        //         buildNoteModifiersList();
        //     }
        // }
    } /* not hevent nor bottom event */
}

void EventAttributesViewController::fixedButtonClicked() {
    densityButtonClicked();
    ui->numOfChildLabel1->setText("Number of Children To Create:");
    ui->numOfChildLabel2->setText("");
    ui->numOfChildLabel3->setText("");
    ui->childCountEntry2->setEnabled(false);
    ui->childCountEntry3->setEnabled(false);
    // for (auto* box : m_layerBoxesStorage) box->setWeightEnabled(false);
}

void EventAttributesViewController::densityButtonClicked() {
    ui->numOfChildLabel1->setText("Density:");
    ui->numOfChildLabel2->setText("Area:");
    ui->numOfChildLabel3->setText("Under One:");
    ui->childCountEntry1->setEnabled(true);
    ui->childCountEntry2->setEnabled(true);
    ui->childCountEntry3->setEnabled(true);
    // for (auto* box : m_layerBoxesStorage) box->setWeightEnabled(false);
}

void EventAttributesViewController::byLayerButtonClicked() {
    ui->numOfChildLabel1->setText("");
    ui->numOfChildLabel2->setText("");
    ui->numOfChildLabel3->setText("");
    ui->childCountEntry1->setEnabled(false);
    ui->childCountEntry2->setEnabled(false);
    ui->childCountEntry3->setEnabled(false);
    // for (auto* box : m_layerBoxesStorage) box->setWeightEnabled(true);
}

void EventAttributesViewController::continuumButtonClicked() {
    ui->childEventDefStack->setCurrentWidget(ui->childEventDefContSweepPage);
    ui->emptyChildEventDefPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->childEventDefDiscretePage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->childEventDefContSweepPage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->childEventDefContSweepPage->adjustSize();
    ui->childEventDefStack->adjustSize();
    // ui->childDefEntry1->setEnabled(true);
    // ui->childDefEntry2->setEnabled(true);
    // ui->childDefEntry3->setEnabled(true);
    // ui->attackSieveEntry->setEnabled(false);
    // ui->durationSieveEntry->setEnabled(false);
    // ui->startTypePercentRadio->setEnabled(true);
    // ui->startTypeUnitsRadio->setEnabled(true);
    // ui->startTypeSecondsRadio->setEnabled(true);
    // ui->durationTypePercentRadio->setEnabled(true);
    // ui->durationTypeUnitsRadio->setEnabled(true);
    // ui->durationTypeSecondsRadio->setEnabled(true);
}

void EventAttributesViewController::sweepButtonClicked() {
    continuumButtonClicked();
}

void EventAttributesViewController::discreteButtonClicked() {
    ui->childEventDefStack->setCurrentWidget(ui->childEventDefDiscretePage);
    ui->emptyChildEventDefPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->childEventDefContSweepPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->childEventDefDiscretePage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->childEventDefDiscretePage->adjustSize();
    ui->childEventDefStack->adjustSize();
    // ui->childDefEntry1->setEnabled(false);
    // ui->childDefEntry2->setEnabled(false);
    // ui->childDefEntry3->setEnabled(false);
    // ui->attackSieveEntry->setEnabled(true);
    // ui->durationSieveEntry->setEnabled(true);
    // ui->startTypePercentRadio->setEnabled(false);
    // ui->startTypeUnitsRadio->setEnabled(false);
    // ui->startTypeSecondsRadio->setEnabled(false);
    // ui->durationTypePercentRadio->setEnabled(false);
    // ui->durationTypeUnitsRadio->setEnabled(false);
    // ui->durationTypeSecondsRadio->setEnabled(false);
}

void EventAttributesViewController::wellTemperedRadioButtonClicked() {
    ui->frequencyStack->setCurrentWidget(ui->BSWellTemperedPage);
    ui->emptyFrequencyPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->BSFundamentalPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->BSContinuumPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    ui->BSWellTemperedPage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->BSWellTemperedPage->adjustSize();
    ui->frequencyStack->adjustSize();
}

void EventAttributesViewController::fundamentalRadioButtonClicked() {
    ui->frequencyStack->setCurrentWidget(ui->BSFundamentalPage);
    ui->emptyFrequencyPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->BSWellTemperedPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->BSContinuumPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    ui->BSFundamentalPage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->BSFundamentalPage->adjustSize();
    ui->frequencyStack->adjustSize();
}

void EventAttributesViewController::continuumRadioButtonClicked() {
    ui->frequencyStack->setCurrentWidget(ui->BSContinuumPage);
    ui->emptyFrequencyPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->BSWellTemperedPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->BSFundamentalPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    ui->BSContinuumPage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->BSContinuumPage->adjustSize();
    ui->frequencyStack->adjustSize();
}

void EventAttributesViewController::maxChildDurFunButtonClicked() {
    insertFunctionString(maxChildDurFunButton);
}

void EventAttributesViewController::unitsPerSecondFunButtonClicked() {
    insertFunctionString(unitsPerSecondFunButton);
}

void EventAttributesViewController::numOfChildFunButton1Clicked() {
    insertFunctionString(numOfChildFunButton1);
}

void EventAttributesViewController::numOfChildFunButton2Clicked() {
    insertFunctionString(numOfChildFunButton2);
}

void EventAttributesViewController::numOfChildFunButton3Clicked() {
    insertFunctionString(numOfChildFunButton3);
}

void EventAttributesViewController::childEventDefStartTimeFunButtonClicked() {
    insertFunctionString(childEventDefStartTimeFunButton);
}

void EventAttributesViewController::childEventDefChildTypeFunButtonClicked() {
    insertFunctionString(childEventDefChildTypeFunButton);
}

void EventAttributesViewController::childEventDefDurationFunButtonClicked() {
    insertFunctionString(childEventDefDurationFunButton);
}

void EventAttributesViewController::childEventAttackSieveButtonClicked() {
    insertFunctionString(childEventDefAttackSieveFunButton);
}

void EventAttributesViewController::childEventDurationSieveButtonClicked() {
    insertFunctionString(childEventDefDurationSieveFunButton);
}
/*
void EventAttributesViewController::deviationFunButtonClicked() {
    insertFunctionString(spectrumDeviationFunButton);
}

void EventAttributesViewController::generatespectrumFunButtonClicked() {
    insertFunctionString(spectrumGenerateFunButton);
}
*/
void EventAttributesViewController::attributesStandardRevButtonClicked() {
    insertFunctionString(attributesRevFunButton);
}

void EventAttributesViewController::attributesStandardFilButtonClicked() {
    insertFunctionString(attributesFilFunButton);
}

void EventAttributesViewController::attributesStandardSpaButtonClicked() {
    insertFunctionString(attributesSpaFunButton);
}

void EventAttributesViewController::BSLoudnessButtonClicked() {
    insertFunctionString(BSLoudnessFunButton);
}
/*
void EventAttributesViewController::BSSpatializationButtonClicked() {
    if (m_currentlyShownEvent->getEventExtraInfo()->getChildTypeFlag() != 0) return;
    insertFunctionString(spectrumDeviationFunButton);
}

void EventAttributesViewController::BSReverbButtonClicked() {
    if (m_currentlyShownEvent->getEventExtraInfo()->getChildTypeFlag() != 0) return;
    insertFunctionString(spectrumGenerateFunButton);
}

void EventAttributesViewController::BSFilterButtonClicked() {
    if (m_currentlyShownEvent->getEventExtraInfo()->getChildTypeFlag() != 0) return;
    insertFunctionString(spectrumGenerateFunButton);
}
*/
void EventAttributesViewController::BSModifierGroupButtonClicked() {
    // if (m_currentlyShownEvent->getEventExtraInfo()->getChildTypeFlag() != 0) return;
    insertFunctionString(BSModGroupFunButton);
}

void EventAttributesViewController::BSWellTemperedButtonClicked() {
    insertFunctionString(BSWellTemperedFunButton);
}

void EventAttributesViewController::BSFunFreqButton1Clicked() {
    insertFunctionString(BSFunFreq1FunButton);
}

void EventAttributesViewController::BSFunFreqButton2Clicked() {
    insertFunctionString(BSFunFreq2FunButton);
}

void EventAttributesViewController::BSContinuumButtonClicked() {
    insertFunctionString(BSContinuumFunButton);
}

void EventAttributesViewController::insertFunctionString(FunctionButton button) {
    QLineEdit* target = nullptr;
    FunctionGenerator* gen = nullptr;

    switch (button) {
    case maxChildDurFunButton:
        target = ui->maxChildDurEntry;
        gen = new FunctionGenerator(nullptr, functionReturnFloat, target->text());
        break;
    case unitsPerSecondFunButton:
        target = ui->unitsPerSecondEntry;
        gen = new FunctionGenerator(nullptr, functionReturnInt, target->text());
        break;
    // case unitsPerBarFunButton:
    //     target = ui->unitsPerBarEntry;
    //     gen = new FunctionGenerator(functionReturnInt, target->text());
    //     break;
    case numOfChildFunButton1:
        target = ui->childCountEntry1;
        gen = new FunctionGenerator(nullptr, functionReturnInt, target->text());
        break;
    case numOfChildFunButton2:
        target = ui->childCountEntry2;
        gen = new FunctionGenerator(nullptr, functionReturnInt, target->text());
        break;
    case numOfChildFunButton3:
        target = ui->childCountEntry3;
        gen = new FunctionGenerator(nullptr, functionReturnInt, target->text());
        break;
    case childEventDefStartTimeFunButton:
        target = ui->childDefEntry1;
        gen = new FunctionGenerator(nullptr, functionReturnFloat, target->text());
        break;
    case childEventDefChildTypeFunButton:
        target = ui->childDefEntry2;
        gen = new FunctionGenerator(nullptr, functionReturnInt, target->text());
        break;
    case childEventDefDurationFunButton:
        target = ui->childDefEntry3;
        gen = new FunctionGenerator(nullptr, functionReturnFloat, target->text());
        break;
    case childEventDefAttackSieveFunButton:
        target = ui->attackSieveEntry;
        gen = new FunctionGenerator(nullptr, functionReturnSIV, target->text());
        break;
    case childEventDefDurationSieveFunButton:
        target = ui->durationSieveEntry;
        gen = new FunctionGenerator(nullptr, functionReturnSIV, target->text());
        break;
    case attributesSpaFunButton:
        target = ui->spaEntry;
        gen = new FunctionGenerator(nullptr, functionReturnSPA, target->text());
        break;
    case attributesRevFunButton:
        target = ui->revEntry;
        gen = new FunctionGenerator(nullptr, functionReturnREV, target->text());
        break;
    case attributesFilFunButton:
        target = ui->filEntry;
        gen = new FunctionGenerator(nullptr, functionReturnFIL, target->text());
        break;
    case BSLoudnessFunButton:
        target = ui->loudnessEntry;
        gen = new FunctionGenerator(nullptr, functionReturnFloat, target->text());
        break;
    case BSModGroupFunButton:
        target = ui->modifierGroupEntry;
        gen = new FunctionGenerator(nullptr, functionReturnSPE, target->text());
        break;
    case BSWellTemperedFunButton:
        target = ui->wellTemperedEntry;
        gen = new FunctionGenerator(nullptr, functionReturnFloat, target->text());
        break;
    case BSFunFreq1FunButton:
        target = ui->funFreqEntry1;
        gen = new FunctionGenerator(nullptr, functionReturnFloat, target->text());
        break;
    case BSFunFreq2FunButton:
        target = ui->funFreqEntry2;
        gen = new FunctionGenerator(nullptr, functionReturnSPE, target->text());
        break;
    case BSContinuumFunButton:
        target = ui->continuumFreqEntry;
        gen = new FunctionGenerator(nullptr, functionReturnSPE, target->text());
        break;
    // case spectrumDeviationFunButton:
    //     target = ui->spectrumDeviationEntry;
    //     gen = new FunctionGenerator(functionReturnFloat, target->text());
    //     break;
    // case spectrumGenerateFunButton:
    //     target = ui->spectrumGenEntry;
    //     gen = new FunctionGenerator(functionReturnSPE, target->text());
    //     break;

    default:
        return;
    }

    if (gen) {
        if (gen->exec() == QDialog::Accepted && !gen->getResultString().isEmpty())
            target->setText(gen->getResultString());
        delete gen;
    }
}
/*
void EventAttributesViewController::addNewLayerButtonClicked() {
    if (!m_currentlyShownEvent || m_currentlyShownEvent->getEventType() >= eventSound)
        return;

    auto* newLayer = m_currentlyShownEvent->addLayer();
    LayerBox* box = new LayerBox(this,
                                 m_sharedPointers->projectView,
                                 newLayer,
                                 ui->discreteButton->isChecked());
    ui->layersLayout->addWidget(box);
    m_layerBoxesStorage.push_back(box);
}
*/
void EventAttributesViewController::addModifierButtonClicked() {
    // if (!m_currentlyShownEvent) return;
    // EventBottomModifier* mod = (m_currentlyShownEvent->getEventType()==eventBottom)
    //     ? m_currentlyShownEvent->getEventExtraInfo()->addModifier()
    //     : m_currentlyShownEvent->addModifier();

    // auto* align = new EventBottomModifierAlignment(mod, this);
    // if (!m_modifiers) m_modifiers = align;
    // else {
    //     auto* tail = m_modifiers;
    //     while (tail->next) tail = tail->next;
    //     tail->next = align;
    //     align->prev = tail;
    // }

    // TO DO: connect add modifier to data structure
    
    ui->modScrollWindow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->modScrollWindowContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    Modifiers* newMod = new Modifiers(ui->modScrollWindow);
    ui->modScrollWindowContent->layout()->addWidget(newMod);

    /*Modifier newModData = {};

    if (newMod->ui->modifierApply->currentText() == "SOUND") {
        newModData.applyhow_flag = false;
    } else if (newMod->ui->modifierApply->currentText() == "PARTIAL") {
        newModData.applyhow_flag = true;
    }
    newModData.group_name = newMod->ui->modifierNameEdit->text();
    qDebug() << "newModData.group_name: " << newModData.group_name;
    newModData.probability = newMod->ui->modifierProbEdit->text();
    newModData.detune_spread = newMod->ui->modifierSpreadEdit->text();
    newModData.detune_direction = newMod->ui->modifierDirEdit->text();
    newModData.detune_velocity = newMod->ui->modifierVelEdit->text();
    newModData.amplitude = newMod->ui->modifierMagEdit->text();
    newModData.width = newMod->ui->modifierWidthEdit->text();
    newModData.partialresult_string = newMod->ui->modifierResEdit->text();
    newModData.rate = newMod->ui->modifierRateEdit->text();

    Eventtype type = m_curreventtype;
    ProjectManager *pm = Inst::get_project_manager();
    switch(type) {
        case top: {
            HEvent& top_event = pm->topevent();
            top_event.modifiers.push_back(newModData);
            break;
        }
        case high: {
            HEvent& high_event = pm->highevents()[m_curreventindex];
            high_event.modifiers.push_back(newModData);
            break;
        }
        case mid: {
            HEvent& mid_event = pm->midevents()[m_curreventindex];
            mid_event.modifiers.push_back(newModData);
            break;   
        }   
        case low: {
            HEvent& low_event = pm->lowevents()[m_curreventindex];
            low_event.modifiers.push_back(newModData);
            break;
        }
        case bottom: {
            BottomEvent& bot_event = pm->bottomevents()[m_curreventindex];
            bot_event.event.modifiers.push_back(newModData);
            break;
        }
        default:
            break;
    }*/
}
/*
void EventAttributesViewController::addPartialButtonClicked() {
    if (!m_currentlyShownEvent) return;
    auto* partial = m_currentlyShownEvent->getEventExtraInfo()->addPartial();
    auto* box = new SoundPartialHBox(partial, this);
    if (!m_soundPartialHboxes) m_soundPartialHboxes = box;
    else {
        auto* tail = m_soundPartialHboxes;
        while (tail->next) tail = tail->next;
        tail->next = box;
        box->prev = tail;
    }
    ui->partialsLayout->addWidget(box);
    m_soundPartialHboxes->setPartialNumber(1);
}
*/
void EventAttributesViewController::tempoAsNoteValueButtonClicked() {
    ui->tempoSecondaryStack->setCurrentWidget(ui->tempoValuePage);
    ui->emptyTempoPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->tempoFractionPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->tempoValuePage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tempoValuePage->adjustSize();
    ui->tempoSecondaryStack->adjustSize();
    // bool modified = true;  // flag from original
    // if (!modified) {
    //     ui->tempoValueEntry->setText(ui->tempoValueEntry->text());
    // } else {
    //     QString s = generateTempoStringByFraction();
    //     FunctionGenerator gen(functionReturnFloat, s.toStdString());
    //     if (gen.run() == 0)
    //         ui->tempoValueEntry->setText(QString::fromStdString(gen.getResultString()));
    // }
}

void EventAttributesViewController::tempoAsFractionButtonClicked() {
    ui->tempoSecondaryStack->setCurrentWidget(ui->tempoFractionPage);
    ui->emptyTempoPage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->tempoValuePage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->tempoFractionPage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tempoFractionPage->adjustSize();
    ui->tempoSecondaryStack->adjustSize();
    // leave text as is
}
/*
void EventAttributesViewController::tempoAsNoteValueEntryChanged(const QString&) {
    m_sharedPointers->projectView->modified();
}

void EventAttributesViewController::refreshChildTypeInLayer() {
    int idx = 0;
    for (auto* box : m_layerBoxesStorage)
        idx = box->refreshChildTypeIndex(idx);
}

bool EventAttributesViewController::deleteLayer(LayerBox* deleteBox) {
    if (m_layerBoxesStorage.size() <= 1) return false;
    ui->layersLayout->removeWidget(deleteBox);
    m_layerBoxesStorage.removeOne(deleteBox);
    delete deleteBox;
    return true;
}

IEvent* EventAttributesViewController::getCurrentEvent() const {
    return m_currentlyShownEvent;
}

bool EventAttributesViewController::checkAttackSieve() const {
    QString text = ui->attackSieveEntry->text();
    return m_sharedPointers->projectView
        ->getEventByTypeAndName(eventSiv, text.toStdString()) != nullptr;
}

bool EventAttributesViewController::checkDurationSieve() const {
    QString text = ui->durationSieveEntry->text();
    return m_sharedPointers->projectView
        ->getEventByTypeAndName(eventSiv, text.toStdString()) != nullptr;
}

void EventAttributesViewController::buildNoteModifiersList() {
    // clear old
    qDeleteAll(m_noteModifierCheckBoxes);
    m_noteModifierCheckBoxes.clear();
    // default
    auto defaults = m_sharedPointers->projectView->getDefaultNoteModifiers();
    int row = 0, col = 0;
    for (auto& kv : defaults) {
        if (!kv.second) continue;
        auto* cb = new QCheckBox(QString::fromStdString(kv.first));
        connect(cb, &QCheckBox::toggled, this, [=]{ m_sharedPointers->projectView->modified(); });
        ui->noteModifiersLayout->addWidget(cb, row, col++);
        if (col == 3) { col = 0; ++row; }
        m_noteModifierCheckBoxes.push_back(cb);
    }
    // custom
    auto customs = m_sharedPointers->projectView->getCustomNoteModifiers();
    for (auto& name : customs) {
        auto* cb = new QCheckBox(QString::fromStdString(name));
        connect(cb, &QCheckBox::toggled, this, [=]{ m_sharedPointers->projectView->modified(); });
        ui->noteModifiersLayout->addWidget(cb, row, col++);
        if (col == 3) { col = 0; ++row; }
        m_noteModifierCheckBoxes.push_back(cb);
    }
}

QString EventAttributesViewController::generateTempoStringByNoteValue() const {
    QString s;
    auto prefix = static_cast<TempoPrefix>(ui->tempoNotePrefixCombo->currentData().toInt());
    auto noteVal = static_cast<TempoNoteValue>(ui->tempoNoteValueCombo->currentData().toInt());
    switch (prefix) {
    case tempoPrefixDotted:         s += "dotted "; break;
    case tempoPrefixDoubleDotted:   s += "double dotted "; break;
    case tempoPrefixTriple:         s += "triple "; break;
    case tempoPrefixQuintuple:      s += "quintuple "; break;
    case tempoPrefixSextuple:       s += "sextuple "; break;
    case tempoPrefixSeptuple:       s += "septuple "; break;
    default: break;
    }
    switch (noteVal) {
    case tempoNoteValueWhole:        s += "whole = "; break;
    case tempoNoteValueHalf:         s += "half = "; break;
    case tempoNoteValueQuarter:      s += "quarter = "; break;
    case tempoNoteValueEighth:       s += "eighth = "; break;
    case tempoNoteValueSixteenth:    s += "sixteenth = "; break;
    case tempoNoteValueThirtySecond: s += "thirtysecond = "; break;
    }
    return s + ui->tempoValueEntry->text();
}

QString EventAttributesViewController::generateTempoStringByFraction() const {
    QString s;
    auto prefix = static_cast<TempoPrefix>(ui->tempoNotePrefixCombo->currentData().toInt());
    auto noteVal = static_cast<TempoNoteValue>(ui->tempoNoteValueCombo->currentData().toInt());
    switch (prefix) {
    case tempoPrefixDotted:         s += "dotted "; break;
    case tempoPrefixDoubleDotted:   s += "double dotted "; break;
    case tempoPrefixTriple:         s += "triple "; break;
    case tempoPrefixQuintuple:      s += "quintuple "; break;
    case tempoPrefixSextuple:       s += "sextuple "; break;
    case tempoPrefixSeptuple:       s += "septuple "; break;
    default: break;
    }
    switch (noteVal) {
    case tempoNoteValueWhole:        s += "whole = "; break;
    case tempoNoteValueHalf:         s += "half = "; break;
    case tempoNoteValueQuarter:      s += "quarter = "; break;
    case tempoNoteValueEighth:       s += "eighth = "; break;
    case tempoNoteValueSixteenth:    s += "sixteenth = "; break;
    case tempoNoteValueThirtySecond: s += "thirtysecond = "; break;
    }
    // convert fraction to denominator notation
    bool ok;
    int num = ui->tempoFractionEntry1->text().toInt(&ok);
    int den = ui->tempoValueEntry->text().toInt(&ok);
    return s + QString::number(num) + "/" + QString::number(den);
}

void EventAttributesViewController::deleteKeyPressed(QWidget* focus) {
    for (auto* box : m_layerBoxesStorage) {
        if (box->isAncestorOf(focus)) {
            box->deleteObject();
            break;
        }
    }
}*/
