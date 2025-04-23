#include <QtWidgets>

#include "mainwindow.hpp"
#include "EnvelopeLibraryWindow.h"
#include "MarkovModelLibraryWindow.h"
#include "ProjectViewController.h"
#include "UI/mainwindow.ui"

#include <QApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QTabWidget>
#include <QCloseEvent>
#include <QSettings>
#include <QStyle>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , envelopeLibraryWindow(new EnvelopeLibraryWindow(this))
    , markovModelLibraryWindow(new MarkovModelLibraryWindow(this))
    , projectViewController(new ProjectViewController(this))
{
    setupUi();
    
    // Set up dock widgets
    ui->projectDockWidget->setWidget(projectViewController);
    ui->modelLibraryDockWidget->setWidget(markovModelLibraryWindow);
    ui->envelopeLibraryDockWidget->setWidget(envelopeLibraryWindow);
    
    // Connect menu actions
    connect(ui->actionNewProject, &QAction::triggered, this, &MainWindow::onNewProject);
    connect(ui->actionOpenProject, &QAction::triggered, this, &MainWindow::onOpenProject);
    connect(ui->actionSaveProject, &QAction::triggered, this, &MainWindow::onSaveProject);
    connect(ui->actionCloseProject, &QAction::triggered, this, &MainWindow::onCloseProject);
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    
    connect(ui->actionNewModel, &QAction::triggered, this, &MainWindow::onNewModel);
    connect(ui->actionOpenModel, &QAction::triggered, this, &MainWindow::onOpenModel);
    connect(ui->actionSaveModel, &QAction::triggered, this, &MainWindow::onSaveModel);
    connect(ui->actionCloseModel, &QAction::triggered, this, &MainWindow::onCloseModel);
    
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);
    
    // Read settings and restore window state
    readSettings();
    
    // Update initial window state
    updateWindowState();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void MainWindow::readSettings()
{
    QSettings settings("DISSCO", "MarkovModelEditor");
    
    // Restore geometry and window state
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    
    // Restore recent files
    recentFiles = settings.value("recentFiles").toStringList();
}

void MainWindow::writeSettings()
{
    QSettings settings("DISSCO", "MarkovModelEditor");
    
    // Save geometry and window state
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    
    // Save recent files
    settings.setValue("recentFiles", recentFiles);
}

void MainWindow::updateWindowState()
{
    bool hasProject = projectViewController->hasCurrentProject();
    bool hasModel = markovModelLibraryWindow->hasCurrentModel();
    
    // Update menu actions based on project state
    ui->actionSaveProject->setEnabled(hasProject);
    ui->actionCloseProject->setEnabled(hasProject);
    
    // Update model actions based on project state
    ui->actionSaveModel->setEnabled(hasModel);
    ui->actionCloseModel->setEnabled(hasModel);
}

void MainWindow::onNewProject()
{
    QString filePath = QFileDialog::getSaveFileName(this,
        tr("New Project"), QString(),
        tr("Project Files (*.proj);;All Files (*)"));
        
    if (!filePath.isEmpty()) {
        projectViewController->createNewProject(filePath);
        recentFiles.prepend(filePath);
        updateWindowState();
    }
}

void MainWindow::onOpenProject()
{
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Open Project"), QString(),
        tr("Project Files (*.proj);;All Files (*)"));
        
    if (!filePath.isEmpty()) {
        projectViewController->openProject(filePath);
        recentFiles.prepend(filePath);
        updateWindowState();
    }
}

void MainWindow::onSaveProject()
{
    projectViewController->saveCurrentProject();
}

void MainWindow::onCloseProject()
{
    projectViewController->closeCurrentProject();
    updateWindowState();
}

void MainWindow::onNewModel()
{
    markovModelLibraryWindow->createNewModel();
}

void MainWindow::onOpenModel()
{
    markovModelLibraryWindow->openModel();
}

void MainWindow::onSaveModel()
{
    markovModelLibraryWindow->saveCurrentModel();
}

void MainWindow::onCloseModel()
{
    markovModelLibraryWindow->closeCurrentModel();
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, tr("About DISSCO"),
        tr("DISSCO - Markov Model Editor\n\n"
           "Version 2.2.0\n\n"
           "A tool for creating and editing Markov models for sound synthesis."));
}

