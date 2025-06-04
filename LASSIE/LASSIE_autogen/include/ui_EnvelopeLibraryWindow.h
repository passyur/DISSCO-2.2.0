/********************************************************************************
** Form generated from reading UI file 'EnvelopeLibraryWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENVELOPELIBRARYWINDOW_H
#define UI_ENVELOPELIBRARYWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "EnvLibDrawingArea.hpp"

QT_BEGIN_NAMESPACE

class Ui_EnvelopeLibraryWindow
{
public:
    QAction *actionNew;
    QAction *actionDuplicate;
    QAction *actionDelete;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QWidget *leftWidget;
    QVBoxLayout *leftLayout;
    QLineEdit *searchEdit;
    QTreeView *envelopeTreeView;
    QHBoxLayout *buttonLayout;
    QPushButton *newButton;
    QPushButton *duplicateButton;
    QWidget *rightWidget;
    QVBoxLayout *rightLayout;
    QLabel *itemLabel;
    QWidget *itemViewWidget;
    QWidget *drawingAreaWidget;
    QGridLayout *valueLayout;
    QLabel *xLabel;
    QLineEdit *xValueEdit;
    QLabel *yLabel;
    QLineEdit *yValueEdit;
    QMenuBar *menubar;
    QMenu *menuEnvelope;
    QToolBar *toolBar;

    void setupUi(QMainWindow *EnvelopeLibraryWindow)
    {
        if (EnvelopeLibraryWindow->objectName().isEmpty())
            EnvelopeLibraryWindow->setObjectName("EnvelopeLibraryWindow");
        EnvelopeLibraryWindow->resize(800, 600);
        actionNew = new QAction(EnvelopeLibraryWindow);
        actionNew->setObjectName("actionNew");
        actionDuplicate = new QAction(EnvelopeLibraryWindow);
        actionDuplicate->setObjectName("actionDuplicate");
        actionDelete = new QAction(EnvelopeLibraryWindow);
        actionDelete->setObjectName("actionDelete");
        centralwidget = new QWidget(EnvelopeLibraryWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Horizontal);
        leftWidget = new QWidget(splitter);
        leftWidget->setObjectName("leftWidget");
        leftLayout = new QVBoxLayout(leftWidget);
        leftLayout->setObjectName("leftLayout");
        leftLayout->setContentsMargins(0, 0, 0, 0);
        searchEdit = new QLineEdit(leftWidget);
        searchEdit->setObjectName("searchEdit");

        leftLayout->addWidget(searchEdit);

        envelopeTreeView = new QTreeView(leftWidget);
        envelopeTreeView->setObjectName("envelopeTreeView");

        leftLayout->addWidget(envelopeTreeView);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        newButton = new QPushButton(leftWidget);
        newButton->setObjectName("newButton");

        buttonLayout->addWidget(newButton);

        duplicateButton = new QPushButton(leftWidget);
        duplicateButton->setObjectName("duplicateButton");

        buttonLayout->addWidget(duplicateButton);


        leftLayout->addLayout(buttonLayout);

        splitter->addWidget(leftWidget);
        rightWidget = new QWidget(splitter);
        rightWidget->setObjectName("rightWidget");
        rightLayout = new QVBoxLayout(rightWidget);
        rightLayout->setObjectName("rightLayout");
        rightLayout->setContentsMargins(0, 0, 0, 0);
        itemLabel = new QLabel(rightWidget);
        itemLabel->setObjectName("itemLabel");

        rightLayout->addWidget(itemLabel);

        itemViewWidget = new QWidget(rightWidget);
        itemViewWidget->setObjectName("itemViewWidget");

        rightLayout->addWidget(itemViewWidget);

        drawingAreaWidget = new QWidget(rightWidget);
        drawingAreaWidget->setObjectName("drawingAreaWidget");

        rightLayout->addWidget(drawingAreaWidget);

        valueLayout = new QGridLayout();
        valueLayout->setObjectName("valueLayout");
        xLabel = new QLabel(rightWidget);
        xLabel->setObjectName("xLabel");

        valueLayout->addWidget(xLabel, 0, 0, 1, 1);

        xValueEdit = new QLineEdit(rightWidget);
        xValueEdit->setObjectName("xValueEdit");

        valueLayout->addWidget(xValueEdit, 0, 1, 1, 1);

        yLabel = new QLabel(rightWidget);
        yLabel->setObjectName("yLabel");

        valueLayout->addWidget(yLabel, 1, 0, 1, 1);

        yValueEdit = new QLineEdit(rightWidget);
        yValueEdit->setObjectName("yValueEdit");

        valueLayout->addWidget(yValueEdit, 1, 1, 1, 1);


        rightLayout->addLayout(valueLayout);

        splitter->addWidget(rightWidget);

        horizontalLayout->addWidget(splitter);

        EnvelopeLibraryWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(EnvelopeLibraryWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 24));
        menuEnvelope = new QMenu(menubar);
        menuEnvelope->setObjectName("menuEnvelope");
        EnvelopeLibraryWindow->setMenuBar(menubar);
        toolBar = new QToolBar(EnvelopeLibraryWindow);
        toolBar->setObjectName("toolBar");
        EnvelopeLibraryWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        menubar->addAction(menuEnvelope->menuAction());
        menuEnvelope->addAction(actionNew);
        menuEnvelope->addAction(actionDuplicate);
        menuEnvelope->addAction(actionDelete);
        toolBar->addAction(actionNew);
        toolBar->addAction(actionDuplicate);
        toolBar->addAction(actionDelete);

        retranslateUi(EnvelopeLibraryWindow);

        QMetaObject::connectSlotsByName(EnvelopeLibraryWindow);
    } // setupUi

    void retranslateUi(QMainWindow *EnvelopeLibraryWindow)
    {
        EnvelopeLibraryWindow->setWindowTitle(QCoreApplication::translate("EnvelopeLibraryWindow", "Envelope Library", nullptr));
        actionNew->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "New", nullptr));
#if QT_CONFIG(shortcut)
        actionNew->setShortcut(QCoreApplication::translate("EnvelopeLibraryWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionDuplicate->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "Duplicate", nullptr));
#if QT_CONFIG(shortcut)
        actionDuplicate->setShortcut(QCoreApplication::translate("EnvelopeLibraryWindow", "Ctrl+D", nullptr));
#endif // QT_CONFIG(shortcut)
        actionDelete->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "Delete", nullptr));
#if QT_CONFIG(shortcut)
        actionDelete->setShortcut(QCoreApplication::translate("EnvelopeLibraryWindow", "Del", nullptr));
#endif // QT_CONFIG(shortcut)
        searchEdit->setPlaceholderText(QCoreApplication::translate("EnvelopeLibraryWindow", "Search envelopes...", nullptr));
        newButton->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "New", nullptr));
        duplicateButton->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "Duplicate", nullptr));
        itemLabel->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "Item View", nullptr));
        xLabel->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "X:", nullptr));
        yLabel->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "Y:", nullptr));
        menuEnvelope->setTitle(QCoreApplication::translate("EnvelopeLibraryWindow", "Envelope", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("EnvelopeLibraryWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EnvelopeLibraryWindow: public Ui_EnvelopeLibraryWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENVELOPELIBRARYWINDOW_H
