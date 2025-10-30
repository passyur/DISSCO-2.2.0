#ifndef EVENTSTRUCT_HPP
#define EVENTSTRUCT_HPP

/* 
   event_struct.hpp defines the composite data types and enums corresponding to
   the Event attributes in the DISSCO Xml spec. To extend the DISSCO Xml spec as
   reflected in LASSIE: add to the corresponding struct, add to the parser, add
   to the UI files in the corresponding window, and add to the save function. 
*/

// When initializing defaults for unsigneds (and flags generally) preference should be given to the value that will correspond to the leftmost option (e.g., in a Radio of three elements from L-R, ABC, prefer A).

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
    QString bar_value;
    QString note_value;
};

typedef struct Tempo Tempo;
struct Tempo {
    unsigned method_flag = 0; // true = as fraction, false = as note value
    QString prefix; /* both */
    QString note_value; /* both */
    QString frentry_1; /* as fraction */
    QString frentry_2; /* as fraction */
    QString valentry; /* as note */
};

/*
typedef enum {
    fixed,
    density,
    bylayer
} Numchildrenflag;
*/

typedef struct NumChildren NumChildren;
struct NumChildren {
    unsigned method_flag = 1; /* from Numchildrenflag */
    QString entry_1;
    QString entry_2;
    QString entry_3;
};

/*
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
*/

typedef struct ChildDef ChildDef;
struct ChildDef {
    QString entry_1; /* start time */
    QString entry_2; /* type */
    QString entry_3; /* duration */
    QString attack_sieve;
    QString duration_sieve;
    unsigned definition_flag = 0; /* from Childdefnflag */
    unsigned starttype_flag = 0; /* from Childdeftimeflag */
    unsigned durationtype_flag = 0; /* from Childdeftimeflag */
};

typedef struct Package Package;
struct Package {
    QString event_name;
    QString event_type;
    QString weight;
    QString attack_envelope;
    QString attackenvelope_scale;
    QString duration_envelope;
    QString durationenvelope_scale;
};

/* oddly, there's a ByLayer tag, which to me, appears to never contain anything
 * -Jacob, 06/28/25 */
typedef struct Layer Layer;
struct Layer {
    QString by_layer;
    QList<Package> discrete_packages;
};

/*
typedef enum {
    welltempered,
    fundamental,
    continuum
} Freqinfofreqflag;

typedef enum {
    hertz,
    power_of_two
} Freqinfocontflag;
*/

typedef struct FreqInfo FreqInfo;
struct FreqInfo {
    unsigned freq_flag = 0; /* from Freqinfofreqflag */
    unsigned continuum_flag = 0; /* from Freqinfocontflag */
    QString entry_1;
    QString entry_2;
};

typedef struct Modifier Modifier;
struct Modifier {
    unsigned type = 0;
    bool applyhow_flag = false; // false == SOUND, true == PARTIAL
    QString probability;
    QString amplitude;
    QString rate;
    QString width;
    QString detune_spread;
    QString detune_direction;
    QString detune_velocity;
    QString group_name;
    QString partialresult_string;
};

typedef struct ExtraInfo ExtraInfo;
struct ExtraInfo {
    unsigned childtype_flag = 0;
    FreqInfo freq_info;
    QString loudness;
    QString spa;
    QString reverb;
    QString filter;
    QString modifier_group;
    QList<Modifier> modifiers;
};

/* HEvents are Top, High, Mid, or Low events */
typedef struct HEvent HEvent;
struct HEvent {
    QString orderinpalette;
    Eventtype type = Eventtype::top;
    QString name;
    QString max_child_duration;
    QString edu_perbeat;
    TimeSignature timesig;
    Tempo tempo;
    NumChildren numchildren;
    ChildDef child_event_def;
    QList<Layer> event_layers;
    QString spa;
    QString reverb;
    QString filter;
    QList<Modifier> modifiers;
};

typedef struct BottomEvent BottomEvent;
struct BottomEvent {
    HEvent event;
    ExtraInfo extra_info;
};

typedef struct Spectrum Spectrum;
struct Spectrum {
    QList<QString> partials;
};

typedef struct SpectrumEvent SpectrumEvent;
struct SpectrumEvent {
    /* implicitly, if we did have an Eventtype variable, it would == 5. */
    QString orderinpalette;
    QString name;
    QString num_partials;
    QString deviation;
    QString generate_spectrum;
    Spectrum spectrum;
};

typedef struct NoteInfo NoteInfo;
struct NoteInfo {
    QString staffs;
    QList<QString> modifiers; /* not to be confused with struct Modifier! */
};

typedef struct NoteEvent NoteEvent;
struct NoteEvent {
    QString orderinpalette;
    QString name;
    NoteInfo note_info;
};

typedef struct EnvelopeEvent EnvelopeEvent;
struct EnvelopeEvent {
    /* eventtype == 6 */
    QString orderinpalette;
    QString name;
    QString envelope_builder;
};

typedef struct SieveEvent SieveEvent;
struct SieveEvent {
    QString orderinpalette;
    QString name;
    QString sieve_builder;
};

typedef struct SpaEvent SpaEvent;
struct SpaEvent {
    QString orderinpalette;
    QString name;
    QString spatialization;
};

typedef struct PatternEvent PatternEvent;
struct PatternEvent {
    QString orderinpalette;
    QString name;
    QString pattern_builder;
};

typedef struct ReverbEvent ReverbEvent;
struct ReverbEvent {
    QString orderinpalette;
    QString name;
    QString reverberation;
};

typedef struct FilterEvent FilterEvent;
struct FilterEvent {
    QString orderinpalette;
    QString name;
    QString filter_builder;
};

#endif