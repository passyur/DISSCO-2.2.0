#include <QtWidgets>
#include <QFileDialog>

#include "mainwindow.hpp"
#include "markovwindow.hpp"
#include "envelopewindow.hpp"
#include "projectview.hpp"

#include "core/project_struct.hpp"

MainWindow* MainWindow::instance_ = 0;

void MainWindow::showStatusMessage(const QString& message){
    statusbar_->showMessage(message, 5000);
}

MainWindow::MainWindow(Inst *m)
{
    Q_ASSERT(instance_ == nullptr);
    instance_ = this;

    main_ = m;

    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    /* Create status bar to print informative messages to */
    statusbar_ = statusBar();

    projview_ = nullptr;

    QWidget *top_filler = new QWidget;
    top_filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget *bottom_filler = new QWidget;
    bottom_filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(5, 5, 5, 5);
    layout->addWidget(top_filler);
    layout->addWidget(bottom_filler);
    widget->setLayout(layout);

    createActions();
    createMenus();
    
    setWindowTitle("LASSIE");
    setMinimumSize(160, 160);
    resize(480, 320);
}

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

void MainWindow::newFile()
{
    if(projview_ == nullptr){
        Project *proj = main_->get_project_manager()->create("Untitled"); 
        projview_ = new ProjectView(proj);
    }
}

void MainWindow::open()
{
    if(projview_ == nullptr){
        QString filepath = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Dissco Files (*.dissco)"));
        if(!filepath.isEmpty()){
            Project *proj = main_->get_project_manager()->open(filepath); 
            projview_ = new ProjectView(proj);
        }
    }
}

void MainWindow::save()
{
    if(current_file_.isEmpty()){
        // saveFileAs();
        showStatusMessage("Save pressed");
        // QStringList filenames = dialog.selectedFiles();
    }
}

void MainWindow::undo()
{
    showStatusMessage("Undo pressed");
}

void MainWindow::redo()
{
    showStatusMessage("Redo pressed");
}

void MainWindow::cut()
{
    showStatusMessage("Cut pressed");
}

void MainWindow::copy()
{
    showStatusMessage("Copy pressed");
}

void MainWindow::paste()
{
    showStatusMessage("Paste pressed");
}

void MainWindow::about()
{
    showStatusMessage("Help pressed");
    QMessageBox::about(this, tr("About Menu"),
                       tr("This is a prototype of the LASSIE UI for DISSCO rewritten in Qt."));
}

void MainWindow::aboutQt()
{
    showStatusMessage("About Qt pressed");
}

void MainWindow::markov()
{
    showStatusMessage("Markov Window pressed");
    MarkovWindow *mw = new MarkovWindow(this);
    mw->setWindowFlag(Qt::Window);
    mw->setAttribute(Qt::WA_DeleteOnClose);
    mw->show();
}

void MainWindow::envelope()
{
    showStatusMessage("Envelope Window pressed");
    EnvelopeWindow *ew = new EnvelopeWindow(this);
    ew->setWindowFlag(Qt::Window);
    ew->setAttribute(Qt::WA_DeleteOnClose);
    ew->show();
}

void MainWindow::createActions()
{
    new_act = new QAction(QIcon::fromTheme("document-new"),
                         tr("&New"), this);
    new_act->setShortcuts(QKeySequence::New);
    new_act->setStatusTip(tr("Create a new file"));
    connect(new_act, &QAction::triggered, this, &MainWindow::newFile);

    open_act = new QAction(QIcon::fromTheme("document-open"),
                          tr("&Open..."), this);
    open_act->setShortcuts(QKeySequence::Open);
    open_act->setStatusTip(tr("Open an existing file"));
    connect(open_act, &QAction::triggered, this, &MainWindow::open);

    save_act = new QAction(QIcon::fromTheme("document-save"),
                          tr("&Save"), this);
    save_act->setShortcuts(QKeySequence::Save);
    save_act->setStatusTip(tr("Save the document to disk"));
    connect(save_act, &QAction::triggered, this, &MainWindow::save);

    exit_act = new QAction(QIcon::fromTheme("application-exit"),
                          tr("&Exit"), this);
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

    markov_act = new QAction(tr("&Markov"), this);
    markov_act->setStatusTip(tr("Open the markov library"));
    connect(markov_act, &QAction::triggered, this, &MainWindow::markov);

    envelope_act = new QAction(tr("&Envelope"), this);
    markov_act->setStatusTip(tr("Open the envelope library"));
    connect(envelope_act, &QAction::triggered, this, &MainWindow::envelope);
}

void MainWindow::createMenus()
{
    file_menu = menuBar()->addMenu(tr("&File"));
    file_menu->addAction(new_act);
    file_menu->addAction(open_act);
    file_menu->addAction(save_act);
    file_menu->addSeparator();
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

    libraries_menu = menuBar()->addMenu(tr("&Libraries"));
    libraries_menu->addAction(markov_act);
    libraries_menu->addSeparator();
    libraries_menu->addAction(envelope_act);
}
