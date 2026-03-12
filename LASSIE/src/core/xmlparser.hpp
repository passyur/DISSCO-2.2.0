#ifndef XMLPARSER_HPP
#define XMLPARSER_HPP

#include "project_struct.hpp"
#include "EnvelopeLibraryEntry.hpp"

#include "../../LASS/src/LASS.h"
#include "MarkovModel.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QProcess>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>

#include <string>
#include <sstream>

namespace XercesParser {
    using namespace xercesc;
    inline QString getFunctionString(const DOMElement* _thisFunctionElement){
        if (const DOMNode* child = _thisFunctionElement->getFirstChild(); child == nullptr) //not containing any child, return string
            return "";

        constexpr XMLCh tempStr[3] = {chLatin_L, chLatin_S, chNull};
        DOMImplementation *impl          = DOMImplementationRegistry::getDOMImplementation(tempStr);
        DOMLSSerializer   *theSerializer = impl->createLSSerializer();
        XMLCh* bla = theSerializer->writeToString(_thisFunctionElement);
        char* toTranscode = XMLString::transcode(bla);
        std::string returnString = std::string(toTranscode);
        XMLString::release(&toTranscode);
        XMLString::release(&bla);

        const int tagLength = static_cast<int>(XMLString::stringLen(_thisFunctionElement->getTagName()));

        delete theSerializer;
        returnString = returnString.substr(tagLength+2, returnString.size() - tagLength * 2 - 5);
        return QString::fromStdString(returnString);
    }

    /// @brief For use when transcoding a basic `std::string` from a `DOMElement*` and assigning it to a `QString` variable.
    /// @param element the DOMElement to be transcoded from
    /// @param lhs the QString to be assigned to
    /// @note Performs the conversion from `std::string` to `QString` inside the function;
    /// @note If the `DOMCharacterData*` from `element`'s first child is `null`, sets `lhs` to the empty string
    inline void transcodeToQString(const DOMElement *element, QString& lhs){
        if(const auto textdata = static_cast<DOMCharacterData *>(element->getFirstChild()); textdata != nullptr){
            char *buffer = XMLString::transcode(textdata->getData());;
            lhs = QString::fromStdString(buffer);
            XMLString::release(&buffer);
        }else{
            lhs = "";
        }
    }

    /// @brief Parse a `DOMElement` corresponding to a discrete package and return a corresponding `Package`
    /// @param package_el the `DOMElement` pointing to the opening tag for the <Package/> attribute
    /// @return the `Package` variable parsed from `package_el`
    inline Package parseForPackage(const DOMElement *package_el){
        Package package;
        const DOMElement *name_el = package_el->getFirstElementChild();
        transcodeToQString(name_el, package.event_name);

        const DOMElement *type_el = name_el->getNextElementSibling();
        transcodeToQString(type_el, package.event_type);

        const DOMElement *weight_el = type_el->getNextElementSibling();
        transcodeToQString(weight_el, package.weight);

        const DOMElement *attackenv_el = weight_el->getNextElementSibling();
        transcodeToQString(attackenv_el, package.attack_envelope);

        const DOMElement *attackenvscale_el = attackenv_el->getNextElementSibling();
        transcodeToQString(attackenvscale_el, package.attackenvelope_scale);

        const DOMElement *durationenv_el = attackenvscale_el->getNextElementSibling();
        transcodeToQString(durationenv_el, package.duration_envelope);

        const DOMElement *durationenvscale_el = durationenv_el->getNextElementSibling();
        transcodeToQString(durationenvscale_el, package.durationenvelope_scale);

        return package;
    }

    inline Layer parseForLayer(const DOMElement *layer_el){
        Layer layer;
        const DOMElement *bylayer_el = layer_el->getFirstElementChild();
        layer.by_layer = getFunctionString(bylayer_el);

        if(const DOMElement *package_el = bylayer_el->getNextElementSibling(); package_el != nullptr){
            package_el = package_el->getFirstElementChild();
            while(package_el != nullptr){
                Package package = parseForPackage(package_el);
                layer.discrete_packages.append(package);
                package_el = package_el->getNextElementSibling();
            }
        }
        return layer;
    }

