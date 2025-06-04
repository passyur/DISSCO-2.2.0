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
#include <QTreeWidgetItem>

ProjectViewController::ProjectViewController(QTreeWidget *treeWidget, QWidget *parent)
    : QWidget(parent)
    , treeWidget(treeWidget)
    , model(nullptr)
    , ui(new Ui::ProjectViewController)
{
    ui->setupUi(this);
    initializeModel();
    setupConnections();
}

ProjectViewController::~ProjectViewController()
{
    delete ui;
}

int ProjectViewController::getProjectCount() const
{
    return model ? model->rowCount() : 0;
}

void ProjectViewController::addProject(const QString &filePath)
{
    if (!model)
        return;

    QStandardItem *item = new QStandardItem(filePath);
    model->appendRow(item);
}

void ProjectViewController::setupConnections()
{
    connect(ui->newButton, &QPushButton::clicked, this, &ProjectViewController::onNewButtonClicked);
    connect(ui->openButton, &QPushButton::clicked, this, &ProjectViewController::onOpenButtonClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &ProjectViewController::onSaveButtonClicked);
    connect(ui->closeButton, &QPushButton::clicked, this, &ProjectViewController::onCloseButtonClicked);

    connect(ui->searchEdit, &QLineEdit::textChanged, this, &ProjectViewController::onSearchTextChanged);

    connect(ui->projectTreeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &ProjectViewController::onProjectSelectionChanged);
}

void ProjectViewController::initializeModel()
{
    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels(QStringList() << tr("Projects"));
    ui->projectTreeView->setModel(model);
}

void ProjectViewController::onNewButtonClicked()
{
    QString filePath = QFileDialog::getSaveFileName(this,
        tr("New Project"), QString(),
        tr("Project Files (*.proj);;All Files (*)"));

    if (!filePath.isEmpty()) {
        addProject(filePath);
    }
}

void ProjectViewController::onOpenButtonClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Open Project"), QString(),
        tr("Project Files (*.proj);;All Files (*)"));

    if (!filePath.isEmpty()) {
        addProject(filePath);
    }
}

void ProjectViewController::onSaveButtonClicked()
{
    QModelIndex currentIndex = ui->projectTreeView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, tr("Save Project"),
                           tr("Please select a project to save."));
        return;
    }

    // TODO: Implement save functionality
}

void ProjectViewController::onCloseButtonClicked()
{
    QModelIndex currentIndex = ui->projectTreeView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, tr("Close Project"),
                           tr("Please select a project to close."));
        return;
    }

    model->removeRow(currentIndex.row());
}

void ProjectViewController::onSearchTextChanged(const QString &text)
{
    if (!model)
        return;

    for (int i = 0; i < model->rowCount(); ++i) {
        QModelIndex index = model->index(i, 0);
        QString itemText = model->data(index).toString();
        bool match = itemText.contains(text, Qt::CaseInsensitive);
        ui->projectTreeView->setRowHidden(i, QModelIndex(), !match);
    }
}

void ProjectViewController::onProjectSelectionChanged()
{
    QModelIndex currentIndex = ui->projectTreeView->currentIndex();
    if (!currentIndex.isValid()) {
        return;
    }

    QString projectPath = model->data(currentIndex).toString();
    // TODO: Update project details view
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