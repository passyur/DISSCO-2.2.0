// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTextEdit>
#include <QtLogging>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU

private slots:
    /*** FILE ACTIONS ***/
    /* should open a dialog to save a new file to disk */
    void newFile();
    /* should open a dialog to open a .dissco file already on disk */
    void open();
    /* should save the current project, only if there is a file */
    void save();
    /* should open a dialog to save the current file at a particular filepath, only if there is a file */
    // void saveAs();

    /*** EDIT ACTIONS ***/
    /* following two should undo/redo the previous action/undo performed in the current view, only if there is a previous action/undo */
    void undo();
    void redo();
    /* following three should do as name suggests using system clipboard, only if there is content/access to system clipboard */
    void cut();
    void copy();
    void paste();

    /*** HELP ACTIONS ***/
    void about();
    void aboutQt();

    /*** LIBRARY ACTIONS ***/
    void markov();
    void envelope();

private:
    /* The following two f'ns should populate the menus and the actions those menus contain, e.g., the "File" menu to contain the actions "New File", "Save As", etc. */
    void createActions();
    void createMenus();
    
    /* loadFile should take a const reference to a valid file_path and attempt to open it with ReadOnly and Text flags, err out if it cannot, parse the .dissco file, and open the project view for that .dissco file. */
    void loadFile(const QString &file_path);

    // void saveToFile(const QString &file_path);

    QMenu *file_menu;
    QMenu *edit_menu;
    QMenu *help_menu;
    QMenu *libraries_menu;
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
    QAction *markov_act;
    QAction *envelope_act;

    QString current_file_;
    QTextEdit *text_edit_;
};
//! [3]

#endif
