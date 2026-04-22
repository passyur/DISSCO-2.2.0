#include "FileNewObject.hpp"
#include "../ui/ui_FileNewObject.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QMessageBox>


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

void FileNewObject::setExistingNames(const QMap<QString, QStringList>& names)
{
    m_existingNames = names;
}

void FileNewObject::accept()
{
    bool typeSelected =
        ui->buttonHigh->isChecked() || ui->buttonMid->isChecked() || ui->buttonLow->isChecked()
        || ui->buttonBottom->isChecked() || ui->buttonSpectrum->isChecked() || ui->buttonNote->isChecked()
        || ui->buttonEnv->isChecked() || ui->buttonSiv->isChecked() || ui->buttonSpa->isChecked()
        || ui->buttonPat->isChecked() || ui->buttonRev->isChecked() || ui->buttonFil->isChecked()
        || ui->buttonMea->isChecked();

    if (!typeSelected || ui->objNameEntry->text().simplified().isEmpty())
        return;

    QString typeStr;
    if      (ui->buttonHigh->isChecked())     typeStr = "High";
    else if (ui->buttonMid->isChecked())      typeStr = "Mid";
    else if (ui->buttonLow->isChecked())      typeStr = "Low";
    else if (ui->buttonBottom->isChecked())   typeStr = "Bottom";
    else if (ui->buttonSpectrum->isChecked()) typeStr = "Spectrum";
    else if (ui->buttonNote->isChecked())     typeStr = "Note";
    else if (ui->buttonEnv->isChecked())      typeStr = "Envelope";
    else if (ui->buttonSiv->isChecked())      typeStr = "Sieve";
    else if (ui->buttonSpa->isChecked())      typeStr = "Spatialization";
    else if (ui->buttonPat->isChecked())      typeStr = "Pattern";
    else if (ui->buttonRev->isChecked())      typeStr = "Reverb";
    else if (ui->buttonFil->isChecked())      typeStr = "Filter";
    else if (ui->buttonMea->isChecked())      typeStr = "Measurement";

    QString name = ui->objNameEntry->text().simplified();
    if (m_existingNames.contains(typeStr) && m_existingNames[typeStr].contains(name)) {
        QMessageBox::warning(this, "Duplicate Name",
            QString("A %1 event named \"%2\" already exists. Please choose a different name.")
                .arg(typeStr, name));
        return;
    }

    QDialog::accept();
}