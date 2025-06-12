#include "envelopewindow.hpp"
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>

EnvelopeWindow::EnvelopeWindow(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("This is the Envelope Window!", this);
    layout->addWidget(label);
    setLayout(layout);
    setWindowTitle("Envelope");
    resize(300, 200);
}
