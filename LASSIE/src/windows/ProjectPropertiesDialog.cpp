#include "ProjectPropertiesDialog.h"
#include "../ui/ui_ProjectPropertiesDialog.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QCheckBox>


ProjectPropertiesDialog::ProjectPropertiesDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProjectPropertiesDialog)
{
    // Sets up the user interface specified by the .ui file
    ui->setupUi(this);
    // Connects the ui OK and CANCEL buttons to their corresponding actions
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

ProjectPropertiesDialog::~ProjectPropertiesDialog()
{
    delete ui;
}