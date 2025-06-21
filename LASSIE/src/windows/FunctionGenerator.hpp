#ifndef FUNCTIONGENERATOR_H
#define FUNCTIONGENERATOR_H

#include <QDialog>
#include <QWidget>
#include <QString>

namespace Ui {
class FunctionGenerator;
}

class FunctionGenerator : public QDialog
{
    Q_OBJECT

public:
    /*Constructor to create the Function Generator Pop Up*/
    explicit FunctionGenerator(QWidget *parent = nullptr);
    /*Destructor to delete the UI*/
    ~FunctionGenerator();
    
    Ui::FunctionGenerator *ui;

private:
    void setupUi();
    
};

#endif // FUNCTIONGENERATOR_H 