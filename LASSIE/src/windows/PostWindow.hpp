#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QToolBar>
#include <QAction>
#include <QProcess>

class PostWindow : public QWidget
{
    Q_OBJECT
public:
    explicit PostWindow(QProcess *process, QWidget *parent = nullptr);

private slots:
    void handleStdout();
    void handleStderr();
    void increaseFont();
    void decreaseFont();
    void clearOutput();
    void termProcess();
    void killProcess();
    void runProcess();

private:
    QTextEdit *textEdit;
    QProcess *proc;
    bool autoscroll = true;

    void closeEvent(QCloseEvent*);
    void appendColored(const QString &text, const QColor &color);
    void scrollToBottom();
};
