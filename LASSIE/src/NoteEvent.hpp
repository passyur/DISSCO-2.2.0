#ifndef NOTEEVENT_HPP
#define NOTEEVENT_HPP

#include "IEvent.hpp"
#include <QString>
#include <QStringList>

class NoteEvent : public IEvent {
public:
    NoteEvent();
    NoteEvent(const NoteEvent& other);
    virtual ~NoteEvent();

    // Note-specific properties
    void setPitch(const QString& pitch);
    QString getPitch() const;

    void setVelocity(const QString& velocity);
    QString getVelocity() const;

    void setDuration(const QString& duration);
    QString getDuration() const;

    void setChannel(const QString& channel);
    QString getChannel() const;

    void setStaffNum(const QString& staffs) override;
    QString getStaffNum() const override;

    // Note modifiers
    QStringList getNoteModifiers() const override;
    void clearNoteModifiers() override;
    void addNoteModifiers(const QString& modifier) override;

    // XML generation
    QString getXMLNote() const override;

    // SuperCollider code generation
    void makeSuperColliderCode() override;

private:
    QString pitch;
    QString velocity;
    QString duration;
    QString channel;
    QString staffNum;
    QStringList noteModifiers;
};

#endif // NOTEEVENT_HPP 