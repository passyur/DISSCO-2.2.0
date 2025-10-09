#include "FunctionGenerator.hpp"
#include "../ui/ui_FunctionGenerator.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QDialogButtonBox>
// #include <QOverload>
#include <QDateTime>
#include <QScrollBar>

#include <string>

#include "../widgets/SPAPartialAlignment.hpp"
#include "../ui/ui_FunGenSPAPartialAlignment.h"
#include "../widgets/REVPartialAlignment.hpp"
#include "../ui/ui_FunGenREVPartialAlignment.h"


FunctionGenerator::FunctionGenerator(QWidget *parent, FunctionReturnType _returnType, QString _originalString)
    : QDialog(parent)
    , ui(new Ui::FunctionGenerator)
{
    // Sets up the user interface specified by the .ui file
    result = "";
    returnType = _returnType;
    originalString = _originalString;
    ui->setupUi(this);
    ui->functionStackedWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
    ui->functionStackedWidget->setFixedSize(ui->functionStackedWidget->widget(0)->sizeHint());
    this->adjustSize();
    setupUi();
    qDebug() << "returnType in FuncGen:" << returnType;
}

FunctionGenerator::~FunctionGenerator()
{
    delete ui;
}

void FunctionGenerator::setupUi()
{
    ui->functionOptions->clear();
    // Add combo box function options
    if (returnType == functionReturnInt){
        ui->functionOptions->addItem("", NOT_A_FUNCTION); // index 0
        ui->functionOptions->addItem("RandomInt", functionRandomInt); // index 2
        ui->functionOptions->addItem("RandomOrderInt", functionRandomOrderInt); // index 3
        ui->functionOptions->addItem("Stochos", functionStochos); // index 11
        ui->functionOptions->addItem("Select", functionSelect); // index 12
        ui->functionOptions->addItem("ValuePick", functionValuePick); // index 13
        ui->functionOptions->addItem("ChooseL", functionChooseL); // index 14
        ui->functionOptions->addItem("GetPattern", functionGetPattern); // index 15
        ui->functionOptions->addItem("GetFromMarkovChain", functionMarkov); // index 10
        ui->functionOptions->addItem("Fibonacci", functionFibonacci); // index 8
        ui->functionOptions->addItem("-------", NOT_A_FUNCTION); // index 0
        ui->functionOptions->addItem("CURRENT_TYPE", function_staticCURRENT_TYPE); // index 0
        ui->functionOptions->addItem("CURRENT_CHILD_NUM", function_staticCURRENT_CHILD_NUM); // index 0
        ui->functionOptions->addItem("CURRENT_PARTIAL_NUM", function_staticCURRENT_PARTIAL_NUM); // index 0
        ui->functionOptions->addItem("CURRENT_SEGMENT", function_staticCURRENT_SEGMENT); // index 0
        ui->functionOptions->addItem("AVAILABLE_EDU", function_staticAVAILABLE_EDU); // index 0
        ui->functionOptions->addItem("PREVIOUS_CHILD_DURATION", function_staticPREVIOUS_CHILD_DURATION); // index 0
        ui->functionOptions->addItem("CURRENT_LAYER", function_staticCURRENT_LAYER); // index 0
    }
    else if (returnType == functionReturnFloat){
        ui->functionOptions->addItem("", NOT_A_FUNCTION); // index 0
        ui->functionOptions->addItem("Random", functionRandom); // index 1
        ui->functionOptions->addItem("RandomInt", functionRandomInt); // index 2
        ui->functionOptions->addItem("RandomOrderInt", functionRandomOrderInt); // index 3
        ui->functionOptions->addItem("Randomizer", functionRandomizer); // index 4
        ui->functionOptions->addItem("RandomDensity", functionRandomDensity); // index 5
        ui->functionOptions->addItem("Inverse", functionInverse); // index 6
        ui->functionOptions->addItem("LN", functionLN); // index 7
        ui->functionOptions->addItem("Fibonacci", functionFibonacci); // index 8
        ui->functionOptions->addItem("Decay", functionDecay); // index 9
        ui->functionOptions->addItem("GetFromMarkovChain", functionMarkov); // index 10
        ui->functionOptions->addItem("-------", NOT_A_FUNCTION); // index 0
        ui->functionOptions->addItem("Stochos", functionStochos); // index 11
        ui->functionOptions->addItem("Select", functionSelect); // index 12
        ui->functionOptions->addItem("ValuePick", functionValuePick); // index 13
        ui->functionOptions->addItem("ChooseL", functionChooseL); // index 14
        ui->functionOptions->addItem("GetPattern", functionGetPattern); // index 15
        ui->functionOptions->addItem("-------", NOT_A_FUNCTION);  // index 0
        ui->functionOptions->addItem("CURRENT_TYPE", function_staticCURRENT_TYPE); // index 0
        ui->functionOptions->addItem("CURRENT_CHILD_NUM", function_staticCURRENT_CHILD_NUM); // index 0
        ui->functionOptions->addItem("CURRENT_PARTIAL_NUM", function_staticCURRENT_PARTIAL_NUM); // index 0
        ui->functionOptions->addItem("CURRENT_SEGMENT", function_staticCURRENT_SEGMENT); // index 0
        ui->functionOptions->addItem("AVAILABLE_EDU", function_staticAVAILABLE_EDU); // index 0
        ui->functionOptions->addItem("PREVIOUS_CHILD_DURATION", function_staticPREVIOUS_CHILD_DURATION); // index 0
        ui->functionOptions->addItem("CURRENT_LAYER", function_staticCURRENT_LAYER); // index 0
    } else if (returnType == functionReturnMakeListFun) {
        ui->functionOptions->addItem("", NOT_A_FUNCTION); // index 0
        ui->functionOptions->addItem("Random", functionRandom); // index 1
        ui->functionOptions->addItem("RandomInt", functionRandomInt); // index 2
        ui->functionOptions->addItem("RandomOrderInt", functionRandomOrderInt); // index 3
        ui->functionOptions->addItem("Randomizer", functionRandomizer); // index 4
        ui->functionOptions->addItem("RandomDensity", functionRandomDensity); // index 5
        ui->functionOptions->addItem("Inverse", functionInverse); // index 6
        ui->functionOptions->addItem("LN", functionLN); // index 7
        ui->functionOptions->addItem("Fibonacci", functionFibonacci); // index 8
        ui->functionOptions->addItem("Decay", functionDecay); // index 9
        ui->functionOptions->addItem("-------", NOT_A_FUNCTION); // index 0
        ui->functionOptions->addItem("Stochos", functionStochos); // index 11
        ui->functionOptions->addItem("Select", functionSelect); // index 12
        ui->functionOptions->addItem("ValuePick", functionValuePick); // index 13
        ui->functionOptions->addItem("ChooseL", functionChooseL); // index 14
        ui->functionOptions->addItem("GetPattern", functionGetPattern); // index 15
        ui->functionOptions->addItem("EnvLib", functionEnvLib); // index 16
        ui->functionOptions->addItem("MakeEnvelope", functionMakeEnvelope); // index 17
        ui->functionOptions->addItem("ReadENVFile", functionReadENVFile); // index 18
        ui->functionOptions->addItem("-------", NOT_A_FUNCTION);  // index 0
        ui->functionOptions->addItem("CURRENT_TYPE", function_staticCURRENT_TYPE); // index 0
        ui->functionOptions->addItem("CURRENT_CHILD_NUM", function_staticCURRENT_CHILD_NUM); // index 0
        ui->functionOptions->addItem("CURRENT_PARTIAL_NUM", function_staticCURRENT_PARTIAL_NUM); // index 0
        ui->functionOptions->addItem("CURRENT_SEGMENT", function_staticCURRENT_SEGMENT); // index 0
        ui->functionOptions->addItem("AVAILABLE_EDU", function_staticAVAILABLE_EDU); // index 0
        ui->functionOptions->addItem("PREVIOUS_CHILD_DURATION", function_staticPREVIOUS_CHILD_DURATION); // index 0
        ui->functionOptions->addItem("CURRENT_LAYER", function_staticCURRENT_LAYER); // index 0
    } else if (returnType == functionReturnList) {
        ui->functionOptions->addItem("", NOT_A_FUNCTION);
        ui->functionOptions->addItem("MakeList", functionMakeList); // index 19
        ui->functionOptions->addItem("RawList", functionRawList); // index 0
    } else if (returnType == functionReturnENV) {
        ui->functionOptions->addItem("", NOT_A_FUNCTION); // index 0
        ui->functionOptions->addItem("MakeEnvelope", functionMakeEnvelope); // index 17
        ui->functionOptions->addItem("ReadENVFile", functionReadENVFile); // index 18
        ui->functionOptions->addItem("EnvLib", functionEnvLib); // index 16
        ui->functionOptions->addItem("Select", functionSelect); // index 12
    } else if (returnType == functionReturnSPA){
        ui->functionOptions->addItem("", NOT_A_FUNCTION); // index 0
        ui->functionOptions->addItem("SPA", functionSPA); // index 20
        ui->functionOptions->addItem("ReadSPAFile", functionReadSPAFile); // index 21
        ui->functionOptions->addItem("Select", functionSelect); // index 12
    } else if (returnType == functionReturnFIL){
        ui->functionOptions->addItem("", NOT_A_FUNCTION); // index 0
        ui->functionOptions->addItem("MakeFilter", functionMakeFilter); // index 22
        ui->functionOptions->addItem("ReadFILFile", functionReadFILFile); // index 23
        ui->functionOptions->addItem("Select", functionSelect); // index 12
    } else if (returnType == functionReturnMGP){
        ui->functionOptions->addItem("", NOT_A_FUNCTION); // index 0
        ui->functionOptions->addItem("Select", functionSelect); // index 12
    } else if (returnType == functionReturnPAT){
        ui->functionOptions->addItem("", NOT_A_FUNCTION); // index 0
        ui->functionOptions->addItem("MakePattern", functionMakePattern); // index 24
        ui->functionOptions->addItem("ExpandPattern", functionExpandPattern); // index 25
        ui->functionOptions->addItem("ReadPATFile", functionReadPATFile); // index 26
        ui->functionOptions->addItem("Select", functionSelect); // index 12
    } else if (returnType == functionReturnREV){
        ui->functionOptions->addItem("", NOT_A_FUNCTION); // index 0
        ui->functionOptions->addItem("REV_Simple", functionREV_Simple); // index 27
        ui->functionOptions->addItem("REV_Medium", functionREV_Medium); // index 27
        ui->functionOptions->addItem("REV_Advanced", functionREV_Advanced); // index 27
        ui->functionOptions->addItem("ReadREVFile", functionReadREVFile); // index 28
        ui->functionOptions->addItem("Select", functionSelect); // index 12
    } else if (returnType == functionReturnSPE){
        ui->functionOptions->addItem("", NOT_A_FUNCTION); // index 0
        ui->functionOptions->addItem("Generate_Spectrum", functionSpectrum_Gen); // index 29
        ui->functionOptions->addItem("Select", functionSelect); // index 12
    } else if (returnType == functionReturnSIV){
        ui->functionOptions->addItem("", NOT_A_FUNCTION); // index 0
        ui->functionOptions->addItem("MakeSieve", functionMakeSieve); // index 30
        ui->functionOptions->addItem("ReadSIVFile", functionReadSIVFile); // index 31
        ui->functionOptions->addItem("Select", functionSelect); // index 12
    }

    // Handles combo box option selected by calling handleFunctionChanged
    connect(ui->functionOptions, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &FunctionGenerator::handleFunctionChanged);
    ui->functionOptions->setCurrentIndex(0);

    // Centers result label
    ui->resultLabel->setAlignment(Qt::AlignCenter);
    // Sets height of result box
    ui->resultTextEdit->setFixedHeight(100);
    // Connects the ui OK and CANCEL buttons to their corresponding actions
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Random signals
    connect(ui->randomLowerBoundInsertFn, &QPushButton::clicked, this, &FunctionGenerator::randomLowBoundFunButtonClicked);
    connect(ui->randomUpperBoundInsertFn, &QPushButton::clicked, this, &FunctionGenerator::randomHighBoundFunButtonClicked);
    connect(ui->randomLowerBoundEdit, &QLineEdit::textChanged, this, &FunctionGenerator::randomEntryChanged);
    connect(ui->randomUpperBoundEdit, &QLineEdit::textChanged, this, &FunctionGenerator::randomEntryChanged);

    // RandomInt signals
    connect(ui->randomIntLowerBoundInsertFn, &QPushButton::clicked, this, &FunctionGenerator::randomIntLowBoundFunButtonClicked);
    connect(ui->randomIntUpperBoundInsertFn, &QPushButton::clicked, this, &FunctionGenerator::randomIntHighBoundFunButtonClicked);
    connect(ui->randomIntLowerBoundEdit, &QLineEdit::textChanged, this, &FunctionGenerator::randomIntEntryChanged);
    connect(ui->randomIntUpperBoundEdit, &QLineEdit::textChanged, this, &FunctionGenerator::randomIntEntryChanged);

    // RandomOrderInt signals
    connect(ui->randomOrderIntLowerBoundInsertFn, &QPushButton::clicked, this, &FunctionGenerator::randomOrderIntLowBoundFunButtonClicked);
    connect(ui->randomOrderIntUpperBoundInsertFn, &QPushButton::clicked, this, &FunctionGenerator::randomOrderIntHighBoundFunButtonClicked);
    connect(ui->randomOrderIntLowerBoundEdit, &QLineEdit::textChanged, this, &FunctionGenerator::randomOrderIntEntryChanged);
    connect(ui->randomOrderIntUpperBoundEdit, &QLineEdit::textChanged, this, &FunctionGenerator::randomOrderIntEntryChanged);

    // Randomizer signals
    connect(ui->randomizerBaseInsertFn, &QPushButton::clicked, this, &FunctionGenerator::randomizerBaseFunButtonClicked);
    connect(ui->randomizerDeviationInsertFn, &QPushButton::clicked, this, &FunctionGenerator::randomizerDeviationFunButtonClicked);
    connect(ui->randomizerBaseEdit, &QLineEdit::textChanged, this, &FunctionGenerator::randomizerEntryChanged);
    connect(ui->randomizerDeviationEdit, &QLineEdit::textChanged, this, &FunctionGenerator::randomizerEntryChanged);

    // RandomDensity signals
    connect(ui->randomDensityEnvEdit, &QLineEdit::textChanged, this, &FunctionGenerator::randomDensityEntryChanged);
    connect(ui->randomDensityStartTimeEdit, &QLineEdit::textChanged, this, &FunctionGenerator::randomDensityEntryChanged);
    connect(ui->randomDensityEndTimeEdit, &QLineEdit::textChanged, this, &FunctionGenerator::randomDensityEntryChanged);

    // Decay signals
    connect(ui->decayBaseInsertFn, &QPushButton::clicked, this, &FunctionGenerator::decayBaseFunButtonClicked);
    connect(ui->decayRateInsertFn, &QPushButton::clicked, this, &FunctionGenerator::decayRateFunButtonClicked);
    connect(ui->decayIndexInsertFn, &QPushButton::clicked, this, &FunctionGenerator::decayIndexFunButtonClicked);
    connect(ui->decayBaseEdit, &QLineEdit::textChanged, this, &FunctionGenerator::decayEntryChanged);
    connect(ui->decayRateEdit, &QLineEdit::textChanged, this, &FunctionGenerator::decayEntryChanged);
    connect(ui->decayIndexEdit, &QLineEdit::textChanged, this, &FunctionGenerator::decayEntryChanged);
    connect(ui->decayTypeExponential, &QRadioButton::toggled, this, &FunctionGenerator::decayEntryChanged);
    connect(ui->decayTypeLinear, &QRadioButton::toggled, this, &FunctionGenerator::decayEntryChanged);

    // Inverse Signals
    connect(ui->numToInverseInsertFn, &QPushButton::clicked, this, &FunctionGenerator::inverseFunButtonClicked);
    connect(ui->numToInverseEdit, &QLineEdit::textChanged, this, &FunctionGenerator::inverseEntryChanged);

    // Markov Signals
    connect(ui->markovEdit, &QLineEdit::textChanged, this, &FunctionGenerator::markovEntryChanged);

    // LN Signals
    connect(ui->lnInsertFn, &QPushButton::clicked, this, &FunctionGenerator::LNFunButtonClicked);
    connect(ui->lnEdit, &QLineEdit::textChanged, this, &FunctionGenerator::LNEntryChanged);

    // Fibonacci Signals
    connect(ui->fibInsertFn, &QPushButton::clicked, this, &FunctionGenerator::FibonacciFunButtonClicked);
    connect(ui->fibEdit, &QLineEdit::textChanged, this, &FunctionGenerator::FibonacciEntryChanged);

    // Select Signals
    connect(ui->selectIndexInsertFn, &QPushButton::clicked, this, &FunctionGenerator::selectIndexFunButtonClicked);
    connect(ui->selectIndexEdit, &QLineEdit::textChanged, this, &FunctionGenerator::selectEntryChanged);
    // connect(ui->selectAddNode, &QPushButton::clicked, this, &FunctionGenerator::selectAddNodeButtonClicked);
    // connect(ui->selectInsertFn, &QPushButton::clicked, this, &FunctionGenerator::selectFunButtonClicked);

    // Stochos Signals
    connect(ui->stochosOffsetEdit, &QLineEdit::textChanged, this, &FunctionGenerator::stochosTextChanged);
    connect(ui->stochosMethodRange, &QRadioButton::toggled, this, &FunctionGenerator::stochosTextChanged);
    connect(ui->stochosMethodFunction, &QRadioButton::toggled, this, &FunctionGenerator::stochosTextChanged);
    // connect(ui->stochosAddNode, &QPushButton::clicked, this, &FunctionGenerator::stochosAddNodeButtonClicked);
    // connect(ui->stochosInsertFn, &QPushButton::clicked, this, &FunctionGenerator::stochosFunButtonClicked);

    // ValuePick Signals
    connect(ui->valuePickElementsMeaningful, &QRadioButton::toggled, this, &FunctionGenerator::valuePickTextChanged);
    connect(ui->valuePickElementsMods, &QRadioButton::toggled, this, &FunctionGenerator::valuePickTextChanged);
    connect(ui->valuePickElementsFake, &QRadioButton::toggled, this, &FunctionGenerator::valuePickTextChanged);
    connect(ui->valuePickElementsFib, &QRadioButton::toggled, this, &FunctionGenerator::valuePickTextChanged);
    connect(ui->valuePickWeightsPeriodic, &QRadioButton::toggled, this, &FunctionGenerator::valuePickTextChanged);
    connect(ui->valuePickWeightsHierarchic, &QRadioButton::toggled, this, &FunctionGenerator::valuePickTextChanged);
    connect(ui->valuePickWeightsInclude, &QRadioButton::toggled, this, &FunctionGenerator::valuePickTextChanged);
    connect(ui->valuePickTypeVariable, &QRadioButton::toggled, this, &FunctionGenerator::valuePickTextChanged);
    connect(ui->valuePickTypeConstant, &QRadioButton::toggled, this, &FunctionGenerator::valuePickTextChanged);

    connect(ui->valuePickAbsRangeEdit, &QLineEdit::textChanged, this, &FunctionGenerator::valuePickTextChanged);
    connect(ui->valuePickLowEdit, &QLineEdit::textChanged, this, &FunctionGenerator::valuePickTextChanged);
    connect(ui->valuePickHighEdit, &QLineEdit::textChanged, this, &FunctionGenerator::valuePickTextChanged);
    connect(ui->valuePickDistEdit, &QLineEdit::textChanged, this, &FunctionGenerator::valuePickTextChanged);

    connect(ui->valuePickValuesElementsEdit, &QLineEdit::textChanged, this, &FunctionGenerator::valuePickTextChanged);
    connect(ui->valuePickOffsetEdit, &QLineEdit::textChanged, this, &FunctionGenerator::valuePickTextChanged);
    connect(ui->valuePickValuesWeightsEdit, &QLineEdit::textChanged, this, &FunctionGenerator::valuePickTextChanged);

    connect(ui->valuePickAbsRangeInsertFn, &QPushButton::clicked, this, &FunctionGenerator::valuePickAbsRangeFunButtonClicked);
    connect(ui->valuePickLowInsertFn, &QPushButton::clicked, this, &FunctionGenerator::valuePickLowFunButtonClicked);
    connect(ui->valuePickHighInsertFn, &QPushButton::clicked, this, &FunctionGenerator::valuePickHighFunButtonClicked);
    connect(ui->valuePickDistInsertFn, &QPushButton::clicked, this, &FunctionGenerator::valuePickDistFunButtonClicked);

    // ChooseL Signals
    connect(ui->chooseLInsertFn, &QPushButton::clicked, this, &FunctionGenerator::chooseLFunButtonClicked);
    connect(ui->chooseLEdit, &QLineEdit::textChanged, this, &FunctionGenerator::chooseLEntryChanged);

    // GetPattern Signals
    connect(ui->getPatternMethodInOrder, &QRadioButton::toggled, this, &FunctionGenerator::getPatternEntryChanged);
    connect(ui->getPatternMethodOther, &QRadioButton::toggled, this, &FunctionGenerator::getPatternEntryChanged);
    connect(ui->getPatternMethodTypeClusters, &QRadioButton::toggled, this, &FunctionGenerator::getPatternEntryChanged);
    connect(ui->getPatternMethodTimeDepend, &QRadioButton::toggled, this, &FunctionGenerator::getPatternEntryChanged);
    connect(ui->getPatternMethodProbability, &QRadioButton::toggled, this, &FunctionGenerator::getPatternEntryChanged);

    connect(ui->getPatternOriginInsertFn, &QPushButton::clicked, this, &FunctionGenerator::getPatternOffsetFunButtonClicked);
    connect(ui->getPatternChooseInsertFn, &QPushButton::clicked, this, &FunctionGenerator::getPatternFunButtonClicked);

    connect(ui->getPatternOriginEdit, &QLineEdit::textChanged, this, &FunctionGenerator::getPatternEntryChanged);
    connect(ui->getPatternChooseEdit, &QLineEdit::textChanged, this, &FunctionGenerator::getPatternEntryChanged);

    // MakeList Signals
    connect(ui->makeListFunctionInsertFn, &QPushButton::clicked, this, &FunctionGenerator::makeListFunctionFunButtonClicked);
    connect(ui->makeListSizeInsertFn, &QPushButton::clicked, this, &FunctionGenerator::makeListSizeFunButtonClicked);
    connect(ui->makeListFunctionEdit, &QLineEdit::textChanged, this, &FunctionGenerator::makeListTextChanged);
    connect(ui->makeListSizeEdit, &QLineEdit::textChanged, this, &FunctionGenerator::makeListTextChanged);

    // EnvLib Signals
    connect(ui->envLibNumInsertFn, &QPushButton::clicked, this, &FunctionGenerator::envLibEnvelopeFunButtonClicked);
    connect(ui->envLibScalingInsertFn, &QPushButton::clicked, this, &FunctionGenerator::envLibScalingFactorFunButtonClicked);
    connect(ui->envLibNumEdit, &QLineEdit::textChanged, this, &FunctionGenerator::envLibTextChanged);
    connect(ui->envLibScalingEdit, &QLineEdit::textChanged, this, &FunctionGenerator::envLibTextChanged);

    // ReadENVFile Signals
    connect(ui->readEnvFileEdit, &QLineEdit::textChanged, this, &FunctionGenerator::readENVFileTextChanged);

    // MakeEnvelope Signals
    connect(ui->makeEnvelopeScalingInsertFn, &QPushButton::clicked, this, &FunctionGenerator::makeEnvelopeScalingFactorFunButtonClicked);
    connect(ui->makeEnvelopeXInsertFn, &QPushButton::clicked, this, &FunctionGenerator::makeEnvelopeXValueFunButtonClicked);
    connect(ui->makeEnvelopeYInsertFn, &QPushButton::clicked, this, &FunctionGenerator::makeEnvelopeYValueFunButtonClicked);
    connect(ui->makeEnvelopeScalingEdit, &QLineEdit::textChanged, this, &FunctionGenerator::makeEnvelopeTextChanged);
    connect(ui->makeEnvelopeXEdit, &QLineEdit::textChanged, this, &FunctionGenerator::makeEnvelopeTextChanged);
    connect(ui->makeEnvelopeYEdit, &QLineEdit::textChanged, this, &FunctionGenerator::makeEnvelopeTextChanged);

    // MakePattern Signals
    connect(ui->makePatternIntervalsInsertFn, &QPushButton::clicked, this, &FunctionGenerator::makePatternIntervalsFunButtonClicked);
    connect(ui->makePatternIntervalsEdit, &QLineEdit::textChanged, this, &FunctionGenerator::makePatternTextChanged);

    // ExpandPattern Signals
    connect(ui->expandPatternMethodEquivalence, &QRadioButton::toggled, this, &FunctionGenerator::expandPatternTextChanged);
    connect(ui->expandPatternMethodSymmetries, &QRadioButton::toggled, this, &FunctionGenerator::expandPatternTextChanged);
    connect(ui->expandPatternMethodDistort, &QRadioButton::toggled, this, &FunctionGenerator::expandPatternTextChanged);

    connect(ui->expandPatternModuloInsertFn, &QPushButton::clicked, this, &FunctionGenerator::expandPatternModuloFunButtonClicked);
    connect(ui->expandPatternLowInsertFn, &QPushButton::clicked, this, &FunctionGenerator::expandPatternLowFunButtonClicked);
    connect(ui->expandPatternHighInsertFn, &QPushButton::clicked, this, &FunctionGenerator::expandPatternHighFunButtonClicked);
    connect(ui->expandPatternPatternInsertFn, &QPushButton::clicked, this, &FunctionGenerator::expandPatternPatternFunButtonClicked);

    connect(ui->expandPatternModuloEdit, &QLineEdit::textChanged, this, &FunctionGenerator::expandPatternTextChanged);
    connect(ui->expandPatternLowEdit, &QLineEdit::textChanged, this, &FunctionGenerator::expandPatternTextChanged);
    connect(ui->expandPatternHighEdit, &QLineEdit::textChanged, this, &FunctionGenerator::expandPatternTextChanged);
    connect(ui->expandPatternPatternEdit, &QLineEdit::textChanged, this, &FunctionGenerator::expandPatternTextChanged);

    // ReadPATFile Signals
    connect(ui->readPatFileEdit, &QLineEdit::textChanged, this, &FunctionGenerator::readPATFileTextChanged);

    // ReadFILFile Signals
    connect(ui->readFilFileEdit, &QLineEdit::textChanged, this, &FunctionGenerator::readFILFileTextChanged);

    // MakeFilter Signals
    connect(ui->makeFilterLPF, &QRadioButton::toggled, this, &FunctionGenerator::makeFilterTextChanged);
    connect(ui->makeFilterHPF, &QRadioButton::toggled, this, &FunctionGenerator::makeFilterTextChanged);
    connect(ui->makeFilterBPF, &QRadioButton::toggled, this, &FunctionGenerator::makeFilterTextChanged);
    connect(ui->makeFilterLSF, &QRadioButton::toggled, this, &FunctionGenerator::makeFilterTextChanged);
    connect(ui->makeFilterHSF, &QRadioButton::toggled, this, &FunctionGenerator::makeFilterTextChanged);
    connect(ui->makeFilterNF, &QRadioButton::toggled, this, &FunctionGenerator::makeFilterTextChanged);
    connect(ui->makeFilterPBEQ, &QRadioButton::toggled, this, &FunctionGenerator::makeFilterTextChanged);

    connect(ui->makeFilterFrequencyInsertFn, &QPushButton::clicked, this, &FunctionGenerator::makeFilterFrequencyFunButtonClicked);
    connect(ui->makeFilterBWInsertFn, &QPushButton::clicked, this, &FunctionGenerator::makeFilterBandWidthFunButtonClicked);
    connect(ui->makeFilterDBInsertFn, &QPushButton::clicked, this, &FunctionGenerator::makeFilterDBGainFunButtonClicked);

    connect(ui->makeFilterFrequencyEdit, &QLineEdit::textChanged, this, &FunctionGenerator::makeFilterTextChanged);
    connect(ui->makeFilterBWEdit, &QLineEdit::textChanged, this, &FunctionGenerator::makeFilterTextChanged);
    connect(ui->makeFilterDBEdit, &QLineEdit::textChanged, this, &FunctionGenerator::makeFilterTextChanged);

    // ReadREVFile Signals
    connect(ui->readRevFileEdit, &QLineEdit::textChanged, this, &FunctionGenerator::readREVFileTextChanged);
    
    // REV signals
    REVApplyFlag = 0; // default apply by sound
    REVMethodFlag = 0; // default to REV_Simple
    REVNumOfPartials = 0; // default no partials
    connect(ui->revApplySound, &QRadioButton::toggled, this, &FunctionGenerator::REVApplyByRadioButtonClicked);
    connect(ui->revApplyPartial, &QRadioButton::toggled, this, &FunctionGenerator::REVApplyByRadioButtonClicked);

    // ReadSIVFile Signals
    connect(ui->readSivFileEdit, &QLineEdit::textChanged, this, &FunctionGenerator::readSIVFileTextChanged);

    // MakeSieve Signals
    connect(ui->makeSieveElementsMeaningful, &QRadioButton::toggled, this, &FunctionGenerator::makeSieveTextChanged);
    connect(ui->makeSieveElementsMods, &QRadioButton::toggled, this, &FunctionGenerator::makeSieveTextChanged);
    connect(ui->makeSieveElementsFake, &QRadioButton::toggled, this, &FunctionGenerator::makeSieveTextChanged);
    connect(ui->makeSieveElementsFib, &QRadioButton::toggled, this, &FunctionGenerator::makeSieveTextChanged);
    connect(ui->makeSieveWeightsPeriodic, &QRadioButton::toggled, this, &FunctionGenerator::makeSieveTextChanged);
    connect(ui->makeSieveWeightsHierarchic, &QRadioButton::toggled, this, &FunctionGenerator::makeSieveTextChanged);
    connect(ui->makeSieveWeightsInclude, &QRadioButton::toggled, this, &FunctionGenerator::makeSieveTextChanged);

    connect(ui->makeSieveLowBoundInsertFn, &QPushButton::clicked, this, &FunctionGenerator::makeSieveLowFunButtonClicked);
    connect(ui->makeSieveHighBoundInsertFn, &QPushButton::clicked, this, &FunctionGenerator::makeSieveHighFunButtonClicked);

    connect(ui->makeSieveLowBoundEdit, &QLineEdit::textChanged, this, &FunctionGenerator::makeSieveTextChanged);
    connect(ui->makeSieveHighBoundEdit, &QLineEdit::textChanged, this, &FunctionGenerator::makeSieveTextChanged);
    connect(ui->makeSieveElementsValuesEdit, &QLineEdit::textChanged, this, &FunctionGenerator::makeSieveTextChanged);
    connect(ui->makeSieveOffsetEdit, &QLineEdit::textChanged, this, &FunctionGenerator::makeSieveTextChanged);
    connect(ui->makeSieveWeightsValuesEdit, &QLineEdit::textChanged, this, &FunctionGenerator::makeSieveTextChanged);

    // ReadSPAFile Signals
    connect(ui->readSpaFileEdit, &QLineEdit::textChanged, this, &FunctionGenerator::readSPAFileTextChanged);

    // SPA Signals
    SPANumOfPartials = 0;
    connect(ui->spaStereo, &QRadioButton::toggled, this, &FunctionGenerator::SPATextChanged);
    connect(ui->spaMultiPan, &QRadioButton::toggled, this, &FunctionGenerator::SPATextChanged);
    connect(ui->spaPolar, &QRadioButton::toggled, this, &FunctionGenerator::SPATextChanged);
    connect(ui->spaApplySound, &QRadioButton::toggled, this, &FunctionGenerator::SPATextChanged);
    connect(ui->spaApplyPartial, &QRadioButton::toggled, this, &FunctionGenerator::SPATextChanged);

    // SpectrumGen Signals
    connect(ui->spectrumGenEnvelopeInsertFn, &QPushButton::clicked, this, &FunctionGenerator::Spectrum_GenEnvelopeFunButtonClicked);
    connect(ui->spectrumGenDistanceInsertFn, &QPushButton::clicked, this, &FunctionGenerator::Spectrum_GenDistanceFunButtonClicked);
    connect(ui->spectrumGenEnvelopeEdit, &QLineEdit::textChanged, this, &FunctionGenerator::Spectrum_GenTextChanged);
    connect(ui->spectrumGenDistanceEdit, &QLineEdit::textChanged, this, &FunctionGenerator::Spectrum_GenTextChanged);


    // TO DO: Parsing to set result string value for all input fields
    XMLPlatformUtils::Initialize();
    XercesDOMParser* parser = new XercesDOMParser();
    std::string stdOriginalString = originalString.toStdString();
    xercesc::MemBufInputSource myxml_buf  ((const XMLByte*)stdOriginalString.c_str(), stdOriginalString.size(),
                                        "function (in memory)");
    parser->parse(myxml_buf);
    DOMDocument* xmlDocument = parser->getDocument();
    DOMElement* root = xmlDocument->getDocumentElement();

    char* functionNameChars;
    char* charBuffer;
    DOMCharacterData* textData;
    std::string functionName;
    DOMElement* thisElement;

    //emptyString
    if (root == NULL){ return; }
    DOMElement* functionNameElement = root->getFirstElementChild();
    textData = ( DOMCharacterData*) functionNameElement->getFirstChild();
    if (textData){
        functionNameChars = XMLString::transcode(textData->getData());
        functionName = string(functionNameChars);
        XMLString::release(&functionNameChars);
    }
    if (functionName == "RandomInt") {
        int index = -1;
        for (int i = 0; i < ui->functionOptions->count(); ++i) {
            QString itemText = ui->functionOptions->itemText(i);
            if (itemText.toStdString() == functionName) {
                index = i;
                break;
            }
        }
        if (index != -1) { ui->functionOptions->setCurrentIndex(index); }
        DOMElement* thisElement = functionNameElement->getNextElementSibling();
        ui->randomIntLowerBoundEdit->setText(QString::fromStdString(getFunctionString(thisElement)));
        thisElement = thisElement->getNextElementSibling();
        ui->randomIntUpperBoundEdit->setText(QString::fromStdString(getFunctionString(thisElement)));
    }
}

