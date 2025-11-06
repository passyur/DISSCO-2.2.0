#include "LayerBox.hpp"
#include "../inst.hpp"

LayerBox::LayerBox(EventAttributesViewController* parentController,
                   ProjectView* projectView
                   //EventLayer* layerModel,
                    // bool showDiscreteColumns
                   )
    : QFrame(nullptr),
      m_attributesView(parentController),
      m_projectView(projectView)
     // m_layerModel(layerModel)
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
    
    /// TODO: implement reordering and deleting

    // Drag and Drop Handling
    m_treeView->setDragDropMode(QAbstractItemView::DropOnly);
    m_treeView->setDefaultDropAction(Qt::MoveAction);
    m_treeView->setDropIndicatorShown(true);
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

        qDebug() << "  stream entry: row=" << row << " col=" << col;
        for (auto it = roleDataMap.constBegin(); it != roleDataMap.constEnd(); ++it) {
            qDebug() << "    role" << it.key() << "=" << it.value().toString();
        }

        QString roleType = roleDataMap.value(Qt::UserRole + 1).toString();
        QString roleName = roleDataMap.value(Qt::UserRole + 2).toString();
        if (!roleType.isEmpty()) {
            foundType = roleType;
            gotType = true;
            qDebug() << "    found type in user role:" << foundType;
        }
        if (!roleName.isEmpty()) {
            foundName = roleName;
            gotName = true;
            qDebug() << "    found name in user role:" << foundName;
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

    qDebug() << "extractItemFromDrop: returning item -> type:" << foundType << " name:" << foundName;
    return item;
}

void LayerBox::dragEnterEvent(QDragEnterEvent* event) {
    qDebug() << "LayerBox::dragEnterEvent triggered";
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void LayerBox::dropEvent(QDropEvent* event) {
    qDebug() << "LayerBox::dropEvent triggered";
    event->acceptProposedAction();

    QStandardItem* droppedItem = extractItemFromDrop(event);
    if (!droppedItem) {
        qDebug() << "extractItemFromDrop returned null";
        return;
    }
    qDebug() << "Dropped item extracted:" << droppedItem->text();

    int newIndex = m_model->rowCount();
    m_model->appendRow({
        new QStandardItem(QString::number(newIndex)),
        new QStandardItem(droppedItem->text()),
        new QStandardItem("Dropped item info here")
    });

    qDebug() << "Row added, total rows:" << m_model->rowCount();
}

bool LayerBox::eventFilter(QObject* obj, QEvent* event) {
    if (obj == m_treeView->viewport()) {
        if (event->type() == QEvent::Drop) {

            ///////////////////////////////////////////////////////////////
            // STEP 1: Extract dropped item data

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

            qDebug() << "eventFilter: Row appended, now" << m_model->rowCount() << "rows";

            ///////////////////////////////////////////////////////////////
            // STEP 2: Access backend ProjectManager
            ProjectManager* pm = Inst::get_project_manager();

            ///////////////////////////////////////////////////////////////
            // STEP 3: Determine which event list to search

            // m_curreventtype

            // wait a minute... we need to figure out whose layer we dropped into, not what was dropped in...
            // we will only be in Top, High, Mid, Low

            QList<HEvent>* eventList = nullptr;
            // // QString& topevent = &pm->topevent(); 
            // eventList = &pm->hevents();

            // check if topevent is null(?)
            // check if eventList is null

            // if (!eventList)
            //     return false;

            ///////////////////////////////////////////////////////////////
            // STEP 4: Find the true backend object by name

        //     HEvent backendEvent;
        //     bool found = false;

        //     for (const HEvent& evt : *eventList) {
        //         if (evt.name == droppedName) {
        //             backendEvent = evt;
        //             found = true;
        //             break;
        //         }
        //     }

        //     if (!found) {
        //         qDebug() << "Dropped event not found in ProjectManager lists!";
        //         return false;
        //     }

            ///////////////////////////////////////////////////////////////
            // STEP 5: Create a Package struct using backend event info
            
            Package pkg;
        //     pkg.event_name = backendEvent.name;
            pkg.event_type = droppedType;
            pkg.weight = "";
            pkg.attack_envelope = "";
            pkg.attackenvelope_scale = "";
            pkg.duration_envelope = "";
            pkg.durationenvelope_scale = "";

            ///////////////////////////////////////////////////////////////
            // STEP 6: Add to this event's Layer
        //     Layer layer;
        //     layer.discrete_packages.append(pkg);

            return true; // handled
        }
    }
    return QFrame::eventFilter(obj, event);
}


LayerBox::~LayerBox() {}