// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtWidgets>

#include "mainwindow.hpp"

//! [0]
MainWindow::MainWindow()
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);
    //! [0]

    //! [1]
    QWidget *top_filler = new QWidget;
    top_filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    info_label = new QLabel(tr("<i>Choose a menu option, or right-click to "
                              "invoke a context menu</i>"));
    info_label->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    info_label->setAlignment(Qt::AlignCenter);

    QWidget *bottom_filler = new QWidget;
    bottom_filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(5, 5, 5, 5);
    layout->addWidget(top_filler);
    layout->addWidget(info_label);
    layout->addWidget(bottom_filler);
    widget->setLayout(layout);
    //! [1]

    //! [2]
    createActions();
    createMenus();

    QString message = tr("A context menu is available by right-clicking");
    statusBar()->showMessage(message);

    setWindowTitle(tr("Menus"));
    setMinimumSize(160, 160);
    resize(480, 320);
}
//! [2]

//! [3]
#ifndef QT_NO_CONTEXTMENU
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(cut_act);
    menu.addAction(copy_act);
    menu.addAction(paste_act);
    menu.exec(event->globalPos());
}
#endif // QT_NO_CONTEXTMENU
//! [3]

void MainWindow::newFile()
{
    info_label->setText(tr("Invoked <b>File|New</b>"));
}

void MainWindow::open()
{
    info_label->setText(tr("Invoked <b>File|Open</b>"));
}

void MainWindow::save()
{
    info_label->setText(tr("Invoked <b>File|Save</b>"));
}

void MainWindow::undo()
{
    info_label->setText(tr("Invoked <b>Edit|Undo</b>"));
}

void MainWindow::redo()
{
    info_label->setText(tr("Invoked <b>Edit|Redo</b>"));
}

void MainWindow::cut()
{
    info_label->setText(tr("Invoked <b>Edit|Cut</b>"));
}

void MainWindow::copy()
{
    info_label->setText(tr("Invoked <b>Edit|Copy</b>"));
}

void MainWindow::paste()
{
    info_label->setText(tr("Invoked <b>Edit|Paste</b>"));
}

void MainWindow::about()
{
    info_label->setText(tr("Invoked <b>Help|About</b>"));
    QMessageBox::about(this, tr("About Menu"),
                       tr("This is a prototype of the LASSIE UI for DISSCO rewritten in Qt."));
}

void MainWindow::aboutQt()
{
    info_label->setText(tr("Invoked <b>Help|About Qt</b>"));
}

//! [4]
void MainWindow::createActions()
{
    //! [5]
    new_act = new QAction(QIcon::fromTheme("document-new"),
                         tr("&New"), this);
    new_act->setShortcuts(QKeySequence::New);
    new_act->setStatusTip(tr("Create a new file"));
    connect(new_act, &QAction::triggered, this, &MainWindow::newFile);
    //! [4]

    open_act = new QAction(QIcon::fromTheme("document-open"),
                          tr("&Open..."), this);
    open_act->setShortcuts(QKeySequence::Open);
    open_act->setStatusTip(tr("Open an existing file"));
    connect(open_act, &QAction::triggered, this, &MainWindow::open);
    //! [5]

    save_act = new QAction(QIcon::fromTheme("document-save"),
                          tr("&Save"), this);
    save_act->setShortcuts(QKeySequence::Save);
    save_act->setStatusTip(tr("Save the document to disk"));
    connect(save_act, &QAction::triggered, this, &MainWindow::save);

    exit_act = new QAction(QIcon::fromTheme("application-exit"),
                          tr("E&xit"), this);
    exit_act->setShortcuts(QKeySequence::Quit);
    exit_act->setStatusTip(tr("Exit the application"));
    connect(exit_act, &QAction::triggered, this, &QWidget::close);

    undo_act = new QAction(QIcon::fromTheme("edit-undo"),
                          tr("&Undo"), this);
    undo_act->setShortcuts(QKeySequence::Undo);
    undo_act->setStatusTip(tr("Undo the last operation"));
    connect(undo_act, &QAction::triggered, this, &MainWindow::undo);

    redo_act = new QAction(QIcon::fromTheme("edit-redo"),
                          tr("&Redo"), this);
    redo_act->setShortcuts(QKeySequence::Redo);
    redo_act->setStatusTip(tr("Redo the last operation"));
    connect(redo_act, &QAction::triggered, this, &MainWindow::redo);

    cut_act = new QAction(QIcon::fromTheme("edit-cut"),
                         tr("Cu&t"), this);
    cut_act->setShortcuts(QKeySequence::Cut);
    cut_act->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cut_act, &QAction::triggered, this, &MainWindow::cut);

    copy_act = new QAction(QIcon::fromTheme("edit-copy"),
                          tr("&Copy"), this);
    copy_act->setShortcuts(QKeySequence::Copy);
    copy_act->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copy_act, &QAction::triggered, this, &MainWindow::copy);

    paste_act = new QAction(QIcon::fromTheme("edit-paste"),
                           tr("&Paste"), this);
    paste_act->setShortcuts(QKeySequence::Paste);
    paste_act->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(paste_act, &QAction::triggered, this, &MainWindow::paste);

    about_act = new QAction(QIcon::fromTheme("help-about"),
                           tr("&About"), this);
    about_act->setStatusTip(tr("Show the application's About box"));
    connect(about_act, &QAction::triggered, this, &MainWindow::about);

    about_qt_act = new QAction(tr("About &Qt"), this);
    about_qt_act->setStatusTip(tr("Show the Qt library's About box"));
    connect(about_qt_act, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(about_qt_act, &QAction::triggered, this, &MainWindow::aboutQt);
}
//! [7]

