#include "IEvent.hpp"
#include "ProjectViewController.hpp"
#include <QDebug>

// EventModifier implementation
EventModifier::EventModifier() : type(ModifierType::Tremolo), applyHowFlag(0) {}

EventModifier::EventModifier(const EventModifier& other) : 
    type(other.type),
    applyHowFlag(other.applyHowFlag),
    probability(other.probability),
    ampValue(other.ampValue),
    detuneDirection(other.detuneDirection),
    detuneSpread(other.detuneSpread),
    detuneVelocity(other.detuneVelocity),
    rateValue(other.rateValue),
    width(other.width),
    groupName(other.groupName),
    partialResultString(other.partialResultString),
    partialNum(other.partialNum) {}

EventModifier::~EventModifier() {}

ModifierType EventModifier::getModifierType() const { return type; }
void EventModifier::setModifierType(ModifierType t) { type = t; }

int EventModifier::getApplyHowFlag() const { return applyHowFlag; }
void EventModifier::setApplyHowFlag(int flag) { applyHowFlag = flag; }

QString EventModifier::getProbability() const { return probability; }
void EventModifier::setProbability(const QString& prob) { probability = prob; }

QString EventModifier::getAmpValue() const { return ampValue; }
void EventModifier::setAmpValue(const QString& amp) { ampValue = amp; }

QString EventModifier::getDetuneSpread() const { return detuneSpread; }
void EventModifier::setDetuneSpread(const QString& spread) { detuneSpread = spread; }

QString EventModifier::getDetuneVelocity() const { return detuneVelocity; }
void EventModifier::setDetuneVelocity(const QString& velocity) { detuneVelocity = velocity; }

QString EventModifier::getDetuneDirection() const { return detuneDirection; }
void EventModifier::setDetuneDirection(const QString& direction) { detuneDirection = direction; }

QString EventModifier::getRateValue() const { return rateValue; }
void EventModifier::setRateValue(const QString& rate) { rateValue = rate; }

QString EventModifier::getWidth() const { return width; }
void EventModifier::setWidth(const QString& w) { width = w; }

QString EventModifier::getGroupName() const { return groupName; }
void EventModifier::setGroupName(const QString& name) { groupName = name; }

QString EventModifier::getPartialNum() const { return partialNum; }
void EventModifier::setPartialNum(const QString& num) { partialNum = num; }

QString EventModifier::getPartialResultString() const { return partialResultString; }
void EventModifier::setPartialResultString(const QString& result) { partialResultString = result; }

QString EventModifier::getXMLString() const {
    QString xml;
    xml += "<modifier>\n";
    xml += QString("  <type>%1</type>\n").arg(static_cast<int>(type));
    xml += QString("  <applyHowFlag>%1</applyHowFlag>\n").arg(applyHowFlag);
    xml += QString("  <probability>%1</probability>\n").arg(probability);
    xml += QString("  <ampValue>%1</ampValue>\n").arg(ampValue);
    xml += QString("  <detuneDirection>%1</detuneDirection>\n").arg(detuneDirection);
    xml += QString("  <detuneSpread>%1</detuneSpread>\n").arg(detuneSpread);
    xml += QString("  <detuneVelocity>%1</detuneVelocity>\n").arg(detuneVelocity);
    xml += QString("  <rateValue>%1</rateValue>\n").arg(rateValue);
    xml += QString("  <width>%1</width>\n").arg(width);
    xml += QString("  <groupName>%1</groupName>\n").arg(groupName);
    xml += QString("  <partialResultString>%1</partialResultString>\n").arg(partialResultString);
    xml += QString("  <partialNum>%1</partialNum>\n").arg(partialNum);
    xml += "</modifier>\n";
    return xml;
}

// EventPackage implementation
EventPackage::EventPackage() : eventType(EventType::Note) {}

EventPackage::EventPackage(const EventPackage& other) :
    event(other.event),
    eventType(other.eventType),
    eventName(other.eventName),
    weight(other.weight),
    attackEnv(other.attackEnv),
    attackEnvScale(other.attackEnvScale),
    durationEnv(other.durationEnv),
    durationEnvScale(other.durationEnvScale) {}

EventPackage::~EventPackage() {}

QSharedPointer<IEvent> EventPackage::getEvent() const { return event; }
void EventPackage::setEvent(const QSharedPointer<IEvent>& e) { event = e; }

EventType EventPackage::getEventType() const { return eventType; }
void EventPackage::setEventType(EventType type) { eventType = type; }

QString EventPackage::getEventName() const { return eventName; }
void EventPackage::setEventName(const QString& name) { eventName = name; }

