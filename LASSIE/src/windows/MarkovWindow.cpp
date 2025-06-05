#include "MarkovWindow.hpp"
#include "../ui/ui_MarkovWindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QDomDocument>
#include <QTableWidget>
#include <QHeaderView>
#include <QCloseEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>

MarkovWindow::MarkovWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MarkovWindow)
    , activeChain(nullptr)
{
    ui->setupUi(this);
    setupUi();
    createActions();
    createMenus();
    loadMarkovChains();
    refreshMarkovList();

    connect(ui->stateLineEdit, &QLineEdit::returnPressed, this, &MarkovWindow::onStateAdded);
    connect(ui->removeStateButton, &QPushButton::clicked, this, &MarkovWindow::onStateRemoved);
    connect(ui->markovList, &QListWidget::currentTextChanged, this, &MarkovWindow::onMarkovSelected);
}

MarkovWindow::~MarkovWindow() = default;

void MarkovWindow::setupUi()
{
    setWindowTitle(tr("Markov Library"));
    
    // Set up transition table
    ui->transitionTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->transitionTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->transitionTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MarkovWindow::createActions()
{
    newMarkovAction = new QAction(QIcon::fromTheme("document-new"), tr("Create &New Markov Chain"), this);
    connect(newMarkovAction, &QAction::triggered, this, &MarkovWindow::onNewMarkovTriggered);

    duplicateMarkovAction = new QAction(QIcon::fromTheme("edit-copy"), tr("&Duplicate Markov Chain"), this);
    connect(duplicateMarkovAction, &QAction::triggered, this, &MarkovWindow::onDuplicateMarkovTriggered);

    saveAction = new QAction(QIcon::fromTheme("document-save"), tr("&Save"), this);
    connect(saveAction, &QAction::triggered, this, &MarkovWindow::onSaveTriggered);
}

void MarkovWindow::createMenus()
{
    contextMenu = new QMenu(this);
    contextMenu->addAction(newMarkovAction);
    contextMenu->addAction(duplicateMarkovAction);
    contextMenu->addSeparator();
    contextMenu->addAction(saveAction);
}

void MarkovWindow::refreshMarkovList()
{
    ui->markovList->clear();
    for (auto it = markovChains.begin(); it != markovChains.end(); ++it) {
        ui->markovList->addItem(it.key());
    }
}

void MarkovWindow::updateTransitionTable()
{
    if (!activeChain)
        return;

    QStringList states = activeChain->getStates();
    int stateCount = states.size();

    ui->transitionTable->setRowCount(stateCount);
    ui->transitionTable->setColumnCount(stateCount);
    ui->transitionTable->setHorizontalHeaderLabels(states);
    ui->transitionTable->setVerticalHeaderLabels(states);

    for (int i = 0; i < stateCount; ++i) {
        for (int j = 0; j < stateCount; ++j) {
            double prob = activeChain->getTransitionProbability(states[i], states[j]);
            QTableWidgetItem *item = new QTableWidgetItem(QString::number(prob));
            ui->transitionTable->setItem(i, j, item);
        }
    }

    // Connect cell changed signal
    connect(ui->transitionTable, &QTableWidget::itemChanged, this, [this](QTableWidgetItem *item) {
        if (!activeChain)
            return;

        QStringList states = activeChain->getStates();
        QString fromState = states[item->row()];
        QString toState = states[item->column()];
        bool ok;
        double value = item->text().toDouble(&ok);
        if (ok) {
            onProbabilityChanged(fromState, toState, value);
        }
    });
}

void MarkovWindow::showMarkovLibrary()
{
    show();
    raise();
    activateWindow();
}

void MarkovWindow::closeEvent(QCloseEvent *event)
{
    saveMarkovChains();
    event->accept();
}

void MarkovWindow::onNewMarkovTriggered()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("New Markov Chain"),
                                       tr("Enter Markov chain name:"), QLineEdit::Normal,
                                       QString(), &ok);
    if (ok && !name.isEmpty()) {
        markovChains.insert(name, QSharedPointer<MarkovChain>(new MarkovChain(name)));
        refreshMarkovList();
        ui->markovList->setCurrentRow(ui->markovList->count() - 1);
    }
}

void MarkovWindow::onDuplicateMarkovTriggered()
{
    if (!activeChain)
        return;

    QString originalName = activeChain->getName();
    QString newName = originalName + "_copy";
    markovChains.insert(newName, QSharedPointer<MarkovChain>(new MarkovChain(*activeChain)));
    refreshMarkovList();
    ui->markovList->setCurrentRow(ui->markovList->count() - 1);
}

void MarkovWindow::onSaveTriggered()
{
    saveMarkovChains();
}

void MarkovWindow::onStateAdded()
{
    if (!activeChain)
        return;

    QString state = ui->stateLineEdit->text().trimmed();
    if (!state.isEmpty()) {
        activeChain->addState(state);
        ui->stateLineEdit->clear();
        updateTransitionTable();
    }
}

void MarkovWindow::onStateRemoved()
{
    if (!activeChain)
        return;

    QString state = ui->markovList->currentItem()->text();
    if (activeChain->getStates().contains(state)) {
        activeChain->removeState(state);
        updateTransitionTable();
    }
}

void MarkovWindow::onProbabilityChanged(const QString &fromState, const QString &toState, double value)
{
    if (!activeChain)
        return;

    activeChain->setTransitionProbability(fromState, toState, value);
    activeChain->normalizeTransitions();
    updateTransitionTable();
}

void MarkovWindow::onMarkovSelected(const QString &name)
{
    auto it = markovChains.find(name);
    if (it != markovChains.end()) {
        activeChain = it.value().data();
        updateTransitionTable();
    }
}

void MarkovWindow::loadMarkovChains()
{
    QString fileName = QApplication::applicationDirPath() + "/markov_chains.xml";
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return;

    QDomDocument doc;
    if (!doc.setContent(&file))
        return;

    QDomElement root = doc.documentElement();
    QDomElement chainElement = root.firstChildElement("chain");
    while (!chainElement.isNull()) {
        MarkovChain *chain = MarkovChain::loadFromXml(chainElement);
        if (chain) {
            markovChains.insert(chain->getName(), QSharedPointer<MarkovChain>(chain));
        }
        chainElement = chainElement.nextSiblingElement("chain");
    }
}

void MarkovWindow::saveMarkovChains()
{
    QString fileName = QApplication::applicationDirPath() + "/markov_chains.xml";
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
        return;

    QDomDocument doc;
    QDomElement root = doc.createElement("markov_chains");
    doc.appendChild(root);

    for (auto it = markovChains.begin(); it != markovChains.end(); ++it) {
        QDomElement chainElement = doc.createElement("chain");
        it.value()->saveToXml(doc, chainElement);
        root.appendChild(chainElement);
    }

    QTextStream out(&file);
    doc.save(out, 4);
} 