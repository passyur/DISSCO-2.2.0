#ifndef IEVENT_HPP
#define IEVENT_HPP

#include <QString>
#include <QSharedPointer>
#include <QList>
#include <QMap>
#include <QStringList>

// Forward declarations
class ProjectViewController;

// Event types
enum class EventType {
    Note,
    Envelope,
    Spectrum,
    Pattern,
    Sieve,
    Spatialization,
    Reverb,
    Filter,
    Measurement,
    ModifierGroup,
    Sound
};

// Modifier types
enum class ModifierType {
    Tremolo,
    Vibrato,
    Glissando,
    Detune,
    Amptrans,
    Freqtrans,
    WaveType
};

// Tempo prefix
enum class TempoPrefix {
    VerySlow,
    Slow,
    Medium,
    Fast,
    VeryFast
};

// Tempo note value
enum class TempoNoteValue {
    Whole,
    Half,
    Quarter,
    Eighth,
    Sixteenth,
    ThirtySecond,
    SixtyFourth
};

// Event modifier class
class EventModifier {
public:
    EventModifier();
    EventModifier(const EventModifier& other);
    ~EventModifier();

    ModifierType getModifierType() const;
    void setModifierType(ModifierType type);

    int getApplyHowFlag() const;
    void setApplyHowFlag(int flag);

    QString getProbability() const;
    void setProbability(const QString& probability);

    QString getAmpValue() const;
    void setAmpValue(const QString& ampValue);

    QString getDetuneSpread() const;
    void setDetuneSpread(const QString& detuneSpread);

    QString getDetuneVelocity() const;
    void setDetuneVelocity(const QString& detuneVelocity);

    QString getDetuneDirection() const;
    void setDetuneDirection(const QString& detuneDirection);

    QString getRateValue() const;
    void setRateValue(const QString& rateValue);

    QString getWidth() const;
    void setWidth(const QString& width);

    QString getGroupName() const;
    void setGroupName(const QString& groupName);

    QString getPartialNum() const;
    void setPartialNum(const QString& partialNum);

    QString getPartialResultString() const;
    void setPartialResultString(const QString& partialResultString);

    QString getXMLString() const;

private:
    ModifierType type;
    int applyHowFlag;
    QString probability;
    QString ampValue;
    QString detuneDirection;
    QString detuneSpread;
    QString detuneVelocity;
    QString rateValue;
    QString width;
    QString groupName;
    QString partialResultString;
    QString partialNum;
};

// Event package class
class EventPackage {
public:
    EventPackage();
    EventPackage(const EventPackage& other);
    ~EventPackage();

    QSharedPointer<class IEvent> getEvent() const;
    void setEvent(const QSharedPointer<class IEvent>& event);

    EventType getEventType() const;
    void setEventType(EventType type);

    QString getEventName() const;
    void setEventName(const QString& name);

    QString getWeight() const;
    void setWeight(const QString& weight);

    QString getAttackEnv() const;
    void setAttackEnv(const QString& attackEnv);

    QString getAttackEnvScale() const;
    void setAttackEnvScale(const QString& attackEnvScale);

    QString getDurationEnv() const;
    void setDurationEnv(const QString& durationEnv);

    QString getDurationEnvScale() const;
    void setDurationEnvScale(const QString& durationEnvScale);

    QString getXMLString() const;
    bool link(ProjectViewController* projectView, class IEvent* thisEvent);

private:
    QSharedPointer<class IEvent> event;
    EventType eventType;
    QString eventName;
    QString weight;
    QString attackEnv;
    QString attackEnvScale;
    QString durationEnv;
    QString durationEnvScale;
};

// Event layer class
class EventLayer {
public:
    EventLayer(class IEvent* thisEvent);
    EventLayer(class IEvent* thisEvent, const EventLayer& other);
    ~EventLayer();

    EventPackage* addChild(const QSharedPointer<class IEvent>& child);
    bool removeChild(EventPackage* child);
    void showContents();
    void deleteLayer();

    double getChildrenWeightSum() const;
    int size() const;
    QString outputChildrenNameString() const;
    QString getXMLString() const;
    void link(ProjectViewController* projectView, class IEvent* thisEvent);

    QList<EventPackage*> children;
    QString getByLayer() const;

private:
    QString byLayer;
    class IEvent* thisIEvent;
};

// Spectrum partial class
class SpectrumPartial {
public:
    SpectrumPartial();
    SpectrumPartial(const SpectrumPartial& other);
    ~SpectrumPartial();

    SpectrumPartial* getPrev() const;
    void setPrev(SpectrumPartial* prev);

