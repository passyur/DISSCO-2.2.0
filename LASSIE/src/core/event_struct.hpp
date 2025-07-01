#ifndef EVENTSTRUCT_HPP
#define EVENTSTRUCT_HPP

/* 
   event_struct.hpp defines the composite data types and enums corresponding to
   the Event attributes in the DISSCO Xml spec. To extend the DISSCO Xml spec as
   reflected in LASSIE: add to the corresponding struct, add to the parser, add
   to the UI files in the corresponding window, and add to the save function. 
*/

#include <QList>
#include <QString>

typedef enum {
    none,
    dotted,
    doubledotted,
    triple,
    quintuple,
    sextuple,
    septuple
} Tempoprefix;

typedef enum {
    whole,
    half,
    quarter,
    eighth,
    sixteenth,
    thirtysecond
} Temponotevalue;

typedef enum {
    top,
    high,
    mid,
    low,
    bottom,
    sound, /* i.e., Spectrum */
    env,
    sieve,
    spa,
    pattern,
    reverb,
    folder,
    note,
    filter,
    mea,
    spec /* NOT ACTUALLY USED */
} Eventtype;

typedef struct TimeSignature TimeSignature;
struct TimeSignature {
    unsigned bar_value;
    unsigned note_value;
};

typedef struct Tempo Tempo;
struct Tempo {
    unsigned method_flag; // true = as fraction, false = as note value
    Tempoprefix prefix;
    Temponotevalue note_value;
    std::string frentry_1;
    std::string frentry_2;
    std::string valentry;
};

typedef enum {
    fixed,
    density,
    bylayer
} Numchildrenflag;

typedef struct NumChildren NumChildren;
struct NumChildren {
    Numchildrenflag method_flag;
    std::string entry_1;
    std::string entry_2;
    std::string entry_3;
};

typedef enum {
    continuumdef,
    sweep,
    discrete
} Childdefnflag;

typedef enum {
    percentage,
    units,
    seconds
} Childdeftimeflag;

typedef struct ChildDef ChildDef;
struct ChildDef {
    std::string entry_1;
    std::string entry_2;
    std::string entry_3;
    std::string attack_sieve;
    std::string duration_sieve;
    Childdefnflag definition_flag;
    Childdeftimeflag starttype_flag;
    Childdeftimeflag durationtype_flag;
};

typedef struct Package Package;
struct Package {
    std::string event_name;
    Eventtype event_type;
    std::string weight;
    std::string attack_envelope;
    std::string attackenvelope_scale;
    std::string duration_envelope;
    std::string durationenvelope_scale;
};

/* oddly, there's a ByLayer tag, which to me, appears to never contain anything
 * -Jacob, 06/28/25 */
typedef struct Layer Layer;
struct Layer {
    std::string by_layer;
    QList<Package> discrete_packages;
};

typedef enum {
    welltempered,
    fundamental,
    continuum
} Freqinfofreqflag;

typedef enum {
    hertz,
    power_of_two
} Freqinfocontflag;

typedef struct FreqInfo FreqInfo;
struct FreqInfo {
    Freqinfofreqflag freq_flag;
    Freqinfocontflag continuum_flag;
    std::string entry_1;
    std::string entry_2;
};

typedef struct Modifier Modifier;
struct Modifier {
    unsigned type;
    bool applyhow_flag;
    std::string probability;
    std::string amplitude;
    std::string rate;
    std::string width;
    std::string detune_spread;
    std::string detune_direction;
    std::string detune_velocity;
    std::string group_name;
    std::string partialresult_string;
};

typedef struct ExtraInfo ExtraInfo;
struct ExtraInfo {
    FreqInfo freq_info;
    std::string loudness;
    std::string spa;
    std::string reverb;
    std::string filter;
    std::string modifier_group;
    QList<Modifier> modifiers;
};

/* HEvents are Top, High, Mid, Low, or Bottom events */
typedef struct HEvent HEvent;
struct HEvent {
    std::string orderinpalette;
    Eventtype type;
    std::string name;
    std::string max_child_duration;
    std::string edu_perbeat;
    TimeSignature timesig;
    Tempo tempo;
    NumChildren numchildren;
    ChildDef child_event_def;
    QList<Layer> event_layers;
    std::string spa;
    std::string reverb;
    std::string filter;
    QList<Modifier> modifiers;
};

typedef struct BottomEvent BottomEvent;
struct BottomEvent {
    HEvent event;
    ExtraInfo extra_info;
};

typedef struct Spectrum Spectrum;
struct Spectrum {
    QList<std::string> partials;
};

typedef struct SpectrumEvent SpectrumEvent;
struct SpectrumEvent {
    /* implicitly, if we did have an Eventtype variable, it would == 5. */
    std::string orderinpalette;
    std::string name;
    std::string num_partials;
    std::string deviation;
    std::string generate_spectrum;
    Spectrum spectrum;
};

typedef struct NoteInfo NoteInfo;
struct NoteInfo {
    std::string staffs;
    QList<std::string> modifiers; /* not to be confused with struct Modifier! */
};

typedef struct NoteEvent NoteEvent;
struct NoteEvent {
    std::string orderinpalette;
    std::string name;
    NoteInfo note_info;
};

typedef struct EnvelopeEvent EnvelopeEvent;
struct EnvelopeEvent {
    /* eventtype == 6 */
    std::string orderinpalette;
    std::string name;
    std::string envelope_builder;
};

typedef struct SieveEvent SieveEvent;
struct SieveEvent {
    std::string orderinpalette;
    std::string name;
    std::string sieve_builder;
};

typedef struct SpaEvent SpaEvent;
struct SpaEvent {
    std::string orderinpalette;
    std::string name;
    std::string spatialization;
};

typedef struct PatternEvent PatternEvent;
struct PatternEvent {
    std::string orderinpalette;
    std::string name;
    std::string pattern_builder;
};

typedef struct ReverbEvent ReverbEvent;
struct ReverbEvent {
    std::string orderinpalette;
    std::string name;
    std::string reverberation;
};

typedef struct FilterEvent FilterEvent;
struct FilterEvent {
    std::string orderinpalette;
    std::string name;
    std::string filter_builder;
};

#endif