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
    ui->spaChannelAlignmentLayout->setContentsMargins(10, 10, 10, 10);
    ui->spaChannelAlignmentLayout->setSpacing(10);

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

SPAPartialAlignment* SPAChannelAlignment::SPAInsertPartial(SPAPartialAlignment* prevSpa, bool silent){
    if (SPAPartialAlignments != nullptr && !m_isPartialMode) { return nullptr; }
    if (!silent && SPAPartialAlignments != nullptr) {
        int idx = getPartialIndex(prevSpa);
        emit partialSyncInsert(idx);
        return nullptr;
    }
    SPANumOfPartials ++;

    SPAPartialAlignment* newSpa = new SPAPartialAlignment(this);
    newSpa->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(newSpa, &SPAPartialAlignment::textChangedSignal, this, &SPAChannelAlignment::textChanged);
    connect(newSpa, &SPAPartialAlignment::removeRequested, this, [this](SPAPartialAlignment* target) { this->SPARemovePartial(target, false); });
    connect(newSpa, &SPAPartialAlignment::insertRequested, this, [this](SPAPartialAlignment* target) { this->SPAInsertPartial(target, false); });

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
void SPAChannelAlignment::SPARemovePartial(SPAPartialAlignment* currSpa, bool silent) {
    if (!currSpa || (currSpa == SPAPartialAlignments && currSpa->next == nullptr)) return;
    if (!silent && SPAPartialAlignments != nullptr) {
        int idx = getPartialIndex(currSpa);
        emit partialSyncRemove(idx);
        return;
    }

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
SPAPartialAlignment* SPAChannelAlignment::getTailPartial() {
    if (SPAPartialAlignments == nullptr) { return nullptr; }

    SPAPartialAlignment* temp = SPAPartialAlignments;
    while (temp->next != nullptr) { temp = temp->next; }
    return temp;
}
int SPAChannelAlignment::getPartialIndex(SPAPartialAlignment* target) {
    int index = 0;
    SPAPartialAlignment* curr = SPAPartialAlignments;
    while (curr) {
        if (curr == target) return index;
        curr = curr->next;
        index++;
    }
    return -1;
}
SPAPartialAlignment* SPAChannelAlignment::getPartialAtIndex(int index) {
    int currIdx = 0;
    SPAPartialAlignment* curr = SPAPartialAlignments;
    while (curr) {
        if (currIdx == index) return curr;
        curr = curr->next;
        currIdx++;
    }
    return nullptr;
}
