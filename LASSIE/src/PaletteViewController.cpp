#include "PaletteViewController.hpp"
#include "UI/PaletteViewController.ui"
#include <QDrag>
#include <QMimeData>
#include <QApplication>

PaletteViewController::PaletteViewController(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PaletteViewController)
{
    setupUi();
    createConnections();
    initializeTools();
}

PaletteViewController::~PaletteViewController()
{
    delete ui;
}

void PaletteViewController::setupUi()
{
    QUiLoader loader;
    QFile file(":/UI/PaletteViewController.ui");
    file.open(QFile::ReadOnly);
    QWidget *formWidget = loader.load(&file, this);
    file.close();
    
    if (formWidget) {
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(formWidget);
        setLayout(layout);
    }
}

void PaletteViewController::createConnections()
{
    connect(ui->toolList, &QListWidget::itemSelectionChanged,
            this, &PaletteViewController::onToolSelectionChanged);
    connect(ui->objectTree, &QTreeWidget::itemSelectionChanged,
            this, &PaletteViewController::onObjectSelectionChanged);
    connect(ui->toolList, &QListWidget::itemPressed,
            this, &PaletteViewController::onToolDragStarted);
    connect(ui->objectTree, &QTreeWidget::itemPressed,
            this, &PaletteViewController::onObjectDragStarted);
}

void PaletteViewController::initializeTools()
{
    // Initialize default tools
    // This will be populated based on the application's requirements
    updateToolList();
}

void PaletteViewController::addTool(const QSharedPointer<Tool> &tool)
{
    if (tool) {
        tools[tool->getName()] = tool;
        updateToolList();
    }
}

void PaletteViewController::removeTool(const QString &toolName)
{
    if (tools.remove(toolName)) {
        updateToolList();
    }
}

void PaletteViewController::addObject(const QSharedPointer<IEvent> &object)
{
    if (object) {
        objects[object->getName()] = object;
        updateObjectTree();
    }
}

void PaletteViewController::removeObject(const QString &objectName)
{
    if (objects.remove(objectName)) {
        updateObjectTree();
    }
}

void PaletteViewController::clearObjects()
{
    objects.clear();
    updateObjectTree();
}

void PaletteViewController::refreshObjects()
{
    updateObjectTree();
}

QSharedPointer<Tool> PaletteViewController::getSelectedTool() const
{
    QListWidgetItem *item = ui->toolList->currentItem();
    if (item) {
        return tools.value(item->text());
    }
    return nullptr;
}

QSharedPointer<IEvent> PaletteViewController::getSelectedObject() const
{
    QTreeWidgetItem *item = ui->objectTree->currentItem();
    if (item) {
        return objects.value(item->text(0));
    }
    return nullptr;
}

void PaletteViewController::onToolSelectionChanged()
{
    QSharedPointer<Tool> tool = getSelectedTool();
    if (tool) {
        // Emit signal or perform action when tool is selected
    }
}

void PaletteViewController::onObjectSelectionChanged()
{
    QSharedPointer<IEvent> object = getSelectedObject();
    if (object) {
        // Emit signal or perform action when object is selected
    }
}

void PaletteViewController::onToolDragStarted()
{
    QListWidgetItem *item = ui->toolList->currentItem();
    if (item) {
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(item->text());
        drag->setMimeData(mimeData);
        drag->exec(Qt::CopyAction);
    }
}

void PaletteViewController::onObjectDragStarted()
{
    QTreeWidgetItem *item = ui->objectTree->currentItem();
    if (item) {
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(item->text(0));
        drag->setMimeData(mimeData);
        drag->exec(Qt::CopyAction);
    }
}

void PaletteViewController::updateToolList()
{
    ui->toolList->clear();
    for (auto it = tools.begin(); it != tools.end(); ++it) {
        ui->toolList->addItem(it.key());
    }
}

void PaletteViewController::updateObjectTree()
{
    ui->objectTree->clear();
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->objectTree);
        item->setText(0, it.key());
        // Add additional columns if needed
    }
} 