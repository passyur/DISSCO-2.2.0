/********************************************************************************
** Form generated from reading UI file 'MarkovModelLibraryWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MARKOVMODELLIBRARYWINDOW_H
#define UI_MARKOVMODELLIBRARYWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MarkovModelLibraryWindow
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionClose;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *searchEdit;
    QPushButton *newButton;
    QPushButton *openButton;
    QPushButton *saveButton;
    QPushButton *closeButton;
    QSplitter *splitter;
    QTreeView *modelTreeView;
    QWidget *modelViewWidget;
    QVBoxLayout *modelViewLayout;
    QLabel *modelLabel;
    QMenuBar *menubar;
    QMenu *menuModel;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MarkovModelLibraryWindow)
    {
        if (MarkovModelLibraryWindow->objectName().isEmpty())
            MarkovModelLibraryWindow->setObjectName("MarkovModelLibraryWindow");
        MarkovModelLibraryWindow->resize(800, 600);
        actionNew = new QAction(MarkovModelLibraryWindow);
        actionNew->setObjectName("actionNew");
        actionOpen = new QAction(MarkovModelLibraryWindow);
        actionOpen->setObjectName("actionOpen");
        actionSave = new QAction(MarkovModelLibraryWindow);
        actionSave->setObjectName("actionSave");
        actionClose = new QAction(MarkovModelLibraryWindow);
        actionClose->setObjectName("actionClose");
        centralwidget = new QWidget(MarkovModelLibraryWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        searchEdit = new QLineEdit(centralwidget);
        searchEdit->setObjectName("searchEdit");

        horizontalLayout->addWidget(searchEdit);

        newButton = new QPushButton(centralwidget);
        newButton->setObjectName("newButton");

        horizontalLayout->addWidget(newButton);

        openButton = new QPushButton(centralwidget);
        openButton->setObjectName("openButton");

        horizontalLayout->addWidget(openButton);

        saveButton = new QPushButton(centralwidget);
        saveButton->setObjectName("saveButton");

        horizontalLayout->addWidget(saveButton);

        closeButton = new QPushButton(centralwidget);
        closeButton->setObjectName("closeButton");

        horizontalLayout->addWidget(closeButton);


        verticalLayout->addLayout(horizontalLayout);

        splitter = new QSplitter(centralwidget);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Horizontal);
        modelTreeView = new QTreeView(splitter);
        modelTreeView->setObjectName("modelTreeView");
        splitter->addWidget(modelTreeView);
        modelViewWidget = new QWidget(splitter);
        modelViewWidget->setObjectName("modelViewWidget");
        modelViewLayout = new QVBoxLayout(modelViewWidget);
        modelViewLayout->setObjectName("modelViewLayout");
        modelViewLayout->setContentsMargins(0, 0, 0, 0);
        modelLabel = new QLabel(modelViewWidget);
        modelLabel->setObjectName("modelLabel");

        modelViewLayout->addWidget(modelLabel);

        splitter->addWidget(modelViewWidget);

        verticalLayout->addWidget(splitter);

        MarkovModelLibraryWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MarkovModelLibraryWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 24));
        menuModel = new QMenu(menubar);
        menuModel->setObjectName("menuModel");
        MarkovModelLibraryWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MarkovModelLibraryWindow);
        statusbar->setObjectName("statusbar");
        MarkovModelLibraryWindow->setStatusBar(statusbar);

        menubar->addAction(menuModel->menuAction());
        menuModel->addAction(actionNew);
        menuModel->addAction(actionOpen);
        menuModel->addAction(actionSave);
        menuModel->addAction(actionClose);

        retranslateUi(MarkovModelLibraryWindow);

        QMetaObject::connectSlotsByName(MarkovModelLibraryWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MarkovModelLibraryWindow)
    {
        MarkovModelLibraryWindow->setWindowTitle(QCoreApplication::translate("MarkovModelLibraryWindow", "Markov Model Library", nullptr));
        actionNew->setText(QCoreApplication::translate("MarkovModelLibraryWindow", "New", nullptr));
#if QT_CONFIG(shortcut)
        actionNew->setShortcut(QCoreApplication::translate("MarkovModelLibraryWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionOpen->setText(QCoreApplication::translate("MarkovModelLibraryWindow", "Open", nullptr));
#if QT_CONFIG(shortcut)
        actionOpen->setShortcut(QCoreApplication::translate("MarkovModelLibraryWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave->setText(QCoreApplication::translate("MarkovModelLibraryWindow", "Save", nullptr));
#if QT_CONFIG(shortcut)
        actionSave->setShortcut(QCoreApplication::translate("MarkovModelLibraryWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionClose->setText(QCoreApplication::translate("MarkovModelLibraryWindow", "Close", nullptr));
        searchEdit->setPlaceholderText(QCoreApplication::translate("MarkovModelLibraryWindow", "Search models...", nullptr));
        newButton->setText(QCoreApplication::translate("MarkovModelLibraryWindow", "New", nullptr));
        openButton->setText(QCoreApplication::translate("MarkovModelLibraryWindow", "Open", nullptr));
        saveButton->setText(QCoreApplication::translate("MarkovModelLibraryWindow", "Save", nullptr));
        closeButton->setText(QCoreApplication::translate("MarkovModelLibraryWindow", "Close", nullptr));
        modelLabel->setText(QCoreApplication::translate("MarkovModelLibraryWindow", "Model View", nullptr));
        menuModel->setTitle(QCoreApplication::translate("MarkovModelLibraryWindow", "Model", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MarkovModelLibraryWindow: public Ui_MarkovModelLibraryWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MARKOVMODELLIBRARYWINDOW_H