std::string FunctionGenerator::getFunctionString(DOMElement* _thisFunctionElement){

  char* charBuffer;
  DOMCharacterData* textData;
  string returnString;
  DOMElement* child = _thisFunctionElement->getFirstElementChild();
  if (child ==NULL){ //not containing any child, return string

    textData = ( DOMCharacterData*) _thisFunctionElement->getFirstChild();
    if (textData){
      charBuffer = XMLString::transcode(textData->getData());
      returnString = string(charBuffer);
      XMLString::release(&charBuffer);
    } else returnString = "";

    return returnString;
  }

  //contain function!
  XMLCh tempStr[3] = {chLatin_L, chLatin_S, chNull};
  DOMImplementation *impl          = DOMImplementationRegistry::getDOMImplementation(tempStr);
  DOMLSSerializer   *theSerializer = ((DOMImplementationLS*)impl)->createLSSerializer();
  XMLCh* bla = theSerializer->writeToString (child);
  returnString = XMLString::transcode(bla);
  XMLString::release(&bla);
  delete theSerializer;
  return returnString;
}

void FunctionGenerator::handleFunctionChanged(int index)
{
    // Gets the function id name of the current index the combo box has selected
    QVariant data = ui->functionOptions->itemData(index);
    if (!data.isValid()) return;
    // Converts the function id name into an integer
    int functionId = data.toInt();
    /* Case statements used to set the stacked widget current index (new form widget added)
     based on the function id */
    int currPageIndex = 0;
    // initializes variables used
    int oldREVMethodFlag = REVMethodFlag;
    switch (functionId) {
        case NOT_A_FUNCTION:
        case functionRawList:
            ui->resultTextEdit->setText("");
            currPageIndex = 0;
            break;
        case function_staticCURRENT_TYPE:
            ui->resultTextEdit->setText("<Fun><Name>CURRENT_TYPE</Name></Fun>");
            currPageIndex = 0;
            break;
        case function_staticCURRENT_CHILD_NUM:
            ui->resultTextEdit->setText("<Fun><Name>CURRENT_CHILD_NUM</Name></Fun>");
            currPageIndex = 0;
            break;
        case function_staticCURRENT_PARTIAL_NUM:
            ui->resultTextEdit->setText("<Fun><Name>CURRENT_PARTIAL_NUM</Name></Fun>");
            currPageIndex = 0;
            break;
        case function_staticCURRENT_SEGMENT:
            ui->resultTextEdit->setText("<Fun><Name>CURRENT_SEGMENT</Name></Fun>");
            currPageIndex = 0;
            break;
        case function_staticAVAILABLE_EDU:
            ui->resultTextEdit->setText("<Fun><Name>AVAILABLE_EDU</Name></Fun>");
            currPageIndex = 0;
            break;
        case function_staticPREVIOUS_CHILD_DURATION:
            ui->resultTextEdit->setText("<Fun><Name>PREVIOUS_CHILD_DURATION</Name></Fun>");
            currPageIndex = 0;
            break;
        case function_staticCURRENT_LAYER:
            ui->resultTextEdit->setText("<Fun><Name>CURRENT_LAYER</Name></Fun>");
            currPageIndex = 0;
            break;
        case functionRandom:
            currPageIndex = 1;
            ui->randomLowerBoundEdit->setText("0");
            ui->randomUpperBoundEdit->setText("1");
            randomEntryChanged();
            break;
        case functionRandomInt:
            currPageIndex = 2;
            ui->randomIntLowerBoundEdit->setText("0");
            ui->randomIntUpperBoundEdit->setText("1");
            randomIntEntryChanged();
            break;
        case functionRandomOrderInt:
            currPageIndex = 3;
            ui->randomOrderIntLowerBoundEdit->setText("0");
            ui->randomOrderIntUpperBoundEdit->setText("1");
            randomOrderIntEntryChanged();
            break;
        case functionRandomizer:
            currPageIndex = 4;
            randomizerEntryChanged();
            break;
        case functionRandomDensity:
            currPageIndex = 5;
            randomDensityEntryChanged();
            break;
        case functionInverse:
            currPageIndex = 6; 
            inverseEntryChanged();
            break;
        case functionLN:
            currPageIndex = 7;
            LNEntryChanged();
            break;
        case functionFibonacci:
            currPageIndex = 8;
            FibonacciEntryChanged();
            break;
        case functionDecay:
            currPageIndex = 9;
            ui->decayIndexEdit->setText("CURRENT_PARTIAL_NUM");
            ui->decayTypeExponential->setChecked(true);
            decayEntryChanged();
            break;
        case functionMarkov:
            currPageIndex = 10;
            markovEntryChanged();
            break;
        case functionStochos:
            currPageIndex = 11;
            ui->stochosMethodRange->setChecked(true);
            stochosTextChanged();
            break;
        case functionSelect:
            currPageIndex = 12;
            selectEntryChanged();
            break;
        case functionValuePick:
            currPageIndex = 13;
            ui->valuePickLowEdit->setText("ENV");
            ui->valuePickHighEdit->setText("ENV");
            ui->valuePickDistEdit->setText("ENV");
            ui->valuePickValuesElementsEdit->setText("INT1, INT2, INT3 ...");
            ui->valuePickOffsetEdit->setText("INT1, INT2, INT3 ...");
            valuePickTextChanged();
            break;
        case functionChooseL:
            currPageIndex = 14;
            ui->chooseLEdit->setText("SIV");
            chooseLEntryChanged();
            break;
        case functionGetPattern:
            currPageIndex = 15;
            ui->getPatternChooseEdit->setText("PAT");
            getPatternEntryChanged();
            break;
        case functionEnvLib:
            currPageIndex = 16;
            ui->envLibScalingEdit->setText("1.0");
            envLibTextChanged();
            break;
        case functionMakeEnvelope:
            currPageIndex = 17;
            makeEnvelopeTextChanged();
            break;
        case functionReadENVFile:
            currPageIndex = 18;
            readENVFileTextChanged();
            break;
        case functionMakeList:
            currPageIndex = 19;
            makeListTextChanged();
            break;
        case functionSPA:
            currPageIndex = 20;
            if (!SPANumOfPartials) { SPAInsertPartial(); }
            SPATextChanged();
            break;
        case functionReadSPAFile:
            currPageIndex = 21;
            readSPAFileTextChanged();
            break;
        case functionMakeFilter:
            currPageIndex = 22;
            makeFilterTextChanged();
            break;
        case functionReadFILFile:
            currPageIndex = 23;
            readFILFileTextChanged();
            break;
        case functionMakePattern:
            currPageIndex = 24;
            ui->makePatternIntervalsEdit->setText("INT1, INT2, INT3 ...");
            makePatternTextChanged();
            break;  
        case functionExpandPattern:
            currPageIndex = 25;
            expandPatternTextChanged();
            break; 
        case functionReadPATFile:
            currPageIndex = 26;
            readPATFileTextChanged();
            break; 
        case functionREV_Simple:
            REVMethodFlag = 0;
            currPageIndex = 27;
            if (!REVNumOfPartials) { REVInsertPartial(); }
            REVApplyByRadioButtonClicked();
            break;
        case functionREV_Medium:
            REVMethodFlag = 1;
            currPageIndex = 27;
            REVApplyByRadioButtonClicked();
            break;
        case functionREV_Advanced:
            REVMethodFlag = 2;
            currPageIndex = 27;
            REVApplyByRadioButtonClicked();
            break;
        case functionReadREVFile:
            currPageIndex = 28;
            readREVFileTextChanged();
            break; 
        case functionSpectrum_Gen:
            currPageIndex = 29;
            ui->spectrumGenEnvelopeEdit->setText("ENV");
            Spectrum_GenTextChanged();
            break;
        case functionMakeSieve:
            currPageIndex = 30;
            ui->makeSieveLowBoundEdit->setText("ENV");
            ui->makeSieveHighBoundEdit->setText("ENV");
            ui->makeSieveElementsValuesEdit->setText("INT1, INT2, INT3 ...");
            ui->makeSieveWeightsValuesEdit->setText("INT1, INT2, INT3 ...");
            ui->makeSieveOffsetEdit->setText("INT1, INT2, INT3 ...");
            makeSieveTextChanged();
            break;
        case functionReadSIVFile:
            currPageIndex = 31;
            readSIVFileTextChanged();
            break;
        default: currPageIndex = 0; break;
    }
    // Udate stacked widget page added
    ui->functionStackedWidget->setCurrentIndex(currPageIndex);
    // Resizes to fit new page
    ui->functionStackedWidget->setFixedSize(ui->functionStackedWidget->widget(currPageIndex)->sizeHint());
    this->adjustSize();
}

