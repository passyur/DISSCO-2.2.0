#ifndef FILENEWOBJECT_HPP
#define FILENEWOBJECT_HPP

#include <QDialog>

namespace Ui {
class FileNewObject;
}

class FileNewObject : public QDialog
{
    Q_OBJECT

public:
    /*Constructor to create the New Object Dialog Interface*/
    explicit FileNewObject(QWidget *parent = nullptr);
    /*Destructor to delete the UI*/
    ~FileNewObject();
    
    Ui::FileNewObject *ui;
    
    // Pre-select the radio button matching typeStr and clear the name field.
    // If typeStr is "Top", clears all selections. Assumes non-empty string.
    void setDefaultType(const QString& typeStr);

public slots:
    void accept() override;
};

#endif // FILENEWOBJECT_HPP