void MainWindow::showEnvelopeLibraryWindow()
{
    envelopeLibraryWindow->show();
    envelopeLibraryWindow->raise();
    envelopeLibraryWindow->activateWindow();
}

void MainWindow::showMarkovModelLibraryWindow()
{
    markovModelLibraryWindow->show();
    markovModelLibraryWindow->raise();
    markovModelLibraryWindow->activateWindow();
}

void MainWindow::setUnsavedTitle()
{
    setWindowTitle(tr("*LASSIE - Unsaved Changes"));
}

void MainWindow::setSavedTitle()
{
    setWindowTitle(tr("LASSIE"));
}

void MainWindow::disableNewAndOpenProject()
{
    ui->actionNewProject->setEnabled(false);
    ui->actionOpenProject->setEnabled(false);
    ui->actionOpenXMLAct->setEnabled(false);
}

void MainWindow::setupUi()
{
    QUiLoader loader;
    QFile file(":/UI/mainwindow.ui");
    file.open(QFile::ReadOnly);
    QWidget *formWidget = loader.load(&file, this);
    file.close();
    
    if (formWidget) {
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(formWidget);
        setLayout(layout);
    }
}

void MainWindow::createActions()
{
    // File actions
    ui->actionNewProject = new QAction(tr("&New Project"), this);
    ui->actionNewProject->setShortcuts(QKeySequence::New);
    ui->actionNewProject->setStatusTip(tr("Create a new project"));
    connect(ui->actionNewProject, &QAction::triggered, this, &MainWindow::onNewProjectClicked);
    
    ui->actionOpenProject = new QAction(tr("&Open..."), this);
    ui->actionOpenProject->setShortcuts(QKeySequence::Open);
    ui->actionOpenProject->setStatusTip(tr("Open an existing project"));
    connect(ui->actionOpenProject, &QAction::triggered, this, &MainWindow::onOpenProjectClicked);
    
    ui->actionSaveProject = new QAction(tr("&Save"), this);
    ui->actionSaveProject->setShortcuts(QKeySequence::Save);
    ui->actionSaveProject->setStatusTip(tr("Save the project"));
    connect(ui->actionSaveProject, &QAction::triggered, this, &MainWindow::onSaveProjectClicked);
    
    ui->actionCloseProject = new QAction(tr("&Close"), this);
    ui->actionCloseProject->setStatusTip(tr("Close the current project"));
    connect(ui->actionCloseProject, &QAction::triggered, this, &MainWindow::onCloseProjectClicked);
    
    ui->actionExit = new QAction(tr("E&xit"), this);
    ui->actionExit->setShortcuts(QKeySequence::Quit);
    ui->actionExit->setStatusTip(tr("Exit the application"));
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);

    // Edit actions
    ui->actionNewModel = new QAction(tr("New &Model"), this);
    ui->actionNewModel->setStatusTip(tr("Create a new model"));
    connect(ui->actionNewModel, &QAction::triggered, this, &MainWindow::onNewModelClicked);
    
    ui->actionOpenModel = new QAction(tr("Open &Model"), this);
    ui->actionOpenModel->setStatusTip(tr("Open an existing model"));
    connect(ui->actionOpenModel, &QAction::triggered, this, &MainWindow::onOpenModelClicked);
    
    ui->actionSaveModel = new QAction(tr("Save &Model"), this);
    ui->actionSaveModel->setStatusTip(tr("Save the current model"));
    connect(ui->actionSaveModel, &QAction::triggered, this, &MainWindow::onSaveModelClicked);
    
    ui->actionCloseModel = new QAction(tr("Close &Model"), this);
    ui->actionCloseModel->setStatusTip(tr("Close the current model"));
    newProjectAct = new QAction(tr("&New Project"), this);
    newProjectAct->setShortcuts(QKeySequence::New);
    newProjectAct->setStatusTip(tr("Create a new project"));
    connect(newProjectAct, &QAction::triggered, this, &MainWindow::menuFileNewProject);
    
    newObjectAct = new QAction(tr("New &Object"), this);
    newObjectAct->setStatusTip(tr("Create a new object"));
    connect(newObjectAct, &QAction::triggered, this, &MainWindow::menuFileNewObject);
    
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing project"));
    connect(openAct, &QAction::triggered, this, &MainWindow::menuFileOpen);
    
    openXMLAct = new QAction(tr("Open &XML..."), this);
    openXMLAct->setStatusTip(tr("Open an XML project"));
    connect(openXMLAct, &QAction::triggered, this, &MainWindow::menuFileOpenXML);
    
    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the project"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::menuFileSave);
    
    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the project under a new name"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::menuFileSaveAs);
    
    closeAct = new QAction(tr("&Close"), this);
    closeAct->setStatusTip(tr("Close the current project"));
    connect(closeAct, &QAction::triggered, this, &MainWindow::menuFileClose);
    
    printAct = new QAction(tr("&Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the current project"));
    connect(printAct, &QAction::triggered, this, &MainWindow::menuFilePrint);
    
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    // Edit actions
    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last action"));
    connect(undoAct, &QAction::triggered, this, &MainWindow::menuEditUndo);
    
    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last undone action"));
    connect(redoAct, &QAction::triggered, this, &MainWindow::menuEditRedo);
    
    cutAct = new QAction(tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
    connect(cutAct, &QAction::triggered, this, &MainWindow::menuEditCut);
    
    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
    connect(copyAct, &QAction::triggered, this, &MainWindow::menuEditCopy);
    
    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
    connect(pasteAct, &QAction::triggered, this, &MainWindow::menuEditPaste);
    
    selectAllAct = new QAction(tr("Select &All"), this);
    selectAllAct->setShortcuts(QKeySequence::SelectAll);
    selectAllAct->setStatusTip(tr("Select all items"));
    connect(selectAllAct, &QAction::triggered, this, &MainWindow::menuEditSelectAll);
    
    preferencesAct = new QAction(tr("&Preferences..."), this);
    preferencesAct->setStatusTip(tr("Edit application preferences"));
    connect(preferencesAct, &QAction::triggered, this, &MainWindow::menuEditPreferences);

    // Project actions
    projectPropertiesAct = new QAction(tr("&Properties..."), this);
    projectPropertiesAct->setStatusTip(tr("Edit project properties"));
    connect(projectPropertiesAct, &QAction::triggered, this, &MainWindow::menuProjectProperties);
    
    projectRunAct = new QAction(tr("&Run"), this);
    projectRunAct->setStatusTip(tr("Run the current project"));
    connect(projectRunAct, &QAction::triggered, this, &MainWindow::menuProjectRun);
    
    projectGenerateSCAct = new QAction(tr("Generate &SC File..."), this);
    projectGenerateSCAct->setStatusTip(tr("Generate SuperCollider file"));
    connect(projectGenerateSCAct, &QAction::triggered, this, &MainWindow::menuProjectGenerateSCFile);
    
    projectConfigureNoteModifiersAct = new QAction(tr("Configure &Note Modifiers..."), this);
    projectConfigureNoteModifiersAct->setStatusTip(tr("Configure note modifiers"));
    connect(projectConfigureNoteModifiersAct, &QAction::triggered, this, &MainWindow::menuProjectConfigureNoteModifiers);

    // View actions
    showEnvelopeLibraryAct = new QAction(tr("&Envelope Library"), this);
    showEnvelopeLibraryAct->setStatusTip(tr("Show the envelope library window"));
    connect(showEnvelopeLibraryAct, &QAction::triggered, this, &MainWindow::showEnvelopeLibraryWindow);
    
    showMarkovModelLibraryAct = new QAction(tr("&Markov Model Library"), this);
    showMarkovModelLibraryAct->setStatusTip(tr("Show the Markov model library window"));
    connect(showMarkovModelLibraryAct, &QAction::triggered, this, &MainWindow::showMarkovModelLibraryWindow);

    // Help actions
    helpContentsAct = new QAction(tr("&Contents"), this);
    helpContentsAct->setStatusTip(tr("Show the help contents"));
    connect(helpContentsAct, &QAction::triggered, this, &MainWindow::menuHelpContents);
    
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, tr("About LASSIE"),
            tr("LASSIE (Library for Algorithmic Sound Synthesis and Interactive Exploration) "
               "is a tool for creating and manipulating sound synthesis algorithms."));
    });

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newProjectAct);
    fileMenu->addAction(newObjectAct);
    fileMenu->addSeparator();
    fileMenu->addAction(openAct);
    fileMenu->addAction(openXMLAct);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(closeAct);
    fileMenu->addSeparator();
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();
    editMenu->addAction(selectAllAct);
    editMenu->addSeparator();
    editMenu->addAction(preferencesAct);

    projectMenu = menuBar()->addMenu(tr("&Project"));
    projectMenu->addAction(projectPropertiesAct);
    projectMenu->addSeparator();
    projectMenu->addAction(projectRunAct);
    projectMenu->addAction(projectGenerateSCAct);
    projectMenu->addAction(projectConfigureNoteModifiersAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(showEnvelopeLibraryAct);
    viewMenu->addAction(showMarkovModelLibraryAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpContentsAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newProjectAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(undoAct);
    editToolBar->addAction(redoAct);
    editToolBar->addSeparator();
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);

    projectToolBar = addToolBar(tr("Project"));
    projectToolBar->addAction(projectRunAct);
    projectToolBar->addAction(projectGenerateSCAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

// File menu actions implementation
void MainWindow::menuFileNewProject()
{
    createNewProject();
}

void MainWindow::menuFileNewObject()
{
    // TODO: Implement new object creation
}

void MainWindow::menuFileOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"),
        QString(), tr("LASSIE Projects (*.lassie);;All Files (*)"));
    if (!fileName.isEmpty()) {
        // TODO: Implement project opening
    }
}

