/* 
The 'project' object that keeps track of project details to deploy
in the associated window (currently, the project view).
*/
#include "project_struct.hpp"
#include "event_struct.hpp"
// #include "IEvent.h"

#include "../windows/MainWindow.hpp"

#include <QFile>
#include <QTextStream>
#include <QFileInfo>
// #include <QtLogging>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>

namespace XercesParser {
    using namespace xercesc;
    inline QString getFunctionString(DOMElement* _thisFunctionElement){
        char* charBuffer;

        DOMCharacterData* textData;
        std::string returnString;
        DOMNode* child = _thisFunctionElement->getFirstChild();
        if (child == NULL){ //not containing any child, return string
            return "";
        }

        XMLCh tempStr[3] = {chLatin_L, chLatin_S, chNull};
        DOMImplementation *impl          = DOMImplementationRegistry::getDOMImplementation(tempStr);
        DOMLSSerializer   *theSerializer = ((DOMImplementationLS*)impl)->createLSSerializer();
        XMLCh* bla = theSerializer->writeToString (_thisFunctionElement);
        char* toTranscode = XMLString::transcode(bla);
        returnString = string (toTranscode);
        XMLString::release(&toTranscode);
        XMLString::release(&bla);

        int tagLength = (int) XMLString::stringLen(_thisFunctionElement->getTagName());

        delete theSerializer;
        returnString = returnString.substr(tagLength+2, returnString.size() - tagLength * 2 - 5);
        return QString::fromStdString(returnString);
    }

    /// @brief For use when transcoding a basic `std::string` from a `DOMElement*` and assigning it to a `std::string` variable.
    /// @param element the DOMElement to be transcoded from
    /// @param lhs the string to be assigned to
    inline void transcodeAndAssign(DOMElement *element, QString& lhs){
        DOMCharacterData *textdata = (DOMCharacterData*)(element->getFirstChild());
        char *buffer = XMLString::transcode(textdata->getData());;
        lhs = QString::fromStdString(buffer);
        XMLString::release(&buffer);
    }

    /// @brief Parse a `DOMElement` corresponding to a discrete package and return a corresponding `Package`
    /// @param package_el the `DOMElement` pointing to the opening tag for the <Package/> attribute
    /// @return the `Package` variable parsed from `package_el`
    inline Package parseForPackage(DOMElement *package_el){
        Package package;
        DOMElement *name_el = package_el->getFirstElementChild();
        transcodeAndAssign(name_el, package.event_name);

        DOMElement *type_el = name_el->getNextElementSibling();
        DOMCharacterData *textdata = (DOMCharacterData*)(type_el->getFirstChild());
        char *buffer = XMLString::transcode(textdata->getData());
        package.event_type = (Eventtype)std::atoi(buffer);
        XMLString::release(&buffer);

        DOMElement *weight_el = type_el->getNextElementSibling();
        transcodeAndAssign(weight_el, package.weight);

        DOMElement *attackenv_el = weight_el->getNextElementSibling();
        transcodeAndAssign(attackenv_el, package.attack_envelope);

        DOMElement *attackenvscale_el = attackenv_el->getNextElementSibling();
        transcodeAndAssign(attackenvscale_el, package.attackenvelope_scale);

        DOMElement *durationenv_el = attackenvscale_el->getNextElementSibling();
        transcodeAndAssign(durationenv_el, package.duration_envelope);

        DOMElement *durationenvscale_el = durationenv_el->getNextElementSibling();
        transcodeAndAssign(durationenvscale_el, package.durationenvelope_scale);    

        return package;
    }

    Layer parseForLayer(DOMElement *layer_el){
        Layer layer; 
        layer_el = layer_el->getFirstElementChild();
        layer.by_layer = getFunctionString(layer_el);

        DOMElement *package_el = layer_el->getNextElementSibling()->getFirstElementChild();
        QList<Package> discrete_packages;
        while(package_el != nullptr){
            Package package = parseForPackage(package_el);
            discrete_packages.append(package);
            package_el = package_el->getNextElementSibling();
        }
        
        return layer;
    }