//! [8]
void MainWindow::createMenus()
{
    //! [9] //! [10]
    file_menu = menuBar()->addMenu(tr("&File"));
    file_menu->addAction(new_act);
    //! [9]
    file_menu->addAction(open_act);
    //! [10]
    file_menu->addAction(save_act);
    //! [11]
    file_menu->addSeparator();
    //! [11]
    file_menu->addAction(exit_act);

    edit_menu = menuBar()->addMenu(tr("&Edit"));
    edit_menu->addAction(undo_act);
    edit_menu->addAction(redo_act);
    edit_menu->addSeparator();
    edit_menu->addAction(cut_act);
    edit_menu->addAction(copy_act);
    edit_menu->addAction(paste_act);
    edit_menu->addSeparator();

    help_menu = menuBar()->addMenu(tr("&Help"));
    help_menu->addAction(about_act);
    help_menu->addAction(about_qt_act);
    //! [8]

}
//! [12]

// #include "mainwindow.h"
// #include "./ui_mainwindow.h"

// #include <QIcon>
// #include <QAction>
// #include <QToolbar>
// #include <QMenuBar>

// MainWindow::MainWindow(QWidget *parent)
//     : QMainWindow(parent)
//     , ui(new Ui::MainWindow)
// {
//     createActions();
//     /* v depends on ^ */
//     createMenus();

//     setWindowTitle("Menu");
//     // setMinimumSize(160, 160);
//     // QAction *exit_act;
//     // file_menu->addAction(exit_act);
//     // menu->addAction("something");

//     // QToolBar *toolbar = addToolBar("main");
//     // toolbar->addAction("File");
//     // toolbar->addAction("Edit");
//     // toolbar->addAction("Project");
//     // toolbar->addAction("Help");
//     ui->setupUi(this);
// }

// MainWindow::~MainWindow()
// {
//     delete ui;
// }

// void MainWindow::newFile(){

// }

// /* Will instantiate the actions to be added to menus in createMenus() */
// void MainWindow::createActions(){
//     new_act = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew),
//                           tr("&New"), this);
//     new_act->setShortcuts(QKeySequence::New);
//     new_act->setStatusTip(tr("Create a new project"));
//     connect(new_act, &QAction::triggered, this, &MainWindow::newFile);

//     // alignment_group = new QActionGroup(this);
//     // alignment_group->addAction()

//     // QAction *open_act;
//     // QAction *save_act;
//     // QAction *save_as_act;
//     // quit_act = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::ApplicationExit),
//     //                        tr("&Quit"), this);
//     // quit_act->setShortcuts(QKeySequence::Quit);
//     // quit_act->setStatusTip(tr("Quit DISSCO"));
//     // connect(quit_act, &QAction::triggered, this, &QApplication::quit);
//     // QAction *create_act;
//     // QAction *properties_act;
//     // QAction *run_act;
//     // QAction *conf_mod_act;
//     // QAction *contents_act;
//     // QAction *about_act;
//     // QAction *about_qt_act;
// }

// /* Will instantiate the menus in the menubar */
// void MainWindow::createMenus(){
//     file_menu = menuBar()->addMenu(tr("&File"));
//     // try{

//     // } catch (std::exception &e) {
//     //     qFatal("Error %s creating menu %s with act %s",
//     //            e.what(), typeid(*menu).name(),
//     //            typeid(*action).name());
//     // }

//     file_menu->addAction(new_act);
//     // file_menu->addSeparator();
//     // file_menu->addAction(quit_act);

//     // QMenu *edit_menu;
//     // QMenu *project_menu;
//     // QMenu *help_menu;
// }

