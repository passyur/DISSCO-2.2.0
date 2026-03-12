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
    void handleStdout() const;
    void handleStderr() const;
    void increaseFont() const;
    void decreaseFont() const;
    void clearOutput() const;
    void termProcess() const;
    void killProcess() const;
    void runProcess() const;

private:
    QTextEdit *textEdit;
    QProcess *proc;
    bool autoscroll = true;

    void closeEvent(QCloseEvent*) override;
    void appendColored(const QString &text, const QColor &color) const;
    void scrollToBottom() const;
};