    DOMElement* parseHEvent(DOMElement *eventtype_el, HEvent& event){
        DOMElement *name_el = eventtype_el->getNextElementSibling();
        transcodeAndAssign(name_el, event.name);

        DOMElement *maxchilddur_el = name_el->getNextElementSibling();
        event.max_child_duration = getFunctionString(maxchilddur_el);

        DOMElement *eduperbeat_el = maxchilddur_el->getNextElementSibling();
        event.edu_perbeat = getFunctionString(eduperbeat_el);

        DOMElement *timesig_el = eduperbeat_el->getNextElementSibling();
        DOMElement *barval_el = timesig_el->getFirstElementChild();
        event.timesig.bar_value = getFunctionString(barval_el).toUInt();

        DOMElement *noteval_el = barval_el->getNextElementSibling();
        event.timesig.note_value = getFunctionString(noteval_el).toUInt();

        DOMElement *tempo_el = noteval_el->getNextElementSibling();
        DOMElement *tempomethodflag_el = tempo_el->getFirstElementChild();
        event.tempo.method_flag = (Numchildrenflag)getFunctionString(tempomethodflag_el).toUInt();

        DOMElement *tempoprefix_el = tempomethodflag_el->getNextElementSibling();
        event.tempo.prefix = (Tempoprefix)getFunctionString(tempoprefix_el).toUInt();

        DOMElement *temponoteval_el = tempoprefix_el->getNextElementSibling();
        event.tempo.note_value = (Temponotevalue)getFunctionString(temponoteval_el).toUInt();

        DOMElement *tempofrentry1_el = temponoteval_el->getNextElementSibling();
        event.tempo.frentry_1 = getFunctionString(tempofrentry1_el);

        DOMElement *tempofrentry2_el = tempofrentry1_el->getNextElementSibling();
        event.tempo.frentry_2 = getFunctionString(tempofrentry2_el);

        DOMElement *tempovalue_el = tempofrentry2_el->getNextElementSibling();
        event.tempo.valentry = getFunctionString(tempovalue_el);

        DOMElement *numchildren_el = tempo_el->getNextElementSibling();
        DOMElement *numchildrenmethodflag_el = numchildren_el->getFirstElementChild();
        event.numchildren.method_flag = (Numchildrenflag)getFunctionString(numchildrenmethodflag_el).toUInt();

        DOMElement *curr = numchildrenmethodflag_el->getNextElementSibling();
        event.numchildren.entry_1 = getFunctionString(curr);

        curr = curr->getNextElementSibling();
        event.numchildren.entry_2 = getFunctionString(curr);

        curr = curr->getNextElementSibling();
        event.numchildren.entry_3 = getFunctionString(curr);

        DOMElement *childdef_el = numchildren_el->getNextElementSibling();
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
        event.child_event_def.definition_flag = (Childdefnflag)getFunctionString(curr).toUInt();

        curr = curr->getNextElementSibling();
        event.child_event_def.starttype_flag = (Childdeftimeflag)getFunctionString(curr).toUInt();

        curr = curr->getNextElementSibling();
        event.child_event_def.durationtype_flag = (Childdeftimeflag)getFunctionString(curr).toUInt();

        DOMElement *layers_el = childdef_el->getNextElementSibling();
        DOMElement *layer_el = layers_el->getFirstElementChild();
        while(layer_el != nullptr){
            Layer layer = parseForLayer(layers_el);
            event.event_layers.append(layer);
            layer_el = layer_el->getNextElementSibling();
        }
        
        DOMElement *spa_el = layers_el->getNextElementSibling();
        event.spa = getFunctionString(spa_el);
        DOMElement *reverb_el = spa_el->getNextElementSibling();
        event.reverb = getFunctionString(reverb_el);
        DOMElement *filter_el = reverb_el->getNextElementSibling();
        event.filter = getFunctionString(filter_el);

        return filter_el;
    }