QString EventPackage::getWeight() const { return weight; }
void EventPackage::setWeight(const QString& w) { weight = w; }

QString EventPackage::getAttackEnv() const { return attackEnv; }
void EventPackage::setAttackEnv(const QString& env) { attackEnv = env; }

QString EventPackage::getAttackEnvScale() const { return attackEnvScale; }
void EventPackage::setAttackEnvScale(const QString& scale) { attackEnvScale = scale; }

QString EventPackage::getDurationEnv() const { return durationEnv; }
void EventPackage::setDurationEnv(const QString& env) { durationEnv = env; }

QString EventPackage::getDurationEnvScale() const { return durationEnvScale; }
void EventPackage::setDurationEnvScale(const QString& scale) { durationEnvScale = scale; }

QString EventPackage::getXMLString() const {
    QString xml;
    xml += "<eventPackage>\n";
    xml += QString("  <eventType>%1</eventType>\n").arg(static_cast<int>(eventType));
    xml += QString("  <eventName>%1</eventName>\n").arg(eventName);
    xml += QString("  <weight>%1</weight>\n").arg(weight);
    xml += QString("  <attackEnv>%1</attackEnv>\n").arg(attackEnv);
    xml += QString("  <attackEnvScale>%1</attackEnvScale>\n").arg(attackEnvScale);
    xml += QString("  <durationEnv>%1</durationEnv>\n").arg(durationEnv);
    xml += QString("  <durationEnvScale>%1</durationEnvScale>\n").arg(durationEnvScale);
    if (event) {
        xml += "  <event>\n";
        xml += event->getXMLString();
        xml += "  </event>\n";
    }
    xml += "</eventPackage>\n";
    return xml;
}

bool EventPackage::link(ProjectViewController* projectView, IEvent* thisEvent) {
    if (!event || !projectView) return false;
    return event->link(projectView);
}

// EventLayer implementation
EventLayer::EventLayer(IEvent* thisEvent) : thisIEvent(thisEvent) {}

EventLayer::EventLayer(IEvent* thisEvent, const EventLayer& other) :
    thisIEvent(thisEvent),
    byLayer(other.byLayer) {
    for (EventPackage* child : other.children) {
        EventPackage* newChild = new EventPackage(*child);
        children.append(newChild);
    }
}

EventLayer::~EventLayer() {
    qDeleteAll(children);
}

EventPackage* EventLayer::addChild(const QSharedPointer<IEvent>& child) {
    EventPackage* package = new EventPackage();
    package->setEvent(child);
    package->setEventType(child->getType());
    package->setEventName(child->getName());
    children.append(package);
    return package;
}

bool EventLayer::removeChild(EventPackage* child) {
    int index = children.indexOf(child);
    if (index == -1) return false;
    children.removeAt(index);
    delete child;
    return true;
}

void EventLayer::showContents() {
    // Implementation depends on UI framework
}

void EventLayer::deleteLayer() {
    qDeleteAll(children);
    children.clear();
}

double EventLayer::getChildrenWeightSum() const {
    double sum = 0.0;
    for (EventPackage* child : children) {
        bool ok;
        double weight = child->getWeight().toDouble(&ok);
        if (ok) sum += weight;
    }
    return sum;
}

int EventLayer::size() const {
    return children.size();
}

QString EventLayer::outputChildrenNameString() const {
    QString result;
    for (EventPackage* child : children) {
        if (!result.isEmpty()) result += ", ";
        result += child->getEventName();
    }
    return result;
}

QString EventLayer::getXMLString() const {
    QString xml;
    xml += "<layer>\n";
    xml += QString("  <byLayer>%1</byLayer>\n").arg(byLayer);
    for (EventPackage* child : children) {
        xml += child->getXMLString();
    }
    xml += "</layer>\n";
    return xml;
}

void EventLayer::link(ProjectViewController* projectView, IEvent* thisEvent) {
    for (EventPackage* child : children) {
        child->link(projectView, thisEvent);
    }
}

QString EventLayer::getByLayer() const {
    return byLayer;
}

// SpectrumPartial implementation
SpectrumPartial::SpectrumPartial() : prev(nullptr), next(nullptr) {}

SpectrumPartial::SpectrumPartial(const SpectrumPartial& other) :
    prev(nullptr),
    next(nullptr),
    envString(other.envString) {}

SpectrumPartial::~SpectrumPartial() {}

SpectrumPartial* SpectrumPartial::getPrev() const { return prev; }
void SpectrumPartial::setPrev(SpectrumPartial* p) { prev = p; }

