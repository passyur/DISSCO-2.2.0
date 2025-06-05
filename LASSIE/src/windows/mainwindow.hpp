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
class MarkovWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // File operations
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();

    // Edit operations
    void undo();
    void redo();

    // Window operations
    void showEnvelopeLibraryWindow();
    void showMarkovWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    void readSettings();
    void writeSettings();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void loadFile(const QString &fileName);
    void saveFile(const QString &fileName);

    std::unique_ptr<Ui::MainWindow> ui;
    std::unique_ptr<EnvelopeLibraryWindow> envelopeLibraryWindow;
    std::unique_ptr<MarkovWindow> markovWindow;

    QString currentFile;
    
    // Actions
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *showEnvelopeLibraryAct;
    QAction *showMarkovAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    // Menus
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    // Toolbars
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
};

#endif // MAINWINDOW_HPP
