#ifndef FUNCTIONGENERATOR_H
#define FUNCTIONGENERATOR_H

#include "LASSIE.h"

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
}CMODFunction;

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