QString FunctionGenerator::getResultString(){
    result = ui->resultTextEdit->toPlainText();
    return result;
}

/* Random Signal Functions */
void FunctionGenerator::randomLowBoundFunButtonClicked(){
  QString initialText = ui->randomLowerBoundEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnFloat, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->randomLowerBoundEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::randomHighBoundFunButtonClicked(){
  QString initialText = ui->randomUpperBoundEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnFloat, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->randomUpperBoundEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::randomEntryChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>Random</Name><Low>" + ui->randomLowerBoundEdit->text() +  "</Low><High>";
    stringbuffer = stringbuffer + ui->randomUpperBoundEdit->text() + "</High></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* RandomInt Signal Functions */
void FunctionGenerator::randomIntLowBoundFunButtonClicked(){
  QString initialText = ui->randomIntLowerBoundEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnInt, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->randomIntLowerBoundEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::randomIntHighBoundFunButtonClicked(){
  QString initialText = ui->randomIntUpperBoundEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnInt, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->randomIntUpperBoundEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::randomIntEntryChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>RandomInt</Name><Low>" + ui->randomIntLowerBoundEdit->text() +  "</Low><High>";
    stringbuffer = stringbuffer + ui->randomIntUpperBoundEdit->text() + "</High></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* RandomOrderInt Signal Functions */
void FunctionGenerator::randomOrderIntLowBoundFunButtonClicked(){
  QString initialText = ui->randomOrderIntLowerBoundEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnInt, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->randomOrderIntLowerBoundEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::randomOrderIntHighBoundFunButtonClicked(){
  QString initialText = ui->randomOrderIntUpperBoundEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnInt, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->randomOrderIntUpperBoundEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::randomOrderIntEntryChanged(){
    QString ts = QString::number(QDateTime::currentSecsSinceEpoch());
    QString stringbuffer;
    stringbuffer = "<Fun><Name>RandomOrderInt</Name><Low>" + ui->randomOrderIntLowerBoundEdit->text() + "</Low><High>";
    stringbuffer = stringbuffer + ui->randomOrderIntUpperBoundEdit->text() + "</High><Id>" + ts + "</Id></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* Randomizer Signal Functions */
void FunctionGenerator::randomizerBaseFunButtonClicked(){
  QString initialText = ui->randomizerBaseEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnFloat, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->randomizerBaseEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::randomizerDeviationFunButtonClicked(){
  QString initialText = ui->randomizerDeviationEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnFloat, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->randomizerDeviationEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::randomizerEntryChanged(){
    QString stringbuffer;
    stringbuffer =  "<Fun><Name>Randomizer</Name><Base>" + ui->randomizerBaseEdit->text() +  "</Base><Deviation>";
    stringbuffer = stringbuffer + ui->randomizerDeviationEdit->text() +  "</Deviation></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* RandomDensity Signal Functions */
void FunctionGenerator::randomDensityEntryChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>RandomDensity</Name><Envelope>" + ui->randomDensityEnvEdit->text() + "</Envelope>";
    stringbuffer = stringbuffer + "<Low>" + ui->randomDensityStartTimeEdit->text() +  "</Low>";
    stringbuffer = stringbuffer + "<High>" + ui->randomDensityEndTimeEdit->text() +  "</High></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* Decay Signal Functions */
void FunctionGenerator::decayBaseFunButtonClicked(){
  QString initialText = ui->decayBaseEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnFloat, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->decayBaseEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::decayRateFunButtonClicked(){
  QString initialText = ui->decayRateEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnFloat, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->decayRateEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::decayIndexFunButtonClicked(){
  QString initialText = ui->decayIndexEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnFloat, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->decayIndexEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::decayEntryChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>Decay</Name><Base>" + ui->decayBaseEdit->text() +  "</Base><Type>";
    if (ui->decayTypeExponential->isChecked()) { stringbuffer = stringbuffer + "EXPONENTIAL"; }
    else if (ui->decayTypeLinear->isChecked()) { stringbuffer = stringbuffer + "LINEAR"; }
    stringbuffer = stringbuffer + "</Type><Rate>" + ui->decayRateEdit->text() + "</Rate><Index>";
    stringbuffer = stringbuffer + ui->decayIndexEdit->text() + "</Index></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* Inverse Signal Functions */
void FunctionGenerator::inverseFunButtonClicked(){
  QString initialText = ui->numToInverseEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnFloat, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->numToInverseEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::inverseEntryChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>Inverse</Name><Entry>" + ui->numToInverseEdit->text() + "</Entry></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* Markov Signal Functions */
void FunctionGenerator::markovEntryChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>Markov</Name><Entry>" + ui->markovEdit->text() + "</Entry></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* LN Signal Functions */
void FunctionGenerator::LNFunButtonClicked(){
  QString initialText = ui->lnEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnFloat, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->lnEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::LNEntryChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>LN</Name><Entry>" + ui->lnEdit->text() + "</Entry></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* Fibonacci Signal Functions */
void FunctionGenerator::FibonacciFunButtonClicked(){
  QString initialText = ui->fibEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnFloat, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->fibEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::FibonacciEntryChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>Fibonacci</Name><Entry>" + ui->fibEdit->text() + "</Entry></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* Select Signal Functions */
void FunctionGenerator::selectIndexFunButtonClicked(){
  QString initialText = ui->selectIndexEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnFloat, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->selectIndexEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::selectEntryChanged(){
    QString stringbuffer;
    stringbuffer =  "<Fun><Name>Select</Name><List>";
    // TO DO: SelectSubAlignments
    stringbuffer =  stringbuffer + "</List><Index>" + ui->selectIndexEdit->text() + "</Index></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* Stochos Signal Functions */
void FunctionGenerator::stochosTextChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>Stochos</Name><Method>";
    if (ui->stochosMethodRange->isChecked()) { stringbuffer = stringbuffer + "RANGE_DISTRIB"; }
    else if (ui->stochosMethodFunction->isChecked()) { stringbuffer = stringbuffer + "FUNCTIONS"; }
    stringbuffer = stringbuffer + "</Method><Envelopes>";
    // TO DO: StochosSubAlignments
    stringbuffer = stringbuffer + "</Envelopes><Offset>" + ui->stochosOffsetEdit->text() + "</Offset></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* ValuePick Signal Functions */
void FunctionGenerator::valuePickAbsRangeFunButtonClicked(){
  QString initialText = ui->valuePickAbsRangeEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnFloat, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->valuePickAbsRangeEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::valuePickLowFunButtonClicked(){
  QString initialText = ui->valuePickLowEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnENV, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->valuePickLowEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::valuePickHighFunButtonClicked(){
  QString initialText = ui->valuePickHighEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnENV, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->valuePickHighEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::valuePickDistFunButtonClicked(){
  QString initialText = ui->valuePickDistEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnENV, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->valuePickDistEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::valuePickTextChanged(){
    QString stringbuffer;
    stringbuffer = stringbuffer + "<Fun><Name>ValuePick</Name><Range>"+ ui->valuePickAbsRangeEdit->text() + "</Range><Low>";
    stringbuffer = stringbuffer + ui->valuePickLowEdit->text()+ "</Low><High>";
    stringbuffer = stringbuffer + ui->valuePickHighEdit->text()+ "</High><Dist>";
    stringbuffer = stringbuffer + ui->valuePickDistEdit->text()+ "</Dist><Method>";
    if (ui->valuePickElementsMeaningful->isChecked()) { stringbuffer = stringbuffer + "MEANINGFUL"; }
    else if (ui->valuePickElementsMods->isChecked()) { stringbuffer = stringbuffer + "MODS"; }
    else if (ui->valuePickElementsFake->isChecked()) { stringbuffer = stringbuffer + "FAKE"; }
    else if (ui->valuePickElementsFib->isChecked()) { stringbuffer = stringbuffer + "FIBONACCI"; }
    stringbuffer = stringbuffer + "</Method><Elements>" + ui->valuePickValuesElementsEdit->text() + "</Elements><WeightMethod>";
    if (ui->valuePickWeightsPeriodic->isChecked()) { stringbuffer = stringbuffer + "PERIODIC"; }
    else if (ui->valuePickWeightsHierarchic->isChecked()) { stringbuffer = stringbuffer + "HIERARCHIC"; }
    else if (ui->valuePickWeightsInclude->isChecked()) { stringbuffer = stringbuffer + "INCLUDE"; }
    stringbuffer = stringbuffer + "</WeightMethod><Weight>" + ui->valuePickValuesWeightsEdit->text() + "</Weight><Type>";
    if (ui->valuePickTypeVariable->isChecked()) { stringbuffer = stringbuffer + "VARIABLE"; }
    else if (ui->valuePickTypeConstant->isChecked()) { stringbuffer = stringbuffer + "CONSTANT"; }
    stringbuffer = stringbuffer + "</Type><Offsets>" + ui->valuePickOffsetEdit->text() + "</Offsets></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* ChooseL Signal Functions */
void FunctionGenerator::chooseLFunButtonClicked(){
  QString initialText = ui->chooseLEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnSIV, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->chooseLEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::chooseLEntryChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>ChooseL</Name><Entry>" + ui->chooseLEdit->text() +  "</Entry></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* GetPattern Signal Functions */
void FunctionGenerator::getPatternOffsetFunButtonClicked(){
  QString initialText = ui->getPatternOriginEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnInt, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->getPatternOriginEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::getPatternFunButtonClicked(){
  QString initialText = ui->getPatternChooseEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnPAT, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->getPatternChooseEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::getPatternEntryChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>GetPattern</Name><Method>";
    if (ui->getPatternMethodInOrder->isChecked()) { stringbuffer = stringbuffer + "IN_ORDER"; }
    else if (ui->getPatternMethodOther->isChecked()) { stringbuffer = stringbuffer + "OTHER"; }
    else if (ui->getPatternMethodTypeClusters->isChecked()) { stringbuffer = stringbuffer + "TYPE_CLUSTERS"; }
    else if (ui->getPatternMethodTimeDepend->isChecked()) { stringbuffer = stringbuffer + "TIME_DEPEND"; }
    else if (ui->getPatternMethodProbability->isChecked()) { stringbuffer = stringbuffer + "PROBABILITY"; }
    stringbuffer = stringbuffer + "</Method><Offset>" + ui->getPatternOriginEdit->text() + "</Offset><Pattern>";
    stringbuffer = stringbuffer + ui->getPatternChooseEdit->text() + "</Pattern></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* MakeList Signal Functions */
void FunctionGenerator::makeListFunctionFunButtonClicked(){
  QString initialText = ui->makeListFunctionEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnMakeListFun, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->makeListFunctionEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::makeListSizeFunButtonClicked(){
  QString initialText = ui->makeListSizeEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnMakeListFun, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->makeListSizeEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::makeListTextChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>MakeList</Name><Func>" + ui->makeListFunctionEdit->text() + "</Func><Size>";
    stringbuffer = stringbuffer + ui->makeListSizeEdit->text() + "</Size></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* EnvLib Signal Functions */
void FunctionGenerator::envLibEnvelopeFunButtonClicked(){
  QString initialText = ui->envLibNumEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnInt, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->envLibNumEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::envLibScalingFactorFunButtonClicked(){
  QString initialText = ui->envLibScalingEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnFloat, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->envLibScalingEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::envLibTextChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>EnvLib</Name><Env>" + ui->envLibNumEdit->text() + "</Env><Scale>";
    stringbuffer = stringbuffer + ui->envLibScalingEdit->text() + "</Scale></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* MakeEnvelope Signal Functions */
void FunctionGenerator::makeEnvelopeScalingFactorFunButtonClicked(){
  QString initialText = ui->makeEnvelopeScalingEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnFloat, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->makeEnvelopeScalingEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::makeEnvelopeXValueFunButtonClicked(){
  QString initialText = ui->makeEnvelopeXEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnFloat, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->makeEnvelopeXEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::makeEnvelopeYValueFunButtonClicked(){
  QString initialText = ui->makeEnvelopeYEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnFloat, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->makeEnvelopeYEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::makeEnvelopeTextChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>MakeEnvelope</Name><Xs>";
    // TO DO: MakeEnvelopeSubAlignment
    stringbuffer = stringbuffer + "<X>" + ui->makeEnvelopeXEdit->text() + "</X></Xs><Ys>";
    stringbuffer = stringbuffer + "<Y>" + ui->makeEnvelopeYEdit->text() + "</Y></Ys><Types>";
    stringbuffer = stringbuffer + "</Types><Pros>";
    stringbuffer = stringbuffer+ "</Pros><Scale>" + ui->makeEnvelopeScalingEdit->text() + "</Scale></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* MakePattern Signal Functions */
void FunctionGenerator::makePatternIntervalsFunButtonClicked(){
  QString initialText = ui->makePatternIntervalsEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnList, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->makePatternIntervalsEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::makePatternTextChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>MakePattern</Name><List>" + ui->makePatternIntervalsEdit->text() + "</List></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* ExpandPattern Signal Functions */
void FunctionGenerator::expandPatternModuloFunButtonClicked(){
  QString initialText = ui->expandPatternModuloEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnInt, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->expandPatternModuloEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::expandPatternLowFunButtonClicked(){
  QString initialText = ui->expandPatternLowEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnInt, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->expandPatternLowEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::expandPatternHighFunButtonClicked(){
  QString initialText = ui->expandPatternHighEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnInt, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->expandPatternHighEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::expandPatternPatternFunButtonClicked(){
  QString initialText = ui->expandPatternPatternEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnPAT, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->expandPatternPatternEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::expandPatternTextChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>ExpandPattern</Name><Method>";
    if (ui->expandPatternMethodEquivalence->isChecked()) { stringbuffer = stringbuffer + "EQUIVALENCE"; }
    else if (ui->expandPatternMethodSymmetries->isChecked()) { stringbuffer = stringbuffer + "SYMMETRIES"; }
    else if (ui->expandPatternMethodDistort->isChecked()) { stringbuffer = stringbuffer + "DISTORT"; }
    stringbuffer = stringbuffer + "</Method><Modulo>" + ui->expandPatternModuloEdit->text() +  "</Modulo><Low>";
    stringbuffer = stringbuffer + ui->expandPatternLowEdit->text() + "</Low><High>";
    stringbuffer = stringbuffer + ui->expandPatternHighEdit->text() + "</High><Pattern>";
    stringbuffer = stringbuffer + ui->expandPatternPatternEdit->text() + "</Pattern></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* MakeFilter Signal Functions */
void FunctionGenerator::makeFilterFrequencyFunButtonClicked(){
  QString initialText = ui->makeFilterFrequencyEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnPAT, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->makeFilterFrequencyEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::makeFilterBandWidthFunButtonClicked(){
  QString initialText = ui->makeFilterBWEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnPAT, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->makeFilterBWEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::makeFilterDBGainFunButtonClicked(){
  QString initialText = ui->makeFilterDBEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnPAT, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->makeFilterDBEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::makeFilterTextChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>MakeFilter</Name><Type>";
    if (ui->makeFilterLPF->isChecked()) {
        stringbuffer = stringbuffer + "LPF";
        ui->makeFilterDBEdit->setEnabled(false);
    }
    if (ui->makeFilterHPF->isChecked()) {
        stringbuffer = stringbuffer + "HPF";
        ui->makeFilterDBEdit->setEnabled(false);
    }
    if (ui->makeFilterBPF->isChecked()) {
        stringbuffer = stringbuffer + "BPF";
        ui->makeFilterDBEdit->setEnabled(false);
    }
    if (ui->makeFilterHSF->isChecked()) {
        stringbuffer = stringbuffer + "HSF";
        ui->makeFilterDBEdit->setEnabled(true);
    }
    if (ui->makeFilterLSF->isChecked()) {
        stringbuffer = stringbuffer + "LSF";
        ui->makeFilterDBEdit->setEnabled(true);
    }
    if (ui->makeFilterNF->isChecked()) {
        stringbuffer = stringbuffer + "NF";
        ui->makeFilterDBEdit->setEnabled(false);
    }
    if (ui->makeFilterPBEQ->isChecked()) {
        stringbuffer = stringbuffer + "PBEQF";
        ui->makeFilterDBEdit->setEnabled(true);
    }
    stringbuffer = stringbuffer +"</Type><Frequency>" + ui->makeFilterFrequencyEdit->text()+ "</Frequency>";
    stringbuffer = stringbuffer +"<BandWidth>" + ui->makeFilterBWEdit->text()+ "</BandWidth>";
    stringbuffer = stringbuffer +"<dBGain>" + ui->makeFilterDBEdit->text()+ "</dBGain></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* Read File Signal Functions */
void FunctionGenerator::readENVFileTextChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>ReadENVFile</Name><File>" + ui->readEnvFileEdit->text() +  "</File></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}
void FunctionGenerator::readPATFileTextChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>ReadPATFile</Name><File>" + ui->readPatFileEdit->text() +  "</File></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}
void FunctionGenerator::readFILFileTextChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>ReadFILFile</Name><File>" + ui->readFilFileEdit->text() +  "</File></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}
void FunctionGenerator::readSIVFileTextChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>ReadSIVFile</Name><File>" + ui->readSivFileEdit->text() +  "</File></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}
void FunctionGenerator::readSPAFileTextChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>ReadSPAFile</Name><File>" + ui->readSpaFileEdit->text() +  "</File></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}
void FunctionGenerator::readREVFileTextChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>ReadREVFile</Name><File>" + ui->readRevFileEdit->text() +  "</File></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* REV Signal Functions */
void FunctionGenerator::REVApplyByRadioButtonClicked() {
    // REVPartialAlignment* curr = REVPartialAlignments;
    if (ui->revApplySound->isChecked()) { 
        REVApplyFlag = 0;
    }
    else if (ui->revApplyPartial->isChecked()) {
        REVApplyFlag = 1;
    }
    REVTextChanged();
}
void FunctionGenerator::REVTextChanged(){
    QString stringbuffer;
    int method;
    int apply;
    switch (REVMethodFlag) {
        case 0: stringbuffer = "<Fun><Name>REV_Simple</Name><Apply>"; break;
        case 1: stringbuffer = "<Fun><Name>REV_Medium</Name><Apply>"; break;
        case 2: stringbuffer = "<Fun><Name>REV_Advanced</Name><Apply>"; break;
    }
    if (REVApplyFlag == 0) { stringbuffer = stringbuffer +"SOUND"; }
    else if (REVApplyFlag == 1) { stringbuffer = stringbuffer +"PARTIAL"; }
    // TO DO: REVPartialAlignments
    REVPartialAlignment* curr;
    switch (REVMethodFlag) {
        case 0:
            stringbuffer = stringbuffer + "</Apply><Sizes>";
            curr = REVPartialAlignments;
            connect(curr->ui->revRoomSizeEdit, &QLineEdit::textChanged, this, &FunctionGenerator::REVTextChanged);
            while (curr != NULL){
                stringbuffer = stringbuffer + "<Size>" + curr->roomSizeTextChanged() + "</Size>";
                if (REVApplyFlag == 0) break;
                curr = curr->next;
            }
            stringbuffer = stringbuffer + "</Sizes></Fun>";
            break;
        case 1:
            break;
        case 2:
            break;
    }   
    ui->resultTextEdit->setText(stringbuffer);
}
REVPartialAlignment* FunctionGenerator::REVInsertPartial(){
    REVNumOfPartials ++;
    ui->revScrollWindow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->revScrollWindowContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    REVPartialAlignment* newSubAlignment = new REVPartialAlignment(ui->revScrollWindow);
    ui->revScrollWindowContent->layout()->addWidget(newSubAlignment);

    int widgetHeight = newSubAlignment->sizeHint().height();
    int widgetWidth = newSubAlignment->sizeHint().width();
    int scrollBarHeight = ui->revScrollWindow->horizontalScrollBar()->sizeHint().height();
    int scrollBarWidth = ui->revScrollWindow->verticalScrollBar()->sizeHint().width();
    ui->revScrollWindow->setMinimumHeight(widgetHeight + scrollBarHeight);  
    ui->revScrollWindow->setMaximumHeight(widgetHeight + scrollBarHeight);
    ui->revScrollWindow->setMinimumWidth(widgetWidth + scrollBarWidth);  
    ui->revScrollWindow->setMaximumWidth(widgetWidth + scrollBarWidth);

    if (REVPartialAlignments == NULL) { REVPartialAlignments = newSubAlignment; }
    else { REVPartialAlignments->appendNewNode(newSubAlignment); }
    REVTextChanged();
    return newSubAlignment;
}

/* MakeSieve Signal Functions */
void FunctionGenerator::makeSieveLowFunButtonClicked(){
  QString initialText = ui->makeSieveLowBoundEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnENV, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->makeSieveLowBoundEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::makeSieveHighFunButtonClicked(){
  QString initialText = ui->makeSieveHighBoundEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnENV, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->makeSieveHighBoundEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::makeSieveTextChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>MakeSieve</Name><Low>"+ ui->makeSieveLowBoundEdit->text() + "</Low><High>";
    stringbuffer = stringbuffer + ui->makeSieveHighBoundEdit->text() + "</High><Method>";
    if (ui->makeSieveElementsMeaningful->isChecked()) { stringbuffer = stringbuffer + "MEANINGFUL"; }
    else if (ui->makeSieveElementsMods->isChecked()) { stringbuffer = stringbuffer + "MODS"; }
    else if (ui->makeSieveElementsFake->isChecked()) { stringbuffer = stringbuffer + "FAKE"; }
    else if (ui->makeSieveElementsFib->isChecked()) { stringbuffer = stringbuffer + "FIBONACCI"; }
    stringbuffer = stringbuffer + "</Method><Elements>" + ui->makeSieveElementsValuesEdit->text() + "</Elements><WeightMethod>";
    if (ui->makeSieveWeightsPeriodic->isChecked()) { stringbuffer = stringbuffer + "PERIODIC"; }
    else if (ui->makeSieveWeightsHierarchic->isChecked()) { stringbuffer = stringbuffer + "HIERARCHIC"; }
    else if (ui->makeSieveWeightsInclude->isChecked()) { stringbuffer = stringbuffer + "INCLUDE"; }
    stringbuffer = stringbuffer + "</WeightMethod><Weight>" + ui->makeSieveWeightsValuesEdit->text() + "</Weight><Offset>";
    stringbuffer = stringbuffer + ui->makeSieveOffsetEdit->text() + "</Offset></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}

/* SPA Signal Functions */
void FunctionGenerator::SPATextChanged() {
    QString stringbuffer;
    int method;
    int apply;
    stringbuffer = "<Fun><Name>SPA</Name><Method>";
    if (ui->spaStereo->isChecked()) {
        stringbuffer = stringbuffer + "STEREO";
        method = 0;
    } else if (ui->spaMultiPan->isChecked()) {
        stringbuffer = stringbuffer + "MULTI_PAN";
        method = 1;
    } else if (ui->spaPolar->isChecked()) {
        stringbuffer = stringbuffer + "POLAR";
        method = 2;
    }
    stringbuffer = stringbuffer + "</Method><Apply>";
    if (ui->spaApplySound->isChecked()) {
        stringbuffer = stringbuffer + "SOUND";
        apply = 0;
    } else if (ui->spaApplyPartial->isChecked()) {
        stringbuffer = stringbuffer + "PARTIAL";
        apply = 1;
    }
    stringbuffer = stringbuffer + "</Apply><Channels>";
    // TO DO: SPAChannelAlignments
    SPAPartialAlignment* temp = SPAPartialAlignments;
    connect(temp->ui->spaPartialEnvelopeEdit, &QLineEdit::textChanged, this, &FunctionGenerator::SPATextChanged);
    stringbuffer = stringbuffer + "<Partials>";
    while (temp != NULL){
        stringbuffer = stringbuffer + "<P>" + temp->textChanged() + "</P>";
        temp = temp->next;
    }
    stringbuffer = stringbuffer + "</Partials><Channels></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}
SPAPartialAlignment* FunctionGenerator::SPAInsertPartial(){
    SPANumOfPartials ++;
    ui->spaScrollWindow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->spaScrollWindowContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    SPAPartialAlignment* newSubAlignment = new SPAPartialAlignment(ui->spaScrollWindow);
    ui->spaScrollWindowContent->layout()->addWidget(newSubAlignment);

    int widgetHeight = newSubAlignment->sizeHint().height();
    int widgetWidth = newSubAlignment->sizeHint().width();
    int scrollBarHeight = ui->spaScrollWindow->horizontalScrollBar()->sizeHint().height();
    int scrollBarWidth = ui->spaScrollWindow->verticalScrollBar()->sizeHint().width();
    ui->spaScrollWindow->setMinimumHeight(widgetHeight + scrollBarHeight);  
    ui->spaScrollWindow->setMaximumHeight(widgetHeight + scrollBarHeight);
    ui->spaScrollWindow->setMinimumWidth(widgetWidth + scrollBarWidth);  
    ui->spaScrollWindow->setMaximumWidth(widgetWidth + scrollBarWidth);

    if (SPAPartialAlignments == NULL) { SPAPartialAlignments = newSubAlignment; }
    //else { REVPartialAlignments->appendNewNode(newSubAlignment); }
    SPATextChanged();
    return newSubAlignment;
}

/* SpectrumGen Signal Functions */
void FunctionGenerator::Spectrum_GenEnvelopeFunButtonClicked(){
  QString initialText = ui->spectrumGenEnvelopeEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnENV, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->spectrumGenEnvelopeEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::Spectrum_GenDistanceFunButtonClicked(){
  QString initialText = ui->spectrumGenDistanceEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnFloat, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->spectrumGenDistanceEdit->setText(result);
    }
  }
  delete generator;
}
void FunctionGenerator::Spectrum_GenTextChanged() {
    QString stringbuffer;
    stringbuffer =  "<Fun><Name>Spectrum_Gen</Name><Envelope>" + ui->spectrumGenEnvelopeEdit->text() +  "</Envelope><Distance>";
    stringbuffer =  stringbuffer + ui->spectrumGenDistanceEdit->text() + "</Distance></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}




