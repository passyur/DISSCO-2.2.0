#include "ProjectViewController.hpp"
#include "ui_ProjectViewController.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QInputDialog>
#include <QFormLayout>
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>

ProjectViewController::ProjectViewController(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProjectViewController)
    , model(nullptr)
    , currentProject(nullptr)
{
    ui->setupUi(this);
    setupConnections();
    initializeModel();
}

ProjectViewController::~ProjectViewController()
{
    delete ui;
}

void ProjectViewController::setupConnections()
{
    // Connect button signals to slots
    connect(ui->newButton, &QPushButton::clicked, this, &ProjectViewController::onNewButtonClicked);
    connect(ui->openButton, &QPushButton::clicked, this, &ProjectViewController::onOpenButtonClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &ProjectViewController::onSaveButtonClicked);
    connect(ui->closeButton, &QPushButton::clicked, this, &ProjectViewController::onCloseButtonClicked);
    
    // Connect search functionality
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &ProjectViewController::onSearchTextChanged);
    
    // Connect tree view selection
    connect(ui->projectTreeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &ProjectViewController::onProjectSelectionChanged);
}

void ProjectViewController::initializeModel()
{
    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels(QStringList() << "Name" << "Status" << "Last Modified");
    ui->projectTreeView->setModel(model);
    ui->projectTreeView->setAlternatingRowColors(true);
    ui->projectTreeView->setSortingEnabled(true);
    ui->projectTreeView->setAnimated(true);
    ui->projectTreeView->setIndentation(20);
    ui->projectTreeView->setUniformRowHeights(true);
    
    // Add some sample projects
    QSharedPointer<Project> sampleProject1 = QSharedPointer<Project>::create("Sample Project 1");
    sampleProject1->setDescription("A sample project with Markov models");
    sampleProject1->setStatus("In Progress");
    
    QSharedPointer<MarkovModel> model1 = QSharedPointer<MarkovModel>::create("Model A");
    model1->setDescription("First-order Markov model");
    model1->setType("First-Order");
    model1->addState("A");
    model1->addState("B");
    model1->addState("C");
    model1->addTransition("A", "B", 0.7);
    model1->addTransition("A", "C", 0.3);
    model1->addTransition("B", "A", 0.5);
    model1->addTransition("B", "C", 0.5);
    model1->addTransition("C", "A", 0.4);
    model1->addTransition("C", "B", 0.6);
    
    QSharedPointer<MarkovModel> model2 = QSharedPointer<MarkovModel>::create("Model B");
    model2->setDescription("Second-order Markov model");
    model2->setType("Second-Order");
    model2->addState("X");
    model2->addState("Y");
    model2->addState("Z");
    model2->addTransition("X", "Y", 0.6);
    model2->addTransition("X", "Z", 0.4);
    model2->addTransition("Y", "X", 0.3);
    model2->addTransition("Y", "Z", 0.7);
    model2->addTransition("Z", "X", 0.5);
    model2->addTransition("Z", "Y", 0.5);
    
    sampleProject1->addModel(model1);
    sampleProject1->addModel(model2);
    
    QSharedPointer<Project> sampleProject2 = QSharedPointer<Project>::create("Sample Project 2");
    sampleProject2->setDescription("Another sample project");
    sampleProject2->setStatus("Completed");
    
    QSharedPointer<MarkovModel> model3 = QSharedPointer<MarkovModel>::create("Model C");
    model3->setDescription("A simple Markov model");
    model3->setType("First-Order");
    model3->addState("1");
    model3->addState("2");
    model3->addTransition("1", "2", 0.8);
    model3->addTransition("2", "1", 0.2);
    
    sampleProject2->addModel(model3);
    
    addProjectToTree(sampleProject1);
    addProjectToTree(sampleProject2);
    
    projects[sampleProject1->name()] = sampleProject1;
    projects[sampleProject2->name()] = sampleProject2;
}

void ProjectViewController::onNewButtonClicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "New Project", "Project Name:", QLineEdit::Normal, "", &ok);
    
    if (ok && !name.isEmpty()) {
        // Check if project with this name already exists
        if (projects.contains(name)) {
            QMessageBox::warning(this, "Error", "A project with this name already exists.");
            return;
        }
        
        // Create new project
        QSharedPointer<Project> newProject = QSharedPointer<Project>::create(name);
        newProject->setDescription("A new project");
        newProject->setStatus("New");
        
        // Add to projects map and tree
        projects[name] = newProject;
        addProjectToTree(newProject);
        
        // Select the new project
        QModelIndexList indices = model->match(model->index(0, 0), Qt::DisplayRole, name, 1, Qt::MatchExactly);
        if (!indices.isEmpty()) {
            ui->projectTreeView->setCurrentIndex(indices.first());
        }
    }
}

