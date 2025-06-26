#ifndef EVENTSTRUCT_HPP
#define EVENTSTRUCT_HPP

// #include <QList>
#include <QtCore/qlist.h>

class EventEntry {
    public:
        QList<EventLayer*> layers;
};

class BottomEventExtraInfo {
  public:
    BottomEventExtraInfo();
    BottomEventExtraInfo(BottomEventExtraInfo* _original);
    BottomEventExtraInfo(int _childTypeFlag); //called when parsing files.
    BottomEventExtraInfo(int _childTypeFlag, DOMElement* _thisElement);
    ~BottomEventExtraInfo();
    int getFrequencyFlag(); // 0 = Well_tempered, 1 = Fundamental, 2 = Continuum
    void setFrequencyFlag(int _flag);
    int getFrequencyContinuumFlag();// 0 = hertz, 1 = power of two
    void setFrequencyContinuumFlag(int _flag);

    std::string  getFrequencyEntry1();
    void  setFrequencyEntry1(std::string _string);
    std::string  getFrequencyEntry2();
    void  setFrequencyEntry2(std::string _string);
    std::string  getLoudness();
    void  setLoudness(std::string _string);
    std::string  getSpatialization();
    void  setSpatialization(std::string _string);
    std::string  getReverb();
    void  setReverb(std::string _string);
    std::string  getFilter();
    void  setFilter(std::string _string);
    std::string  getModifierGroup();
    void  setModifierGroup(std::string _string);
    EventBottomModifier* getModifiers();
    EventBottomModifier* addModifier();
    void removeModifier(EventBottomModifier* _modifier);
    void setChildTypeFlag(int _type);
    int getChildTypeFlag();

    EventBottomModifier* modifiers;
    bool haveString(string _string);
    static EventBottomModifier* buildModifiersFromDOMElement(DOMElement* _thisModifierElement);

  private:
    int frequencyFlag; // 0 = Well_tempered, 1 = Fundamental, 2 = Continuum
    int frequencyContinuumFlag; //0 = hertz, 1 =] power of two
    int childTypeFlag; // 0 = sound, 1 = note, 2 = visual
    std::string frequencyEntry1;
    std::string frequencyEntry2;
    std::string loudness;
    std::string spatialization;
    std::string reverb;
    std::string filter;
    std::string modifierGroup; // ZIYUAN CHEN, July 2023
};

// Envelope
class EnvelopeExtraInfo {
    public:
        EnvelopeExtraInfo(EnvelopeExtraInfo* _original){
            envelopeBuilder = _original->envelopeBuilder;}
        EnvelopeExtraInfo(){envelopeBuilder = "";}
        ~EnvelopeExtraInfo(){}
        std::string getEnvelopeBuilder();
        void setEnvelopeBuilder(std::string _string);
        bool haveString(std::string _string);

    private:
        std::string envelopeBuilder;

};

class EventEntry {
    
};

#endif