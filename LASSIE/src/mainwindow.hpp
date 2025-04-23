// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QTabWidget>
#include <memory>
#include <vector>
#include <QSharedPointer>
#include "ProjectViewController.hpp"
#include "MarkovModelLibraryWindow.hpp"
#include <QCloseEvent>
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class EnvelopeLibraryWindow;
class MarkovModelLibraryWindow;
class ProjectViewController;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showEnvelopeLibraryWindow();
    void showMarkovModelLibraryWindow();
    void setUnsavedTitle();
    void setSavedTitle();
    void disableNewAndOpenProject();

protected:
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void readSettings();
    void writeSettings();
    void updateWindowState();
    
    // File menu actions
    void menuFileNewProject();
    void menuFileNewObject();
    void menuFileOpen();
    void menuFileOpenXML();
    void menuFileSave();
    void menuFileSaveAs();
    void menuFileClose();
    void menuFilePrint();
    void menuFileQuit();
    
    // Edit menu actions
    void menuEditUndo();
    void menuEditRedo();
    void menuEditCut();
    void menuEditCopy();
    void menuEditPaste();
    void menuEditSelectAll();
    void menuEditPreferences();
    
    // Help menu actions
    void menuHelpContents();
    void menuHelpAbout();
    
    // Project actions
    void menuProjectProperties();
    void menuProjectRun();
    void menuProjectGenerateSCFile();
    void menuProjectConfigureNoteModifiers();

    void onNewProjectClicked();
    void onOpenProjectClicked();
    void onSaveProjectClicked();
    void onCloseProjectClicked();
    void onNewModelClicked();
    void onOpenModelClicked();
    void onSaveModelClicked();
    void onCloseModelClicked();
    void onAboutClicked();

    // Project view controller signals
    void onProjectCreated();
    void onProjectOpened();
    void onProjectClosed();

    void onNewProject();
    void onOpenProject();
    void onSaveProject();
    void onCloseProject();
    
    void onNewModel();
    void onOpenModel();
    void onSaveModel();
    void onCloseModel();
    
    void onAbout();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createNewProject();
    void changeCurrentProjectViewTo(ProjectViewController* newProject);
    bool checkTerm(const QString& command);
    QString getTerm();

    Ui::MainWindow *ui;
    std::unique_ptr<EnvelopeLibraryWindow> envelopeLibraryWindow;
    std::unique_ptr<MarkovModelLibraryWindow> markovModelLibraryWindow;
    
    std::vector<std::unique_ptr<ProjectViewController>> projects;
    ProjectViewController* currentProjectView;
    QTabWidget* projectTabWidget;

    // Menus
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QMenu *projectMenu;

    // Toolbars
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *projectToolBar;

    // Actions
    QAction *newProjectAct;
    QAction *newObjectAct;
    QAction *openAct;
    QAction *openXMLAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *closeAct;
    QAction *printAct;
    QAction *exitAct;
    
    QAction *undoAct;
    QAction *redoAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *selectAllAct;
    QAction *preferencesAct;
    
    QAction *projectPropertiesAct;
    QAction *projectRunAct;
    QAction *projectGenerateSCAct;
    QAction *projectConfigureNoteModifiersAct;
    
    QAction *showEnvelopeLibraryAct;
    QAction *showMarkovModelLibraryAct;
    
    QAction *helpContentsAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    QSharedPointer<ProjectViewController> projectViewController;
    QSharedPointer<MarkovModelLibraryWindow> modelLibraryWindow;

    QStringList recentFiles;
};

#endif // MAINWINDOW_HPP
