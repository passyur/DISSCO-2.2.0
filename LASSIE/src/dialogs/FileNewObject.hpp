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
    
public slots:
    void accept() override;
};

#endif // FILENEWOBJECT_HPP