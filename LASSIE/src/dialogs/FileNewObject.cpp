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