    inline const DOMElement* parseHEvent(const DOMElement *eventtype_el, HEvent& event){
        const DOMElement *name_el = eventtype_el->getNextElementSibling();
        transcodeToQString(name_el, event.name);

        const DOMElement *maxchilddur_el = name_el->getNextElementSibling();
        event.max_child_duration = getFunctionString(maxchilddur_el);

        const DOMElement *eduperbeat_el = maxchilddur_el->getNextElementSibling();
        event.edu_perbeat = getFunctionString(eduperbeat_el);

        const DOMElement *timesig_el = eduperbeat_el->getNextElementSibling();
        const DOMElement *barval_el = timesig_el->getFirstElementChild();
        event.timesig.bar_value = getFunctionString(barval_el);

        const DOMElement *noteval_el = barval_el->getNextElementSibling();
        event.timesig.note_value = getFunctionString(noteval_el);

        const DOMElement *tempo_el = timesig_el->getNextElementSibling();
        const DOMElement *tempomethodflag_el = tempo_el->getFirstElementChild();
        event.tempo.method_flag = getFunctionString(tempomethodflag_el).toUInt();

        const DOMElement *tempoprefix_el = tempomethodflag_el->getNextElementSibling();
        event.tempo.prefix = getFunctionString(tempoprefix_el);

        const DOMElement *temponoteval_el = tempoprefix_el->getNextElementSibling();
        event.tempo.note_value = getFunctionString(temponoteval_el);

        const DOMElement *tempofrentry1_el = temponoteval_el->getNextElementSibling();
        event.tempo.frentry_1 = getFunctionString(tempofrentry1_el);

        const DOMElement *tempofrentry2_el = tempofrentry1_el->getNextElementSibling();
        event.tempo.frentry_2 = getFunctionString(tempofrentry2_el);

        const DOMElement *tempovalue_el = tempofrentry2_el->getNextElementSibling();
        event.tempo.valentry = getFunctionString(tempovalue_el);

        const DOMElement *numchildren_el = tempo_el->getNextElementSibling();
        const DOMElement *numchildrenmethodflag_el = numchildren_el->getFirstElementChild();
        event.numchildren.method_flag = getFunctionString(numchildrenmethodflag_el).toUInt();

        const DOMElement *curr = numchildrenmethodflag_el->getNextElementSibling();
        event.numchildren.entry_1 = getFunctionString(curr);

        curr = curr->getNextElementSibling();
        event.numchildren.entry_2 = getFunctionString(curr);

        curr = curr->getNextElementSibling();
        event.numchildren.entry_3 = getFunctionString(curr);

        const DOMElement *childdef_el = numchildren_el->getNextElementSibling();
        curr = childdef_el->getFirstElementChild();
        event.child_event_def.entry_1 = getFunctionString(curr);

        curr = curr->getNextElementSibling();
        event.child_event_def.entry_2 = getFunctionString(curr);

        curr = curr->getNextElementSibling();
        event.child_event_def.entry_3 = getFunctionString(curr);

        curr = curr->getNextElementSibling();
        event.child_event_def.attack_sieve = getFunctionString(curr);

        curr = curr->getNextElementSibling();
        event.child_event_def.duration_sieve = getFunctionString(curr);

        curr = curr->getNextElementSibling();
        event.child_event_def.definition_flag = getFunctionString(curr).toUInt();

        curr = curr->getNextElementSibling();
        event.child_event_def.starttype_flag = getFunctionString(curr).toUInt();

        curr = curr->getNextElementSibling();
        event.child_event_def.durationtype_flag = getFunctionString(curr).toUInt();

        const DOMElement *layers_el = childdef_el->getNextElementSibling();
        const DOMElement *layer_el = layers_el->getFirstElementChild();
        while(layer_el != nullptr){
            Layer layer = parseForLayer(layer_el);
            event.event_layers.append(layer);
            layer_el = layer_el->getNextElementSibling();
        }

        const DOMElement *spa_el = layers_el->getNextElementSibling();
        event.spa = getFunctionString(spa_el);
        const DOMElement *reverb_el = spa_el->getNextElementSibling();
        event.reverb = getFunctionString(reverb_el);
        const DOMElement *filter_el = reverb_el->getNextElementSibling();
        event.filter = getFunctionString(filter_el);

        return filter_el;
    }

