#ifndef FUNCTIONGENERATOR_HPP
#define FUNCTIONGENERATOR_HPP

#include "../../../LASS/src/LASS.h"
#include "../../../CMOD/src/CMOD.h"
#include "../../../CMOD/src/Utilities.h"

#include <QDialog>
#include <QWidget>
#include <QString>

typedef enum {
  functionRandom,
  functionRandomInt,
  functionRandomOrderInt,
  functionRandomSeed,
  functionRandomizer,
  functionRandomDensity,
  functionInverse,
  functionLN,
  functionFibonacci,// not in used??
  functionDecay,

  functionMarkov, // generate from markov

  functionStochos,
  functionEnvLib,
  functionSelect,
  functionValuePick,

  functionChooseL,
  functionGetPattern,
  functionExpandPattern,
  functionAdjustPattern, // not in used??
  functionMakeEnvelope,
  functionMakeSieve,
  functionMakePattern,
  functionMakeList,
  functionRawList,
  functionReadENVFile,
  functionReadSIVFile,
  functionReadPATFile,
  functionReadSPAFile,
  functionReadREVFile,
  functionReadFILFile,

  functionSPA,

  functionREV_Simple,
  functionREV_Medium,
  functionREV_Advanced,

  functionMakeFilter,

  function_staticCURRENT_TYPE,
  function_staticCURRENT_CHILD_NUM,
  function_staticCURRENT_PARTIAL_NUM,
  function_staticCURRENT_DENSITY,
  function_staticCURRENT_SEGMENT,
  function_staticAVAILABLE_EDU,
  function_staticCURRENT_LAYER,
  function_staticPREVIOUS_CHILD_DURATION,

  functionSpectrum_Gen,

  NOT_A_FUNCTION
} CMODFunction;

typedef enum {
  functionReturnInt,
  functionReturnFloat,
  functionReturnSPA,
  functionReturnREV,
  functionReturnENV,
  functionReturnSIV,
  functionReturnPAT,
  funcitonReturnMEA,
  functionReturnFIL, // added for filter object
  functionReturnMGP, // ZIYUAN CHEN, July 2023 - added for "Modifier Group"
  functionReturnSPE, //added for generating spectrum from distance
  functionReturnIntList,
  functionReturnFloatList,
  functionReturnEnvelopeList,
  functionReturnString, //used to parse FileValue
  functionReturnList,
  functionReturnMakeListFun

} FunctionReturnType;



typedef enum{
  modifierTremolo = 0,
  modifierVibrato = 1,
  modifierGlissando = 2,
  modifierDetune = 3,
  modifierAmptrans = 4,
  modifierFreqtrans = 5,
  modifierWave_type = 6
} ModifierType;

class SPAPartialAlignment;
class REVPartialAlignment;

namespace Ui {
class FunctionGenerator;
}

class FunctionGenerator : public QDialog
{
    Q_OBJECT

public:
    /*Constructor to create the Function Generator Pop Up*/
    explicit FunctionGenerator(QWidget *parent = nullptr, FunctionReturnType _returnType = functionReturnFloat, QString _originalString = "");
    /*Destructor to delete the UI*/
    ~FunctionGenerator();
    
    Ui::FunctionGenerator *ui;

    
    // Store the type of combo box the generator should return
    FunctionReturnType returnType;
    // Store the inputted string
    QString originalString;
    // Store the result string
    QString result;
    // Returns the result string
    QString getResultString();
    std::string static getFunctionString(FileValue* _value, FunctionReturnType _returnType);
    std::string static getFunctionString(DOMElement* _thisFunctionElement);
    // Changes the combo box to the right type
    void handleFunctionChanged(int index);

    // Signal handlers
    // Random
    void randomEntryChanged();
    void randomLowBoundFunButtonClicked();
    void randomHighBoundFunButtonClicked();

    // RandomInt
    void randomIntEntryChanged();
    void randomIntLowBoundFunButtonClicked();
    void randomIntHighBoundFunButtonClicked();