    Modifier parseForModifier(DOMElement *modifier_el){
        Modifier modifier;
        DOMElement *type_el = modifier_el->getFirstElementChild();
        modifier.type = getFunctionString(type_el).toUInt();

        DOMElement *applyhow_el = type_el->getNextElementSibling();
        modifier.applyhow_flag = (getFunctionString(applyhow_el) == "0");

        DOMElement *probability_el = applyhow_el->getNextElementSibling();
        modifier.probability = getFunctionString(probability_el);

        DOMElement *amplitude_el = applyhow_el->getNextElementSibling();
        modifier.amplitude = getFunctionString(amplitude_el);

        DOMElement *rate_el = amplitude_el->getNextElementSibling();
        modifier.rate = getFunctionString(rate_el);

        DOMElement *width_el = rate_el->getNextElementSibling();
        modifier.width = getFunctionString(width_el);

        DOMElement *detune_spread_el = width_el->getNextElementSibling();
        modifier.detune_spread = getFunctionString(detune_spread_el);

        DOMElement *detune_direction_el = detune_spread_el->getNextElementSibling();
        modifier.detune_direction = getFunctionString(detune_direction_el);

        DOMElement *detune_velocity_el = detune_direction_el->getNextElementSibling();
        modifier.detune_velocity = getFunctionString(detune_velocity_el);

        DOMElement *group_name_el = detune_velocity_el->getNextElementSibling();
        modifier.group_name = getFunctionString(group_name_el);

        DOMElement *partialresultstring_el = group_name_el->getNextElementSibling();
        modifier.partialresult_string = getFunctionString(partialresultstring_el);

        return modifier;
    }

    void parseEndOfHEvent(DOMElement *filter_el, HEvent& event){
        DOMElement *modifiers_el = filter_el->getNextElementSibling();
        DOMElement *modifier_el = modifiers_el->getFirstElementChild();
        while(modifier_el != nullptr){
            event.modifiers.append(parseForModifier(modifier_el));
            modifier_el = modifier_el->getNextElementSibling();
        }
    }

    /// @brief Parses <ExtraInfo/> for `ExtraInfo` struct; does _not_ set childtype_flag.
    /// @param extrainfo_el `DOMElement*` for <ExtraInfo/>
    /// @return resulting `ExtraInfo`
    ExtraInfo parseExtraInfo(DOMElement *extrainfo_el){
        ExtraInfo extrainfo;
        DOMElement *freqinfo_el = extrainfo_el->getFirstElementChild();
        DOMElement *freqflag_el = freqinfo_el->getFirstElementChild();
        extrainfo.freq_info.freq_flag = (Freqinfofreqflag)getFunctionString(freqflag_el).toUInt();

        DOMElement *freqcontflag_el = freqflag_el->getNextElementSibling();
        extrainfo.freq_info.continuum_flag = (Freqinfocontflag)getFunctionString(freqcontflag_el).toUInt();

        DOMElement *freqentry1_el = freqcontflag_el->getNextElementSibling();
        extrainfo.freq_info.entry_1 = getFunctionString(freqentry1_el);

        DOMElement *freqentry2_el = freqentry1_el->getNextElementSibling();
        extrainfo.freq_info.entry_2 = getFunctionString(freqentry2_el);
        
        DOMElement *loudness_el = freqinfo_el->getNextElementSibling();
        extrainfo.loudness = getFunctionString(loudness_el);

        DOMElement *spa_el = loudness_el->getNextElementSibling();
        extrainfo.spa = getFunctionString(spa_el);

        DOMElement *reverb_el = spa_el->getNextElementSibling();
        extrainfo.reverb = getFunctionString(reverb_el);

        DOMElement *filter_el = reverb_el->getNextElementSibling();
        extrainfo.filter = getFunctionString(filter_el);

        DOMElement *modifiergroup_el = filter_el->getNextElementSibling();
        extrainfo.modifier_group = getFunctionString(modifiergroup_el);

        DOMElement *modifiers_el = modifiergroup_el->getNextElementSibling();
        DOMElement *modifier_el = modifiers_el->getFirstElementChild();
        while(modifier_el != nullptr){
            extrainfo.modifiers.append(parseForModifier(modifier_el));
            modifier_el = modifier_el->getNextElementSibling();
        }
        
        return extrainfo;
    }

