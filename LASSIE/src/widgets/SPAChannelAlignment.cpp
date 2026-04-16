#include "SPAChannelAlignment.hpp"
#include "SPAPartialAlignment.hpp"
#include "../ui/ui_FunGenSPAPartialAlignment.h"
#include "../ui/ui_FunGenSPAChannelAlignment.h"

#include <QFrame>
#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QDialogButtonBox>
// #include <QOverload>
#include <QDateTime>

#include <string>

#include "../dialogs/FunctionGenerator.hpp"
#include "../ui/ui_FunctionGenerator.h"

SPAChannelAlignment::SPAChannelAlignment(int channelNum, bool isPartialMode, QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::SPAChannelAlignment)
    , m_isPartialMode(isPartialMode)
{
    ui->setupUi(this); 

    prev = nullptr;
    next = nullptr;
    SPAPartialAlignments = nullptr;

    connect(ui->spaChannelRemoveButton, &QPushButton::clicked, this, [this](){ emit removeChannelRequested(this); });
    connect(ui->spaChannelInsertButton, &QPushButton::clicked, this, [this](){ emit insertChannelRequested(this); });
    SPANumOfPartials = 0;
    SPAInsertPartial(nullptr);
}

SPAChannelAlignment::~SPAChannelAlignment() {
    clearSpaPartials();
    delete ui;
}

SPAPartialAlignment* SPAChannelAlignment::SPAInsertPartial(SPAPartialAlignment* prevSpa){
    if (SPAPartialAlignments != nullptr && !m_isPartialMode) { return nullptr; }

    SPANumOfPartials ++;

    SPAPartialAlignment* newSpa = new SPAPartialAlignment(this);
    newSpa->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(newSpa, &SPAPartialAlignment::textChangedSignal, this, &SPAChannelAlignment::textChanged);
    connect(newSpa, &SPAPartialAlignment::removeRequested, this, &SPAChannelAlignment::SPARemovePartial);
    connect(newSpa, &SPAPartialAlignment::insertRequested, this, &SPAChannelAlignment::SPAInsertPartial);

    if (SPAPartialAlignments == nullptr) {
        SPAPartialAlignments = newSpa;
        ui->spaScrollLayout->insertWidget(0, newSpa);
    } else if (prevSpa != nullptr) {
        newSpa->next = prevSpa->next;
        newSpa->prev = prevSpa;
        if (prevSpa->next) prevSpa->next->prev = newSpa;
        prevSpa->next = newSpa;

        int index = ui->spaScrollLayout->indexOf(prevSpa);
        ui->spaScrollLayout->insertWidget(index+1, newSpa);
    }
    updateSpaLabels();
    emit textChanged();
    return newSpa;
}
void SPAChannelAlignment::SPARemovePartial(SPAPartialAlignment* currSpa) {
    if (!currSpa || (currSpa == SPAPartialAlignments && currSpa->next == nullptr)) return;

    if (currSpa->prev) currSpa->prev->next = currSpa->next;
    if (currSpa->next) currSpa->next->prev = currSpa->prev;
    if (currSpa == SPAPartialAlignments) SPAPartialAlignments = currSpa->next;

    currSpa->deleteLater();
    SPANumOfPartials --;
    updateSpaLabels();
    emit textChanged();
}
void SPAChannelAlignment::updateSpaLabels() {
    SPAPartialAlignment* temp = SPAPartialAlignments;
    int count = 1;
    while (temp != nullptr) {
        if (m_isPartialMode) { temp->setLabelText(QString("Partial %1:").arg(count++));
        } else { temp->setLabelText("Envelope:"); }
        temp = temp->next;
    }
}
void SPAChannelAlignment::clearSpaPartials() {
    SPAPartialAlignment* currSpa = SPAPartialAlignments;
    while (currSpa != nullptr) {
        SPAPartialAlignment* nextSpa = currSpa->next;
        ui->spaScrollLayout->removeWidget(currSpa);
        currSpa->deleteLater();
        currSpa = nextSpa;
    }
    SPAPartialAlignments = nullptr;
    SPANumOfPartials = 0;
}
QString SPAChannelAlignment::getPartialsText() {
    QString partialsText;
    SPAPartialAlignment* temp = SPAPartialAlignments;
    while (temp) {
        partialsText += "<P>" + temp->textChanged() + "</P>";
        temp = temp->next;
    }
    return partialsText;
}
void SPAChannelAlignment::setChannelText(const QString& text) {
    ui->spaChannelEditLabel->setText(text);
}