    inline Modifier parseForModifier(const DOMElement *modifier_el){
        Modifier modifier;
        const DOMElement *type_el = modifier_el->getFirstElementChild();
        modifier.type = getFunctionString(type_el).toUInt();

        const DOMElement *applyhow_el = type_el->getNextElementSibling();
        modifier.applyhow_flag = (getFunctionString(applyhow_el) == "0");

        const DOMElement *probability_el = applyhow_el->getNextElementSibling();
        modifier.probability = getFunctionString(probability_el);

        const DOMElement *amplitude_el = probability_el->getNextElementSibling();
        modifier.amplitude = getFunctionString(amplitude_el);

        const DOMElement *rate_el = amplitude_el->getNextElementSibling();
        modifier.rate = getFunctionString(rate_el);

        const DOMElement *width_el = rate_el->getNextElementSibling();
        modifier.width = getFunctionString(width_el);

        const DOMElement *detune_spread_el = width_el->getNextElementSibling();
        modifier.detune_spread = getFunctionString(detune_spread_el);

        const DOMElement *detune_direction_el = detune_spread_el->getNextElementSibling();
        modifier.detune_direction = getFunctionString(detune_direction_el);

        const DOMElement *detune_velocity_el = detune_direction_el->getNextElementSibling();
        modifier.detune_velocity = getFunctionString(detune_velocity_el);

        const DOMElement *group_name_el = detune_velocity_el->getNextElementSibling();
        modifier.group_name = getFunctionString(group_name_el);
        qDebug() << "SHOWING modifier.group_name: " << modifier.group_name;

        const DOMElement *partialresultstring_el = group_name_el->getNextElementSibling();
        modifier.partialresult_string = getFunctionString(partialresultstring_el);
        qDebug() << "SHOWING modifier.partialresult_string: " << modifier.partialresult_string;

        return modifier;
    }

    inline void parseEndOfHEvent(const DOMElement *filter_el, HEvent& event){
        const DOMElement *modifiers_el = filter_el->getNextElementSibling();
        const DOMElement *modifier_el = modifiers_el->getFirstElementChild();
        while(modifier_el != nullptr){
            event.modifiers.append(parseForModifier(modifier_el));
            modifier_el = modifier_el->getNextElementSibling();
        }
    }

    /// @brief Parses <ExtraInfo/> for `ExtraInfo` struct; does _not_ set childtype_flag.
    /// @param extrainfo_el `DOMElement*` for <ExtraInfo/>
    /// @return resulting `ExtraInfo`
    inline ExtraInfo parseExtraInfo(const DOMElement *extrainfo_el){
        ExtraInfo extrainfo;
        const DOMElement *freqinfo_el = extrainfo_el->getFirstElementChild();
        const DOMElement *freqflag_el = freqinfo_el->getFirstElementChild();
        extrainfo.freq_info.freq_flag = getFunctionString(freqflag_el).toUInt();

        const DOMElement *freqcontflag_el = freqflag_el->getNextElementSibling();
        extrainfo.freq_info.continuum_flag = getFunctionString(freqcontflag_el).toUInt();

        const DOMElement *freqentry1_el = freqcontflag_el->getNextElementSibling();
        extrainfo.freq_info.entry_1 = getFunctionString(freqentry1_el);

        const DOMElement *freqentry2_el = freqentry1_el->getNextElementSibling();
        extrainfo.freq_info.entry_2 = getFunctionString(freqentry2_el);

        const DOMElement *loudness_el = freqinfo_el->getNextElementSibling();
        extrainfo.loudness = getFunctionString(loudness_el);

        const DOMElement *spa_el = loudness_el->getNextElementSibling();
        extrainfo.spa = getFunctionString(spa_el);

        const DOMElement *reverb_el = spa_el->getNextElementSibling();
        extrainfo.reverb = getFunctionString(reverb_el);

        const DOMElement *filter_el = reverb_el->getNextElementSibling();
        extrainfo.filter = getFunctionString(filter_el);

        const DOMElement *modifiergroup_el = filter_el->getNextElementSibling();
        extrainfo.modifier_group = getFunctionString(modifiergroup_el);

        const DOMElement *modifiers_el = modifiergroup_el->getNextElementSibling();
        const DOMElement *modifier_el = modifiers_el->getFirstElementChild();
        while(modifier_el != nullptr){
            extrainfo.modifiers.append(parseForModifier(modifier_el));
            modifier_el = modifier_el->getNextElementSibling();
        }

        return extrainfo;
    }

