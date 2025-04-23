#include "NoteEvent.hpp"
#include <QDebug>

NoteEvent::NoteEvent() : IEvent() {
    setType(EventType::Note);
}

NoteEvent::NoteEvent(const NoteEvent& other) : IEvent(other) {
    pitch = other.pitch;
    velocity = other.velocity;
    duration = other.duration;
    channel = other.channel;
    staffNum = other.staffNum;
    noteModifiers = other.noteModifiers;
}

NoteEvent::~NoteEvent() {}

// Note-specific properties
void NoteEvent::setPitch(const QString& p) {
    pitch = p;
    setModifiedButNotSaved();
}

QString NoteEvent::getPitch() const {
    return pitch;
}

void NoteEvent::setVelocity(const QString& v) {
    velocity = v;
    setModifiedButNotSaved();
}

QString NoteEvent::getVelocity() const {
    return velocity;
}

void NoteEvent::setDuration(const QString& d) {
    duration = d;
    setModifiedButNotSaved();
}

QString NoteEvent::getDuration() const {
    return duration;
}

void NoteEvent::setChannel(const QString& c) {
    channel = c;
    setModifiedButNotSaved();
}

QString NoteEvent::getChannel() const {
    return channel;
}

void NoteEvent::setStaffNum(const QString& staffs) {
    staffNum = staffs;
    setModifiedButNotSaved();
}

QString NoteEvent::getStaffNum() const {
    return staffNum;
}

// Note modifiers
QStringList NoteEvent::getNoteModifiers() const {
    return noteModifiers;
}

void NoteEvent::clearNoteModifiers() {
    noteModifiers.clear();
    setModifiedButNotSaved();
}

void NoteEvent::addNoteModifiers(const QString& modifier) {
    noteModifiers.append(modifier);
    setModifiedButNotSaved();
}

// XML generation
QString NoteEvent::getXMLNote() const {
    QString xml;
    xml += "<note>\n";
    xml += QString("  <pitch>%1</pitch>\n").arg(pitch);
    xml += QString("  <velocity>%1</velocity>\n").arg(velocity);
    xml += QString("  <duration>%1</duration>\n").arg(duration);
    xml += QString("  <channel>%1</channel>\n").arg(channel);
    xml += QString("  <staffNum>%1</staffNum>\n").arg(staffNum);
    
    // Note modifiers
    if (!noteModifiers.isEmpty()) {
        xml += "  <modifiers>\n";
        for (const QString& modifier : noteModifiers) {
            xml += QString("    <modifier>%1</modifier>\n").arg(modifier);
        }
        xml += "  </modifiers>\n";
    }
    
    xml += "</note>\n";
    return xml;
}

// SuperCollider code generation
void NoteEvent::makeSuperColliderCode() {
    // Implementation depends on SuperCollider requirements
    // This is a placeholder for the actual implementation
    qDebug() << "Generating SuperCollider code for note:" << getName();
} 