SpectrumPartial* SpectrumPartial::getNext() const { return next; }
void SpectrumPartial::setNext(SpectrumPartial* n) { next = n; }

QString SpectrumPartial::getEnvString() const { return envString; }
void SpectrumPartial::setEnvString(const QString& env) { envString = env; }

QString SpectrumPartial::getXMLString() const {
    QString xml;
    xml += "<partial>\n";
    xml += QString("  <envString>%1</envString>\n").arg(envString);
    xml += "</partial>\n";
    return xml;
}

// IEvent implementation
IEvent::IEvent() :
    type(EventType::Note),
    changedButNotSaved(false),
    eventOrderInPalette(0),
    tempoMethodFlag(0),
    tempoPrefix(TempoPrefix::Medium),
    tempoNoteValue(TempoNoteValue::Quarter),
    flagNumChildren(0),
    flagChildEventDef(0),
    flagChildEventDefStartType(0),
    flagChildEventDefDurationType(0) {}

IEvent::~IEvent() {
    qDeleteAll(layers);
    qDeleteAll(modifiers);
}

// Basic event properties
void IEvent::setName(const QString& n) { name = n; }
QString IEvent::getName() const { return name; }

void IEvent::setType(EventType t) { type = t; }
EventType IEvent::getType() const { return type; }

QString IEvent::getTypeString() const {
    switch (type) {
        case EventType::Note: return "Note";
        case EventType::Envelope: return "Envelope";
        case EventType::Spectrum: return "Spectrum";
        case EventType::Pattern: return "Pattern";
        case EventType::Sieve: return "Sieve";
        case EventType::Spatialization: return "Spatialization";
        case EventType::Reverb: return "Reverb";
        case EventType::Filter: return "Filter";
        case EventType::Measurement: return "Measurement";
        case EventType::ModifierGroup: return "ModifierGroup";
        case EventType::Sound: return "Sound";
        default: return "Unknown";
    }
}

QString IEvent::getFolderName() const {
    return getTypeString().toLower();
}

void IEvent::setMaxChildDur(const QString& dur) { maxChildDur = dur; }
QString IEvent::getMaxChildDur() const { return maxChildDur; }

void IEvent::setUnitsPerSecond(const QString& units) { unitsPerSecond = units; }
QString IEvent::getUnitsPerSecond() const { return unitsPerSecond; }

void IEvent::setChangedButNotSaved(bool value) { changedButNotSaved = value; }

void IEvent::addParent(IEvent* parent) {
    if (!parents.contains(parent)) {
        parents.append(parent);
    }
}

bool IEvent::removeParent(IEvent* event) {
    return parents.removeOne(event);
}

void IEvent::deleteLayer(EventLayer* deleteLayer) {
    int index = layers.indexOf(deleteLayer);
    if (index != -1) {
        layers.removeAt(index);
        delete deleteLayer;
    }
}

// Event hierarchy
void IEvent::addChildOrObject(const QSharedPointer<IEvent>& newChild) {
    if (layers.isEmpty()) {
        layers.append(new EventLayer(this));
    }
    layers.first()->addChild(newChild);
    newChild->addParent(this);
}

void IEvent::removeChildOrObject(const QSharedPointer<IEvent>& childToBeRemoved) {
    for (EventLayer* layer : layers) {
        for (EventPackage* package : layer->children) {
            if (package->getEvent() == childToBeRemoved) {
                layer->removeChild(package);
                childToBeRemoved->removeParent(this);
                return;
            }
        }
    }
}

int IEvent::getNumberOfLayers() const {
    return layers.size();
}

void IEvent::showAllChildren() {
    for (EventLayer* layer : layers) {
        layer->showContents();
    }
}

// Event attributes
QString IEvent::getSpa() const { return spa; }
void IEvent::setSpa(const QString& s) { spa = s; }

QString IEvent::getRev() const { return rev; }
void IEvent::setRev(const QString& r) { rev = r; }

QString IEvent::getFil() const { return fil; }
void IEvent::setFil(const QString& f) { fil = f; }

QString IEvent::getMea() const { return mea; }
void IEvent::setMea(const QString& m) { mea = m; }

// Time signature
QString IEvent::getTimeSignatureEntry1() const { return timeSignatureEntry1; }
void IEvent::setTimeSignatureEntry1(const QString& entry) { timeSignatureEntry1 = entry; }

QString IEvent::getTimeSignatureEntry2() const { return timeSignatureEntry2; }
void IEvent::setTimeSignatureEntry2(const QString& entry) { timeSignatureEntry2 = entry; }

