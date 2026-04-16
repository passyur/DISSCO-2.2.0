#include "SPAPartialAlignment.hpp"
#include "../ui/ui_FunGenSPAPartialAlignment.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QDialogButtonBox>
// #include <QOverload>
#include <QDateTime>

#include <string>

#include "../dialogs/FunctionGenerator.hpp"
#include "../ui/ui_FunctionGenerator.h"

SPAPartialAlignment::SPAPartialAlignment(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SPAPartialAlignment)
{
    ui->setupUi(this); 
    setupUi(); 
    ui->spaPartialAlignmentLayout->setContentsMargins(10, 10, 10, 10);
    ui->spaPartialAlignmentLayout->setSpacing(10);
    prev = nullptr;
    next = nullptr;
}

SPAPartialAlignment::~SPAPartialAlignment()
{
    delete ui;
}

void SPAPartialAlignment::setupUi() {
    connect(ui->spaPartialFunButton, &QPushButton::clicked, this, &SPAPartialAlignment::funButtonClicked);
    connect(ui->spaPartialRemoveButton, &QPushButton::clicked, this, [this](){ emit removeRequested(this); });
    connect(ui->spaPartialInsertButton, &QPushButton::clicked, this, [this](){ emit insertRequested(this); });
    connect(ui->spaPartialEdit, &QLineEdit::textChanged, this, &SPAPartialAlignment::textChangedSignal);
    ui->spaPartialEdit->setFixedHeight(20);
}

void SPAPartialAlignment::funButtonClicked(){
  QString initialText = ui->spaPartialEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnENV, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->spaPartialEdit->setText(result);
    }
  }
  delete generator;
}

QString SPAPartialAlignment::textChanged() {
    return ui->spaPartialEdit->text();
}

void SPAPartialAlignment::setLabelText(const QString& text) {
    ui->spaPartialEditLabel->setText(text);
}