    void parseEndOfBottomEvent(DOMElement *filter_el, BottomEvent& bottom_event){
        DOMElement *extrainfo_el = filter_el->getNextElementSibling();
        bottom_event.extra_info = parseExtraInfo(extrainfo_el);

        QString prefix = bottom_event.event.name[0];
        unsigned childtype_flag;
        if(prefix=="s")
            childtype_flag = 0;
        else if(prefix=="n")
            childtype_flag = 1;
        else
            childtype_flag = 2;
        
        bottom_event.extra_info.childtype_flag = childtype_flag;
    }

    Spectrum parseForSpectrum(DOMElement *spectrum_el) {
        Spectrum spectrum;
        DOMElement *partial_el = spectrum_el->getFirstElementChild();
        while (partial_el) {
            spectrum.partials.append(getFunctionString(partial_el));
            partial_el = partial_el->getNextElementSibling();
        }
        return spectrum;
    }

    void parseSpectrumEvent(DOMElement *orderinpalette_el, SpectrumEvent& event) {
        DOMElement *child = orderinpalette_el->getNextElementSibling();
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

    NoteInfo parseForNoteInfo(DOMElement *noteinfo_el) {
        NoteInfo ni;
        DOMElement *staffs_el = noteinfo_el->getFirstElementChild();
        ni.staffs = getFunctionString(staffs_el);

        DOMElement *modifiers_el = staffs_el->getNextElementSibling();
        DOMElement *modifier_el = modifiers_el->getFirstElementChild();
        while(modifier_el != nullptr){
            ni.modifiers.append(getFunctionString(modifier_el));
            modifier_el = modifier_el->getNextElementSibling();
        }

        return ni;
    }

    void parseNoteEvent(DOMElement *event_el, NoteEvent& event) {
        DOMElement *child = event_el->getNextElementSibling();
        event.name = getFunctionString(child);

        child = child->getNextElementSibling();
        event.note_info = parseForNoteInfo(child);
    }

    void parseEnvelopeEvent(DOMElement* event_el, EnvelopeEvent& ev) {
        DOMElement *child = event_el->getFirstElementChild();
        ev.orderinpalette = getFunctionString(child);

        child = child->getNextElementSibling();
        ev.name = getFunctionString(child);

        child = child->getNextElementSibling();
        ev.envelope_builder = getFunctionString(child);
    }

    void parseSieveEvent(DOMElement* event_el, SieveEvent& ev) {
        DOMElement *child = event_el->getFirstElementChild();
        ev.orderinpalette = getFunctionString(child);

        child = child->getNextElementSibling();
        ev.name = getFunctionString(child);

        child = child->getNextElementSibling();
        ev.sieve_builder = getFunctionString(child);
    }

    void parseSpaEvent(DOMElement* event_el, SpaEvent& ev) {
        DOMElement *child = event_el->getFirstElementChild();
        ev.orderinpalette = getFunctionString(child);

        child = child->getNextElementSibling();
        ev.name = getFunctionString(child);

        child = child->getNextElementSibling();
        ev.spatialization = getFunctionString(child);
    }

    void parsePatternEvent(DOMElement* event_el, PatternEvent& ev) {
        DOMElement *child = event_el->getFirstElementChild();
        ev.orderinpalette = getFunctionString(child);

        child = child->getNextElementSibling();
        ev.name = getFunctionString(child);

        child = child->getNextElementSibling();
        ev.pattern_builder = getFunctionString(child);
    }

    void parseReverbEvent(DOMElement* event_el, ReverbEvent& ev) {
        DOMElement *child = event_el->getFirstElementChild();
        ev.orderinpalette = getFunctionString(child);

        child = child->getNextElementSibling();
        ev.name = getFunctionString(child);

        child = child->getNextElementSibling();
        ev.reverberation = getFunctionString(child);
    }

    void parseFilterEvent(DOMElement* event_el, FilterEvent& ev) {
        DOMElement *child = event_el->getFirstElementChild();
        ev.orderinpalette = getFunctionString(child);

        child = child->getNextElementSibling();
        ev.name = getFunctionString(child);

        child = child->getNextElementSibling();
        ev.filter_builder = getFunctionString(child);
    }
}

void Project::parseEvents(xercesc::DOMElement *event_start){
    using namespace xercesc;
    XMLCh *xmldata = XMLString::transcode("orderInPalette");
    std::string orderinpalette = XMLString::transcode(event_start->getAttribute(xmldata));
    XMLString::release(&xmldata);
    
    DOMElement *eventtype_el = event_start->getFirstElementChild();
    DOMCharacterData* textdata = (DOMCharacterData*)eventtype_el->getFirstChild();
    char* buffer = XMLString::transcode(textdata->getData());
    Eventtype type = (Eventtype)std::stoi(buffer);
    XMLString::release(&buffer);

    switch(type){
        case top:
        case high:
        case mid:
        case low: {
            HEvent eh;
            eh.orderinpalette = QString::fromStdString(orderinpalette);
            eh.type = type;
            DOMElement *filter_el = XercesParser::parseHEvent(eventtype_el, eh);
            XercesParser::parseEndOfHEvent(filter_el, eh);
            h_events.append(eh);
            break;
        }
        case bottom: {
            BottomEvent eb;
            eb.event.orderinpalette = QString::fromStdString(orderinpalette);
            eb.event.type = type;
            DOMElement *filter_el = XercesParser::parseHEvent(eventtype_el, eb.event);
            XercesParser::parseEndOfBottomEvent(filter_el, eb);
            bottom_events.append(eb);
            break;
        }
        case sound: {
            SpectrumEvent espec;
            espec.orderinpalette = QString::fromStdString(orderinpalette);
            XercesParser::parseSpectrumEvent(eventtype_el, espec);
            spectrum_events.append(espec);
            break;
        }
        case note: {
            NoteEvent en;
            en.orderinpalette = QString::fromStdString(orderinpalette);
            XercesParser::parseNoteEvent(eventtype_el, en);
            note_events.append(en);
            break;
        }
        case env: {
            EnvelopeEvent ee;
            ee.orderinpalette = QString::fromStdString(orderinpalette);
            XercesParser::parseEnvelopeEvent(eventtype_el, ee);
            envelope_events.append(ee);
            break;
        }
        case sieve: {
            SieveEvent esi;
            esi.orderinpalette = QString::fromStdString(orderinpalette);
            XercesParser::parseSieveEvent(eventtype_el, esi);
            sieve_events.append(esi);
            break;
        }
        case spa: {
            SpaEvent espa;
            espa.orderinpalette = QString::fromStdString(orderinpalette);
            XercesParser::parseSpaEvent(eventtype_el, espa);
            spa_events.append(espa);
            break;
        }
        case pattern: {
            PatternEvent ep;
            ep.orderinpalette = QString::fromStdString(orderinpalette);
            XercesParser::parsePatternEvent(eventtype_el, ep);
            pattern_events.append(ep);
            break;
        }
        case reverb: {
            ReverbEvent er;
            er.orderinpalette = QString::fromStdString(orderinpalette);
            XercesParser::parseReverbEvent(eventtype_el, er);
            reverb_events.append(er);
            break;
        }
        case filter: {
            FilterEvent ef;
            ef.orderinpalette = QString::fromStdString(orderinpalette);
            XercesParser::parseFilterEvent(eventtype_el, ef);
            filter_events.append(ef);
            break;
        }
        default:
            qDebug() << "ERROR: parsing event gave event type outside defined types";
    }
}
void ProjectManager::parse(Project *p, const QString& filepath){
    using namespace xercesc;
    XMLPlatformUtils::Initialize();
    XercesDOMParser* parser = new XercesDOMParser();

    std::string file_name = filepath.toStdString();
    parser->parse(file_name.c_str());
    DOMDocument *document = parser->getDocument();
    DOMElement *root = document->getDocumentElement();

    char *buffer;
    DOMCharacterData *char_data;

    //Configurations
    DOMElement* configuration = root->getFirstElementChild();
    
    //fileFlag
    DOMElement* element = configuration->getFirstElementChild();
    element = element->getNextElementSibling(); // skip Title attribute
    p->file_flag = XercesParser::getFunctionString(element);
    qDebug() << "Fileflag: " << p->file_flag;

    //topEvent
    element = element->getNextElementSibling();
    p->top_event = XercesParser::getFunctionString(element);
    qDebug() << "Top event: " << p->top_event;
    // pieceStartTime
    element = element->getNextElementSibling(); //skipped

    //duration
    element = element->getNextElementSibling();
    p->duration = XercesParser::getFunctionString(element);

    //synthesis
    element = element->getNextElementSibling();
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
    char_data = ( DOMCharacterData*) element->getFirstChild();
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

    while (modifier != NULL){
        char_data = (DOMCharacterData*)modifier->getFirstChild();
        buffer = XMLString::transcode(char_data->getData());
        p->custom_note_modifiers.append(QString::fromStdString(string(buffer)));
        XMLString::release(&buffer);
        modifier = modifier->getNextElementSibling();
    }
    qDebug() << "Passed modifiers";

    DOMElement *envlibelement = noteModifiers->getNextElementSibling();
    char_data = (DOMCharacterData*) envlibelement->getFirstChild();

    buffer = XMLString::transcode(char_data->getData());
    std::string fileString = file_name + ".lib.temp";
    QFile file(QString::fromStdString(fileString));
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
    }