    inline void parseEndOfBottomEvent(const DOMElement *filter_el, BottomEvent& bottom_event){
        const DOMElement *extrainfo_el = filter_el->getNextElementSibling();
        bottom_event.extra_info = parseExtraInfo(extrainfo_el);

        const QString prefix = bottom_event.event.name[0];
        unsigned childtype_flag;
        if(prefix=="s")
            childtype_flag = 0;
        else if(prefix=="n")
            childtype_flag = 1;
        else
            childtype_flag = 2;

        bottom_event.extra_info.childtype_flag = childtype_flag;
    }

    inline Spectrum parseForSpectrum(const DOMElement *spectrum_el) {
        Spectrum spectrum;
        const DOMElement *partial_el = spectrum_el->getFirstElementChild();
        while (partial_el) {
            spectrum.partials.append(getFunctionString(partial_el));
            partial_el = partial_el->getNextElementSibling();
        }
        return spectrum;
    }

    inline void parseSpectrumEvent(const DOMElement *orderinpalette_el, SpectrumEvent& event) {
        const DOMElement *child = orderinpalette_el->getNextElementSibling();
        event.name = getFunctionString(child);

        child = child->getNextElementSibling();
        event.num_partials = getFunctionString(child);

        child = child->getNextElementSibling();
        event.deviation = getFunctionString(child);

        child = child->getNextElementSibling();
        event.generate_spectrum = getFunctionString(child);

        child = child->getNextElementSibling();
        event.spectrum = parseForSpectrum(child);
    }

    inline NoteInfo parseForNoteInfo(const DOMElement *noteinfo_el) {
        NoteInfo ni;
        const DOMElement *staffs_el = noteinfo_el->getFirstElementChild();
        ni.staffs = getFunctionString(staffs_el);

        const DOMElement *modifiers_el = staffs_el->getNextElementSibling();
        const DOMElement *modifier_el = modifiers_el->getFirstElementChild();
        while(modifier_el != nullptr){
            ni.modifiers.append(getFunctionString(modifier_el));
            modifier_el = modifier_el->getNextElementSibling();
        }

        return ni;
    }

    inline void parseNoteEvent(const DOMElement *event_el, NoteEvent& event) {
        const DOMElement *child = event_el->getNextElementSibling();
        event.name = getFunctionString(child);

        child = child->getNextElementSibling();
        event.note_info = parseForNoteInfo(child);
    }

    inline void parseEnvelopeEvent(const DOMElement* event_el, EnvelopeEvent& ev) {
        const DOMElement *child = event_el->getNextElementSibling();
        ev.name = getFunctionString(child);

        child = child->getNextElementSibling();
        ev.envelope_builder = getFunctionString(child);
    }

    inline void parseSieveEvent(const DOMElement* event_el, SieveEvent& ev) {
        const DOMElement *child = event_el->getNextElementSibling();
        ev.name = getFunctionString(child);

        child = child->getNextElementSibling();
        ev.sieve_builder = getFunctionString(child);
    }

    inline void parseSpaEvent(const DOMElement* event_el, SpaEvent& ev) {
        const DOMElement *child = event_el->getNextElementSibling();
        ev.name = getFunctionString(child);

        child = child->getNextElementSibling();
        ev.spatialization = getFunctionString(child);
    }

