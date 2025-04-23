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
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EnvelopeLibraryWindow
{
public:
    QVBoxLayout *vbox1;
    QWidget *EnvelopeListAlignment;
    QVBoxLayout *verticalLayout;
    QTreeView *envelopeTreeView;
    QHBoxLayout *horizontalLayout;
    QPushButton *newButton;
    QPushButton *duplicateButton;
    QFrame *hseparator1;
    QHBoxLayout *hbox4;
    QLabel *ScaleLabel;
    QWidget *DrawingAreaAlignment;
    QVBoxLayout *verticalLayout_2;
    QWidget *drawingAreaWidget;
    QHBoxLayout *hbox2;
    QLabel *label2;
    QLineEdit *xValueEdit;
    QHBoxLayout *hbox3;
    QLabel *label3;
    QLineEdit *yValueEdit;
    QHBoxLayout *hbox1;
    QLabel *CoordinateLabel;
    QLabel *label1;

    void setupUi(QWidget *EnvelopeLibraryWindow)
    {
        if (EnvelopeLibraryWindow->objectName().isEmpty())
            EnvelopeLibraryWindow->setObjectName(QString::fromUtf8("EnvelopeLibraryWindow"));
        EnvelopeLibraryWindow->resize(600, 400);
        vbox1 = new QVBoxLayout(EnvelopeLibraryWindow);
        vbox1->setObjectName(QString::fromUtf8("vbox1"));
        EnvelopeListAlignment = new QWidget(EnvelopeLibraryWindow);
        EnvelopeListAlignment->setObjectName(QString::fromUtf8("EnvelopeListAlignment"));
        EnvelopeListAlignment->setMinimumSize(QSize(300, 300));
        verticalLayout = new QVBoxLayout(EnvelopeListAlignment);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        envelopeTreeView = new QTreeView(EnvelopeListAlignment);
        envelopeTreeView->setObjectName(QString::fromUtf8("envelopeTreeView"));
        envelopeTreeView->setHeaderHidden(true);
        envelopeTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
        envelopeTreeView->setDragEnabled(true);
        envelopeTreeView->setAcceptDrops(true);
        envelopeTreeView->setDropIndicatorShown(true);

        verticalLayout->addWidget(envelopeTreeView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        newButton = new QPushButton(EnvelopeListAlignment);
        newButton->setObjectName(QString::fromUtf8("newButton"));

        horizontalLayout->addWidget(newButton);

        duplicateButton = new QPushButton(EnvelopeListAlignment);
        duplicateButton->setObjectName(QString::fromUtf8("duplicateButton"));

        horizontalLayout->addWidget(duplicateButton);


        verticalLayout->addLayout(horizontalLayout);


        vbox1->addWidget(EnvelopeListAlignment);

        hseparator1 = new QFrame(EnvelopeLibraryWindow);
        hseparator1->setObjectName(QString::fromUtf8("hseparator1"));
        hseparator1->setFrameShape(QFrame::HLine);
        hseparator1->setFrameShadow(QFrame::Sunken);

        vbox1->addWidget(hseparator1);

        hbox4 = new QHBoxLayout();
        hbox4->setObjectName(QString::fromUtf8("hbox4"));
        ScaleLabel = new QLabel(EnvelopeLibraryWindow);
        ScaleLabel->setObjectName(QString::fromUtf8("ScaleLabel"));

        hbox4->addWidget(ScaleLabel);

        DrawingAreaAlignment = new QWidget(EnvelopeLibraryWindow);
        DrawingAreaAlignment->setObjectName(QString::fromUtf8("DrawingAreaAlignment"));
        DrawingAreaAlignment->setMinimumSize(QSize(280, 200));
        verticalLayout_2 = new QVBoxLayout(DrawingAreaAlignment);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        drawingAreaWidget = new QWidget(DrawingAreaAlignment);
        drawingAreaWidget->setObjectName(QString::fromUtf8("drawingAreaWidget"));

        verticalLayout_2->addWidget(drawingAreaWidget);


        hbox4->addWidget(DrawingAreaAlignment);


        vbox1->addLayout(hbox4);

        hbox2 = new QHBoxLayout();
        hbox2->setObjectName(QString::fromUtf8("hbox2"));
        label2 = new QLabel(EnvelopeLibraryWindow);
        label2->setObjectName(QString::fromUtf8("label2"));

        hbox2->addWidget(label2);

        xValueEdit = new QLineEdit(EnvelopeLibraryWindow);
        xValueEdit->setObjectName(QString::fromUtf8("xValueEdit"));
        xValueEdit->setMinimumWidth(100);

        hbox2->addWidget(xValueEdit);


        vbox1->addLayout(hbox2);

        hbox3 = new QHBoxLayout();
        hbox3->setObjectName(QString::fromUtf8("hbox3"));
        label3 = new QLabel(EnvelopeLibraryWindow);
        label3->setObjectName(QString::fromUtf8("label3"));

        hbox3->addWidget(label3);

        yValueEdit = new QLineEdit(EnvelopeLibraryWindow);
        yValueEdit->setObjectName(QString::fromUtf8("yValueEdit"));
        yValueEdit->setMinimumWidth(100);

        hbox3->addWidget(yValueEdit);


        vbox1->addLayout(hbox3);

        hbox1 = new QHBoxLayout();
        hbox1->setObjectName(QString::fromUtf8("hbox1"));
        CoordinateLabel = new QLabel(EnvelopeLibraryWindow);
        CoordinateLabel->setObjectName(QString::fromUtf8("CoordinateLabel"));

        hbox1->addWidget(CoordinateLabel);

        label1 = new QLabel(EnvelopeLibraryWindow);
        label1->setObjectName(QString::fromUtf8("label1"));

        hbox1->addWidget(label1);


        vbox1->addLayout(hbox1);


        retranslateUi(EnvelopeLibraryWindow);

        QMetaObject::connectSlotsByName(EnvelopeLibraryWindow);
    } // setupUi

    void retranslateUi(QWidget *EnvelopeLibraryWindow)
    {
        newButton->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "New", nullptr));
        duplicateButton->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "Duplicate", nullptr));
        ScaleLabel->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "1.0\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"0.0", nullptr));
        label2->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "X value:", nullptr));
        label3->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "Y value: ", nullptr));
        CoordinateLabel->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "X:0.0\n"
"Y:0.0", nullptr));
        label1->setText(QCoreApplication::translate("EnvelopeLibraryWindow", "Right click the graph to see available actions or click-and-drag a node to adjust the envelope.\n"
"Thick segment = Flexible; Thin segment = Fixed.\n"
"Blue = Linear; Green = Spline; Red = Exponential.", nullptr));
        (void)EnvelopeLibraryWindow;
    } // retranslateUi

};

namespace Ui {
    class EnvelopeLibraryWindow: public Ui_EnvelopeLibraryWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENVELOPELIBRARYWINDOW_H
