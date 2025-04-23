#include "MarkovModelLibraryWindow.hpp"
#include "ui_MarkovModelLibraryWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QInputDialog>
#include <QFormLayout>
#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QDebug>

MarkovModelLibraryWindow::MarkovModelLibraryWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MarkovModelLibraryWindow)
    , model(nullptr)
    , currentModel(nullptr)
{
    ui->setupUi(this);
    setupConnections();
    initializeModel();
}

MarkovModelLibraryWindow::~MarkovModelLibraryWindow()
{
    delete ui;
}

void MarkovModelLibraryWindow::setupConnections()
{
    // Connect button signals to slots
    connect(ui->newButton, &QPushButton::clicked, this, &MarkovModelLibraryWindow::onNewButtonClicked);
    connect(ui->openButton, &QPushButton::clicked, this, &MarkovModelLibraryWindow::onOpenButtonClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &MarkovModelLibraryWindow::onSaveButtonClicked);
    connect(ui->closeButton, &QPushButton::clicked, this, &MarkovModelLibraryWindow::onCloseButtonClicked);
    
    // Connect search functionality
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &MarkovModelLibraryWindow::onSearchTextChanged);
    
    // Connect tree view selection
    connect(ui->modelTreeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MarkovModelLibraryWindow::onModelSelectionChanged);
}

void MarkovModelLibraryWindow::initializeModel()
{
    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels(QStringList() << "Name" << "Type" << "Description");
    ui->modelTreeView->setModel(model);
    ui->modelTreeView->setAlternatingRowColors(true);
    ui->modelTreeView->setSortingEnabled(true);
    ui->modelTreeView->setAnimated(true);
    ui->modelTreeView->setIndentation(20);
    ui->modelTreeView->setUniformRowHeights(true);
    
    // Add some sample models
    QSharedPointer<MarkovModel> sampleModel1 = QSharedPointer<MarkovModel>::create("Sample Model 1");
    sampleModel1->setDescription("A first-order Markov model");
    sampleModel1->setType("First-Order");
    sampleModel1->addState("A");
    sampleModel1->addState("B");
    sampleModel1->addState("C");
    sampleModel1->addTransition("A", "B", 0.7);
    sampleModel1->addTransition("A", "C", 0.3);
    sampleModel1->addTransition("B", "A", 0.5);
    sampleModel1->addTransition("B", "C", 0.5);
    sampleModel1->addTransition("C", "A", 0.4);
    sampleModel1->addTransition("C", "B", 0.6);
    
    QSharedPointer<MarkovModel> sampleModel2 = QSharedPointer<MarkovModel>::create("Sample Model 2");
    sampleModel2->setDescription("A second-order Markov model");
    sampleModel2->setType("Second-Order");
    sampleModel2->addState("X");
    sampleModel2->addState("Y");
    sampleModel2->addState("Z");
    sampleModel2->addTransition("X", "Y", 0.6);
    sampleModel2->addTransition("X", "Z", 0.4);
    sampleModel2->addTransition("Y", "X", 0.3);
    sampleModel2->addTransition("Y", "Z", 0.7);
    sampleModel2->addTransition("Z", "X", 0.5);
    sampleModel2->addTransition("Z", "Y", 0.5);
    
    addModelToTree(sampleModel1);
    addModelToTree(sampleModel2);
    
    models[sampleModel1->name()] = sampleModel1;
    models[sampleModel2->name()] = sampleModel2;
}

void MarkovModelLibraryWindow::onNewButtonClicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "New Markov Model", "Model Name:", QLineEdit::Normal, "", &ok);
    
    if (ok && !name.isEmpty()) {
        // Check if model with this name already exists
        if (models.contains(name)) {
            QMessageBox::warning(this, "Error", "A model with this name already exists.");
            return;
        }
        
        // Create new model
        QSharedPointer<MarkovModel> newModel = QSharedPointer<MarkovModel>::create(name);
        newModel->setDescription("A new Markov model");
        newModel->setType("First-Order");
        
        // Add to models map and tree
        models[name] = newModel;
        addModelToTree(newModel);
        
        // Select the new model
        QModelIndexList indices = model->match(model->index(0, 0), Qt::DisplayRole, name, 1, Qt::MatchExactly);
        if (!indices.isEmpty()) {
            ui->modelTreeView->setCurrentIndex(indices.first());
        }
    }
}

void MarkovModelLibraryWindow::onOpenButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Open Markov Model", QString(),
        "Markov Model Files (*.mm);;All Files (*)");
        
    if (!fileName.isEmpty()) {
        QSharedPointer<MarkovModel> loadedModel = QSharedPointer<MarkovModel>::create("");
        if (loadedModel->loadFromFile(fileName)) {
            QString name = loadedModel->name();
            
            // Check if model with this name already exists
            if (models.contains(name)) {
                int result = QMessageBox::question(this, "Model Exists", 
                    "A model with this name already exists. Do you want to replace it?",
                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
                
                if (result == QMessageBox::No) {
                    return;
                }
                
                // Remove existing model
                removeModelFromTree(name);
            }
            
            // Add to models map and tree
            models[name] = loadedModel;
            addModelToTree(loadedModel);
            
            // Select the loaded model
            QModelIndexList indices = model->match(model->index(0, 0), Qt::DisplayRole, name, 1, Qt::MatchExactly);
            if (!indices.isEmpty()) {
                ui->modelTreeView->setCurrentIndex(indices.first());
            }
            
            QMessageBox::information(this, "Model Loaded", 
                "Successfully loaded model: " + name);
        } else {
            QMessageBox::warning(this, "Error", 
                "Failed to load model from file: " + fileName);
        }
    }
}

void MarkovModelLibraryWindow::onSaveButtonClicked()
{
    if (!currentModel) {
        QMessageBox::warning(this, "Error", "No model selected.");
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(this,
        "Save Markov Model", currentModel->name() + ".mm",
        "Markov Model Files (*.mm);;All Files (*)");
        
    if (!fileName.isEmpty()) {
        if (currentModel->saveToFile(fileName)) {
            QMessageBox::information(this, "Model Saved", 
                "Successfully saved model to: " + fileName);
        } else {
            QMessageBox::warning(this, "Error", 
                "Failed to save model to file: " + fileName);
        }
    }
}

void MarkovModelLibraryWindow::onCloseButtonClicked()
{
    close();
}

void MarkovModelLibraryWindow::onSearchTextChanged(const QString &text)
{
    filterModels(text);
}

void MarkovModelLibraryWindow::onModelSelectionChanged(const QItemSelection &selected,
                                                     const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    
    if (selected.indexes().isEmpty()) {
        clearModelView();
        currentModel = nullptr;
        return;
    }
    
    QModelIndex index = selected.indexes().first();
    QString modelName = model->data(index).toString();
    
    if (models.contains(modelName)) {
        currentModel = models[modelName];
        updateModelView(currentModel);
    } else {
        clearModelView();
        currentModel = nullptr;
    }
}

void MarkovModelLibraryWindow::onAddStateClicked()
{
    if (!currentModel) {
        QMessageBox::warning(this, "Error", "No model selected.");
        return;
    }
    
    bool ok;
    QString stateName = QInputDialog::getText(this, "Add State", "State Name:", QLineEdit::Normal, "", &ok);
    
    if (ok && !stateName.isEmpty()) {
        currentModel->addState(stateName);
        updateModelView(currentModel);
    }
}

void MarkovModelLibraryWindow::onRemoveStateClicked()
{
    if (!currentModel) {
        QMessageBox::warning(this, "Error", "No model selected.");
        return;
    }
    
    QVector<QString> states = currentModel->transitions().keys();
    if (states.isEmpty()) {
        QMessageBox::information(this, "No States", "There are no states to remove.");
        return;
    }
    
    bool ok;
    QString stateName = QInputDialog::getItem(this, "Remove State", "Select State to Remove:",
                                             states, 0, false, &ok);
    
    if (ok && !stateName.isEmpty()) {
        currentModel->removeState(stateName);
        updateModelView(currentModel);
    }
}

void MarkovModelLibraryWindow::onAddTransitionClicked()
{
    if (!currentModel) {
        QMessageBox::warning(this, "Error", "No model selected.");
        return;
    }
    
    QVector<QString> states = currentModel->transitions().keys();
    if (states.size() < 2) {
        QMessageBox::information(this, "Not Enough States", 
            "You need at least two states to add a transition.");
        return;
    }
    
    // Create dialog for transition details
    QDialog dialog(this);
    dialog.setWindowTitle("Add Transition");
    
    QFormLayout *formLayout = new QFormLayout(&dialog);
    
    QComboBox *fromStateCombo = new QComboBox(&dialog);
    fromStateCombo->addItems(states);
    formLayout->addRow("From State:", fromStateCombo);
    
    QComboBox *toStateCombo = new QComboBox(&dialog);
    toStateCombo->addItems(states);
    formLayout->addRow("To State:", toStateCombo);
    
    QDoubleSpinBox *probabilitySpin = new QDoubleSpinBox(&dialog);
    probabilitySpin->setRange(0.0, 1.0);
    probabilitySpin->setSingleStep(0.1);
    probabilitySpin->setValue(0.5);
    formLayout->addRow("Probability:", probabilitySpin);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    formLayout->addRow(buttonBox);
    
    if (dialog.exec() == QDialog::Accepted) {
        QString fromState = fromStateCombo->currentText();
        QString toState = toStateCombo->currentText();
        double probability = probabilitySpin->value();
        
        currentModel->addTransition(fromState, toState, probability);
        updateModelView(currentModel);
    }
}

void MarkovModelLibraryWindow::onRemoveTransitionClicked()
{
    if (!currentModel) {
        QMessageBox::warning(this, "Error", "No model selected.");
        return;
    }
    
    QMap<QString, QVector<QPair<QString, double>>> transitions = currentModel->transitions();
    if (transitions.isEmpty()) {
        QMessageBox::information(this, "No Transitions", "There are no transitions to remove.");
        return;
    }
    
    // Create list of transitions
    QStringList transitionList;
    for (auto it = transitions.begin(); it != transitions.end(); ++it) {
        QString fromState = it.key();
        for (const auto& transition : it.value()) {
            QString toState = transition.first;
            double probability = transition.second;
            transitionList.append(QString("%1 -> %2 (%.2f)").arg(fromState).arg(toState).arg(probability));
        }
    }
    
    bool ok;
    QString transitionStr = QInputDialog::getItem(this, "Remove Transition", "Select Transition to Remove:",
                                                transitionList, 0, false, &ok);
    
    if (ok && !transitionStr.isEmpty()) {
        // Parse the transition string
        QRegExp regex("(\\w+)\\s*->\\s*(\\w+)\\s*\\((\\d+\\.\\d+)\\)");
        if (regex.indexIn(transitionStr) != -1) {
            QString fromState = regex.cap(1);
            QString toState = regex.cap(2);
            
            currentModel->removeTransition(fromState, toState);
            updateModelView(currentModel);
        }
    }
}

void MarkovModelLibraryWindow::onGenerateSequenceClicked()
{
    if (!currentModel) {
        QMessageBox::warning(this, "Error", "No model selected.");
        return;
    }
    
    bool ok;
    int length = QInputDialog::getInt(this, "Generate Sequence", "Sequence Length:",
                                    10, 1, 100, 1, &ok);
    
    if (ok) {
        QString sequence = currentModel->generateSequence(length);
        QMessageBox::information(this, "Generated Sequence", sequence);
    }
}

void MarkovModelLibraryWindow::updateModelView(const QSharedPointer<MarkovModel>& model)
{
    // Update model details in the right panel
    ui->modelLabel->setText("Model: " + model->name());
    
    // Create a text display for the model details
    QTextEdit *textEdit = new QTextEdit(ui->modelViewWidget);
    textEdit->setReadOnly(true);
    
    QString details;
    details += "Name: " + model->name() + "\n";
    details += "Type: " + model->type() + "\n";
    details += "Description: " + model->description() + "\n\n";
    
    details += "States and Transitions:\n";
    QMap<QString, QVector<QPair<QString, double>>> transitions = model->transitions();
    for (auto it = transitions.begin(); it != transitions.end(); ++it) {
        QString state = it.key();
        details += "State: " + state + "\n";
        
        for (const auto& transition : it.value()) {
            QString toState = transition.first;
            double probability = transition.second;
            details += "  -> " + toState + " (probability: " + QString::number(probability) + ")\n";
        }
        details += "\n";
    }
    
    textEdit->setText(details);
    
    // Clear any existing layout
    QLayout *layout = ui->modelViewWidget->layout();
    if (layout) {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete layout;
    }
    
    // Set new layout
    QVBoxLayout *newLayout = new QVBoxLayout(ui->modelViewWidget);
    newLayout->addWidget(textEdit);
    ui->modelViewWidget->setLayout(newLayout);
}

void MarkovModelLibraryWindow::clearModelView()
{
    ui->modelLabel->setText("Model View");
    
    // Clear any existing layout
    QLayout *layout = ui->modelViewWidget->layout();
    if (layout) {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete layout;
    }
    
    // Set new empty layout
    QVBoxLayout *newLayout = new QVBoxLayout(ui->modelViewWidget);
    ui->modelViewWidget->setLayout(newLayout);
}

void MarkovModelLibraryWindow::addModelToTree(const QSharedPointer<MarkovModel>& model)
{
    QList<QStandardItem*> row;
    row << new QStandardItem(model->name());
    row << new QStandardItem(model->type());
    row << new QStandardItem(model->description());
    
    this->model->appendRow(row);
}

void MarkovModelLibraryWindow::removeModelFromTree(const QString& modelName)
{
    QModelIndexList indices = this->model->match(this->model->index(0, 0), Qt::DisplayRole, modelName, 1, Qt::MatchExactly);
    if (!indices.isEmpty()) {
        this->model->removeRow(indices.first().row());
    }
}

void MarkovModelLibraryWindow::filterModels(const QString& searchText)
{
    // Show all rows if search text is empty
    if (searchText.isEmpty()) {
        for (int i = 0; i < model->rowCount(); ++i) {
            ui->modelTreeView->setRowHidden(i, false);
        }
        return;
    }
    
    // Hide rows that don't match the search text
    for (int i = 0; i < model->rowCount(); ++i) {
        QModelIndex nameIndex = model->index(i, 0);
        QModelIndex typeIndex = model->index(i, 1);
        QModelIndex descIndex = model->index(i, 2);
        
        QString name = model->data(nameIndex).toString();
        QString type = model->data(typeIndex).toString();
        QString desc = model->data(descIndex).toString();
        
        bool match = name.contains(searchText, Qt::CaseInsensitive) ||
                    type.contains(searchText, Qt::CaseInsensitive) ||
                    desc.contains(searchText, Qt::CaseInsensitive);
        
        ui->modelTreeView->setRowHidden(i, !match);
    }
} 