#ifndef SPAPARTIALALIGNMENT_HPP
#define SPAPARTIALALIGNMENT_HPP

#include "../dialogs/FunctionGenerator.hpp"

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
    void setLabelText(const QString& text);

signals:
    void removeRequested(SPAPartialAlignment* node);
    void insertRequested(SPAPartialAlignment* node);
    void textChangedSignal();

private:
    /* storing function generator */
    //QDialog* funtionGenerator;
    void setupUi();
    void funButtonClicked();
    // void removePartialButtonClicked();
    // void insertPartialButtonClicked();
};

#endif // SPAPARTIALALIGNMENT_HPP