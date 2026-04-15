#include "Select.hpp"
#include "../inst.hpp"
#include "../dialogs/FunctionGenerator.hpp"

Select::Select(int selectIndex, QWidget *parent, QTextEdit* resultEntry)
    : QFrame(parent),
      m_selectIndex(selectIndex)
{
    // Main Layout
    m_mainLayout = new QVBoxLayout(this);
    this->setFrameShape(QFrame::NoFrame);
    QHBoxLayout* selectHBox = new QHBoxLayout();

    auto* valLabel = new QLabel("Value:");
    m_valEntry = new QLineEdit;
    m_removeSelectButton = new QPushButton("Remove Node");

    connect(m_removeSelectButton, &QPushButton::clicked, this, &Select::onRemoveNodeClicked);
    connect(m_valEntry, &QLineEdit::textChanged, this, &Select::onNodeTextChanged);
    connect(m_valEntry, &QLineEdit::cursorPositionChanged, this, [this](){ emit editFocused(m_valEntry); });

    selectHBox->addWidget(valLabel);
    selectHBox->addWidget(m_valEntry);
    selectHBox->addWidget(m_removeSelectButton);

    this->m_valEntry->setFixedHeight(20);
    m_mainLayout->addLayout(selectHBox);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    this->setFixedHeight(50);
}

QString Select::getNodeText() {
    return m_valEntry->text();
}


void Select::onNodeTextChanged() { emit nodeTextChanged(this); }

void Select::onRemoveNodeClicked() { emit deleteRequested(this); }

Select::~Select() {}