    QTextStream out(&file);
    out << buffer;
    XMLString::release(&buffer);
    file.close();

#ifdef ENVELOPE
    EnvelopeLibrary* envelopeLibrary = new EnvelopeLibrary();
    envelopeLibrary->loadLibraryNewFormat((char*)fileString.c_str());
    std::string deleteCommand = "rm " + fileString;
    system(deleteCommand.c_str());

    EnvelopeLibraryEntry* previousEntry = NULL;
    Envelope* thisEnvelope;

    for (int i = 1; i <= envelopeLibrary->size(); i ++){
        thisEnvelope = envelopeLibrary->getEnvelope(i);
        EnvelopeLibraryEntry* thisEntry = new EnvelopeLibraryEntry(thisEnvelope, i);
        delete thisEnvelope;

        if (previousEntry ==NULL){
            p->elentry = thisEntry;
            previousEntry = thisEntry;
            thisEntry->prev = NULL;
        }else{
            previousEntry->next = thisEntry;
            thisEntry->prev = previousEntry;
        }
        previousEntry = thisEntry;
    }

    delete envelopeLibrary;
#endif
    qDebug() << "Passed envelopes";

    DOMElement* currentElement = envlibelement;

    DOMElement* markovModelLibraryElement = envlibelement->getNextElementSibling();
// #ifdef MARKOV
//     std::string tagName = buffer = XMLString::transcode(markovModelLibraryElement->getTagName());
//     XMLString::release(&buffer);
//     if (tagName == "MarkovModelLibrary") {
//         currentElement = markovModelLibraryElement;
//         DOMText* text = dynamic_cast<DOMText*>(markovModelLibraryElement->getFirstChild());
//         std::string data = buffer = XMLString::transcode(text->getWholeText());
//         XMLString::release(&buffer);

//         // QTextStream ts(QString::fromStdString(data));

//         // // read the number of models
//         // int size;
//         // ts >> size;
//         // p->markovModels.resize(size);

//         // // read individual models
//         // std::string modelText;
//         // QString line;
//         // getline(ts, line, '\n');
//         // for (int i = 0; i < size; i++) {
//         //     line = ts.readLine();
//         //     modelText = line + '\n';
//         //     for (int j = 0; j < 1; j++){
//         //         line = ts.readLine();
//         //         modelText += line + '\n';
//         //     }
//         //     line = ts.readLine();
//         //     modelText += line;
//         std::stringstream ss(data);

//         // read the number of models
//         long long size;
//         ss >> size;
//         p->markovModels.resize(size);

//         // read individual models
//         std::string modelText, line;
//         getline(ss, line, '\n');
//         for (int i = 0; i < size; i++) {
//             getline(ss, line, '\n');
//             modelText = line + '\n';
//             getline(ss, line, '\n');
//             modelText += line + '\n';
//             getline(ss, line, '\n');
//             modelText += line + '\n';
//             getline(ss, line, '\n');
//             modelText += line;

//             (p->markovModels[i]).from_str(modelText);
//         }
//     }
// #endif
    qDebug() << "Skipped markov";

#define EVENTS
#ifdef EVENTS
    DOMElement *domEvents = currentElement->getNextElementSibling();
    DOMElement *eventElement = domEvents->getFirstElementChild();