    SpectrumPartial* getNext() const;
    void setNext(SpectrumPartial* next);

    QString getEnvString() const;
    void setEnvString(const QString& envString);

    QString getXMLString() const;

private:
    SpectrumPartial* prev;
    SpectrumPartial* next;
    QString envString;
};

// Main IEvent interface
class IEvent {
public:
    IEvent();
    virtual ~IEvent();

    // Basic event properties
    void setName(const QString& name);
    QString getName() const;
    
    void setType(EventType type);
    EventType getType() const;
    QString getTypeString() const;
    QString getFolderName() const;
    
    void setMaxChildDur(const QString& maxChildDur);
    QString getMaxChildDur() const;
    
    void setUnitsPerSecond(const QString& unitsPerSecond);
    QString getUnitsPerSecond() const;
    
    void setChangedButNotSaved(bool value);
    void addParent(IEvent* parent);
    bool removeParent(IEvent* event);
    void deleteLayer(EventLayer* deleteLayer);
    
    // Event hierarchy
    void addChildOrObject(const QSharedPointer<IEvent>& newChild);
    void removeChildOrObject(const QSharedPointer<IEvent>& childToBeRemoved);
    int getNumberOfLayers() const;
    void showAllChildren();
    
    // Event attributes
    QString getSpa() const;
    void setSpa(const QString& spa);
    
    QString getRev() const;
    void setRev(const QString& rev);
    
    QString getFil() const;
    void setFil(const QString& fil);
    
    QString getMea() const;
    void setMea(const QString& mea);
    
    // Time signature
    QString getTimeSignatureEntry1() const;
    void setTimeSignatureEntry1(const QString& entry);
    
    QString getTimeSignatureEntry2() const;
    void setTimeSignatureEntry2(const QString& entry);
    
    // Tempo
    int getTempoMethodFlag() const;
    void setTempoMethodFlag(int flag);
    
    TempoPrefix getTempoPrefix() const;
    void setTempoPrefix(TempoPrefix prefix);
    
    TempoNoteValue getTempoNoteValue() const;
    void setTempoNoteValue(TempoNoteValue noteValue);
    
    QString getTempoFractionEntry1() const;
    void setTempoFractionEntry1(const QString& entry);
    
    QString getTempoFractionEntry2() const;
    void setTempoFractionEntry2(const QString& entry);
    
    QString getTempoValueEntry() const;
    void setTempoValueEntry(const QString& entry);
    
    // Children definition
    void setFlagNumChildren(int flag);
    int getFlagNumChildren() const;
    
    QString getNumChildrenEntry1() const;
    void setNumChildrenEntry1(const QString& entry);
    
    QString getNumChildrenEntry2() const;
    void setNumChildrenEntry2(const QString& entry);
    
    QString getNumChildrenEntry3() const;
    void setNumChildrenEntry3(const QString& entry);
    
    void setFlagChildEventDef(int flag);
    int getFlagChildEventDef() const;
    
    void setFlagChildEventDefStartType(int flag);
    int getFlagChildEventDefStartType() const;
    
    void setFlagChildEventDefDurationType(int flag);
    int getFlagChildEventDefDurationType() const;
    
    QString getChildEventDefEntry1() const;
    void setChildEventDefEntry1(const QString& entry);
    
    QString getChildEventDefEntry2() const;
    void setChildEventDefEntry2(const QString& entry);
    
    QString getChildEventDefEntry3() const;
    void setChildEventDefEntry3(const QString& entry);
    
    QString getChildEventDefAttackSieve() const;
    void setChildEventDefAttackSieve(const QString& sieve);
    
    QString getChildEventDefDurationSieve() const;
    void setChildEventDefDurationSieve(const QString& sieve);
    
    // Modifiers
    void removeModifier(EventModifier* modifier);
    void setEventOrderInPalette(int number);
    int getEventOrderInPalette() const;
    
    // File operations
    void setModifiedButNotSaved();
    void saveToDisk(const QString& pathOfProject);
    void saveAsToDisk(const QString& pathOfProject);
    void saveToDiskHelper(const QString& pathOfProject, bool forced);
    
    // XML generation
    QString getXMLString() const;
    QString getXMLTHMLB() const;
    QString getXMLSound() const;
    QString getXMLEnv() const;
    QString getXMLSiv() const;
    QString getXMLSpa() const;
    QString getXMLPat() const;
    QString getXMLRev() const;
    QString getXMLFil() const;
    QString getXMLNote() const;
    QString getXMLMea() const;
    