    // RandomOrderInt
    void randomOrderIntEntryChanged();
    void randomOrderIntLowBoundFunButtonClicked();
    void randomOrderIntHighBoundFunButtonClicked();

    // Randomizer
    void randomizerEntryChanged();
    void randomizerBaseFunButtonClicked();
    void randomizerDeviationFunButtonClicked();

    // RandomDensity
    void randomDensityEntryChanged();

    // Decay
    void decayBaseFunButtonClicked();
    void decayRateFunButtonClicked();
    void decayIndexFunButtonClicked();
    void decayEntryChanged();

    // Inverse
    void inverseFunButtonClicked();
    void inverseEntryChanged();

    // Markov
    void markovEntryChanged();

    // LN
    void LNFunButtonClicked();
    void LNEntryChanged();

    // Fibonacci
    void FibonacciFunButtonClicked();
    void FibonacciEntryChanged();

    // Select
    void selectIndexFunButtonClicked();
    void selectEntryChanged();

    // Stochos
    void stochosTextChanged();

    // ValuePick
    void valuePickAbsRangeFunButtonClicked();
    void valuePickLowFunButtonClicked();
    void valuePickHighFunButtonClicked();
    void valuePickDistFunButtonClicked();
    void valuePickTextChanged();

    // chooseL
    void chooseLFunButtonClicked();
    void chooseLEntryChanged();

    // GetPattern
    void getPatternOffsetFunButtonClicked();
    void getPatternFunButtonClicked();
    void getPatternEntryChanged();

    // MakeList
    void makeListFunctionFunButtonClicked();
    void makeListSizeFunButtonClicked();
    void makeListTextChanged();

    // EnvLib
    void envLibEnvelopeFunButtonClicked();
    void envLibScalingFactorFunButtonClicked();
    void envLibTextChanged();

    // MakeEnvelopes
    void makeEnvelopeScalingFactorFunButtonClicked();
    void makeEnvelopeXValueFunButtonClicked();
    void makeEnvelopeYValueFunButtonClicked();
    void makeEnvelopeTextChanged();

    // MakePattern
    void makePatternIntervalsFunButtonClicked();
    void makePatternTextChanged();

    // ExpandPattern
    void expandPatternModuloFunButtonClicked();
    void expandPatternLowFunButtonClicked();
    void expandPatternHighFunButtonClicked();
    void expandPatternPatternFunButtonClicked();
    void expandPatternTextChanged();

    // MakeFilter
    void makeFilterFrequencyFunButtonClicked();
    void makeFilterBandWidthFunButtonClicked();
    void makeFilterDBGainFunButtonClicked();
    void makeFilterTextChanged();

    // REV
    int REVApplyFlag; // 0 = sound, 1 = partial
    int REVMethodFlag; // 0 = simple, 1 = medium, 2 = advanced
    int REVNumOfPartials;
    void REVApplyByRadioButtonClicked();
    void REVTextChanged();
    void appendNewNode(REVPartialAlignment* _newNode);
    REVPartialAlignment* REVInsertPartial();
    REVPartialAlignment* REVPartialAlignments;

    // MakeSieve
    void makeSieveLowFunButtonClicked();
    void makeSieveHighFunButtonClicked();
    void makeSieveTextChanged();

    // SPA
    int SPANumOfPartials;
    // void SPAMethodRadioButtonClicked();
    // void SPAApplyByRadioButtonClicked();
    void SPATextChanged();
    SPAPartialAlignment* SPAInsertPartial();
    SPAPartialAlignment* SPAPartialAlignments;

    // Spectrum Gen
    void Spectrum_GenEnvelopeFunButtonClicked();
    void Spectrum_GenDistanceFunButtonClicked();
    void Spectrum_GenTextChanged();

    // Read Files
    void readENVFileTextChanged();
    void readSIVFileTextChanged();
    void readPATFileTextChanged();
    void readSPAFileTextChanged();
    void readREVFileTextChanged();
    void readFILFileTextChanged();

private:
    void setupUi();    
    
};

#endif // FUNCTIONGENERATOR_H 