    inline void parsePatternEvent(const DOMElement* event_el, PatternEvent& ev) {
        const DOMElement *child = event_el->getNextElementSibling();
        ev.name = getFunctionString(child);

        child = child->getNextElementSibling();
        ev.pattern_builder = getFunctionString(child);
    }

    inline void parseReverbEvent(const DOMElement* event_el, ReverbEvent& ev) {
        const DOMElement *child = event_el->getNextElementSibling();
        ev.name = getFunctionString(child);

        child = child->getNextElementSibling();
        ev.reverberation = getFunctionString(child);
    }

    inline void parseFilterEvent(const DOMElement* event_el, FilterEvent& ev) {
        const DOMElement *child = event_el->getNextElementSibling();
        ev.name = getFunctionString(child);

        child = child->getNextElementSibling();
        ev.filter_builder = getFunctionString(child);
    }
} // namespace XercesParser

inline void Project::parseEvent(xercesc::DOMElement *event_start){
    using namespace xercesc;
    XMLCh *xmldata = XMLString::transcode("orderInPalette");
    std::string orderinpalette = XMLString::transcode(event_start->getAttribute(xmldata));
    XMLString::release(&xmldata);

    DOMElement *eventtype_el = event_start->getFirstElementChild();
    auto textdata = static_cast<DOMCharacterData*>(eventtype_el->getFirstChild());
    char* buffer = XMLString::transcode(textdata->getData());
    auto type = static_cast<Eventtype>(std::stoi(buffer));
    XMLString::release(&buffer);

    switch(type){
        case top:
        case high:
        case mid:
        case low: {
            HEvent eh;
            eh.orderinpalette = QString::fromStdString(orderinpalette);
            eh.type = type;
            const DOMElement *filter_el = XercesParser::parseHEvent(eventtype_el, eh);
            XercesParser::parseEndOfHEvent(filter_el, eh);
            qDebug() << "parsed" << eh.type << "event named" << eh.name;
            switch(type){
                case top:
                    top_event = eh;
                    break;
                case high:
                    high_events.append(eh);
                    break;
                case mid:
                    mid_events.append(eh);
                    break;
                case low:
                    low_events.append(eh);
                    break;
            }
            break;
        }
        case bottom: {
            BottomEvent eb;
            eb.event.orderinpalette = QString::fromStdString(orderinpalette);
            eb.event.type = type;
            const DOMElement *filter_el = XercesParser::parseHEvent(eventtype_el, eb.event);
            XercesParser::parseEndOfBottomEvent(filter_el, eb);
            qDebug() << "parsed Bottom event named" << eb.event.name;
            bottom_events.append(eb);
            break;
        }
        case sound: {
            SpectrumEvent espec;
            espec.orderinpalette = QString::fromStdString(orderinpalette);
            XercesParser::parseSpectrumEvent(eventtype_el, espec);
            qDebug() << "parsed Spectrum event named" << espec.name;
            spectrum_events.append(espec);
            break;
        }
        case note: {
            NoteEvent en;
            en.orderinpalette = QString::fromStdString(orderinpalette);
            XercesParser::parseNoteEvent(eventtype_el, en);
            qDebug() << "parsed Note event named " << en.name;
            note_events.append(en);
            break;
        }
        case env: {
            EnvelopeEvent ee;
            ee.orderinpalette = QString::fromStdString(orderinpalette);
            XercesParser::parseEnvelopeEvent(eventtype_el, ee);
            qDebug() << "parsed Envelope event named" << ee.name;
            envelope_events.append(ee);
            break;
        }
        case sieve: {
            SieveEvent esi;
            esi.orderinpalette = QString::fromStdString(orderinpalette);
            XercesParser::parseSieveEvent(eventtype_el, esi);
            qDebug() << "parsed Sieve event named" << esi.name;
            sieve_events.append(esi);
            break;
        }
        case spa: {
            SpaEvent espa;
            espa.orderinpalette = QString::fromStdString(orderinpalette);
            XercesParser::parseSpaEvent(eventtype_el, espa);
            qDebug() << "parsed Spa event named" << espa.name;
            spa_events.append(espa);
            break;
        }
        case pattern: {
            PatternEvent ep;
            ep.orderinpalette = QString::fromStdString(orderinpalette);
            XercesParser::parsePatternEvent(eventtype_el, ep);
            qDebug() << "parsed Pattern event named" << ep.name;
            pattern_events.append(ep);
            break;
        }
        case reverb: {
            ReverbEvent er;
            er.orderinpalette = QString::fromStdString(orderinpalette);
            XercesParser::parseReverbEvent(eventtype_el, er);
            qDebug() << "parsed Reverb event named" << er.name;
            reverb_events.append(er);
            break;
        }
        case filter: {
            FilterEvent ef;
            ef.orderinpalette = QString::fromStdString(orderinpalette);
            XercesParser::parseFilterEvent(eventtype_el, ef);
            qDebug() << "parsed Filter event named" << ef.name;
            filter_events.append(ef);
            break;
        }
        default:
            qDebug() << "ERROR: parsing event gave event type outside defined types";
    }
}

