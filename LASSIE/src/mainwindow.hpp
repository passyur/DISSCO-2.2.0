// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class EnvelopeLibraryWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showEnvelopeLibraryWindow();
    void readSettings();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

    std::unique_ptr<Ui::MainWindow> ui;
    std::unique_ptr<EnvelopeLibraryWindow> envelopeLibraryWindow;

    // Menus
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    // Toolbars
    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    // Actions
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *showEnvelopeLibraryAct;
};

#endif // MAINWINDOW_HPP
