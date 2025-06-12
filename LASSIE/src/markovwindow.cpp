#include "markovwindow.hpp"
#include <QLabel>
#include <QVBoxLayout>

MarkovWindow::MarkovWindow(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("This is the Markov Window!", this);
    layout->addWidget(label);
    setLayout(layout);
    setWindowTitle("Markov");
    resize(300, 200);
}
