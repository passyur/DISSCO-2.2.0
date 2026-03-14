#include "Partials.hpp"
#include "../inst.hpp"

Partials::Partials(unsigned eventIndex, int partialIndex, QWidget* parent)
    : QFrame(parent),
      m_eventIndex(eventIndex),
      m_partialIndex(partialIndex)
{
    // Main Layout
    m_mainLayout = new QVBoxLayout();
    this->setLayout(m_mainLayout);
    this->setFrameShape(QFrame::NoFrame);
//     this->setLineWidth(1);

    // Header
    auto* partialHBox = new QHBoxLayout;
    m_partialLabel = new QLabel("Partial " + QString::number(partialIndex + 1) + ":");
    m_partialEntry = new QLineEdit;
    m_insertFuncButton = new QPushButton("Insert Function");
    m_removePartialButton = new QPushButton("Remove Partial");

    connect(m_insertFuncButton, &QPushButton::clicked,
            this, &Partials::onInsertFunctionClicked);
    connect(m_removePartialButton, &QPushButton::clicked,
            this, &Partials::onRemovePartialClicked);
    connect(m_partialEntry, &QLineEdit::textChanged,
            this, &Partials::onPartialChanged);

    partialHBox->addWidget(m_partialLabel);
    partialHBox->addWidget(m_partialEntry);
    partialHBox->addWidget(m_insertFuncButton);
    partialHBox->addWidget(m_removePartialButton);

    m_mainLayout->addLayout(partialHBox);

    this->setMinimumHeight(50);

    // Populate UI from backend (handles reload when switching events)
    if (m_partialIndex >= 0) {
        QString partial = getBackendLayer().partials[m_partialIndex];
        // Block signals so onPartialChanged doesn't write back during initialisation
        m_partialEntry->blockSignals(true);
        m_partialEntry->setText(partial);
        m_partialEntry->blockSignals(false);
    }
}


Spectrum& Partials::getBackendLayer() {
    ProjectManager* pm = Inst::get_project_manager();
    // return sevent->spectrum->partials[m_partialIndex];
    return (&pm->spectrumevents()[m_eventIndex])->spectrum;
}


void Partials::onPartialChanged(const QString& text) {
    getBackendLayer().partials[m_partialIndex] = text;
}

void Partials::onRemovePartialClicked() {
    emit deleteRequested(this);
}

Partials::~Partials() {}