    // Virtual methods for specific event types
    virtual void setNumPartials(const QString& numPartials) {}
    virtual QString getNumPartials() const { return ""; }
    virtual void setDeviation(const QString& deviation) {}
    virtual QString getDeviation() const { return ""; }
    virtual QString getSpectrumMetaData() const { return ""; }
    virtual QString getSpectrumXMLString() const { return ""; }
    virtual QString getSoundSpectrumEnvelopesString() const { return ""; }
    virtual QString getSpectrumGenBuilder() const { return ""; }
    virtual void setSpectrumGenBuilder(const QString& builder) {}
    virtual int getPartialWindowMaxNumPartials() const { return 1; }
    virtual bool deletePartial(SpectrumPartial* partial) { return false; }
    
    // Sound attributes
    virtual int getFrequencyFlag() const { return -1; }
    virtual void setFrequencyFlag(int flag) {}
    virtual int getFrequencyContinuumFlag() const { return -1; }
    virtual void setFrequencyContinuumFlag(int flag) {}
    virtual QString getFrequencyEntry1() const { return ""; }
    virtual void setFrequencyEntry1(const QString& entry) {}
    virtual QString getFrequencyEntry2() const { return ""; }
    virtual void setFrequencyEntry2(const QString& entry) {}
    virtual QString getLoudness() const { return ""; }
    virtual void setLoudness(const QString& loudness) {}
    virtual QString getSpatialization() const { return ""; }
    virtual void setSpatialization(const QString& spatialization) {}
    virtual QString getReverb() const { return ""; }
    virtual void setReverb(const QString& reverb) {}
    virtual QString getFilter() const { return ""; }
    virtual void setFilter(const QString& filter) {}
    virtual QString getModifierGroup() const { return ""; }
    virtual void setModifierGroup(const QString& group) {}
    
    // Builders
    virtual QString getEnvelopeBuilder() const { return ""; }
    virtual void setEnvelopeBuilder(const QString& builder) {}
    virtual QString getPatternBuilder() const { return ""; }
    virtual void setPatternBuilder(const QString& builder) {}
    virtual QString getSieveBuilder() const { return ""; }
    virtual void setSieveBuilder(const QString& builder) {}
    virtual QString getReverbBuilder() const { return ""; }
    virtual void setReverbBuilder(const QString& builder) {}
    virtual QString getFilterBuilder() const { return ""; }
    virtual void setFilterBuilder(const QString& builder) {}
    virtual QString getSpatializationBuilder() const { return ""; }
    virtual void setSpatializationBuilder(const QString& builder) {}
    virtual QString getMeasureBuilder() const { return ""; }
    virtual void setMeasureBuilder(const QString& builder) {}
    
    // Child type
    virtual void setChildTypeFlag(int type) {}
    virtual int getChildTypeFlag() const { return -1; }
    
    // Measurement
    virtual QString getMeasurement() const { return ""; }
    virtual void setMeasurement(const QString& measurement) {}
    
    // Note modifiers
    virtual QStringList getNoteModifiers() const { return QStringList(); }
    virtual void clearNoteModifiers() {}
    virtual void addNoteModifiers(const QString& modifier) {}
    virtual void setStaffNum(const QString& staffs) {}
    virtual QString getStaffNum() const { return ""; }
    
    // SuperCollider code generation
    virtual void makeSuperColliderCode() {}
    
    // Linking
    virtual void link(ProjectViewController* projectView) {}
    
    // Helper methods
    virtual bool haveString(const QString& string) const { return false; }
    
protected:
    QString name;
    EventType type;
    QString maxChildDur;
    QString unitsPerSecond;
    bool changedButNotSaved;
    QList<IEvent*> parents;
    QList<EventLayer*> layers;
    QList<EventModifier*> modifiers;
    int eventOrderInPalette;
    
    // Time signature
    QString timeSignatureEntry1;
    QString timeSignatureEntry2;
    
    // Tempo
    int tempoMethodFlag;
    TempoPrefix tempoPrefix;
    TempoNoteValue tempoNoteValue;
    QString tempoFractionEntry1;
    QString tempoFractionEntry2;
    QString tempoValueEntry;
    
    // Children definition
    int flagNumChildren;
    QString numChildrenEntry1;
    QString numChildrenEntry2;
    QString numChildrenEntry3;
    
    int flagChildEventDef;
    int flagChildEventDefStartType;
    int flagChildEventDefDurationType;
    QString childEventDefEntry1;
    QString childEventDefEntry2;
    QString childEventDefEntry3;
    QString childEventDefAttackSieve;
    QString childEventDefDurationSieve;
    
    // Sound attributes
    QString spa;
    QString rev;
    QString fil;
    QString mea;
};

#endif // IEVENT_HPP 