void MainWindow::menuFileOpenXML()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open XML Project"),
        QString(), tr("XML Files (*.xml);;All Files (*)"));
    if (!fileName.isEmpty()) {
        // TODO: Implement XML project opening
    }
}

void MainWindow::menuFileSave()
{
    // TODO: Implement project saving
}

void MainWindow::menuFileSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project As"),
        QString(), tr("LASSIE Projects (*.lassie);;All Files (*)"));
    if (!fileName.isEmpty()) {
        // TODO: Implement project saving
    }
}

void MainWindow::menuFileClose()
{
    if (currentProjectView) {
        int index = projectTabWidget->indexOf(currentProjectView);
        if (index != -1) {
            projectTabWidget->removeTab(index);
            projects.erase(projects.begin() + index);
            currentProjectView = nullptr;
        }
    }
}

void MainWindow::menuFilePrint()
{
    // TODO: Implement printing
}

// Edit menu actions implementation
void MainWindow::menuEditUndo()
{
    // TODO: Implement undo
}

void MainWindow::menuEditRedo()
{
    // TODO: Implement redo
}

void MainWindow::menuEditCut()
{
    // TODO: Implement cut
}

void MainWindow::menuEditCopy()
{
    // TODO: Implement copy
}

void MainWindow::menuEditPaste()
{
    // TODO: Implement paste
}

