#include "FunctionGenerator.hpp"
#include "../ui/ui_FunctionGenerator.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QDialogButtonBox>


FunctionGenerator::FunctionGenerator(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FunctionGenerator)
{
    // Sets up the user interface specified by the .ui file
    ui->setupUi(this);
    setupUi();
}

FunctionGenerator::~FunctionGenerator()
{
    delete ui;
}

void FunctionGenerator::setupUi()
{
    // Add combo box function options
    ui->functionOptions->addItem("");
    ui->functionOptions->addItem("Random");
    ui->functionOptions->addItem("RandomInt");
    ui->functionOptions->addItem("RandomOrderInt");
    ui->functionOptions->addItem("Randomizer");
    ui->functionOptions->addItem("RandomDensity");
    ui->functionOptions->addItem("Inverse");
    ui->functionOptions->addItem("LN");
    ui->functionOptions->addItem("Fibonacci");
    ui->functionOptions->addItem("Decay");
    ui->functionOptions->addItem("GetFromMarkovChain");
    ui->functionOptions->addItem("-------");

    // Stochos option initializations
    ui->functionOptions->addItem("Stochos");
    // connect(ui->stochosMethodRange, &QRadioButton::isChecked, ui->functionStackedWidget, &FunctionGenerator::stochosMethodRadioButtonClicked);
    // connect(ui->stochosMethodFunction, &QRadioButton::isChecked, ui->functionStackedWidget, &FunctionGenerator::stochosMethodRadioButtonClicked);
    // connect(ui->stochosAddNode, &QPushButton::clicked, ui->functionStackedWidget, &FunctionGenerator::stochosAddNodeButtonClicked);
    
    // ui->functionOptions->addItem("Select", fnSelect);
    // ui->functionOptions->addItem("ValuePick", fnValuePick);
    // ui->functionOptions->addItem("GetPattern", fnGetPattern);
    // ui->functionOptions->addItem("-------");
    // ui->functionOptions->addItem("CURRENT_TYPE", fnCurrType);
    // ui->functionOptions->addItem("CURRENT_CHILD_NUM", fnCurrChildNum);
    // ui->functionOptions->addItem("CURRENT_PARTIAL_NUM", fnCurrPartialNum);
    // ui->functionOptions->addItem("CURRENT_SEGMENT", fnCurrSegment);
    // ui->functionOptions->addItem("AVAILABLE_EDU", fnAvailEdu);
    // ui->functionOptions->addItem("PREVIOUS_CHILD_DURATION", fnPrevChildDur);
    // ui->functionOptions->addItem("CURRENT_LAYER", fnCurrLayer);

    //
    connect(ui->functionOptions, QOverload<int>::of(&QComboBox::currentIndexChanged),
        ui->functionStackedWidget, &QStackedWidget::setCurrentIndex);
    ui->functionOptions->setCurrentIndex(0);

    // Centers result label
    ui->resultLabel->setAlignment(Qt::AlignCenter);
    // Sets height of result box
    ui->resultTextEdit->setFixedHeight(100);
    // Connects the ui OK and CANCEL buttons to their corresponding actions
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