// Tempo
int IEvent::getTempoMethodFlag() const { return tempoMethodFlag; }
void IEvent::setTempoMethodFlag(int flag) { tempoMethodFlag = flag; }

TempoPrefix IEvent::getTempoPrefix() const { return tempoPrefix; }
void IEvent::setTempoPrefix(TempoPrefix prefix) { tempoPrefix = prefix; }

TempoNoteValue IEvent::getTempoNoteValue() const { return tempoNoteValue; }
void IEvent::setTempoNoteValue(TempoNoteValue noteValue) { tempoNoteValue = noteValue; }

QString IEvent::getTempoFractionEntry1() const { return tempoFractionEntry1; }
void IEvent::setTempoFractionEntry1(const QString& entry) { tempoFractionEntry1 = entry; }

QString IEvent::getTempoFractionEntry2() const { return tempoFractionEntry2; }
void IEvent::setTempoFractionEntry2(const QString& entry) { tempoFractionEntry2 = entry; }

QString IEvent::getTempoValueEntry() const { return tempoValueEntry; }
void IEvent::setTempoValueEntry(const QString& entry) { tempoValueEntry = entry; }

// Children definition
void IEvent::setFlagNumChildren(int flag) { flagNumChildren = flag; }
int IEvent::getFlagNumChildren() const { return flagNumChildren; }

QString IEvent::getNumChildrenEntry1() const { return numChildrenEntry1; }
void IEvent::setNumChildrenEntry1(const QString& entry) { numChildrenEntry1 = entry; }

QString IEvent::getNumChildrenEntry2() const { return numChildrenEntry2; }
void IEvent::setNumChildrenEntry2(const QString& entry) { numChildrenEntry2 = entry; }

QString IEvent::getNumChildrenEntry3() const { return numChildrenEntry3; }
void IEvent::setNumChildrenEntry3(const QString& entry) { numChildrenEntry3 = entry; }

void IEvent::setFlagChildEventDef(int flag) { flagChildEventDef = flag; }
int IEvent::getFlagChildEventDef() const { return flagChildEventDef; }

void IEvent::setFlagChildEventDefStartType(int flag) { flagChildEventDefStartType = flag; }
int IEvent::getFlagChildEventDefStartType() const { return flagChildEventDefStartType; }

void IEvent::setFlagChildEventDefDurationType(int flag) { flagChildEventDefDurationType = flag; }
int IEvent::getFlagChildEventDefDurationType() const { return flagChildEventDefDurationType; }

QString IEvent::getChildEventDefEntry1() const { return childEventDefEntry1; }
void IEvent::setChildEventDefEntry1(const QString& entry) { childEventDefEntry1 = entry; }

QString IEvent::getChildEventDefEntry2() const { return childEventDefEntry2; }
void IEvent::setChildEventDefEntry2(const QString& entry) { childEventDefEntry2 = entry; }

QString IEvent::getChildEventDefEntry3() const { return childEventDefEntry3; }
void IEvent::setChildEventDefEntry3(const QString& entry) { childEventDefEntry3 = entry; }

QString IEvent::getChildEventDefAttackSieve() const { return childEventDefAttackSieve; }
void IEvent::setChildEventDefAttackSieve(const QString& sieve) { childEventDefAttackSieve = sieve; }

QString IEvent::getChildEventDefDurationSieve() const { return childEventDefDurationSieve; }
void IEvent::setChildEventDefDurationSieve(const QString& sieve) { childEventDefDurationSieve = sieve; }

// Modifiers
void IEvent::removeModifier(EventModifier* modifier) {
    modifiers.removeOne(modifier);
    delete modifier;
}

void IEvent::setEventOrderInPalette(int number) { eventOrderInPalette = number; }
int IEvent::getEventOrderInPalette() const { return eventOrderInPalette; }

// File operations
void IEvent::setModifiedButNotSaved() { changedButNotSaved = true; }

void IEvent::saveToDisk(const QString& pathOfProject) {
    saveToDiskHelper(pathOfProject, false);
}

void IEvent::saveAsToDisk(const QString& pathOfProject) {
    saveToDiskHelper(pathOfProject, true);
}

void IEvent::saveToDiskHelper(const QString& pathOfProject, bool forced) {
    // Implementation depends on file system and project structure
    if (forced || changedButNotSaved) {
        // Save event to disk
        changedButNotSaved = false;
    }
}

