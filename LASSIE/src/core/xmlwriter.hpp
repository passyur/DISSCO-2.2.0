#ifndef XMLWRITER_HPP
#define XMLWRITER_HPP

#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QRegularExpression>
#include <QMap>

#include "project_struct.hpp"
#include "EnvelopeLibraryEntry.hpp"

namespace XmlWriter {

inline void writeInlineXml(QXmlStreamWriter& xmlWriter, const QString& xmlString) {
    QDomDocument tempDoc;
    if (tempDoc.setContent(xmlString.trimmed()) && !tempDoc.documentElement().isNull()) {
        // Write raw XML
        QString str = tempDoc.toString();
        str.remove(QRegularExpression("[\\n\\t\\r]+"));
        str.replace(QRegularExpression(">\\s+<"), "><");
        xmlWriter.writeCharacters("");
        xmlWriter.device()->write(str.toUtf8());
    } else {
        // String/Number
        xmlWriter.writeCharacters(xmlString);
    }
}

inline void writeProject(QXmlStreamWriter& xmlWriter, ProjectManager* pm, const QMap<QString, bool>& default_note_modifiers) {
    xmlWriter.writeStartDocument("1.0");
    xmlWriter.writeStartElement("ProjectRoot");
        xmlWriter.writeStartElement("ProjectConfiguration");

            xmlWriter.writeStartElement("Title");
                xmlWriter.writeCharacters(pm->title());
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("FileFlag");
                xmlWriter.writeCharacters(pm->fileflag());
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("TopEvent");
                xmlWriter.writeCharacters(pm->topevent().name);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("PieceStartTime");
                xmlWriter.writeCharacters(pm->starttime());
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("Duration");
                writeInlineXml(xmlWriter, pm->duration());
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("Synthesis");
                xmlWriter.writeCharacters(pm->synthesis() ? "True" : "False");
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("Score");
                xmlWriter.writeCharacters(pm->score() ? "True" : "False");
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("GrandStaff");
                xmlWriter.writeCharacters(pm->grandstaff() ? "True" : "False");
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("NumberOfStaff");
                xmlWriter.writeCharacters(pm->numstaffs());
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("NumberOfChannels");
                xmlWriter.writeCharacters(pm->numchannels());
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("SampleRate");
                xmlWriter.writeCharacters(pm->samplerate());
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("SampleSize");
                xmlWriter.writeCharacters(pm->samplesize());
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("NumberOfThreads");
                xmlWriter.writeCharacters(pm->numthreads());
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("OutputParticel");
                xmlWriter.writeCharacters(pm->outputparticel() ? "True" : "False");
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("Seed");
                xmlWriter.writeCharacters(pm->seed());
            xmlWriter.writeEndElement();

        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("NoteModifiers");
            xmlWriter.writeStartElement("DefaultModifiers");
                auto iter = default_note_modifiers.begin();
                while (iter != default_note_modifiers.end()){
                    if ((*iter)) { xmlWriter.writeCharacters("1"); }
                    else { xmlWriter.writeCharacters("0"); }
                    iter++;
                    if (iter != default_note_modifiers.end()){
                        xmlWriter.writeCharacters(", ");
                    }
                }
            xmlWriter.writeEndElement();
            /* STILL IN PROGRESS  */
            xmlWriter.writeStartElement("CustomModifiers");
                xmlWriter.writeCharacters("");
                if (pm->customnotemodifiers().size() != 0){
                    auto iter2 = pm->customnotemodifiers().begin();
                    while (iter2 != pm->customnotemodifiers().end()){
                        xmlWriter.writeStartElement("Modifier");
                        xmlWriter.writeCharacters(*iter2);
                        xmlWriter.writeEndElement();
                        iter2++;
                    }
                }
            xmlWriter.writeEndElement();
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("EnvelopeLibrary");
            QString stringBuffer;
            if (pm->envlibentries() != NULL){
                EnvelopeLibraryEntry* envLib = pm->envlibentries();
                int count = envLib->count();
                stringBuffer = stringBuffer + "\n" + QString::number(count) + "\n";
                count = 1;
                while (envLib != NULL) {
                    stringBuffer = stringBuffer + "Envelope " + QString::number(count) + "\n";
                    int lineNumber = envLib->head->countNumOfNodes();
                    stringBuffer = stringBuffer + QString::number(lineNumber) + "\n";

                    EnvLibEntryNode* currentNode;
                    EnvLibEntrySeg* libSeg = envLib->head->rightSeg;
                    while (libSeg != NULL){
                        currentNode = libSeg->leftNode;
                        stringBuffer = stringBuffer + QString::number(currentNode->x, 'f', 3);
                        stringBuffer = stringBuffer + "     ";
                        stringBuffer = stringBuffer + QString::number(currentNode->y, 'f', 3);
                        stringBuffer = stringBuffer + "     ";

                        if (libSeg->segmentType == envSegmentTypeLinear){
                            stringBuffer = stringBuffer + "LINEAR              ";
                        } else if (libSeg->segmentType == envSegmentTypeExponential){
                            stringBuffer = stringBuffer + "EXPONENTIAL         ";
                        } else {
                            stringBuffer = stringBuffer + "CUBIC_SPLINE        ";
                        }

                        if (libSeg->segmentProperty == envSegmentPropertyFlexible){
                            stringBuffer = stringBuffer + "FLEXIBLE    ";
                        } else {
                            stringBuffer = stringBuffer + "FIXED       ";
                        }

                        stringBuffer = stringBuffer + QString::number((libSeg->rightNode->x) - (currentNode->x), 'f', 3) + "\n";
                        libSeg = libSeg->rightNode->rightSeg;
                    }

                    currentNode = currentNode->rightSeg->rightNode;
                    stringBuffer = stringBuffer + QString::number(currentNode->x, 'f', 3) + "     ";
                    stringBuffer = stringBuffer + QString::number(currentNode->y, 'f', 3) + "\n";

                    count++;
                    envLib = envLib->next;
                }
                stringBuffer = stringBuffer + "    ";
            }
            xmlWriter.writeCharacters(stringBuffer);
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("MarkovModelLibrary");
            /* STILL IN PROGRESS  */
            //nhi: proper Markov model handling with attributes
            xmlWriter.writeCharacters("");
            for (int i = 0; i < pm->markovmodels().size(); ++i) {
                xmlWriter.writeStartElement("MarkovModel");
                xmlWriter.writeAttribute("id", QString::number(i));
                xmlWriter.writeAttribute("name", QString("Markov Model %1").arg(i));
                // Add Markov model data here when available
                xmlWriter.writeEndElement();
            }
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("Events");
            /* STILL IN PROGRESS  */
            /* really ugly to have this be copied when it should be a const reference & implicitly shared, but I'm still mulling how to const-initialize this with the contents of 4 QLists.. -jacob */
            QList<HEvent> pHevents;
            // populate vector with all HEvent subcategories
            pHevents.append(pm->topevent());
            pHevents.append(pm->highevents());
            pHevents.append(pm->midevents());
            pHevents.append(pm->lowevents());

            for (const HEvent& item : pHevents) {
              xmlWriter.writeStartElement("Event");
              xmlWriter.writeAttribute("orderInPalette", item.orderinpalette);
                xmlWriter.writeStartElement("EventType");
                    xmlWriter.writeCharacters(QString("%1").arg(item.type));
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Name");
                    xmlWriter.writeCharacters(item.name);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("MaxChildDuration");
                    xmlWriter.writeCharacters(item.max_child_duration);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("EDUPerBeat");
                    writeInlineXml(xmlWriter, item.edu_perbeat);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("TimeSignature");
                    xmlWriter.writeStartElement("Entry1");
                        xmlWriter.writeCharacters(QString("%1").arg(item.timesig.bar_value));
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry2");
                        xmlWriter.writeCharacters(QString("%1").arg(item.timesig.note_value));
                    xmlWriter.writeEndElement();
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Tempo");
                    xmlWriter.writeStartElement("MethodFlag");
                        xmlWriter.writeCharacters(QString("%1").arg(item.tempo.method_flag));
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Prefix");
                        xmlWriter.writeCharacters(QString("%1").arg(item.tempo.prefix));
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("NoteValue");
                        xmlWriter.writeCharacters(QString("%1").arg(item.tempo.note_value));
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("FractionEntry1");
                        xmlWriter.writeCharacters(item.tempo.frentry_1);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("FractionEntry2");
                        xmlWriter.writeCharacters(item.tempo.frentry_2);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("ValueEntry");
                        xmlWriter.writeCharacters(item.tempo.valentry);
                    xmlWriter.writeEndElement();
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("NumberOfChildren");
                    xmlWriter.writeStartElement("MethodFlag");
                        xmlWriter.writeCharacters(QString("%1").arg(item.numchildren.method_flag));
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry1");
                        writeInlineXml(xmlWriter, item.numchildren.entry_1);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry2");
                        writeInlineXml(xmlWriter, item.numchildren.entry_2);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry3");
                        writeInlineXml(xmlWriter, item.numchildren.entry_3);
                    xmlWriter.writeEndElement();
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("ChildEventDefinition");
                    xmlWriter.writeStartElement("Entry1");
                        writeInlineXml(xmlWriter, item.child_event_def.entry_1);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry2");
                        writeInlineXml(xmlWriter, item.child_event_def.entry_2);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry3");
                        writeInlineXml(xmlWriter, item.child_event_def.entry_3);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("AttackSieve");
                        xmlWriter.writeCharacters(item.child_event_def.attack_sieve);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("DurationSieve");
                        xmlWriter.writeCharacters(item.child_event_def.duration_sieve);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("DefinitionFlag");
                        xmlWriter.writeCharacters(QString("%1").arg(item.child_event_def.definition_flag));
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("StartTypeFlag");
                        xmlWriter.writeCharacters(QString("%1").arg(item.child_event_def.starttype_flag));
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("DurationTypeFlag");
                        xmlWriter.writeCharacters(QString("%1").arg(item.child_event_def.durationtype_flag));
                    xmlWriter.writeEndElement();
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Layers");
                for (const Layer& itemLayer : item.event_layers) {
                    xmlWriter.writeStartElement("Layer");
                        xmlWriter.writeStartElement("ByLayer");
                            xmlWriter.writeCharacters(itemLayer.by_layer);
                        xmlWriter.writeEndElement();
                        xmlWriter.writeStartElement("DiscretePackages");
                        for (Package layerPkg : itemLayer.discrete_packages) {
                            xmlWriter.writeStartElement("Package");
                                xmlWriter.writeStartElement("EventName");
                                    xmlWriter.writeCharacters(layerPkg.event_name);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("EventType");
                                    xmlWriter.writeCharacters(layerPkg.event_type);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("Weight");
                                    xmlWriter.writeCharacters(layerPkg.weight);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("AttackEnvelope");
                                    xmlWriter.writeCharacters(layerPkg.attack_envelope);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("AttackEnvelopeScale");
                                    xmlWriter.writeCharacters(layerPkg.attackenvelope_scale);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("DurationEnvelope");
                                    xmlWriter.writeCharacters(layerPkg.duration_envelope);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("DurationEnvelopeScale");
                                    xmlWriter.writeCharacters(layerPkg.durationenvelope_scale);
                                xmlWriter.writeEndElement();
                            xmlWriter.writeEndElement();
                        }
                        xmlWriter.writeEndElement();
                    xmlWriter.writeEndElement();
                }
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Spatialization");
                        xmlWriter.writeCharacters(item.spa);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Reverb");
                        xmlWriter.writeCharacters(item.reverb);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Filter");
                        xmlWriter.writeCharacters(item.filter);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Modifiers");
                    for (const Modifier& itemMod : item.modifiers) {
                        xmlWriter.writeStartElement("Modifier");
                            xmlWriter.writeStartElement("Type");
                                xmlWriter.writeCharacters(QString("%1").arg(itemMod.type));
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("ApplyHow");
                                xmlWriter.writeCharacters(itemMod.applyhow_flag ? "0" : "1");
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("Probability");
                                writeInlineXml(xmlWriter, itemMod.probability);
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("Amplitude");
                                writeInlineXml(xmlWriter, itemMod.amplitude);
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("Rate");
                                writeInlineXml(xmlWriter, itemMod.rate);
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("Width");
                                writeInlineXml(xmlWriter, itemMod.width);
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("DetuneSpread");
                                xmlWriter.writeCharacters(itemMod.detune_spread);
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("DetuneDirection");
                                xmlWriter.writeCharacters(itemMod.detune_direction);
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("DetuneVelocity");
                                xmlWriter.writeCharacters(itemMod.detune_velocity);
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("GroupName");
                                xmlWriter.writeCharacters(itemMod.group_name);
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("PartialResultString");
                                writeInlineXml(xmlWriter, itemMod.partialresult_string);
                            xmlWriter.writeEndElement();
                        xmlWriter.writeEndElement();
                    }
                    xmlWriter.writeEndElement();
              xmlWriter.writeEndElement();
            }

            const QList<BottomEvent>& pBevents = pm->bottomevents();
            for (const BottomEvent& item : pBevents) {
              xmlWriter.writeStartElement("Event");
              xmlWriter.writeAttribute("orderInPalette", item.event.orderinpalette);
                xmlWriter.writeStartElement("EventType");
                    xmlWriter.writeCharacters(QString("%1").arg(item.event.type));
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Name");
                    xmlWriter.writeCharacters(item.event.name);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("MaxChildDuration");
                    writeInlineXml(xmlWriter, item.event.max_child_duration);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("EDUPerBeat");
                    xmlWriter.writeCharacters(item.event.edu_perbeat);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("TimeSignature");
                    xmlWriter.writeStartElement("Entry1");
                        xmlWriter.writeCharacters(item.event.timesig.bar_value);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry2");
                        xmlWriter.writeCharacters(item.event.timesig.note_value);
                    xmlWriter.writeEndElement();
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Tempo");
                    xmlWriter.writeStartElement("MethodFlag");
                        xmlWriter.writeCharacters(QString("%1").arg(item.event.tempo.method_flag));
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Prefix");
                        xmlWriter.writeCharacters(QString("%1").arg(item.event.tempo.prefix));
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("NoteValue");
                        xmlWriter.writeCharacters(QString("%1").arg(item.event.tempo.note_value));
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("FractionEntry1");
                        xmlWriter.writeCharacters(item.event.tempo.frentry_1);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("FractionEntry2");
                        xmlWriter.writeCharacters(item.event.tempo.frentry_2);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("ValueEntry");
                        xmlWriter.writeCharacters(item.event.tempo.valentry);
                    xmlWriter.writeEndElement();
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("NumberOfChildren");
                    xmlWriter.writeStartElement("MethodFlag");
                        xmlWriter.writeCharacters(QString("%1").arg(item.event.numchildren.method_flag));
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry1");
                        writeInlineXml(xmlWriter, item.event.numchildren.entry_1);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry2");
                        writeInlineXml(xmlWriter, item.event.numchildren.entry_2);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry3");
                        writeInlineXml(xmlWriter, item.event.numchildren.entry_3);
                    xmlWriter.writeEndElement();
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("ChildEventDefinition");
                    xmlWriter.writeStartElement("Entry1");
                        writeInlineXml(xmlWriter, item.event.child_event_def.entry_1);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry2");
                        writeInlineXml(xmlWriter, item.event.child_event_def.entry_2);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry3");
                        writeInlineXml(xmlWriter, item.event.child_event_def.entry_3);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("AttackSieve");
                        xmlWriter.writeCharacters(item.event.child_event_def.attack_sieve);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("DurationSieve");
                        xmlWriter.writeCharacters(item.event.child_event_def.duration_sieve);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("DefinitionFlag");
                        xmlWriter.writeCharacters(QString("%1").arg(item.event.child_event_def.definition_flag));
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("StartTypeFlag");
                        xmlWriter.writeCharacters(QString("%1").arg(item.event.child_event_def.starttype_flag));
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("DurationTypeFlag");
                        xmlWriter.writeCharacters(QString("%1").arg(item.event.child_event_def.durationtype_flag));
                    xmlWriter.writeEndElement();
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Layers");
                for (const Layer& itemLayer : item.event.event_layers) {
                    xmlWriter.writeStartElement("Layer");
                        xmlWriter.writeStartElement("ByLayer");
                            xmlWriter.writeCharacters(itemLayer.by_layer);
                        xmlWriter.writeEndElement();
                        xmlWriter.writeStartElement("DiscretePackages");
                        for (const Package& layerPkg : itemLayer.discrete_packages) {
                            xmlWriter.writeStartElement("Package");
                                xmlWriter.writeStartElement("EventName");
                                    xmlWriter.writeCharacters(layerPkg.event_name);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("EventType");
                                    xmlWriter.writeCharacters(layerPkg.event_type);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("Weight");
                                    xmlWriter.writeCharacters(layerPkg.weight);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("AttackEnvelope");
                                    xmlWriter.writeCharacters(layerPkg.attack_envelope);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("AttackEnvelopeScale");
                                    xmlWriter.writeCharacters(layerPkg.attackenvelope_scale);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("DurationEnvelope");
                                    xmlWriter.writeCharacters(layerPkg.duration_envelope);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("DurationEnvelopeScale");
                                    xmlWriter.writeCharacters(layerPkg.durationenvelope_scale);
                                xmlWriter.writeEndElement();
                            xmlWriter.writeEndElement();
                        }
                        xmlWriter.writeEndElement();
                    xmlWriter.writeEndElement();
                }
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Spatialization");
                        writeInlineXml(xmlWriter, item.event.spa);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Reverb");
                        writeInlineXml(xmlWriter, item.event.reverb);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Filter");
                        writeInlineXml(xmlWriter, item.event.filter);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("ExtraInfo");
                        xmlWriter.writeStartElement("FrequencyInfo");
                            xmlWriter.writeStartElement("FrequencyFlag");
                                xmlWriter.writeCharacters(QString("%1").arg(item.extra_info.freq_info.freq_flag));
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("FrequencyContinuumFlag");
                                xmlWriter.writeCharacters(QString("%1").arg(item.extra_info.freq_info.continuum_flag));
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("FrequencyEntry1");
                                writeInlineXml(xmlWriter, item.extra_info.freq_info.entry_1);
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("FrequencyEntry2");
                                writeInlineXml(xmlWriter, item.extra_info.freq_info.entry_2);
                            xmlWriter.writeEndElement();
                        xmlWriter.writeEndElement();
                        xmlWriter.writeStartElement("Loudness");
                            writeInlineXml(xmlWriter, item.extra_info.loudness);
                        xmlWriter.writeEndElement();
                        xmlWriter.writeStartElement("Spatialization");
                            writeInlineXml(xmlWriter, item.extra_info.spa);
                        xmlWriter.writeEndElement();
                        xmlWriter.writeStartElement("Reverb");
                            writeInlineXml(xmlWriter, item.extra_info.reverb);
                        xmlWriter.writeEndElement();
                        xmlWriter.writeStartElement("Filter");
                            writeInlineXml(xmlWriter, item.extra_info.filter);
                        xmlWriter.writeEndElement();
                        xmlWriter.writeStartElement("ModifierGroup");
                            writeInlineXml(xmlWriter, item.extra_info.modifier_group);
                        xmlWriter.writeEndElement();
                        xmlWriter.writeStartElement("Modifiers");
                        for (const Modifier& itemMod : item.extra_info.modifiers) {
                            xmlWriter.writeStartElement("Modifier");
                                xmlWriter.writeStartElement("Type");
                                    xmlWriter.writeCharacters(QString("%1").arg(itemMod.type));
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("ApplyHow");
                                    xmlWriter.writeCharacters(itemMod.applyhow_flag ? "0" : "1");
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("Probability");
                                    writeInlineXml(xmlWriter, itemMod.probability);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("Amplitude");
                                    writeInlineXml(xmlWriter, itemMod.amplitude);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("Rate");
                                    writeInlineXml(xmlWriter, itemMod.rate);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("Width");
                                    writeInlineXml(xmlWriter, itemMod.width);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("DetuneSpread");
                                    xmlWriter.writeCharacters(itemMod.detune_spread);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("DetuneDirection");
                                    xmlWriter.writeCharacters(itemMod.detune_direction);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("DetuneVelocity");
                                    xmlWriter.writeCharacters(itemMod.detune_velocity);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("GroupName");
                                    xmlWriter.writeCharacters(itemMod.group_name);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("PartialResultString");
                                    writeInlineXml(xmlWriter, itemMod.partialresult_string);
                                xmlWriter.writeEndElement();
                            xmlWriter.writeEndElement();
                        }
                        xmlWriter.writeEndElement();
                    xmlWriter.writeEndElement();
              xmlWriter.writeEndElement();
            }

            for (const SpectrumEvent& item : pm->spectrumevents()) {
              xmlWriter.writeStartElement("Event");
              xmlWriter.writeAttribute("orderInPalette", item.orderinpalette);
                xmlWriter.writeStartElement("EventType");
                    xmlWriter.writeCharacters(QString("%1").arg(sound));
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Name");
                    xmlWriter.writeCharacters(item.name);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("NumberOfPartials");
                    xmlWriter.writeCharacters(item.num_partials);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Deviation");
                    xmlWriter.writeCharacters(item.deviation);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("GenerateSpectrum");
                    xmlWriter.writeCharacters(item.generate_spectrum);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Spectrum");
                    for (QString partial : item.spectrum.partials) {
                        xmlWriter.writeStartElement("Partial");
                            writeInlineXml(xmlWriter, partial);
                        xmlWriter.writeEndElement();
                    }
                xmlWriter.writeEndElement();
              xmlWriter.writeEndElement();
            }

            const QList<NoteEvent>& pNevents = pm->noteevents();
            for (const NoteEvent& item : pNevents) {
              xmlWriter.writeStartElement("Event");
              xmlWriter.writeAttribute("orderInPalette", item.orderinpalette);
                xmlWriter.writeStartElement("EventType");
                    xmlWriter.writeCharacters(QString("%1").arg(note));
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Name");
                    xmlWriter.writeCharacters(item.name);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("NoteInfo");
                    xmlWriter.writeStartElement("Staffs");
                        xmlWriter.writeCharacters(item.note_info.staffs);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Modifiers");
                    for (QString modifier : item.note_info.modifiers) {
                        xmlWriter.writeStartElement("Modifier");
                            writeInlineXml(xmlWriter, modifier);
                        xmlWriter.writeEndElement();
                    }
                    xmlWriter.writeEndElement();
                xmlWriter.writeEndElement();
              xmlWriter.writeEndElement();
            }

            const QList<EnvelopeEvent>& pEevents = pm->envelopeevents();
            for (const EnvelopeEvent& item : pEevents) {
              xmlWriter.writeStartElement("Event");
              xmlWriter.writeAttribute("orderInPalette", item.orderinpalette);
                xmlWriter.writeStartElement("EventType");
                    xmlWriter.writeCharacters(QString("%1").arg(env));
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Name");
                    xmlWriter.writeCharacters(item.name);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("EnvelopeBuilder");
                    writeInlineXml(xmlWriter, item.envelope_builder);
                xmlWriter.writeEndElement();
              xmlWriter.writeEndElement();
            }

            const QList<SieveEvent>& pSivevents = pm->sieveevents();
            for (const SieveEvent& item : pSivevents) {
              xmlWriter.writeStartElement("Event");
              xmlWriter.writeAttribute("orderInPalette", item.orderinpalette);
                xmlWriter.writeStartElement("EventType");
                    xmlWriter.writeCharacters(QString("%1").arg(sieve));
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Name");
                    xmlWriter.writeCharacters(item.name);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("SieveBuilder");
                    writeInlineXml(xmlWriter, item.sieve_builder);
                xmlWriter.writeEndElement();
              xmlWriter.writeEndElement();
            }

            const QList<SpaEvent>& pSpaevents = pm->spaevents();
            for (const SpaEvent& item : pSpaevents) {
              xmlWriter.writeStartElement("Event");
              xmlWriter.writeAttribute("orderInPalette", item.orderinpalette);
                xmlWriter.writeStartElement("EventType");
                    xmlWriter.writeCharacters(QString("%1").arg(spa));
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Name");
                    xmlWriter.writeCharacters(item.name);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Spatialization");
                    writeInlineXml(xmlWriter, item.spatialization);
                xmlWriter.writeEndElement();
              xmlWriter.writeEndElement();
            }

            const QList<PatternEvent>& pPevents = pm->patternevents();
            for (const PatternEvent& item : pPevents) {
              xmlWriter.writeStartElement("Event");
              xmlWriter.writeAttribute("orderInPalette", item.orderinpalette);
                xmlWriter.writeStartElement("EventType");
                    xmlWriter.writeCharacters(QString("%1").arg(pattern));
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Name");
                    xmlWriter.writeCharacters(item.name);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("PatternBuilder");
                    writeInlineXml(xmlWriter, item.pattern_builder);
                xmlWriter.writeEndElement();
              xmlWriter.writeEndElement();
            }

            const QList<ReverbEvent>& pRevents = pm->reverbevents();
            for (const ReverbEvent& item : pRevents) {
              xmlWriter.writeStartElement("Event");
              xmlWriter.writeAttribute("orderInPalette", item.orderinpalette);
                xmlWriter.writeStartElement("EventType");
                    xmlWriter.writeCharacters(QString("%1").arg(reverb));
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Name");
                    xmlWriter.writeCharacters(item.name);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Reverberation");
                    writeInlineXml(xmlWriter, item.reverberation);
                xmlWriter.writeEndElement();
              xmlWriter.writeEndElement();
            }

            const QList<FilterEvent>& pFevents = pm->filterevents();
            for (const FilterEvent& item : pFevents) {
              xmlWriter.writeStartElement("Event");
              xmlWriter.writeAttribute("orderInPalette", item.orderinpalette);
                xmlWriter.writeStartElement("EventType");
                    xmlWriter.writeCharacters(QString("%1").arg(filter));
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Name");
                    xmlWriter.writeCharacters(item.name);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("FilterBuilder");
                    writeInlineXml(xmlWriter, item.filter_builder);
                xmlWriter.writeEndElement();
              xmlWriter.writeEndElement();
            }

        xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
}

} // namespace XmlWriter

#endif // XMLWRITER_HPP
