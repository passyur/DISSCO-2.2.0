// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
QT_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU
    //! [0]

    //! [1]
private slots:
    void newFile();
    void open();
    void save();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void about();
    void aboutQt();
    //! [1]

    //! [2]
private:
    void createActions();
    void createMenus();
    //! [2]

    //! [3]
    // Main *mmenu;
    
    QMenu *file_menu;
    QMenu *edit_menu;
    QMenu *help_menu;
    QAction *new_act;
    QAction *open_act;
    QAction *save_act;
    QAction *exit_act;
    QAction *undo_act;
    QAction *redo_act;
    QAction *cut_act;
    QAction *copy_act;
    QAction *paste_act;
    QAction *about_act;
    QAction *about_qt_act;
    QLabel *info_label;
};
//! [3]

#endif
// #ifndef MAINWINDOW_H
// #define MAINWINDOW_H

// #include <QMainWindow>

// QT_BEGIN_NAMESPACE
// namespace Ui {
// class MainWindow;
// }
// QT_END_NAMESPACE

// class MainWindow : public QMainWindow
// {
//     Q_OBJECT

// public:
//     MainWindow(QWidget *parent = nullptr);
//     ~MainWindow();

// private slots:
//     void newFile();
//     // void open();
//     // void save();
//     // void undo();
//     // void redo();
//     // void about();
//     // void aboutQt();

// private:
//     Ui::MainWindow *ui;
//     void createActions();
//     void createMenus();

//     QMenu *file_menu;
//     // QMenu *edit_menu;
//     // QMenu *project_menu;
//     // QMenu *help_menu;
//     // QActionGroup *alignment_group;
//     QAction *new_act;
//     // QAction *open_act;
//     // QAction *save_act;
//     // QAction *save_as_act;
//     QAction *quit_act;
//     // QAction *create_act;
//     // QAction *properties_act;
//     // QAction *run_act;
//     // QAction *conf_mod_act;
//     // QAction *contents_act;
//     // QAction *about_act;
//     // QAction *about_qt_act;
// };
// #endif // MAINWINDOW_H
