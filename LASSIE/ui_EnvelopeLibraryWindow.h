/********************************************************************************
** Form generated from reading UI file 'EnvelopeLibraryWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENVELOPELIBRARYWINDOW_H
#define UI_ENVELOPELIBRARYWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
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
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EnvelopeLibraryWindow
{
public:
    QAction *actionSave;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QWidget *leftWidget;
    QVBoxLayout *verticalLayout;
    QTreeView *envelopeTreeView;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *newButton;
    QPushButton *duplicateButton;
    QWidget *rightWidget;
    QVBoxLayout *verticalLayout_2;
    QWidget *drawingAreaWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *xValueEdit;
    QLabel *label_2;
    QLineEdit *yValueEdit;
    QLabel *coordinateLabel;
    QLabel *scaleLabel;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *EnvelopeLibraryWindow)
    {
        if (EnvelopeLibraryWindow->objectName().isEmpty())
            EnvelopeLibraryWindow->setObjectName(QString::fromUtf8("EnvelopeLibraryWindow"));
        EnvelopeLibraryWindow->resize(800, 600);
        actionSave = new QAction(EnvelopeLibraryWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        centralwidget = new QWidget(EnvelopeLibraryWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        leftWidget = new QWidget(splitter);
        leftWidget->setObjectName(QString::fromUtf8("leftWidget"));
        verticalLayout = new QVBoxLayout(leftWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        envelopeTreeView = new QTreeView(leftWidget);
        envelopeTreeView->setObjectName(QString::fromUtf8("envelopeTreeView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(envelopeTreeView->sizePolicy().hasHeightForWidth());
        envelopeTreeView->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(envelopeTreeView);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        newButton = new QPushButton(leftWidget);
        newButton->setObjectName(QString::fromUtf8("newButton"));

        horizontalLayout_2->addWidget(newButton);

        duplicateButton = new QPushButton(leftWidget);
        duplicateButton->setObjectName(QString::fromUtf8("duplicateButton"));

        horizontalLayout_2->addWidget(duplicateButton);


        verticalLayout->addLayout(horizontalLayout_2);

        splitter->addWidget(leftWidget);
        rightWidget = new QWidget(splitter);
        rightWidget->setObjectName(QString::fromUtf8("rightWidget"));
        verticalLayout_2 = new QVBoxLayout(rightWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        drawingAreaWidget = new QWidget(rightWidget);
        drawingAreaWidget->setObjectName(QString::fromUtf8("drawingAreaWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(drawingAreaWidget->sizePolicy().hasHeightForWidth());
        drawingAreaWidget->setSizePolicy(sizePolicy1);

        verticalLayout_2->addWidget(drawingAreaWidget);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(rightWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        xValueEdit = new QLineEdit(rightWidget);
        xValueEdit->setObjectName(QString::fromUtf8("xValueEdit"));

        gridLayout->addWidget(xValueEdit, 0, 1, 1, 1);

        label_2 = new QLabel(rightWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        yValueEdit = new QLineEdit(rightWidget);
        yValueEdit->setObjectName(QString::fromUtf8("yValueEdit"));

        gridLayout->addWidget(yValueEdit, 0, 3, 1, 1);

        coordinateLabel = new QLabel(rightWidget);
        coordinateLabel->setObjectName(QString::fromUtf8("coordinateLabel"));

        gridLayout->addWidget(coordinateLabel, 1, 0, 1, 4);

        scaleLabel = new QLabel(rightWidget);
        scaleLabel->setObjectName(QString::fromUtf8("scaleLabel"));

        gridLayout->addWidget(scaleLabel, 2, 0, 1, 4);


        verticalLayout_2->addLayout(gridLayout);

        splitter->addWidget(rightWidget);

        horizontalLayout->addWidget(splitter);

        EnvelopeLibraryWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(EnvelopeLibraryWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 24));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        EnvelopeLibraryWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(EnvelopeLibraryWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        EnvelopeLibraryWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionSave);

        retranslateUi(EnvelopeLibraryWindow);

        QMetaObject::connectSlotsByName(EnvelopeLibraryWindow);
    } // setupUi

    void retranslateUi(QMainWindow *EnvelopeLibraryWindow)
    {
        EnvelopeLibraryWindow->setWindowTitle(QCoreApplication::translate("EnvelopeLibraryWindow", "Envelope Library", nullptr));
        actionSave->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "Save", nullptr));
#if QT_CONFIG(shortcut)
        actionSave->setShortcut(QCoreApplication::translate("EnvelopeLibraryWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        newButton->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "New", nullptr));
        duplicateButton->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "Duplicate", nullptr));
        label->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "X:", nullptr));
        label_2->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "Y:", nullptr));
        coordinateLabel->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "Mouse: (0.000, 0.000)", nullptr));
        scaleLabel->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "Scale: 0.000 - 1.000", nullptr));
        menuFile->setTitle(QCoreApplication::translate("EnvelopeLibraryWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EnvelopeLibraryWindow: public Ui_EnvelopeLibraryWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENVELOPELIBRARYWINDOW_H
