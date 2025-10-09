#include "Modifiers.hpp"
#include "../ui/ui_Modifiers.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QDialogButtonBox>
// #include <QOverload>
#include <QDateTime>

#include <string>

#include "EventAttributesViewController.hpp"
#include "../ui/ui_Attributes.h"

Modifiers::Modifiers(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Modifiers)
{
    ui->setupUi(this); 
    setupUi(); 
}

Modifiers::~Modifiers()
{
    delete ui;
}

void Modifiers::setupUi() {
    // connect(ui->revRoomSizeFunButton, &QPushButton::clicked, this, &REVPartialAlignment::roomSizeFunButtonClicked);
}

