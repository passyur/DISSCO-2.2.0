#include <QtWidgets>

#include "mainwindow.hpp"
#include "EnvelopeLibraryWindow.hpp"
#include "MarkovModelLibraryWindow.hpp"
#include "ProjectViewController.hpp"
#include "ui_mainwindow.h"

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
    , ui(new Ui::MainWindow)
    , envelopeLibraryWindow(new EnvelopeLibraryWindow(this))
    , markovModelLibraryWindow(new MarkovModelLibraryWindow(this))
    , projectViewController(new ProjectViewController(ui->treeWidget, this))
{
    ui->setupUi(this);
    
    // Set up dock widgets
    ui->projectDockWidget->setWidget(projectViewController.get());
    ui->modelLibraryDockWidget->setWidget(markovModelLibraryWindow.get());
    ui->envelopeLibraryDockWidget->setWidget(envelopeLibraryWindow.get());
    
    // Connect menu actions
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNewProject);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpenProject);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveProject);
    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::onCloseProject);
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
    bool hasProject = projectViewController->getProjectCount() > 0;
    bool hasModel = markovModelLibraryWindow->getActiveModel() != nullptr;
    
    // Update menu actions based on project state
    ui->actionSave->setEnabled(hasProject);
    ui->actionClose->setEnabled(hasProject);
    
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
        projectViewController->addProject(filePath);
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
        projectViewController->addProject(filePath);
        recentFiles.prepend(filePath);
        updateWindowState();
    }
}

void MainWindow::onSaveProject()
{
    // TODO: Implement save project
}

void MainWindow::onCloseProject()
{
    // TODO: Implement close project
    updateWindowState();
}

void MainWindow::onNewModel()
{
    markovModelLibraryWindow->onNewModel();
}

void MainWindow::onOpenModel()
{
    markovModelLibraryWindow->onOpenModel();
}

void MainWindow::onSaveModel()
{
    markovModelLibraryWindow->onSaveModel();
}

void MainWindow::onCloseModel()
{
    markovModelLibraryWindow->onCloseModel();
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
    ui->actionNew->setEnabled(false);
    ui->actionOpen->setEnabled(false);
}
