#include "FileNewObject.hpp"
#include "../ui/ui_FileNewObject.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>


FileNewObject::FileNewObject(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FileNewObject)
{
    // Sets up the user interface specified by the .ui file
    ui->setupUi(this);
    // Connects the ui OK and CANCEL buttons to their corresponding actions
    connect(ui->objButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->objButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

FileNewObject::~FileNewObject()
{
    delete ui;
}

void FileNewObject::accept()
{
    if(
        (ui->buttonHigh->isChecked() || ui->buttonMid->isChecked() || ui->buttonLow->isChecked()
        || ui->buttonBottom->isChecked() || ui->buttonSpectrum->isChecked() || ui->buttonNote->isChecked()
        || ui->buttonEnv->isChecked() || ui->buttonSiv->isChecked() || ui->buttonSpa->isChecked()
        || ui->buttonPat->isChecked() || ui->buttonRev->isChecked() || ui->buttonFil->isChecked()
        || ui->buttonMea->isChecked())
      && 
        !(ui->objNameEntry->text().simplified().isEmpty())
    )
        QDialog::accept();
    else
        return;
}