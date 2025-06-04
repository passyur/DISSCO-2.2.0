/********************************************************************************
** Form generated from reading UI file 'ProjectViewController.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECTVIEWCONTROLLER_H
#define UI_PROJECTVIEWCONTROLLER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProjectViewController
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *searchEdit;
    QTreeView *projectTreeView;
    QHBoxLayout *buttonLayout;
    QPushButton *newButton;
    QPushButton *openButton;
    QPushButton *saveButton;
    QPushButton *closeButton;

    void setupUi(QWidget *ProjectViewController)
    {
        if (ProjectViewController->objectName().isEmpty())
            ProjectViewController->setObjectName("ProjectViewController");
        ProjectViewController->resize(400, 600);
        verticalLayout = new QVBoxLayout(ProjectViewController);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        searchEdit = new QLineEdit(ProjectViewController);
        searchEdit->setObjectName("searchEdit");

        horizontalLayout->addWidget(searchEdit);


        verticalLayout->addLayout(horizontalLayout);

        projectTreeView = new QTreeView(ProjectViewController);
        projectTreeView->setObjectName("projectTreeView");

        verticalLayout->addWidget(projectTreeView);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        newButton = new QPushButton(ProjectViewController);
        newButton->setObjectName("newButton");

        buttonLayout->addWidget(newButton);

        openButton = new QPushButton(ProjectViewController);
        openButton->setObjectName("openButton");

        buttonLayout->addWidget(openButton);

        saveButton = new QPushButton(ProjectViewController);
        saveButton->setObjectName("saveButton");

        buttonLayout->addWidget(saveButton);

        closeButton = new QPushButton(ProjectViewController);
        closeButton->setObjectName("closeButton");

        buttonLayout->addWidget(closeButton);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(ProjectViewController);

        QMetaObject::connectSlotsByName(ProjectViewController);
    } // setupUi

    void retranslateUi(QWidget *ProjectViewController)
    {
        ProjectViewController->setWindowTitle(QCoreApplication::translate("ProjectViewController", "Project View", nullptr));
        searchEdit->setPlaceholderText(QCoreApplication::translate("ProjectViewController", "Search projects...", nullptr));
        newButton->setText(QCoreApplication::translate("ProjectViewController", "New", nullptr));
        openButton->setText(QCoreApplication::translate("ProjectViewController", "Open", nullptr));
        saveButton->setText(QCoreApplication::translate("ProjectViewController", "Save", nullptr));
        closeButton->setText(QCoreApplication::translate("ProjectViewController", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProjectViewController: public Ui_ProjectViewController {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECTVIEWCONTROLLER_H
