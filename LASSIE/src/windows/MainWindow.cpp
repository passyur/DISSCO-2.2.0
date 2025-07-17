#include <QtWidgets>

#include "MainWindow.hpp"
#include "EnvelopeLibraryWindow.hpp"
#include "../ui/ui_mainwindow.h"
#include "MarkovModelLibraryWindow.hpp"
#include "ProjectViewController.hpp"
#include "../core/project_struct.hpp"

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
#include <QFileInfo>
#include <QDir>
#include<QDebug>


MainWindow* MainWindow::instance_ = 0;

void MainWindow::showStatusMessage(const QString& message){
    statusbar_->showMessage(message, 5000);
}

MainWindow::MainWindow(Inst* m)
    : QMainWindow()
    , ui(std::make_unique<Ui::MainWindow>())
    , envelopeLibraryWindow(std::make_unique<EnvelopeLibraryWindow>(this))
    , markovWindow(std::make_unique<MarkovModelLibraryWindow>(this))
{
    Q_ASSERT(instance_ == nullptr);
    instance_ = this;
    main_ = m;

    ui->setupUi(this);

    ui->tabWidget->hide();
    ui->paletteWidget->hide();
    
    createActions();
    enableProjectActions(false);
    createMenus();
    createToolBars();
    createStatusBar();
    
    readSettings();
    
    setWindowTitle(tr("LASSIE"));
    setUnifiedTitleAndToolBarOnMac(true);

    connect(ui->envButton, &QPushButton::clicked, this, &MainWindow::showEnvelopeLibraryWindow);
    connect(ui->markovButton, &QPushButton::clicked, this, &MainWindow::showMarkovWindow);

}

//MainWindow::~MainWindow() = default;
MainWindow::~MainWindow() {
    for(std::vector<ProjectView*>::
        iterator it = projects.begin();
      it!=projects.end();
      it++){
    delete *it;
  }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void MainWindow::newFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("New Project"),
                                                  QString(),
                                                  tr("DISSCO Files (*.dissco);;All Files (*)"));
    if (fileName.isEmpty()) {
        return;
    }

    QFileInfo fileInfo(fileName);
    QString baseDir = fileInfo.absolutePath();
    QString projectName = fileInfo.completeBaseName();
    QString projectFolder = baseDir + "/" + projectName;
    QDir dir;
    if (!dir.exists(projectFolder)) {
        dir.mkdir(projectFolder);
    }
    QString fullFilePath = projectFolder + "/" + projectName + ".dissco";
    currentFile = fullFilePath;

    setUnsavedTitle(currentFile);
    statusBar()->showMessage(tr("File created"), 2000);

    Project *p = Inst::get_project_manager()->build(currentFile, NULL);
    // Inst::get_project_manager()->set_curr_project(p);
    projectView = new ProjectView(this, currentFile);
    projects.push_back(projectView);
    projectView->setProperties();
    
    //nhi: connect envelope library to new project
    envelopeLibraryWindow->setActiveProject(projectView);

    ui->tabWidget->show();
    ui->paletteWidget->show();
    enableProjectActions(true);
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                  QString(),
                                                  tr("DISSCO Files (*.dissco);;All Files (*)"));
    if (!fileName.isEmpty()) {
        loadFile(fileName);
        // Project *p = Inst::get_project_manager()->open();
    }
}

void MainWindow::saveFile()
{
    if (currentFile.isEmpty()) {
        saveFileAs();
    } else {
        saveFile(currentFile);
    }
}

void MainWindow::saveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                  currentFile,
                                                  tr("DISSCO Files (*.dissco);;All Files (*)"));
    if (!fileName.isEmpty()) {
        saveFile(fileName);
    }
}

void MainWindow::undo()
{
    // TODO: Implement undo functionality
    statusBar()->showMessage(tr("Undo"), 2000);
}

void MainWindow::redo()
{
    // TODO: Implement redo functionality
    statusBar()->showMessage(tr("Redo"), 2000);
}

void MainWindow::showEnvelopeLibraryWindow()
{
    //nhi: use show() instead of showEnvelopeLibrary()
    envelopeLibraryWindow->show();
    //nhi: connect to current project if available
    if (projectView) {
        envelopeLibraryWindow->setActiveProject(projectView);
    }
}

void MainWindow::showMarkovWindow()
{
    //nhi: use show() instead of showMarkovLibrary()
    markovWindow->show();
}

void MainWindow::showPropertiesDialog()
{
    projectView->setProperties();
}

void MainWindow::showFileNewObjectDialog()
{
    projectView->insertObject();
    
}

void MainWindow::readSettings()
{
    QSettings settings;
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() * 2/3, availableGeometry.height() * 2/3);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
}

void MainWindow::createActions()
{
    // File actions
    newAct = new QAction(QIcon::fromTheme("document-new"), tr("&New Project"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new project"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    
    openAct = new QAction(QIcon::fromTheme("document-open"), tr("&Open Project"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing project"));
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
    
    saveAct = new QAction(QIcon::fromTheme("document-save"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the project to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFile()));
    
    saveAsAct = new QAction(QIcon::fromTheme("document-save"), tr("Save &As"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the project under a new name"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveFileAs);
    
    exitAct = new QAction(QIcon::fromTheme("application-exit"), tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    // Edit actions
    undoAct = new QAction(QIcon::fromTheme("edit-undo"), tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last operation"));
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);
    
    redoAct = new QAction(QIcon::fromTheme("edit-redo"), tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last operation"));
    connect(redoAct, &QAction::triggered, this, &MainWindow::redo);
    
    cutAct = new QAction(QIcon::fromTheme("edit-cut"), tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
    
    copyAct = new QAction(QIcon::fromTheme("edit-copy"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
    
    pasteAct = new QAction(QIcon::fromTheme("edit-paste"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current selection"));

    // Project actions
    newObjAct = new QAction(QIcon::fromTheme("list-add"), tr("New Object"), this);
    newObjAct->setStatusTip(tr("Create a new object"));
    connect(newObjAct, &QAction::triggered, this, &MainWindow::showFileNewObjectDialog);

    setPropAct = new QAction(tr("Set Properties"), this);
    setPropAct->setStatusTip(tr("Set project properties"));
    connect(setPropAct, &QAction::triggered, this, &MainWindow::showPropertiesDialog);

    runAct = new QAction(tr("Run"), this);
    runAct->setStatusTip(tr("Run"));

    configNoteModAct = new QAction(tr("Configure Note Modifiers"), this);
    configNoteModAct->setStatusTip(tr("Configure note modifiers"));

    // View actions
    showEnvelopeLibraryAct = new QAction(tr("&Envelope Library"), this);
    showEnvelopeLibraryAct->setStatusTip(tr("Show the envelope library window"));
    connect(showEnvelopeLibraryAct, &QAction::triggered, this, &MainWindow::showEnvelopeLibraryWindow);

    showMarkovAct = new QAction(tr("&Markov Library"), this);
    showMarkovAct->setStatusTip(tr("Show the Markov chain library window"));
    connect(showMarkovAct, &QAction::triggered, this, &MainWindow::showMarkovWindow);

    // Help actions
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

void MainWindow::enableProjectActions(bool enabled) {
    saveAct->setEnabled(enabled);
    saveAsAct->setEnabled(enabled);
    showEnvelopeLibraryAct->setEnabled(enabled);
    showMarkovAct->setEnabled(enabled);
    newObjAct->setEnabled(enabled);
    setPropAct->setEnabled(enabled);
    runAct->setEnabled(enabled);
    configNoteModAct->setEnabled(enabled);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    //editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    projectMenu = menuBar()->addMenu(tr("&Project"));
    projectMenu->addAction(newObjAct);
    projectMenu->addAction(setPropAct);
    projectMenu->addAction(runAct);
    projectMenu->addAction(configNoteModAct);
    
    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(showEnvelopeLibraryAct);
    viewMenu->addAction(showMarkovAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(saveAsAct);
    fileToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    projectToolBar = addToolBar(tr("Project"));
    projectToolBar->addAction(newObjAct);
    //nhi: envelope library action moved to view menu instead of project toolbar
    projectToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // editToolBar = addToolBar(tr("Edit"));
    // editToolBar->addAction(undoAct);
    // editToolBar->addAction(redoAct);
    // //editToolBar->addSeparator();
    // editToolBar->addAction(cutAct);
    // editToolBar->addAction(copyAct);
    // editToolBar->addAction(pasteAct);
    // fileToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("LASSIE"),
                           tr("Cannot read file %1:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName),
                               file.errorString()));
        return;
    }

    // TODO: Implement file loading
    currentFile = fileName;
    qDebug() << "Looking for:" << fileName;
    if (projectView == NULL) {
        projectView = new ProjectView(this, currentFile);
        projects.push_back(projectView);
    }
    setWindowTitle(tr("%1 - %2").arg(currentFile, tr("LASSIE")));
    statusBar()->showMessage(tr("File loaded"), 2000);
    projectView->setProperties();
    
    //nhi: connect envelope library to loaded project
    envelopeLibraryWindow->setActiveProject(projectView);

    ui->tabWidget->show();
    ui->paletteWidget->show();
    enableProjectActions(true);
}

void MainWindow::saveFile(const QString &fileName)
{
    currentFile = fileName;
    qDebug() << "Current File:" << currentFile;
    
    //nhi: ensure directory exists before saving
    QFileInfo fileInfo(fileName);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            QMessageBox::critical(this, tr("Error"),
                                tr("Failed to create directory:\n%1")
                                .arg(dir.absolutePath()));
            return;
        }
    }
    
    qDebug() << "In Main Window Save Function";
    projectView->save();
    
    //nhi: update window title and status after successful save
    setWindowTitle(tr("%1 - %2").arg(currentFile, tr("LASSIE")));
    statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::setUnsavedTitle(QString unsavedFile){
    qDebug() << "In setUnsavedTitle";
    currentFile = unsavedFile;
    setWindowTitle(tr("%1 - %2").arg("*" + currentFile, tr("LASSIE")));
    qDebug() << "*currentFile: " << currentFile;
}
