#ifndef MARKOVWINDOW_HPP
#define MARKOVWINDOW_HPP

#include <QMainWindow>
#include <QMap>
#include <QSharedPointer>
#include "MarkovChain.hpp"

namespace Ui {
class MarkovWindow;
}

class MarkovWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MarkovWindow(QWidget *parent = nullptr);
    ~MarkovWindow();

    void showMarkovLibrary();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onNewMarkovTriggered();
    void onDuplicateMarkovTriggered();
    void onSaveTriggered();
    void onStateAdded();
    void onStateRemoved();
    void onProbabilityChanged(const QString &fromState, const QString &toState, double value);
    void onMarkovSelected(const QString &name);

private:
    void setupUi();
    void createActions();
    void createMenus();
    void refreshMarkovList();
    void updateTransitionTable();
    void loadMarkovChains();
    void saveMarkovChains();

    std::unique_ptr<Ui::MarkovWindow> ui;
    QMenu *contextMenu;
    
    QAction *newMarkovAction;
    QAction *duplicateMarkovAction;
    QAction *saveAction;

    QMap<QString, QSharedPointer<MarkovChain>> markovChains;
    MarkovChain *activeChain;
};

#endif // MARKOVWINDOW_HPP 