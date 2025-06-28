#include "SPAPartialAlignment.hpp"
#include "../ui/ui_FunGenSPAPartialAlignment.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QOverload>
#include <QDateTime>

#include <string>

#include "FunctionGenerator.hpp"
#include "../ui/ui_FunctionGenerator.h"

SPAPartialAlignment::SPAPartialAlignment(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SPAPartialAlignment)
{
    ui->setupUi(this); 
    setupUi(); 
    prev = NULL;
    next = NULL;
}

SPAPartialAlignment::~SPAPartialAlignment()
{
    delete ui;
}

void SPAPartialAlignment::setupUi() {
    connect(ui->spaPartialFunButton, &QPushButton::clicked, this, &SPAPartialAlignment::funButtonClicked);
    //connect(ui->spaPartialRemoveButton, &QPushButton::clicked, this, &SPAPartialAlignment::removePartialButtonClicked);
    //connect(ui->spaPartialInsertButton, &QPushButton::clicked, this, &SPAPartialAlignment::insertPartialButtonClicked);
    //connect(ui->spaPartialEnvelopeEdit, &QLineEdit::textChanged, this, &SPAPartialAlignment::textChanged);
}

void SPAPartialAlignment::funButtonClicked(){
  QString initialText = ui->spaPartialEnvelopeEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnENV, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->spaPartialEnvelopeEdit->setText(result);
    }
  }
  delete generator;
}

// void SPAPartialAlignment::removePartialButtonClicked(){
//     parent->SPARemovePartial(this);
// }

// void SPAPartialAlignment::insertPartialButtonClicked(){
//     parent->SPAInsertPartial(this);
// }

QString SPAPartialAlignment::textChanged() {
    return ui->spaPartialEnvelopeEdit->text();
}

