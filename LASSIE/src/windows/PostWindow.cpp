#include "PostWindow.hpp"

#include <QMessageBox>

#include <QVBoxLayout>
#include <QTextCharFormat>

PostWindow::PostWindow(QProcess *process, QWidget *parent)
    : QWidget(parent), proc(process)
{
    setWindowTitle("Process Output");

    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    textEdit->setStyleSheet("background:white;");

    QToolBar *toolbar = new QToolBar(this);
    QAction *bigger = toolbar->addAction("Zoom in");
    QAction *smaller = toolbar->addAction("Zoom out");
    QAction *clear = toolbar->addAction("Clear");
    QAction *toggleScroll = toolbar->addAction("Scroll");
    toggleScroll->setCheckable(true);
    toggleScroll->setChecked(true);

    connect(bigger, &QAction::triggered, this, &PostWindow::increaseFont);
    connect(smaller, &QAction::triggered, this, &PostWindow::decreaseFont);
    connect(clear, &QAction::triggered, this, &PostWindow::clearOutput);
    connect(toggleScroll, &QAction::toggled, this, 
        [this](bool checked){
            autoscroll = checked;
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

        int ret = message.exec();

        switch(ret){
            case QMessageBox::Yes:
                proc->kill();
                break;
            case QMessageBox::No:
                return;
        }
    }

    QWidget::closeEvent(event);
}

void PostWindow::appendColored(const QString &text, const QColor &color)
{
    QTextCharFormat fmt;
    fmt.setForeground(color);

    textEdit->setCurrentCharFormat(fmt);
    textEdit->append(text);

    scrollToBottom();
}

void PostWindow::handleStdout()
{
    QString data = QString::fromLocal8Bit(proc->readAllStandardOutput());
    appendColored(data.trimmed(), Qt::black);
}

void PostWindow::handleStderr()
{
    QString data = QString::fromLocal8Bit(proc->readAllStandardError());
    appendColored(data.trimmed(), Qt::red);
}

void PostWindow::increaseFont()
{
    QFont f = textEdit->font();
    f.setPointSize(f.pointSize() + 1);
    textEdit->setFont(f);
}

void PostWindow::decreaseFont()
{
    QFont f = textEdit->font();
    f.setPointSize(std::max(1, f.pointSize() - 1));
    textEdit->setFont(f);
}

void PostWindow::clearOutput()
{
    textEdit->clear();
}


void PostWindow::scrollToBottom()
{
    if (autoscroll) {
        textEdit->moveCursor(QTextCursor::End);
        textEdit->ensureCursorVisible();
    }
}
