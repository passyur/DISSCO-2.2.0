// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <memory>
#include <QTextEdit>
// #include <QtLogging>
#include <QStatusBar>

#include "../inst.hpp"
#include "ProjectViewController.hpp" //nhi:  use ProjectViewController.hpp instead of projectview.hpp

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class EnvelopeLibraryWindow;
class MarkovModelLibraryWindow; //nhi: more descriptive than MarkovWindow
class ProjectView;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        //MainWindow(QWidget *parent = nullptr);
        MainWindow(Inst*);
        static MainWindow* instance() { return instance_; }
        ~MainWindow();
        void setUnsavedTitle(QString unsavedFile);

        std::unique_ptr<Ui::MainWindow> ui;
        std::unique_ptr<EnvelopeLibraryWindow> envelopeLibraryWindow;
        std::unique_ptr<MarkovModelLibraryWindow> markovWindow;

    public slots:
        void showStatusMessage(const QString& message);

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
        void showPropertiesDialog();
        void showFileNewObjectDialog();

    protected:
        void closeEvent(QCloseEvent *event) override;
        void readSettings();
        void writeSettings();

    private:
        void createActions();
        void enableProjectActions(bool enabled);
        void createMenus();
        void createToolBars();
        void createStatusBar();
        void loadFile(const QString &fileName);
        void saveFile(const QString &fileName);


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

        QAction *newObjAct;
        QAction *setPropAct;
        QAction *runAct;
        QAction *configNoteModAct;

        QAction *showEnvelopeLibraryAct;
        QAction *showMarkovAct;
        QAction *aboutAct;
        QAction *aboutQtAct;

        // Menus
        QMenu *fileMenu;
        QMenu *editMenu;
        QMenu *projectMenu;
        QMenu *viewMenu;
        QMenu *helpMenu;

        // Toolbars
        QToolBar *fileToolBar;
        QToolBar *projectToolBar;
        QToolBar *editToolBar;

        QStatusBar *statusbar_;
        
        // projectView pointer for testing
        std::vector<ProjectView*> projects;
        ProjectView* projectView;
        
        Inst *main_;
        static MainWindow *instance_;
};

#endif // MAINWINDOW_HPP
