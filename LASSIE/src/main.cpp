#include "mainwindow.hpp"
#include "inst.hpp"

#include <QApplication>
#include <QFile>
#include "windows/mainwindow.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Inst *m = Inst::instance();
    MainWindow *w = new MainWindow(m);
    w->show();
    return a.exec();
}