    while (eventElement != NULL){
        p->parseEvents(eventElement);
        eventElement = eventElement->getNextElementSibling();
    }

    // auto eventsIter = p->events.begin();
    // for (; eventsIter != p->events.end(); ++eventsIter){
    //     (*eventsIter)->link(p);
    // }
#endif
    qDebug() << "Passed events";
}


/** the empty constructor for a NEW project, will return a ProjectView  **/
    
    // filepath = "";
    // project_title = "Untitled-" + std::to_string(counter);
    // file_flag = "";
    // duration = "";
    // num_channels = "2";
    // sample_rate = std::to_string(SAMPLING_RATE);
    // sample_size = "16";
    // num_threads = "1";
    // num_staffs = "1";
    // top_event_num = "0";

    // synthesis = true;
    // score = false;
    // grand_staff = false;

    // topwin = NULL;
    // highwin = NULL;
    // midwin = NULL;
    // lowwin = NULL;
    // bottomwin = NULL;
    // spectrumwin = NULL;
    // envwin = NULL;
    // sievewin = NULL;
    // spatialwin = NULL;
    // patternwin = NULL;
    // reverbwin = NULL;
    // notewin = NULL;
    // filterwin = NULL;
    // measurewin = NULL;
    // env_lib_entries = NULL;

