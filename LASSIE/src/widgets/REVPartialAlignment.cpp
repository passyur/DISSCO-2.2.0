#include "REVPartialAlignment.hpp"
#include "../ui/ui_FunGenREVPartialAlignment.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QDialogButtonBox>
// #include <QOverload>
#include <QDateTime>

#include "../ui/ui_FunctionGenerator.h"

REVPartialAlignment::REVPartialAlignment(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::REVPartialAlignment)
{
    ui->setupUi(this); 
    setupUi(); 
    prev = NULL;
    next = NULL;
}

REVPartialAlignment::~REVPartialAlignment()
{
    delete ui;
}

void REVPartialAlignment::setupUi() {
    connect(ui->revRoomSizeFunButton, &QPushButton::clicked, this, &REVPartialAlignment::roomSizeFunButtonClicked);
}

void REVPartialAlignment::roomSizeFunButtonClicked(){
  QString initialText = ui->revRoomSizeEdit->text();
  FunctionGenerator* generator = new FunctionGenerator(this, functionReturnFloat, initialText);
  if (generator->exec() == QDialog::Accepted) {
    QString result = generator->getResultString();
    if (!result.isEmpty()) {
        ui->revRoomSizeEdit->setText(result);
    }
  }
  delete generator;
}

QString REVPartialAlignment::roomSizeTextChanged() {
    return ui->revRoomSizeEdit->text();
}

void REVPartialAlignment::appendNewNode(REVPartialAlignment* _newNode) {
  if (next == NULL){
    next = _newNode;
    _newNode->prev = this;
  } else {
    next-> appendNewNode(_newNode);
  }
}

