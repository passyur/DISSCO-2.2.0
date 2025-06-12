#ifndef MARKOVWINDOW_HPP
#define MARKOVWINDOW_HPP

#include "mainwindow.hpp"

#include <QWidget>

class MarkovWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MarkovWindow(QWidget *parent = nullptr);
};

#endif // MARKOVWINDOW_HPP