// XML generation
QString IEvent::getXMLString() const {
    QString xml;
    xml += "<event>\n";
    xml += QString("  <name>%1</name>\n").arg(name);
    xml += QString("  <type>%1</type>\n").arg(static_cast<int>(type));
    xml += QString("  <maxChildDur>%1</maxChildDur>\n").arg(maxChildDur);
    xml += QString("  <unitsPerSecond>%1</unitsPerSecond>\n").arg(unitsPerSecond);
    
    // Time signature
    xml += "  <timeSignature>\n";
    xml += QString("    <entry1>%1</entry1>\n").arg(timeSignatureEntry1);
    xml += QString("    <entry2>%1</entry2>\n").arg(timeSignatureEntry2);
    xml += "  </timeSignature>\n";
    
    // Tempo
    xml += "  <tempo>\n";
    xml += QString("    <methodFlag>%1</methodFlag>\n").arg(tempoMethodFlag);
    xml += QString("    <prefix>%1</prefix>\n").arg(static_cast<int>(tempoPrefix));
    xml += QString("    <noteValue>%1</noteValue>\n").arg(static_cast<int>(tempoNoteValue));
    xml += QString("    <fractionEntry1>%1</fractionEntry1>\n").arg(tempoFractionEntry1);
    xml += QString("    <fractionEntry2>%1</fractionEntry2>\n").arg(tempoFractionEntry2);
    xml += QString("    <valueEntry>%1</valueEntry>\n").arg(tempoValueEntry);
    xml += "  </tempo>\n";
    
    // Children definition
    xml += "  <childrenDefinition>\n";
    xml += QString("    <flagNumChildren>%1</flagNumChildren>\n").arg(flagNumChildren);
    xml += QString("    <numChildrenEntry1>%1</numChildrenEntry1>\n").arg(numChildrenEntry1);
    xml += QString("    <numChildrenEntry2>%1</numChildrenEntry2>\n").arg(numChildrenEntry2);
    xml += QString("    <numChildrenEntry3>%1</numChildrenEntry3>\n").arg(numChildrenEntry3);
    xml += QString("    <flagChildEventDef>%1</flagChildEventDef>\n").arg(flagChildEventDef);
    xml += QString("    <flagChildEventDefStartType>%1</flagChildEventDefStartType>\n").arg(flagChildEventDefStartType);
    xml += QString("    <flagChildEventDefDurationType>%1</flagChildEventDefDurationType>\n").arg(flagChildEventDefDurationType);
    xml += QString("    <childEventDefEntry1>%1</childEventDefEntry1>\n").arg(childEventDefEntry1);
    xml += QString("    <childEventDefEntry2>%1</childEventDefEntry2>\n").arg(childEventDefEntry2);
    xml += QString("    <childEventDefEntry3>%1</childEventDefEntry3>\n").arg(childEventDefEntry3);
    xml += QString("    <childEventDefAttackSieve>%1</childEventDefAttackSieve>\n").arg(childEventDefAttackSieve);
    xml += QString("    <childEventDefDurationSieve>%1</childEventDefDurationSieve>\n").arg(childEventDefDurationSieve);
    xml += "  </childrenDefinition>\n";
    
    // Sound attributes
    xml += "  <soundAttributes>\n";
    xml += QString("    <spa>%1</spa>\n").arg(spa);
    xml += QString("    <rev>%1</rev>\n").arg(rev);
    xml += QString("    <fil>%1</fil>\n").arg(fil);
    xml += QString("    <mea>%1</mea>\n").arg(mea);
    xml += "  </soundAttributes>\n";
    
    // Layers
    for (EventLayer* layer : layers) {
        xml += layer->getXMLString();
    }
    
    // Modifiers
    for (EventModifier* modifier : modifiers) {
        xml += modifier->getXMLString();
    }
    
    xml += "</event>\n";
    return xml;
}

QString IEvent::getXMLTHMLB() const {
    // Implementation depends on specific requirements
    return "";
}

QString IEvent::getXMLSound() const {
    // Implementation depends on specific requirements
    return "";
}

QString IEvent::getXMLEnv() const {
    // Implementation depends on specific requirements
    return "";
}

QString IEvent::getXMLSiv() const {
    // Implementation depends on specific requirements
    return "";
}

QString IEvent::getXMLSpa() const {
    // Implementation depends on specific requirements
    return "";
}

QString IEvent::getXMLPat() const {
    // Implementation depends on specific requirements
    return "";
}

QString IEvent::getXMLRev() const {
    // Implementation depends on specific requirements
    return "";
}

QString IEvent::getXMLFil() const {
    // Implementation depends on specific requirements
    return "";
}

QString IEvent::getXMLNote() const {
    // Implementation depends on specific requirements
    return "";
}

QString IEvent::getXMLMea() const {
    // Implementation depends on specific requirements
    return "";
} 