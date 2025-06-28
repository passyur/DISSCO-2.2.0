#ifndef REVPARTIALALIGNMENT_H
#define REVPARTIALALIGNMENT_H

#include "LASSIE.h"

#include "FunctionGenerator.hpp"

#include <QDialog>
#include <QWidget>
#include <QString>

class FunctionGenerator;

namespace Ui {
class REVPartialAlignment;
}

class REVPartialAlignment : public QWidget
{
    Q_OBJECT

public:
    /*Constructor to create the Function Generator Pop Up*/
    explicit REVPartialAlignment(QWidget *parent = nullptr);
    /*Destructor to delete the UI*/
    ~REVPartialAlignment();
    
    Ui::REVPartialAlignment *ui;

    void appendNewNode(REVPartialAlignment* _newNode);
    REVPartialAlignment* next;
    REVPartialAlignment* prev;

    QString roomSizeTextChanged();

private:
    /* storing function generator */
    //QDialog* funtionGenerator;
    void setupUi();
    void roomSizeFunButtonClicked();
};

#endif // REVPARTIALALIGNMENT_H 