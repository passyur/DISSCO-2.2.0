#include "Stochos.hpp"
#include "../inst.hpp"
#include "../dialogs/FunctionGenerator.hpp"

Stochos::Stochos(int methodType, int stochosIndex, QWidget *parent, QTextEdit* resultEntry)
    : QFrame(parent),
      m_stochosIndex(stochosIndex),
      m_methodType(methodType)
{
    // Main Layout
    m_mainLayout = new QVBoxLayout(this);
    this->setFrameShape(QFrame::NoFrame);
    QHBoxLayout* stochosHBox = new QHBoxLayout();

    if (methodType == 0) {
        auto* minLabel = new QLabel("Min:");
        m_minEntry = new QLineEdit;
        auto* maxLabel = new QLabel("Max:");
        m_maxEntry = new QLineEdit;
        auto* distLabel = new QLabel("Distribution:");
        m_distEntry = new QLineEdit;
        m_removeStochosButton = new QPushButton("Remove Node");

        connect(m_removeStochosButton, &QPushButton::clicked, this, &Stochos::onRemoveNodeClicked);
        connect(m_minEntry, &QLineEdit::textChanged, this, &Stochos::onNodeTextChanged);
        connect(m_maxEntry, &QLineEdit::textChanged, this, &Stochos::onNodeTextChanged);
        connect(m_distEntry, &QLineEdit::textChanged, this, &Stochos::onNodeTextChanged);
        connect(m_minEntry, &QLineEdit::cursorPositionChanged, this, [this](){ emit editFocused(m_minEntry); });
        connect(m_maxEntry, &QLineEdit::cursorPositionChanged, this, [this](){ emit editFocused(m_maxEntry); });
        connect(m_distEntry, &QLineEdit::cursorPositionChanged, this, [this](){ emit editFocused(m_distEntry); });

        stochosHBox->addWidget(minLabel);
        stochosHBox->addWidget(m_minEntry);
        stochosHBox->addWidget(maxLabel);
        stochosHBox->addWidget(m_maxEntry);
        stochosHBox->addWidget(distLabel);
        stochosHBox->addWidget(m_distEntry);
        stochosHBox->addWidget(m_removeStochosButton);

        this->m_minEntry->setFixedHeight(20);
        this->m_maxEntry->setFixedHeight(20);
        this->m_distEntry->setFixedHeight(20);
    }
    else if (methodType == 1) {
        auto* valLabel = new QLabel("Value:");
        m_valEntry = new QLineEdit;
        m_removeStochosButton = new QPushButton("Remove Node");

        connect(m_removeStochosButton, &QPushButton::clicked, this, &Stochos::onRemoveNodeClicked);
        connect(m_valEntry, &QLineEdit::textChanged, this, &Stochos::onNodeTextChanged);
        connect(m_valEntry, &QLineEdit::cursorPositionChanged, this, [this](){ emit editFocused(m_valEntry); });

        stochosHBox->addWidget(valLabel);
        stochosHBox->addWidget(m_valEntry);
        stochosHBox->addWidget(m_removeStochosButton);

        this->m_valEntry->setFixedHeight(20);
    }
    m_mainLayout->addLayout(stochosHBox);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    this->setFixedHeight(50);
}

QString Stochos::getNodeText() {
    QString nodeText = "";
    if (m_methodType == 0) { // Range
        nodeText += "<Envelope>" + m_minEntry->text() + "</Envelope>";
        nodeText += "<Envelope>" + m_maxEntry->text() + "</Envelope>";
        nodeText += "<Envelope>" + m_distEntry->text() + "</Envelope>";
    } else { // Function/Value
        nodeText += "<Envelope>" + m_valEntry->text() + "</Envelope>";
    }
    return nodeText;
}


void Stochos::onNodeTextChanged() { emit nodeTextChanged(this); }

void Stochos::onRemoveNodeClicked() { emit deleteRequested(this); }

Stochos::~Stochos() {}