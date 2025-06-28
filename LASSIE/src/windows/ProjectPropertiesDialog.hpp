#ifndef PROJECTPROPERTIESDIALOG_HPP
#define PROJECTPROPERTIESDIALOG_HPP

#include <QDialog>

namespace Ui {
class ProjectPropertiesDialog;
}

class ProjectPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    /*Constructor to create the Set Properties / Project Properties Dialog Interface*/
    explicit ProjectPropertiesDialog(QWidget *parent = nullptr);
    /*Destructor to delete the UI*/
    ~ProjectPropertiesDialog();
    
    Ui::ProjectPropertiesDialog *ui;
    
};

#endif // PROJECTPROPERTIESDIALOG_HPP