void ProjectViewController::onOpenButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Open Project", QString(),
        "Project Files (*.proj);;All Files (*)");
        
    if (!fileName.isEmpty()) {
        QSharedPointer<Project> loadedProject = QSharedPointer<Project>::create("");
        if (loadedProject->loadFromFile(fileName)) {
            QString name = loadedProject->name();
            
            // Check if project with this name already exists
            if (projects.contains(name)) {
                int result = QMessageBox::question(this, "Project Exists", 
                    "A project with this name already exists. Do you want to replace it?",
                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
                
                if (result == QMessageBox::No) {
                    return;
                }
                
                // Remove existing project
                removeProjectFromTree(name);
            }
            
            // Add to projects map and tree
            projects[name] = loadedProject;
            addProjectToTree(loadedProject);
            
            // Select the loaded project
            QModelIndexList indices = model->match(model->index(0, 0), Qt::DisplayRole, name, 1, Qt::MatchExactly);
            if (!indices.isEmpty()) {
                ui->projectTreeView->setCurrentIndex(indices.first());
            }
            
            QMessageBox::information(this, "Project Loaded", 
                "Successfully loaded project: " + name);
        } else {
            QMessageBox::warning(this, "Error", 
                "Failed to load project from file: " + fileName);
        }
    }
}

