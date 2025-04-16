/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *projectTab;
    QWidget *envelopeTab;
    QWidget *markovTab;
    QWidget *attributesTab;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1200, 800);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        projectTab = new QWidget();
        projectTab->setObjectName(QString::fromUtf8("projectTab"));
        tabWidget->addTab(projectTab, QString());
        envelopeTab = new QWidget();
        envelopeTab->setObjectName(QString::fromUtf8("envelopeTab"));
        tabWidget->addTab(envelopeTab, QString());
        markovTab = new QWidget();
        markovTab->setObjectName(QString::fromUtf8("markovTab"));
        tabWidget->addTab(markovTab, QString());
        attributesTab = new QWidget();
        attributesTab->setObjectName(QString::fromUtf8("attributesTab"));
        tabWidget->addTab(attributesTab, QString());

        verticalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1200, 24));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "LASSIE", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(projectTab), QCoreApplication::translate("MainWindow", "Project", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(envelopeTab), QCoreApplication::translate("MainWindow", "Envelopes", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(markovTab), QCoreApplication::translate("MainWindow", "Markov Models", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(attributesTab), QCoreApplication::translate("MainWindow", "Event Attributes", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
