#include "LayerBox.hpp"

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
    m_mainLayout = new QVBoxLayout();
    this->setLayout(m_mainLayout);
    this->setFrameShape(QFrame::StyledPanel);
    this->setLineWidth(1);

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
    m_model = new QStandardItemModel;
    m_model->setHorizontalHeaderLabels({"Child Type", "Class", "Name"});
    m_treeView->setModel(m_model);
    m_treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_mainLayout->addWidget(m_treeView);

    this->setMinimumHeight(200);
}

LayerBox::~LayerBox() {}