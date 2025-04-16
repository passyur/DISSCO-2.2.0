#include <QApplication>
#include <QFile>
#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application information
    QApplication::setApplicationName("LASSIE");
    QApplication::setApplicationVersion("2.2.0");
    QApplication::setOrganizationName("DISSCO");
    QApplication::setOrganizationDomain("dissco.sourceforge.net");

    // Load and apply stylesheet
    QFile styleFile(":/styles/main.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        app.setStyleSheet(styleSheet);
    }

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
