#include "LayerBox.hpp"
#include "../inst.hpp"

LayerBox::LayerBox(Layer layer, QWidget* parent)
    : QFrame(parent),
      m_layer(layer)
{
    // Main Layout
    m_mainLayout = new QVBoxLayout();
    this->setLayout(m_mainLayout);
    this->setFrameShape(QFrame::StyledPanel);
    this->setLineWidth(1);

    // Header
    auto* weightHBox = new QHBoxLayout;
    auto* weightLabel = new QLabel("Number of children in this layer:");
    m_weightEntry = new QLineEdit;
    m_weightFuncButton = new QPushButton("Insert Function");
    m_deleteLayerButton = new QPushButton("Delete This Layer");

    connect(m_weightFuncButton, &QPushButton::clicked,
            this, &LayerBox::onWeightFunctionClicked);
    connect(m_deleteLayerButton, &QPushButton::clicked,
            this, &LayerBox::onDeleteLayerClicked);

    weightHBox->addWidget(weightLabel);
    weightHBox->addWidget(m_weightEntry);
    weightHBox->addWidget(m_weightFuncButton);
    weightHBox->addWidget(m_deleteLayerButton);

    m_mainLayout->addLayout(weightHBox);

    // Tree View
    m_treeView = new QTreeView;
    m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_model = new QStandardItemModel;
    m_model->setHorizontalHeaderLabels({"Child Type", "Class", "Name"});

    m_treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_treeView->header()->setStretchLastSection(false);

    m_treeView->setModel(m_model);
    m_treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_treeView->setAcceptDrops(true);
    m_treeView->setDropIndicatorShown(true);

    m_treeView->setDragDropMode(QAbstractItemView::DropOnly);
    m_treeView->setDefaultDropAction(Qt::MoveAction);
    m_treeView->setRootIsDecorated(false);
    m_treeView->setIndentation(0);

    m_treeView->viewport()->installEventFilter(this);

    m_mainLayout->addWidget(m_treeView);

    this->setMinimumHeight(200);
}


QStandardItem* LayerBox::extractItemFromDrop(QDropEvent* event)
{
    const QMimeData* mime = event->mimeData();
    if (!mime->hasFormat("application/x-qabstractitemmodeldatalist")) {
        qDebug() << "extractItemFromDrop: MIME format not matched";
        return nullptr;
    }

    QByteArray encoded = mime->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&encoded, QIODevice::ReadOnly);

    // To be extracted
    QString foundType;
    QString foundName;
    bool gotType = false;
    bool gotName = false;

    while (!stream.atEnd()) {
        int row, col;
        QMap<int, QVariant> roleDataMap;
        stream >> row >> col >> roleDataMap;

        // qDebug() << "  stream entry: row=" << row << " col=" << col;
        // for (auto it = roleDataMap.constBegin(); it != roleDataMap.constEnd(); ++it) {
        //     qDebug() << "    role" << it.key() << "=" << it.value().toString();
        // }

        QString roleType = roleDataMap.value(Qt::UserRole + 1).toString();
        QString roleName = roleDataMap.value(Qt::UserRole + 2).toString();
        if (!roleType.isEmpty()) {
            foundType = roleType;
            gotType = true;
        }
        if (!roleName.isEmpty()) {
            foundName = roleName;
            gotName = true;
        }

        // If both found, stop reading further entries
        if (gotType && gotName) break;
    }

    if (!gotType && !gotName) {
        qDebug() << "extractItemFromDrop: no usable data found";
        return nullptr;
    }

    // Build a temporary QStandardItem and attach both pieces of info in UserRole slots
    QStandardItem* item = new QStandardItem;
    if (gotName) {
        item->setText(foundName);
    } else {
        item->setText(foundType);
    }
    item->setData(foundType, Qt::UserRole + 1);
    item->setData(foundName, Qt::UserRole + 2);

    // qDebug() << "extractItemFromDrop: returning item -> type:" << foundType << " name:" << foundName;
    return item;
}

void LayerBox::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void LayerBox::dropEvent(QDropEvent* event) {

    event->acceptProposedAction();

    QStandardItem* droppedItem = extractItemFromDrop(event);
    if (!droppedItem) {
        qDebug() << "extractItemFromDrop returned null";
        return;
    }

    int newIndex = m_model->rowCount();
    m_model->appendRow({
        new QStandardItem(QString::number(newIndex)),
        new QStandardItem(droppedItem->text()),
        new QStandardItem("Dropped item info here")
    });

    // qDebug() << "Row added, total rows:" << m_model->rowCount();
}

bool LayerBox::eventFilter(QObject* obj, QEvent* event) {
    if (obj == m_treeView->viewport()) {
        if (event->type() == QEvent::Drop) {

            ///////////////////////////////////////////////////////////////
            // Extract dropped item data

            QDropEvent* dropEvent = static_cast<QDropEvent*>(event);
            QStandardItem* droppedItem = extractItemFromDrop(dropEvent);

            if (!droppedItem) {
                qDebug() << "eventFilter: extractItemFromDrop returned null";
                return false;
            }

            qDebug() << "eventFilter: Item is" << droppedItem->data(Qt::UserRole + 2).toString();

            QString droppedType = droppedItem->data(Qt::UserRole + 1).toString();
            QString droppedName = droppedItem->data(Qt::UserRole + 2).toString();

            int index = m_model->rowCount();
            m_model->appendRow({
                new QStandardItem(QString::number(index)),
                new QStandardItem(droppedType),
                new QStandardItem(droppedName)
            });

            ///////////////////////////////////////////////////////////////
            // Create a Package struct using backend event info
            
            Package pkg;
            pkg.event_name = droppedName;
            pkg.event_type = droppedType;
            // pkg.weight = "";
            // pkg.attack_envelope = "";
            // pkg.attackenvelope_scale = "";
            // pkg.duration_envelope = "";
            // pkg.durationenvelope_scale = "";

            qDebug() << "Created package";

            ///////////////////////////////////////////////////////////////
            // Add to this event's Layer
            m_layer.discrete_packages.append(pkg);
            qDebug() << "Added package" << droppedName 
                    << "- total packages:" << m_layer.discrete_packages.size();

            return true;
        }
    }
    return QFrame::eventFilter(obj, event);
}


LayerBox::~LayerBox() {}