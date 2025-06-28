#ifndef SPAPARTIALALIGNMENT_HPP
#define SPAPARTIALALIGNMENT_HPP

#include "FunctionGenerator.hpp"

#include <QDialog>
#include <QWidget>
#include <QString>

class FunctionGenerator;

namespace Ui {
class SPAPartialAlignment;
}

class SPAPartialAlignment : public QWidget
{
    Q_OBJECT

public:
    /*Constructor to create the Function Generator Pop Up*/
    explicit SPAPartialAlignment(QWidget *parent = nullptr);
    /*Destructor to delete the UI*/
    ~SPAPartialAlignment();
    
    Ui::SPAPartialAlignment *ui;

    SPAPartialAlignment* next;
    SPAPartialAlignment* prev;

    QString textChanged();

private:
    /* storing function generator */
    //QDialog* funtionGenerator;
    void setupUi();
    void funButtonClicked();
    // void removePartialButtonClicked();
    // void insertPartialButtonClicked();
};

#endif // SPAPARTIALALIGNMENT_HPP