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
    // Store the result string
    QString result;
    // Returns the result string
    QString getResultString();
    // Changes the combo box to the right type
    void handleFunctionChanged(int index);

    // Signal handlers
    // Random
    void randomLowBoundFunButtonClicked();

    // ValuePick
    void valuePickTextChanged();

    // chooseL
    void chooseLEntryChanged();

    // GetPattern
    void getPatternEntryChanged();

    // ExpandPattern
    void expandPatternTextChanged();

    // Envelopes
    // void makeEnvelopeTextChanged();
    void readENVFileTextChanged();

private:
    void setupUi();
    
};

#endif // FUNCTIONGENERATOR_H 