inline void ProjectManager::parse(Project *p, const QString& filepath){
    using namespace xercesc;
    XMLPlatformUtils::Initialize();
    auto parser = new XercesDOMParser();

    std::string file_name = filepath.toStdString();
    parser->parse(file_name.c_str());
    DOMDocument *document = parser->getDocument();
    DOMElement *root = document->getDocumentElement();

    char *buffer;
    DOMCharacterData *char_data;

    //Configurations
    DOMElement* configuration = root->getFirstElementChild();
    p->config_el = configuration;

    //fileFlag
    DOMElement* element = configuration->getFirstElementChild();
    element = element->getNextElementSibling(); // skip Title attribute
    p->file_flag = XercesParser::getFunctionString(element);

    // topEvent
    element = element->getNextElementSibling(); //skipped, always = "0"
    p->top_event.name = "0";

    // pieceStartTime
    element = element->getNextElementSibling();
    p->start_time = XercesParser::getFunctionString(element);

    //duration
    element = element->getNextElementSibling();
    p->duration = XercesParser::getFunctionString(element);

    //synthesis
    element = element->getNextElementSibling();
    qDebug() << "Synthesis: " << element->getTagName();

    if(XercesParser::getFunctionString(element) == "True")
        p->synthesis = true;
    else
        p->synthesis = false;

    //score
    element = element->getNextElementSibling();
    if(XercesParser::getFunctionString(element) == "True")
        p->score = true;
    else
        p->score = false;

    //grandstaff
    element = element->getNextElementSibling();
    qDebug() << "Grandstaff: " << element->getTagName();
    if(XercesParser::getFunctionString(element) == "True")
        p->grand_staff = true;
    else
        p->grand_staff = false;

    //numOfStaff
    element = element->getNextElementSibling();
    p->num_staffs = XercesParser::getFunctionString(element);

    //numOfChannels
    element = element->getNextElementSibling();
    p->num_channels = XercesParser::getFunctionString(element);

    //sampleRate
    element = element->getNextElementSibling();
    p->sample_rate = XercesParser::getFunctionString(element);

    //sampleSize
    element = element->getNextElementSibling();
    p->sample_size = XercesParser::getFunctionString(element);

    //numOfThreads
    element = element->getNextElementSibling();
    p->num_threads = XercesParser::getFunctionString(element);

    // Output Particel
    element = element->getNextElementSibling();
    if(element != nullptr /* why this nullptr check? -6/23/25 */ && XercesParser::getFunctionString(element) == "True")
        p->output_particel = true;
    else
        p->output_particel = false;

    DOMElement* noteModifiers = configuration->getNextElementSibling();
    element = noteModifiers->getFirstElementChild(); //default modifiers
    char_data = static_cast<DOMCharacterData*>(element->getFirstChild());
    buffer = XMLString::transcode(char_data->getData());

    // auto modifierMapIter = p->default_note_modifiers.begin();
    // int index = 0;
    // for(auto modifierMapIter = p->default_note_modifiers.begin(); modifierMapIter != p->default_note_modifiers.end(); ++modifierMapIter){
    //     modifierMapIter.value() = true;
    // //    (*modifierMapIter).second = (charBuffer[index]=='1')?true:false;
    //     index = index + 3;
    // }
    XMLString::release(&buffer);

    element = element->getNextElementSibling(); //custom modifiers
    DOMElement* modifier = element->getFirstElementChild();

    while (modifier != nullptr){
        char_data = static_cast<DOMCharacterData *>(modifier->getFirstChild());
        buffer = XMLString::transcode(char_data->getData());
        p->custom_note_modifiers.append(QString::fromStdString(std::string(buffer)));
        XMLString::release(&buffer);
        modifier = modifier->getNextElementSibling();
    }
    qDebug() << "Passed modifiers";

    DOMElement *envlibelement = noteModifiers->getNextElementSibling();
    char_data = static_cast<DOMCharacterData*>(envlibelement->getFirstChild());

    auto envelopeLibrary = new EnvelopeLibrary();
    if(char_data != nullptr){
        buffer = XMLString::transcode(char_data->getData());
        std::string fileString = file_name + ".lib.temp"; /* the conversion from char* to QString is MUCH better than the other way around */
        QFile file(QString::fromStdString(fileString));
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);
        out << buffer;
        XMLString::release(&buffer);
        file.close();

        envelopeLibrary->loadLibraryNewFormat(const_cast<char*>(fileString.c_str()));
        QString deleteCommand = "rm " + QString::fromStdString(fileString);
        QProcess::execute(deleteCommand);
    }

    /// \todo refactor this (from old lassie)
    EnvelopeLibraryEntry* previousEntry = nullptr;
    Envelope* thisEnvelope;

    for (int i = 1; i <= envelopeLibrary->size(); i ++){
        thisEnvelope = envelopeLibrary->getEnvelope(i);
        auto thisEntry = new EnvelopeLibraryEntry(thisEnvelope, i);
        delete thisEnvelope;

        if (previousEntry == nullptr){
            p->elentry = thisEntry;
            thisEntry->prev = nullptr;
        }else{
            previousEntry->next = thisEntry;
            thisEntry->prev = previousEntry;
        }
        previousEntry = thisEntry;
    }

    delete envelopeLibrary;
    qDebug() << "Passed envelopes";

    DOMElement *currentElement = envlibelement;

    /// \todo remove need for that "MarkovModelLibrary" check
    DOMElement *markovModelLibraryElement = envlibelement->getNextElementSibling();
    buffer = XMLString::transcode(markovModelLibraryElement->getTagName());
    auto tagName = std::string(buffer);
    XMLString::release(&buffer);
    if (tagName == "MarkovModelLibrary") {
        currentElement = markovModelLibraryElement;
        if(auto text = dynamic_cast<DOMText*>(markovModelLibraryElement->getFirstChild()); text != nullptr){
            std::string data = buffer = XMLString::transcode(text->getWholeText());
            XMLString::release(&buffer);

            std::stringstream ss(data);
            // read the number of models
            long long size;
            ss >> size;
            p->markov_models.resize(size);
            // read individual models
            std::string modelText, line;
            std::getline(ss, line, '\n');
            for (int i = 0; i < size; i++) {
                std::getline(ss, line, '\n');
                modelText = line + '\n';
                std::getline(ss, line, '\n');
                modelText += line + '\n';
                std::getline(ss, line, '\n');
                modelText += line + '\n';
                std::getline(ss, line, '\n');
                modelText += line;

                (p->markov_models[i]).from_str(modelText);
            }
        }
    }
    qDebug() << "Passed markov";

    DOMElement *domEvents = currentElement->getNextElementSibling();
    DOMElement *eventElement = domEvents->getFirstElementChild();
    qDebug() << "eventElement: " << eventElement;

    while (eventElement != nullptr){
        p->parseEvent(eventElement);
        eventElement = eventElement->getNextElementSibling();
    }
}

#endif // XMLPARSER_HPP