void ProjectViewController::onSaveButtonClicked()
{
    if (!currentProject) {
        QMessageBox::warning(this, "Error", "No project selected.");
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(this,
        "Save Project", currentProject->name() + ".proj",
        "Project Files (*.proj);;All Files (*)");
        
    if (!fileName.isEmpty()) {
        if (currentProject->saveToFile(fileName)) {
            QMessageBox::information(this, "Project Saved", 
                "Successfully saved project to: " + fileName);
        } else {
            QMessageBox::warning(this, "Error", 
                "Failed to save project to file: " + fileName);
        }
    }
}

void ProjectViewController::onCloseButtonClicked()
{
    close();
}

void ProjectViewController::onSearchTextChanged(const QString &text)
{
    filterProjects(text);
}

void ProjectViewController::onProjectSelectionChanged(const QItemSelection &selected,
                                                   const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    
    if (selected.indexes().isEmpty()) {
        clearProjectView();
        currentProject = nullptr;
        return;
    }
    
    QModelIndex index = selected.indexes().first();
    QString projectName = model->data(index).toString();
    
    if (projects.contains(projectName)) {
        currentProject = projects[projectName];
        updateProjectView(currentProject);
    } else {
        clearProjectView();
        currentProject = nullptr;
    }
}

void ProjectViewController::onAddModelClicked()
{
    if (!currentProject) {
        QMessageBox::warning(this, "Error", "No project selected.");
        return;
    }
    
    bool ok;
    QString name = QInputDialog::getText(this, "Add Model", "Model Name:", QLineEdit::Normal, "", &ok);
    
    if (ok && !name.isEmpty()) {
        // Check if model with this name already exists in the project
        if (currentProject->getModel(name)) {
            QMessageBox::warning(this, "Error", "A model with this name already exists in the project.");
            return;
        }
        
        // Create new model
        QSharedPointer<MarkovModel> newModel = QSharedPointer<MarkovModel>::create(name);
        newModel->setDescription("A new Markov model");
        newModel->setType("First-Order");
        
        // Add to project
        currentProject->addModel(newModel);
        
        // Update view
        updateProjectView(currentProject);
    }
}

void ProjectViewController::onRemoveModelClicked()
{
    if (!currentProject) {
        QMessageBox::warning(this, "Error", "No project selected.");
        return;
    }
    
    QVector<QSharedPointer<MarkovModel>> models = currentProject->models();
    if (models.isEmpty()) {
        QMessageBox::information(this, "No Models", "There are no models to remove.");
        return;
    }
    
    QStringList modelNames;
    for (const auto& model : models) {
        modelNames.append(model->name());
    }
    
    bool ok;
    QString modelName = QInputDialog::getItem(this, "Remove Model", "Select Model to Remove:",
                                            modelNames, 0, false, &ok);
    
    if (ok && !modelName.isEmpty()) {
        currentProject->removeModel(modelName);
        updateProjectView(currentProject);
    }
}

void ProjectViewController::onEditModelClicked()
{
    if (!currentProject) {
        QMessageBox::warning(this, "Error", "No project selected.");
        return;
    }
    
    QVector<QSharedPointer<MarkovModel>> models = currentProject->models();
    if (models.isEmpty()) {
        QMessageBox::information(this, "No Models", "There are no models to edit.");
        return;
    }
    
    QStringList modelNames;
    for (const auto& model : models) {
        modelNames.append(model->name());
    }
    
    bool ok;
    QString modelName = QInputDialog::getItem(this, "Edit Model", "Select Model to Edit:",
                                            modelNames, 0, false, &ok);
    
    if (ok && !modelName.isEmpty()) {
        QSharedPointer<MarkovModel> model = currentProject->getModel(modelName);
        if (model) {
            // Create dialog for model details
            QDialog dialog(this);
            dialog.setWindowTitle("Edit Model");
            
            QFormLayout *formLayout = new QFormLayout(&dialog);
            
            QLineEdit *nameEdit = new QLineEdit(model->name(), &dialog);
            formLayout->addRow("Name:", nameEdit);
            
            QLineEdit *typeEdit = new QLineEdit(model->type(), &dialog);
            formLayout->addRow("Type:", typeEdit);
            
            QTextEdit *descEdit = new QTextEdit(&dialog);
            descEdit->setText(model->description());
            formLayout->addRow("Description:", descEdit);
            
            QDialogButtonBox *buttonBox = new QDialogButtonBox(
                QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
            connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
            connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
            formLayout->addRow(buttonBox);
            
            if (dialog.exec() == QDialog::Accepted) {
                QString newName = nameEdit->text();
                
                // Check if the new name conflicts with another model
                if (newName != modelName && currentProject->getModel(newName)) {
                    QMessageBox::warning(this, "Error", "A model with this name already exists in the project.");
                    return;
                }
                
                // Update model
                model->setName(newName);
                model->setType(typeEdit->text());
                model->setDescription(descEdit->toPlainText());
                
                // If name changed, update the model in the project
                if (newName != modelName) {
                    currentProject->removeModel(modelName);
                    currentProject->addModel(model);
                }
                
                // Update view
                updateProjectView(currentProject);
            }
        }
    }
}

void ProjectViewController::updateProjectView(const QSharedPointer<Project>& project)
{
    // Update project details in the right panel
    ui->itemLabel->setText("Project: " + project->name());
    
    // Create a text display for the project details
    QTextEdit *textEdit = new QTextEdit(ui->itemViewWidget);
    textEdit->setReadOnly(true);
    
    QString details;
    details += "Name: " + project->name() + "\n";
    details += "Status: " + project->status() + "\n";
    details += "Description: " + project->description() + "\n";
    details += "Created: " + project->createdDate().toString() + "\n";
    details += "Last Modified: " + project->modifiedDate().toString() + "\n\n";
    
    details += "Models:\n";
    QVector<QSharedPointer<MarkovModel>> models = project->models();
    if (models.isEmpty()) {
        details += "  No models in this project.\n";
    } else {
        for (const auto& model : models) {
            details += "  - " + model->name() + " (" + model->type() + ")\n";
            details += "    Description: " + model->description() + "\n";
        }
    }
    
    textEdit->setText(details);
    
    // Clear any existing layout
    QLayout *layout = ui->itemViewWidget->layout();
    if (layout) {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete layout;
    }
    
    // Set new layout
    QVBoxLayout *newLayout = new QVBoxLayout(ui->itemViewWidget);
    newLayout->addWidget(textEdit);
    ui->itemViewWidget->setLayout(newLayout);
}

void ProjectViewController::clearProjectView()
{
    ui->itemLabel->setText("Item View");
    
    // Clear any existing layout
    QLayout *layout = ui->itemViewWidget->layout();
    if (layout) {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete layout;
    }
    
    // Set new empty layout
    QVBoxLayout *newLayout = new QVBoxLayout(ui->itemViewWidget);
    ui->itemViewWidget->setLayout(newLayout);
}

void ProjectViewController::addProjectToTree(const QSharedPointer<Project>& project)
{
    QList<QStandardItem*> row;
    row << new QStandardItem(project->name());
    row << new QStandardItem(project->status());
    row << new QStandardItem(project->modifiedDate().toString());
    
    model->appendRow(row);
}

void ProjectViewController::removeProjectFromTree(const QString& projectName)
{
    QModelIndexList indices = model->match(model->index(0, 0), Qt::DisplayRole, projectName, 1, Qt::MatchExactly);
    if (!indices.isEmpty()) {
        model->removeRow(indices.first().row());
    }
}

void ProjectViewController::filterProjects(const QString& searchText)
{
    // Show all rows if search text is empty
    if (searchText.isEmpty()) {
        for (int i = 0; i < model->rowCount(); ++i) {
            ui->projectTreeView->setRowHidden(i, false);
        }
        return;
    }
    
    // Hide rows that don't match the search text
    for (int i = 0; i < model->rowCount(); ++i) {
        QModelIndex nameIndex = model->index(i, 0);
        QModelIndex statusIndex = model->index(i, 1);
        
        QString name = model->data(nameIndex).toString();
        QString status = model->data(statusIndex).toString();
        
        bool match = name.contains(searchText, Qt::CaseInsensitive) ||
                    status.contains(searchText, Qt::CaseInsensitive);
        
        ui->projectTreeView->setRowHidden(i, !match);
    }
} 