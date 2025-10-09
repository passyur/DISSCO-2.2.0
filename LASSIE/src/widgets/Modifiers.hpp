#ifndef MODIFIERS_HPP
#define MODIFIERS_HPP


#include "EventAttributesViewController.hpp"

#include <QWidget>
#include <QString>

class EventAttributesViewController;

namespace Ui {
class Modifiers;
}

class Modifiers : public QWidget
{
    Q_OBJECT

public:
    /*Constructor to create the modifier*/
    explicit Modifiers(QWidget *parent = nullptr);
    /*Destructor to delete the UI*/
    ~Modifiers();
    
    Ui::Modifiers *ui;

private:
    void setupUi();
};

#endif // MODIFIERS_HPP