Project::Project(const QString& _title, const QByteArray& _id){
    if(_title.isEmpty()){
        title = tr("Untitled");
    }else{
        title = _title;
    }
#ifdef TABEDITOR
    if(_id.isEmpty())
        id = QUuid::createUuid().toString().toLatin1();
    else
        id = _id;
#endif
}

/* create a most barebones Project object: just the title and the UUID. Add it to the hash! */
Project* ProjectManager::create(const QString& title, const QByteArray& id){
    Project *project = new Project(title, id);
#ifdef TABEDITOR
    project_hash_.insert(project->id, project);
#endif

    return project;
}

Project* ProjectManager::open(const QString& filepath, const QByteArray& id){
    QFileInfo info(filepath);
    QString cpath = info.canonicalFilePath();
    info.setFile(cpath);

    if(cpath.isEmpty() == true){
        MainWindow::instance()->showStatusMessage(QStringLiteral("Cannot open file: file does not exist at %s\n").arg(filepath));
        return 0;
    }

    QFile file(cpath);
    if(file.open(QIODevice::ReadWrite) == false){
        MainWindow::instance()->showStatusMessage("Cannot open file: cannot read and write\n");
        return 0;
    }
    file.close();

    Project *project = create(info.fileName(), id);
    QFileInfo fileinfo(filepath);
    project->fileinfo = fileinfo;
    project->dat_path = fileinfo.absolutePath();
    project->lib_path = fileinfo.absoluteFilePath();

    curr_project_ = project;

    qDebug() << "Now parsing " << filepath;
    parse(project, filepath);
    
    return project;
}

