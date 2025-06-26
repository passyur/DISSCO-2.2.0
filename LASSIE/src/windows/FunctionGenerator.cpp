#include "FunctionGenerator.hpp"
#include "../ui/ui_FunctionGenerator.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QOverload>
#include <QDateTime>


FunctionGenerator::FunctionGenerator(QWidget *parent, FunctionReturnType _returnType, QString _originalString)
    : QDialog(parent)
    , ui(new Ui::FunctionGenerator)
{
    // Sets up the user interface specified by the .ui file
    result = "";
    returnType = _returnType;
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

    // Connecting Random Buttons
    connect(ui->randomLowerBoundInsertFn, &QPushButton::clicked, this, &FunctionGenerator::randomLowBoundFunButtonClicked);


    // Centers result label
    ui->resultLabel->setAlignment(Qt::AlignCenter);
    // Sets height of result box
    ui->resultTextEdit->setFixedHeight(100);
    // Connects the ui OK and CANCEL buttons to their corresponding actions
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
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
    QString ts;
    switch (functionId) {
        case NOT_A_FUNCTION:
        case function_staticCURRENT_TYPE:
        case function_staticCURRENT_CHILD_NUM:
        case function_staticCURRENT_PARTIAL_NUM:
        case function_staticCURRENT_SEGMENT:
        case function_staticAVAILABLE_EDU:
        case function_staticPREVIOUS_CHILD_DURATION:
        case function_staticCURRENT_LAYER:
        case functionRawList:
            currPageIndex = 0;
            break;
        case functionRandom:
            currPageIndex = 1;
            ui->resultTextEdit->setText("<Fun><Name>Random</Name><Low>0</Low><High>1</High></Fun>");
            ui->randomLowerBoundEdit->setText("0");
            ui->randomUpperBoundEdit->setText("1");
            break;
        case functionRandomInt:
            currPageIndex = 2;
            ui->resultTextEdit->setText("<Fun><Name>RandomInt</Name><LowBound>0</LowBound><HighBound>1</HighBound></Fun>");
            ui->randomIntLowerBoundEdit->setText("0");
            ui->randomIntUpperBoundEdit->setText("1");
            break;
        case functionRandomOrderInt:
            currPageIndex = 3;
            ts = QString::number(QDateTime::currentSecsSinceEpoch());
            ui->resultTextEdit->setText("<Fun><Name>RandomOrderInt</Name><LowBound>0</LowBound><HighBound>1</HighBound><Id>" + ts + "</Id></Fun>");
            ui->randomOrderIntLowerBoundEdit->setText("0");
            ui->randomOrderIntUpperBoundEdit->setText("1");
            break;
        case functionRandomizer:
            currPageIndex = 4;
            ui->resultTextEdit->setText("<Fun><Name>Randomizer</Name><Base></Base><Deviation></Deviation></Fun>");
            break;
        case functionRandomDensity:
            currPageIndex = 5;
            ui->resultTextEdit->setText("<Fun><Name>RandomDensity</Name><Envelope></Envelope><Low></Low><High></High></Fun>");
            break;
        case functionInverse:
            currPageIndex = 6; 
            ui->resultTextEdit->setText("<Fun><Name>Inverse</Name><Entry></Entry></Fun>");
            break;
        case functionLN:
            currPageIndex = 7;
            ui->resultTextEdit->setText("<Fun><Name>LN</Name><Entry></Entry></Fun>");
            break;
        case functionFibonacci:
            currPageIndex = 8;
            ui->resultTextEdit->setText("<Fun><Name>Fibonacci</Name><Entry></Entry></Fun>");
            break;
        case functionDecay:
            currPageIndex = 9;
            ui->resultTextEdit->setText("<Fun><Name>Decay</Name><Base></Base><Type>EXPONENTIAL</Type><Rate></Rate><Index>CURRENT_PARTIAL_NUM</Index></Fun>");
            ui->decayIndexEdit->setText("CURRENT_PARTIAL_NUM");
            ui->decayTypeExponential->setChecked(true);
            break;
        case functionMarkov:
            currPageIndex = 10;
            ui->resultTextEdit->setText("<Fun><Name>Markov</Name></Entry></Entry></Fun>");
            break;
        case functionStochos:
            currPageIndex = 11;
            ui->resultTextEdit->setText("<Fun><Name>Stochos</Name><Method>RANGE_DISTRIB</Method><Envelopes></Envelopes><Offset></Offset></Fun>");
            ui->stochosMethodRange->setChecked(true);
            break;
        case functionSelect:
            currPageIndex = 12;
            ui->resultTextEdit->setText("<Fun><Name>Select</Name><List></List><Index></Index></Fun>");
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
            break;
        case functionMakeEnvelope:
            currPageIndex = 17;
            // void makeEnvelopeTextChanged();
            break;
        case functionReadENVFile:
            currPageIndex = 18;
            readENVFileTextChanged();
            break;
        case functionMakeList: currPageIndex = 19; break;
        case functionSPA: currPageIndex = 20; break;
        case functionReadSPAFile: currPageIndex = 21; break;
        case functionMakeFilter: currPageIndex = 22; break;
        case functionReadFILFile: currPageIndex = 23; break;
        case functionMakePattern: currPageIndex = 24; break;  
        case functionExpandPattern:
            currPageIndex = 25;
            expandPatternTextChanged();
            break; 
        case functionReadPATFile: currPageIndex = 26; break; 
        case functionREV_Simple:
        case functionREV_Medium:
        case functionREV_Advanced: currPageIndex = 27; break;
        case functionReadREVFile: currPageIndex = 28; break; 
        case functionSpectrum_Gen: currPageIndex = 29; break;
        case functionMakeSieve: currPageIndex = 30; break;
        case functionReadSIVFile: currPageIndex = 31; break;
        default: currPageIndex = 0; break;
    }
    // Udate stacked widget page added
    ui->functionStackedWidget->setCurrentIndex(currPageIndex);
    // Resizes to fit new page
    ui->functionStackedWidget->setFixedSize(ui->functionStackedWidget->widget(currPageIndex)->sizeHint());
    this->adjustSize();
}

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

QString FunctionGenerator::getResultString(){
  return result;
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

void FunctionGenerator::chooseLEntryChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>ChooseL</Name><Entry>" + ui->chooseLEdit->text() +  "</Entry></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
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

void FunctionGenerator::readENVFileTextChanged(){
    QString stringbuffer;
    stringbuffer = "<Fun><Name>ReadENVFile</Name><File>" + ui->readEnvFileEdit->text() +  "</File></Fun>";
    ui->resultTextEdit->setText(stringbuffer);
}
