#include "ObjectWindow.hpp"
#include "UI/ObjectWindow.ui"
#include <QMessageBox>
#include <QInputDialog>
#include <QMenu>
#include <QContextMenuEvent>

ObjectWindow::ObjectWindow(EventType type, QWidget *parent)
    : QWidget(parent)
    , eventType(type)
    , activeProject(nullptr)
    , objectModel(new QStandardItemModel(this))
{
    setupUi();
    createConnections();
    
    objectModel->setHorizontalHeaderLabels(QStringList() << "Name" << "Type" << "Description");
    ui->objectTreeView->setModel(objectModel);
}

ObjectWindow::~ObjectWindow()
{
    delete ui;
}

void ObjectWindow::setupUi()
{
    QUiLoader loader;
    QFile file(":/UI/ObjectWindow.ui");
    file.open(QFile::ReadOnly);
    QWidget *formWidget = loader.load(&file, this);
    file.close();
    
    if (formWidget) {
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(formWidget);
        setLayout(layout);
    }
    
    setWindowTitle(QString("Object Window - %1").arg(QString::fromStdString(getEventTypeString(eventType))));
}

void ObjectWindow::createConnections()
{
    connect(ui->addButton, &QPushButton::clicked, this, &ObjectWindow::onAddClicked);
    connect(ui->removeButton, &QPushButton::clicked, this, &ObjectWindow::onRemoveClicked);
    connect(ui->editButton, &QPushButton::clicked, this, &ObjectWindow::onEditClicked);
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &ObjectWindow::onSearchTextChanged);
    connect(ui->objectTreeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &ObjectWindow::onSelectionChanged);
}

void ObjectWindow::setActiveProject(ProjectViewController* project)
{
    activeProject = project;
    refresh();
}

void ObjectWindow::refresh()
{
    if (!activeProject) return;
    
    clearObjectView();
    updateObjectView();
}

QString ObjectWindow::getSelectedFolder() const
{
    QModelIndex index = ui->objectTreeView->currentIndex();
    if (!index.isValid()) return QString();
    
    return objectModel->data(index, Qt::DisplayRole).toString();
}

QSharedPointer<IEvent> ObjectWindow::getCurrentSelectedEvent() const
{
    QModelIndex index = ui->objectTreeView->currentIndex();
    if (!index.isValid()) return nullptr;
    
    QString name = objectModel->data(index, Qt::DisplayRole).toString();
    return objects.value(name);
}

void ObjectWindow::onAddClicked()
{
    if (!activeProject) return;
    
    bool ok;
    QString name = QInputDialog::getText(this, tr("Add Object"),
                                       tr("Enter object name:"), QLineEdit::Normal,
                                       QString(), &ok);
    
    if (ok && !name.isEmpty()) {
        auto event = QSharedPointer<IEvent>::create(eventType);
        event->setName(name.toStdString());
        objects[name] = event;
        addObjectToView(event);
    }
}

void ObjectWindow::onRemoveClicked()
{
    QModelIndex index = ui->objectTreeView->currentIndex();
    if (!index.isValid()) return;
    
    QString name = objectModel->data(index, Qt::DisplayRole).toString();
    QSharedPointer<IEvent> event = objects.value(name);
    
    if (event) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Remove Object"),
            tr("Are you sure you want to remove '%1'?").arg(name),
            QMessageBox::Yes | QMessageBox::No);
            
        if (reply == QMessageBox::Yes) {
            removeObjectFromView(event);
            objects.remove(name);
        }
    }
}

void ObjectWindow::onEditClicked()
{
    QModelIndex index = ui->objectTreeView->currentIndex();
    if (!index.isValid()) return;
    
    QString name = objectModel->data(index, Qt::DisplayRole).toString();
    QSharedPointer<IEvent> event = objects.value(name);
    
    if (event) {
        // TODO: Open event editor dialog
    }
}

void ObjectWindow::onSearchTextChanged(const QString &text)
{
    for (int row = 0; row < objectModel->rowCount(); ++row) {
        QModelIndex index = objectModel->index(row, 0);
        QString name = objectModel->data(index, Qt::DisplayRole).toString();
        ui->objectTreeView->setRowHidden(row, !name.contains(text, Qt::CaseInsensitive));
    }
}

void ObjectWindow::onSelectionChanged()
{
    bool hasSelection = ui->objectTreeView->currentIndex().isValid();
    ui->removeButton->setEnabled(hasSelection);
    ui->editButton->setEnabled(hasSelection);
}

void ObjectWindow::updateObjectView()
{
    if (!activeProject) return;
    
    // TODO: Get objects from active project and add them to the view
    // This will depend on how objects are stored in the ProjectViewController
}

void ObjectWindow::clearObjectView()
{
    objectModel->removeRows(0, objectModel->rowCount());
    objects.clear();
}

void ObjectWindow::addObjectToView(const QSharedPointer<IEvent> &event)
{
    QList<QStandardItem*> items;
    items << new QStandardItem(QString::fromStdString(event->getName()))
          << new QStandardItem(QString::fromStdString(getEventTypeString(event->getType())))
          << new QStandardItem(QString::fromStdString(event->getDescription()));
          
    objectModel->appendRow(items);
}

void ObjectWindow::removeObjectFromView(const QSharedPointer<IEvent> &event)
{
    for (int row = 0; row < objectModel->rowCount(); ++row) {
        QModelIndex index = objectModel->index(row, 0);
        QString name = objectModel->data(index, Qt::DisplayRole).toString();
        if (name == QString::fromStdString(event->getName())) {
            objectModel->removeRow(row);
            break;
        }
    }
} 