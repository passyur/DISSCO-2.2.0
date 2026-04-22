#ifndef REVCHANNEL_HPP
#define REVCHANNEL_HPP

#include <QFrame>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "generic/FunctionEntryRow.hpp"

class REVChannel : public QFrame {
    Q_OBJECT
public:
    REVChannel(int id, int revMode, QWidget* parent = nullptr);

    QString getXmlRoom();
    QString getXmlReverb();
    QString getXmlHillow();
    QString getXmlAllGain();
    QString getXmlDelay();
    QString getXmlCombGain();
    QString getXmlLPGain();
    void hideButtons();
    void setTitle(const QString& text);

    void setRoomSizeText(const QString& text);
    void setReverbText(const QString& text);
    void setHillowText(const QString& text);
    void setAllGainText(const QString& text);
    void setDelayText(const QString& text);
    void setCombGainText(const QString& text);
    void setLPGainText(const QString& text);

signals:
    void textChanged();
    void insertChannelRequested(REVChannel* self);
    void removeChannelRequested(REVChannel* self);

private:
    int m_revMode;
    QLabel* m_titleLabel;
    QPushButton *m_insBtn, *m_rmBtn;
    QWidget* m_rowsContainer;
    QVBoxLayout* m_rowsLayout;
    
    FunctionEntryRow* room_size = nullptr;
    FunctionEntryRow* reverb = nullptr;
    FunctionEntryRow* hillow = nullptr;
    FunctionEntryRow* all_gain = nullptr;
    FunctionEntryRow* delay = nullptr;
    FunctionEntryRow* comb_gain = nullptr;
    FunctionEntryRow* lp_gain = nullptr;
};

#endif