void MainWindow::menuEditSelectAll()
{
    // TODO: Implement select all
}

void MainWindow::menuEditPreferences()
{
    // TODO: Implement preferences dialog
}

// Help menu actions implementation
void MainWindow::menuHelpContents()
{
    // TODO: Implement help contents
}

// Project actions implementation
void MainWindow::menuProjectProperties()
{
    // TODO: Implement project properties dialog
}

void MainWindow::menuProjectRun()
{
    // TODO: Implement project running
}

void MainWindow::menuProjectGenerateSCFile()
{
    // TODO: Implement SC file generation
}

void MainWindow::menuProjectConfigureNoteModifiers()
{
    // TODO: Implement note modifiers configuration
}

void MainWindow::createNewProject()
{
    auto project = std::make_unique<ProjectViewController>(this);
    projects.push_back(std::move(project));
    currentProjectView = projects.back().get();
    projectTabWidget->addTab(currentProjectView, tr("New Project"));
    projectTabWidget->setCurrentWidget(currentProjectView);
}

void MainWindow::changeCurrentProjectViewTo(ProjectViewController* newProject)
{
    if (newProject) {
        currentProjectView = newProject;
        projectTabWidget->setCurrentWidget(newProject);
    }
}

bool MainWindow::checkTerm(const QString& command)
{
    // TODO: Implement term checking
    return true;
}

QString MainWindow::getTerm()
{
    // TODO: Implement term getting
    return QString();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // TODO: Implement event filtering for keyboard shortcuts
    return QMainWindow::eventFilter(obj, event);
}
