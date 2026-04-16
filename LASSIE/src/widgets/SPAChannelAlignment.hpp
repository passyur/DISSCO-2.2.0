#ifndef SPACHANNELALIGNMENT_HPP
#define SPACHANNELALIGNMENT_HPP

#include "../dialogs/FunctionGenerator.hpp"
#include "SPAPartialAlignment.hpp"

#include <QDialog>
#include <QWidget>
#include <QString>
#include <QFrame>

class FunctionGenerator;

namespace Ui {
class SPAChannelAlignment;
}

class SPAChannelAlignment : public QFrame
{
    Q_OBJECT

public:
    /*Constructor to create the Function Generator Pop Up*/
    explicit SPAChannelAlignment(int channelNum, bool isPartialMode, QWidget *parent = nullptr);
    /*Destructor to delete the UI*/
    ~SPAChannelAlignment();
    
    Ui::SPAChannelAlignment *ui;

    SPAChannelAlignment* next;
    SPAChannelAlignment* prev;
    SPAPartialAlignment* SPAPartialAlignments = nullptr;

    int SPANumOfPartials;

    SPAPartialAlignment* SPAInsertPartial(SPAPartialAlignment* prevSpa);
    void SPARemovePartial(SPAPartialAlignment* currSpa);
    void updateSpaLabels();
    void clearSpaPartials();
    
    QString getPartialsText();
    void setChannelText(const QString& text);

signals:
    void removeChannelRequested(SPAChannelAlignment* node);
    void insertChannelRequested(SPAChannelAlignment* node);
    void textChanged();

private:
    /* storing function generator */
    void setupUi();
    bool m_isPartialMode;
};

#endif // SPACHANNELALIGNMENT_HPP