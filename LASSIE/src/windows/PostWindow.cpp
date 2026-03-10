#include "PostWindow.hpp"

#include "../inst.hpp"

#include <QMessageBox>
#include <QCloseEvent>
//#include <QOverload>

#include <QVBoxLayout>
#include <QTextCharFormat>

PostWindow::PostWindow(QProcess *process, QWidget *parent)
    : QWidget(parent), proc(process)
{
    setWindowTitle("Process Output");

    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    textEdit->setStyleSheet("background:white;");

    const auto toolbar = new QToolBar(this);
    const QAction *bigger = toolbar->addAction("Zoom in");
    const QAction *smaller = toolbar->addAction("Zoom out");
    const QAction *clear = toolbar->addAction("Clear");
    QAction *toggleScroll = toolbar->addAction("Scroll");
    toggleScroll->setCheckable(true);
    toggleScroll->setChecked(true);

    // spacer for clarity since the remaining actions will be related to the process
    const auto spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolbar->addWidget(spacer);

    QAction *termProc = toolbar->addAction("Terminate");
    termProc->setToolTip("Ask CMOD to stop running");
    termProc->setEnabled(false);
    QAction *killProc = toolbar->addAction("Kill");
    killProc->setToolTip("Force CMOD to stop running");
    killProc->setEnabled(false);
    QAction *runProc = toolbar->addAction("Run");
    runProc->setToolTip("Run project (if none currently running)");
    runProc->setEnabled(false);
    
    connect(bigger, &QAction::triggered, this, &PostWindow::increaseFont);
    connect(smaller, &QAction::triggered, this, &PostWindow::decreaseFont);
    connect(clear, &QAction::triggered, this, &PostWindow::clearOutput);
    connect(toggleScroll, &QAction::toggled, this, 
        [this](bool checked){
            autoscroll = checked;
        });
    connect(termProc, &QAction::triggered, this, &PostWindow::termProcess);
    connect(killProc, &QAction::triggered, this, &PostWindow::killProcess);
    connect(runProc, &QAction::triggered, this, &PostWindow::runProcess);
    
    connect(proc, &QProcess::stateChanged, this, [=]{
        if(proc->state() != QProcess::NotRunning){
            termProc->setEnabled(true);
            killProc->setEnabled(true);
            runProc->setEnabled(false);
        }else{
            termProc->setEnabled(false);
            killProc->setEnabled(false);
        }
    });

    connect(proc, &QProcess::finished, this, [=]{
        runProc->setEnabled(true);
        if(proc->exitStatus() == QProcess::NormalExit)
            appendColored("*** Process exited normally (exit code" + QString::number(proc->exitCode()) + ") ***", Qt::black);
        else
            appendColored("*** Process crashed (abnormal exit; exit code" + QString::number(proc->exitCode()) + ") *** ", Qt::red);
    });
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(toolbar);
    layout->addWidget(textEdit);
    setLayout(layout);

    // feed output to window
    connect(proc, &QProcess::readyReadStandardOutput, this, &PostWindow::handleStdout);
    connect(proc, &QProcess::readyReadStandardError,  this, &PostWindow::handleStderr);
}

void PostWindow::closeEvent(QCloseEvent *event)
{
    if(proc->state() != QProcess::NotRunning){
        QMessageBox message;
        if(proc->state() == QProcess::Running)
            message.setText("CMOD has not yet finished running.");
        else
            message.setText("CMOD has not yet started running.");
        message.setInformativeText("Abort CMOD process?");
        message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        message.setDefaultButton(QMessageBox::No);

        switch(int ret = message.exec()){
            case QMessageBox::Yes:
                proc->kill();
                break;
            case QMessageBox::No:
                event->ignore();
                return;
        }
    }

    QWidget::closeEvent(event);
}

void PostWindow::appendColored(const QString &text, const QColor &color) const {
    QTextCharFormat fmt;
    fmt.setForeground(color);

    textEdit->setCurrentCharFormat(fmt);
    textEdit->append(text);

    scrollToBottom();
}

void PostWindow::handleStdout() const {
    const QString data = QString::fromLocal8Bit(proc->readAllStandardOutput());
    appendColored(data.trimmed(), Qt::black);
}

void PostWindow::handleStderr() const {
    const QString data = QString::fromLocal8Bit(proc->readAllStandardError());
    appendColored(data.trimmed(), Qt::red);
}

void PostWindow::increaseFont() const {
    QFont f = textEdit->font();
    f.setPointSize(f.pointSize() + 1);
    textEdit->setFont(f);
}

void PostWindow::decreaseFont() const {
    QFont f = textEdit->font();
    f.setPointSize(std::max(1, f.pointSize() - 1));
    textEdit->setFont(f);
}

void PostWindow::clearOutput() const {
    textEdit->clear();
}

void PostWindow::termProcess() const {
    proc->terminate();
    appendColored("*** User requested process terminate ***", Qt::red);
}

void PostWindow::killProcess() const {
    proc->kill();
    appendColored("*** Process killed by user ***", Qt::red);
}

void PostWindow::runProcess() const {
    proc->start(proc->program(), proc->arguments());
}

void PostWindow::scrollToBottom() const {
    if (autoscroll) {
        textEdit->moveCursor(QTextCursor::End);
        textEdit->ensureCursorVisible();
    }
}
