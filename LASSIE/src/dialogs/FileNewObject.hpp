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
    
    // Pre-select the radio button matching typeStr and clear the name field
    // assumes that it is passed a non-empty string (if so, does nothing)
    void setDefaultType(const QString& typeStr);

public slots:
    void accept() override;
};

#endif // FILENEWOBJECT_HPP
