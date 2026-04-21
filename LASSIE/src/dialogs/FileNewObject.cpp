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

void FileNewObject::setDefaultType(const QString& typeStr)
{
    ui->objNameEntry->clear();

    if (typeStr == "Top") {
        // Top events cannot be created via New Object; clear any prior selection
        QList<QRadioButton*> buttons = {
            ui->buttonHigh, ui->buttonMid, ui->buttonLow, ui->buttonBottom,
            ui->buttonSpectrum, ui->buttonNote, ui->buttonEnv, ui->buttonSiv,
            ui->buttonSpa, ui->buttonPat, ui->buttonRev, ui->buttonFil, ui->buttonMea
        };
        for (QRadioButton* btn : buttons)
            btn->setAutoExclusive(false);
        for (QRadioButton* btn : buttons) {
            btn->setChecked(false);
            btn->setAutoExclusive(true);
        }
        return;
    }

    if      (typeStr == "High")           ui->buttonHigh->setChecked(true);
    else if (typeStr == "Mid")            ui->buttonMid->setChecked(true);
    else if (typeStr == "Low")            ui->buttonLow->setChecked(true);
    else if (typeStr == "Bottom")         ui->buttonBottom->setChecked(true);
    else if (typeStr == "Spectrum")       ui->buttonSpectrum->setChecked(true);
    else if (typeStr == "Note")           ui->buttonNote->setChecked(true);
    else if (typeStr == "Envelope")       ui->buttonEnv->setChecked(true);
    else if (typeStr == "Sieve")          ui->buttonSiv->setChecked(true);
    else if (typeStr == "Spatialization") ui->buttonSpa->setChecked(true);
    else if (typeStr == "Pattern")        ui->buttonPat->setChecked(true);
    else if (typeStr == "Reverb")         ui->buttonRev->setChecked(true);
    else if (typeStr == "Filter")         ui->buttonFil->setChecked(true);
    else if (typeStr == "Measurement")    ui->buttonMea->setChecked(true);
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