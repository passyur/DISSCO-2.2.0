#include "EventAttributesViewController.hpp"
#include "../ui/ui_Attributes.h"

#include "FunctionGenerator.hpp"
#include "ProjectViewController.hpp"
//#include "PaletteViewController.h"
//#include "PartialWindow.h"

#include <QMessageBox>
#include <QKeyEvent>

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
            this, &EventAttributesViewController::addNewLayerButtonClicked);
    connect(ui->addModifierButton, &QPushButton::clicked,
            this, &EventAttributesViewController::addModifierButtonClicked);
    connect(ui->addPartialButton, &QPushButton::clicked,
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

    LayerBox* box = new LayerBox(this, e_projectView);
    ui->layersLayout->addWidget(box);
}

EventAttributesViewController::~EventAttributesViewController() {
    // Qt will delete child widgets automatically
}

LayerBox::LayerBox(EventAttributesViewController* parentController,
                   ProjectView* projectView
                   //EventLayer* layerModel,
                    // bool showDiscreteColumns
                   )
    : QFrame(nullptr),
      m_attributesView(parentController),
      m_projectView(projectView)
     // m_layerModel(layerModel)
{
    m_mainLayout = new QVBoxLayout();
    this->setLayout(m_mainLayout);
    this->setFrameShape(QFrame::StyledPanel);
    this->setLineWidth(1);

    auto* weightHBox = new QHBoxLayout;
    auto* weightLabel = new QLabel("Number of children in this layer:");
    m_weightEntry = new QLineEdit;
    m_weightFuncButton = new QPushButton("Insert Function");
    m_deleteLayerButton = new QPushButton("Delete This Layer");

    connect(m_weightFuncButton, &QPushButton::clicked,
            this, &LayerBox::onWeightFunctionClicked);
    connect(m_deleteLayerButton, &QPushButton::clicked,
            this, &LayerBox::onDeleteLayerClicked);

    weightHBox->addWidget(weightLabel);
    weightHBox->addWidget(m_weightEntry);
    weightHBox->addWidget(m_weightFuncButton);
    weightHBox->addWidget(m_deleteLayerButton);

    m_mainLayout->addLayout(weightHBox);

    m_treeView = new QTreeView;
    m_model = new QStandardItemModel;
    m_model->setHorizontalHeaderLabels({"Child Type", "Class", "Name"});
    m_treeView->setModel(m_model);
    m_treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_mainLayout->addWidget(m_treeView);

    this->setMinimumHeight(200);
}

LayerBox::~LayerBox() {}

