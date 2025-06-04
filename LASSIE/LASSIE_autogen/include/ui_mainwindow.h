/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionClose;
    QAction *actionExit;
    QAction *actionNewModel;
    QAction *actionOpenModel;
    QAction *actionSaveModel;
    QAction *actionCloseModel;
    QAction *actionAbout;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTreeWidget *treeWidget;
    QDockWidget *projectDockWidget;
    QDockWidget *modelLibraryDockWidget;
    QDockWidget *envelopeLibraryDockWidget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuModel;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName("actionNew");
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName("actionOpen");
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName("actionSave");
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName("actionClose");
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        actionNewModel = new QAction(MainWindow);
        actionNewModel->setObjectName("actionNewModel");
        actionOpenModel = new QAction(MainWindow);
        actionOpenModel->setObjectName("actionOpenModel");
        actionSaveModel = new QAction(MainWindow);
        actionSaveModel->setObjectName("actionSaveModel");
        actionCloseModel = new QAction(MainWindow);
        actionCloseModel->setObjectName("actionCloseModel");
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        treeWidget = new QTreeWidget(centralwidget);
        treeWidget->setObjectName("treeWidget");

        verticalLayout->addWidget(treeWidget);

        MainWindow->setCentralWidget(centralwidget);
        projectDockWidget = new QDockWidget(MainWindow);
        projectDockWidget->setObjectName("projectDockWidget");
        projectDockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        projectDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        MainWindow->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, projectDockWidget);
        modelLibraryDockWidget = new QDockWidget(MainWindow);
        modelLibraryDockWidget->setObjectName("modelLibraryDockWidget");
        modelLibraryDockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        modelLibraryDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        MainWindow->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, modelLibraryDockWidget);
        envelopeLibraryDockWidget = new QDockWidget(MainWindow);
        envelopeLibraryDockWidget->setObjectName("envelopeLibraryDockWidget");
        envelopeLibraryDockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        envelopeLibraryDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        MainWindow->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, envelopeLibraryDockWidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuModel = new QMenu(menubar);
        menuModel->setObjectName("menuModel");
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuModel->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionClose);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuModel->addAction(actionNewModel);
        menuModel->addAction(actionOpenModel);
        menuModel->addAction(actionSaveModel);
        menuModel->addAction(actionCloseModel);
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "LASSIE", nullptr));
        actionNew->setText(QCoreApplication::translate("MainWindow", "New", nullptr));
#if QT_CONFIG(shortcut)
        actionNew->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionOpen->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
#if QT_CONFIG(shortcut)
        actionOpen->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
#if QT_CONFIG(shortcut)
        actionSave->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionClose->setText(QCoreApplication::translate("MainWindow", "Close", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionNewModel->setText(QCoreApplication::translate("MainWindow", "New Model", nullptr));
        actionOpenModel->setText(QCoreApplication::translate("MainWindow", "Open Model", nullptr));
        actionSaveModel->setText(QCoreApplication::translate("MainWindow", "Save Model", nullptr));
        actionCloseModel->setText(QCoreApplication::translate("MainWindow", "Close Model", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        projectDockWidget->setWindowTitle(QCoreApplication::translate("MainWindow", "Project", nullptr));
        modelLibraryDockWidget->setWindowTitle(QCoreApplication::translate("MainWindow", "Model Library", nullptr));
        envelopeLibraryDockWidget->setWindowTitle(QCoreApplication::translate("MainWindow", "Envelope Library", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuModel->setTitle(QCoreApplication::translate("MainWindow", "Model", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