void EventAttributesViewController::showAttributesOfEvent(Eventtype event) { // (IEvent* event)
    // if (!event) {
    //     m_currentlyShownEvent = nullptr;
    //     ui->stackedWidget->setCurrentWidget(ui->emptyPage);
    //     // clear dynamic widgets
    //     ui->layersLayout->deleteLater();
    //     ui->modifiersLayout->deleteLater();
    //     ui->partialsLayout->deleteLater();
    //     return;
    // }

    // saveCurrentShownEventData();
    // m_currentlyShownEvent = event;
    m_currentlyShownEvent = event;
    showCurrentEventData();
}
/*
void EventAttributesViewController::saveCurrentShownEventData() {
    if (!m_currentlyShownEvent) return;

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
    }
}
*/
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

    // choose page
    // int type = m_currentlyShownEvent->getEventType();
    int type = m_currentlyShownEvent;
    switch (type) {
    case top: case high: case mid: case low: case bottom: {
        ui->stackedWidget->setCurrentWidget(ui->standardPage);
        if (type == bottom) {  
            ui->frequencyContainer->setVisible(true);
            ui->loudnessContainer->setVisible(true);
            ui->modGroupContainer->setVisible(true);
        }
        else {
            ui->frequencyContainer->setVisible(false);
            ui->loudnessContainer->setVisible(false);
            ui->modGroupContainer->setVisible(false);
        }
        break;
    }
    case sound:
        ui->stackedWidget->setCurrentWidget(ui->soundPage);
        break;
    case env:
        ui->stackedWidget->setCurrentWidget(ui->envPage);
        break;
    case sieve:
        ui->stackedWidget->setCurrentWidget(ui->sievePage);
        break;
    case spa:
        ui->stackedWidget->setCurrentWidget(ui->spaPage);
        break;
    case pattern:
        ui->stackedWidget->setCurrentWidget(ui->patPage);
        break;
    case reverb:
        ui->stackedWidget->setCurrentWidget(ui->revPage);
        break;
    case filter:
        ui->stackedWidget->setCurrentWidget(ui->filPage);
        break;
    case mea:
        ui->stackedWidget->setCurrentWidget(ui->meaPage);
        break;
    case note:
        ui->stackedWidget->setCurrentWidget(ui->notePage);
        break;
    default:
        ui->stackedWidget->setCurrentWidget(ui->emptyPage);
    }

    /*
    // populate fields
    ui->nameEntry->setText(QString::fromStdString(m_currentlyShownEvent->getEventName()));

    if (type <= eventBottom) {
        ui->maxChildDurEntry->setText(QString::fromStdString(m_currentlyShownEvent->getMaxChildDur()));
        ui->timeSig1Entry->setText(QString::fromStdString(m_currentlyShownEvent->getTimeSignatureEntry1()));
        ui->timeSig2Entry->setText(QString::fromStdString(m_currentlyShownEvent->getTimeSignatureEntry2()));
        ui->unitsPerSecondEntry->setText(QString::fromStdString(m_currentlyShownEvent->getUnitsPerSecond()));

        // tempo
        bool asNote = (m_currentlyShownEvent->getTempoMethodFlag() == 0);
        ui->tempoAsNoteValueRadio->setChecked(asNote);
        ui->tempoAsFractionRadio->setChecked(!asNote);
        ui->tempoNotePrefixCombo->setCurrentIndex(static_cast<int>(m_currentlyShownEvent->getTempoPrefix()));
        ui->tempoNoteValueCombo->setCurrentIndex(static_cast<int>(m_currentlyShownEvent->getTempoNoteValue()));
        ui->tempoFractionEntry1->setText(QString::fromStdString(m_currentlyShownEvent->getTempoFractionEntry1()));
        ui->tempoValueEntry->setText(QString::fromStdString(m_currentlyShownEvent->getTempoValueEntry()));

        // num children
        int flag = m_currentlyShownEvent->getFlagNumChildren();
        ui->fixedButton->setChecked(flag == 0);
        ui->densityButton->setChecked(flag == 1);
        ui->byLayerButton->setChecked(flag == 2);
        ui->childCountEntry1->setText(QString::fromStdString(m_currentlyShownEvent->getNumChildrenEntry1()));
        ui->childCountEntry2->setText(QString::fromStdString(m_currentlyShownEvent->getNumChildrenEntry2()));
        ui->childCountEntry3->setText(QString::fromStdString(m_currentlyShownEvent->getNumChildrenEntry3()));

        // build layers
        for (auto* layer : m_currentlyShownEvent->layers) {
            LayerBox* box = new LayerBox(this, m_sharedPointers->projectView, layer,
                                         (flag==2));
            ui->layersLayout->addWidget(box);
            m_layerBoxesStorage.push_back(box);
        }
        refreshChildTypeInLayer();

        // child‐event‐def
        int ced = m_currentlyShownEvent->getFlagChildEventDef();
        ui->continuumButton->setChecked(ced == 0);
        ui->sweepButton->setChecked(ced == 1);
        ui->discreteButton->setChecked(ced == 2);
        ui->childDefEntry1->setText(QString::fromStdString(m_currentlyShownEvent->getChildEventDefEntry1()));
        ui->childDefEntry2->setText(QString::fromStdString(m_currentlyShownEvent->getChildEventDefEntry2()));
        ui->childDefEntry3->setText(QString::fromStdString(m_currentlyShownEvent->getChildEventDefEntry3()));
        ui->attackSieveEntry->setText(QString::fromStdString(m_currentlyShownEvent->getChildEventDefAttackSieve()));
        ui->durationSieveEntry->setText(QString::fromStdString(m_currentlyShownEvent->getChildEventDefDurationSieve()));
        int st = m_currentlyShownEvent->getFlagChildEventDefStartType();
        ui->startTypePercentRadio->setChecked(st==0);
        ui->startTypeUnitsRadio->setChecked(st==1);
        ui->startTypeSecondsRadio->setChecked(st==2);
        int dt = m_currentlyShownEvent->getFlagChildEventDefDurationType();
        ui->durationTypePercentRadio->setChecked(dt==0);
        ui->durationTypeUnitsRadio->setChecked(dt==1);
        ui->durationTypeSecondsRadio->setChecked(dt==2);

        // environment
        if (type != eventBottom) {
            ui->spaEntry->setText(QString::fromStdString(m_currentlyShownEvent->getSpa()));
            ui->revEntry->setText(QString::fromStdString(m_currentlyShownEvent->getRev()));
            ui->filEntry->setText(QString::fromStdString(m_currentlyShownEvent->getFil()));
        }

        // bottom‐extra
        if (type == eventBottom) {
            auto* extra = m_currentlyShownEvent->getEventExtraInfo();
            ui->wellTemperedRadio->setChecked(extra->getFrequencyFlag()==0);
            ui->fundamentalRadio->setChecked(extra->getFrequencyFlag()==1);
            ui->contFreqRadio->setChecked(extra->getFrequencyFlag()==2);
            ui->wellTemperedEntry->setText(QString::fromStdString(extra->getFrequencyEntry1()));
            ui->funFreqEntry1->setText(QString::fromStdString(extra->getFrequencyEntry1()));
            ui->funFreqEntry2->setText(QString::fromStdString(extra->getFrequencyEntry2()));
            ui->contFreqEntry->setText(QString::fromStdString(extra->getFrequencyEntry1()));
            ui->hertzRadio->setChecked(extra->getFrequencyContinuumFlag()==0);
            ui->powerOfTwoRadio->setChecked(extra->getFrequencyContinuumFlag()==1);
            ui->loudnessEntry->setText(QString::fromStdString(extra->getLoudness()));
            ui->spatializationEntry->setText(QString::fromStdString(extra->getSpatialization()));
            ui->reverbEntry->setText(QString::fromStdString(extra->getReverb()));
            ui->filterEntry->setText(QString::fromStdString(extra->getFilter()));
            ui->modifierGroupEntry->setText(QString::fromStdString(extra->getModifierGroup()));

            // modifiers
            if (ui->childTypeSoundRadio->isChecked()) {
                auto* em = extra->getModifiers();
                while (em) {
                    auto* align = new EventBottomModifierAlignment(em, this);
                    if (!m_modifiers) m_modifiers = align;
                    else {
                        auto* tail = m_modifiers;
                        while (tail->next) tail = tail->next;
                        tail->next = align;
                        align->prev = tail;
                    }
                    ui.modifiersLayout->addWidget(align);
                    em = em->next;
                }
            }
        }
    }
    // Sound event
    else if (type == eventSound) {
        auto* extra = m_currentlyShownEvent->getEventExtraInfo();
        ui->spectrumNumPartialEntry->setText(QString::fromStdString(extra->getNumPartials()));
        ui->spectrumDeviationEntry->setText(QString::fromStdString(extra->getDeviation()));
        ui->spectrumGenEntry->setText(QString::fromStdString(extra->getSpectrumGenBuilder()));
        auto* sp = extra->getSpectrumPartials();
        if (sp) {
            m_soundPartialHboxes = new SoundPartialHBox(sp, this);
            ui.partialsLayout->addWidget(m_soundPartialHboxes);
            sp = sp->next;
            while (sp) {
                auto* nextBox = new SoundPartialHBox(sp, this);
                m_soundPartialHboxes->next = nextBox;
                nextBox->prev = m_soundPartialHboxes;
                ui->partialsLayout->addWidget(nextBox);
                sp = sp->next;
            }
            m_soundPartialHboxes->setPartialNumber(1);
        }
    }

    // envelope, sieve, spa, pat, rev, fil, mea, note
    else {
        auto* extra = m_currentlyShownEvent->getEventExtraInfo();
        if (type == eventEnv) {
            ui->envNameEntry->setText(QString::fromStdString(m_currentlyShownEvent->getEventName()));
            ui->envBuilderEntry->setText(QString::fromStdString(extra->getEnvelopeBuilder()));
        }
        else if (type == eventSiv) {
            ui->sieveNameEntry->setText(QString::fromStdString(m_currentlyShownEvent->getEventName()));
            ui->sieveBuilderEntry->setText(QString::fromStdString(extra->getSieveBuilder()));
        }
        else if (type == eventSpa) {
            ui->spaNameEntry->setText(QString::fromStdString(m_currentlyShownEvent->getEventName()));
            ui->spaBuilderEntry->setText(QString::fromStdString(extra->getSpatializationBuilder()));
        }
        else if (type == eventPat) {
            ui->patNameEntry->setText(QString::fromStdString(m_currentlyShownEvent->getEventName()));
            ui->patBuilderEntry->setText(QString::fromStdString(extra->getPatternBuilder()));
        }
        else if (type == eventRev) {
            ui->revNameEntry->setText(QString::fromStdString(m_currentlyShownEvent->getEventName()));
            ui->revBuilderEntry->setText(QString::fromStdString(extra->getReverbBuilder()));
        }
        else if (type == eventFil) {
            ui->filNameEntry->setText(QString::fromStdString(m_currentlyShownEvent->getEventName()));
            ui->filBuilderEntry->setText(QString::fromStdString(extra->getFilterBuilder()));
        }
        else if (type == eventMea) {
            ui->meaNameEntry->setText(QString::fromStdString(m_currentlyShownEvent->getEventName()));
            ui->meaBuilderEntry->setText(QString::fromStdString(extra->getMeasureBuilder()));
        }
        else if (type == eventNote) {
            ui->noteNameEntry->setText(QString::fromStdString(m_currentlyShownEvent->getEventName()));
            ui->staffNumberEntry->setText(QString::fromStdString(extra->getStaffNum()));
            buildNoteModifiersList();
        }
    }*/
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

void EventAttributesViewController::addModifierButtonClicked() {
    if (!m_currentlyShownEvent) return;
    EventBottomModifier* mod = (m_currentlyShownEvent->getEventType()==eventBottom)
        ? m_currentlyShownEvent->getEventExtraInfo()->addModifier()
        : m_currentlyShownEvent->addModifier();

    auto* align = new EventBottomModifierAlignment(mod, this);
    if (!m_modifiers) m_modifiers = align;
    else {
        auto* tail = m_modifiers;
        while (tail->next) tail = tail->next;
        tail->next = align;
        align->prev = tail;
    }
    ui->modifiersLayout->addWidget(align);
}

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
