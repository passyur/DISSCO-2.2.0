/* The 'project' object that keeps track of project details to deploy
 * in the associated window (currently, the project view).
 *
 * Copyright (c) 2025, DISSCO authors */

#include <QFile>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QIODevice>
#include <QXmlStreamWriter>
#include <QDebug>
#include <QTextStream>
#include <QDomDocument>

#include <QDialog>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QMessageBox>
#include <QScrollArea>
#include <QTreeView>
#include <QStandardItem>

#include "ProjectViewController.hpp"
#include "MainWindow.hpp"
#include "../ui/ui_mainwindow.h"
#include "../inst.hpp"
#include "../core/EnvelopeLibraryEntry.hpp"
#include "ProjectPropertiesDialog.hpp"
#include "../ui/ui_ProjectPropertiesDialog.h"
#include "FunctionGenerator.hpp"
#include "../ui/ui_FunctionGenerator.h"
#include "ObjectWindow.hpp"
#include "PaletteViewController.hpp"
#include "../core/event_struct.hpp"
#include "EventAttributesViewController.hpp"
#include "FileNewObject.hpp"
#include "../ui/ui_FileNewObject.h"
// #include "../core/IEvent.h"
// #include "muParser.h"

#include "../utilities.hpp"

namespace PVCHelper {
    /* called before appearing (see immediately succeeding f'n) the checkboxes; init'd when a user brings up the configure note modifier menu */
    // void initializeNoteModifierButton(QMap<QString, bool>& default_note_modifiers, QCheckBox& checkbox, QString key_name, QString button_name){
    //     note_modifiers_configuration_dialog_ref_builder->get_widget(button_name, checkbox);
    //     /* let the compiler do this, branchless */
    //     checkbox->set_active(default_note_modifiers[key_name]);
    // }

    // void showNoteModifierButton(QMap<QString, bool>& default_note_modifiers, QCheckBox& checkbox, QString key_name, QString button_name){
    //     note_modifiers_configuration_dialog_ref_builder->get_widget(button_name, checkbox);
    //     default_note_modifiers[key_name] = checkbox->get_active();
    // }
}
 
// unsigned int counter = 1;
/** the empty constructor for a NEW project, will return a ProjectView  **/
// ProjectView::ProjectView(){
//     std::string top_name = "0";
    
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
// 
    // 
// }

/* ProjectView constructor initializing values for XML file*/
ProjectView::ProjectView(MainWindow* _mainWindow, QString _pathAndName) {

    ProjectManager *pm = Inst::get_project_manager();
    qDebug() << "In PV Constructor p:" << pm->get_curr_project();

    mainWindow = _mainWindow;
    modifiedButNotSaved = true;

    // // Initialize PaletteViewController
    paletteView = new PaletteViewController(this);
    mainWindow->ui->paletteLayout->insertWidget(0, paletteView);

    // Initialize Events Attribute View Controller
    mainWindow->ui->eventsScrollArea->setWidgetResizable(true);
    eventAttributesView = new EventAttributesViewController(this);
    mainWindow->ui->eventsScrollArea->setWidget(eventAttributesView);

    // // Create ObjectWindow instances
    topWindow = new ObjectWindow(eventTop, this);
    highWindow = new ObjectWindow(eventHigh, this);
    midWindow = new ObjectWindow(eventMid, this);
    lowWindow = new ObjectWindow(eventLow, this);
    bottomWindow = new ObjectWindow(eventBottom, this);
    spectrumWindow = new ObjectWindow(eventSound, this);
    envWindow = new ObjectWindow(eventEnv, this);
    sivWindow = new ObjectWindow(eventSiv, this);
    spaWindow = new ObjectWindow(eventSpa, this);
    patWindow = new ObjectWindow(eventPat, this);
    revWindow = new ObjectWindow(eventRev, this);
    filWindow = new ObjectWindow(eventFil, this);
    noteWindow = new ObjectWindow(eventNote, this);
    meaWindow = new ObjectWindow(eventMea, this);

    updatePaletteView();
}

// Function to write XML Formatting
void ProjectView::writeInlineXml(QXmlStreamWriter& xmlWriter, QString& xmlString) {
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

/* Function that creates and saves the xml .dissco file */
void ProjectView::save(){
    qDebug() << "In Project View Save Function";

    ProjectManager *pm = Inst::get_project_manager();
    modifiedButNotSaved = false; // changes bool value because file is being saved

    // ensure directory exists before creating file
    QFileInfo fileInfo = pm->fileinfo();
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qDebug() << "Failed to create directory:" << dir.absolutePath();
            return;
        }
    }

    // creates the file with the specified /path/name.dissco
    QFile file(pm->libpath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << file.fileName();
        qDebug() << "Error reason:" << file.errorString();
        return;
    }
    
    // QXmlStreamWriter class writes to the XML file
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
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
                xmlWriter.writeCharacters(pm->topevent());
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("PieceStartTime");	
                xmlWriter.writeCharacters("0");
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
                    if (iter!= default_note_modifiers.end()){
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
                        iter2 ++;  
                    }
                }
            xmlWriter.writeEndElement();       
        xmlWriter.writeEndElement(); 

        xmlWriter.writeStartElement("EnvelopeLibrary");
            QString stringBuffer = "";
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
            }
            stringBuffer = stringBuffer + "    ";
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
            QList<HEvent>& pHevents = pm->hevents();
            xmlWriter.writeStartElement("Event");
            for (HEvent& item : pHevents) {
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
                    xmlWriter.writeCharacters(item.edu_perbeat);	
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
                        xmlWriter.writeCharacters(item.numchildren.entry_1);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry2");
                        xmlWriter.writeCharacters(item.numchildren.entry_2);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry3");
                        xmlWriter.writeCharacters(item.numchildren.entry_3);
                    xmlWriter.writeEndElement();
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("ChildEventDefinition");
                    xmlWriter.writeStartElement("Entry1");
                        xmlWriter.writeCharacters(item.child_event_def.entry_1);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry2");
                        xmlWriter.writeCharacters(item.child_event_def.entry_2);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry3");
                        xmlWriter.writeCharacters(item.child_event_def.entry_3);
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
                for (Layer itemLayer : item.event_layers) {
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
                                    xmlWriter.writeCharacters(QString("%1").arg(layerPkg.event_type));
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
                    xmlWriter.writeCharacters("");
                    for (Modifier itemMod : item.modifiers) {
                        xmlWriter.writeStartElement("Modifier");
                            xmlWriter.writeStartElement("Type");
                                xmlWriter.writeCharacters(QString("%1").arg(itemMod.type));
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("ApplyHow");
                                xmlWriter.writeCharacters(itemMod.applyhow_flag ? "True" : "False");
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("Probability");
                                xmlWriter.writeCharacters(itemMod.probability);
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("Amplitude");
                                xmlWriter.writeCharacters(itemMod.amplitude);
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("Rate");
                                xmlWriter.writeCharacters(itemMod.rate);
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("Width");
                                xmlWriter.writeCharacters(itemMod.width);
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
                                xmlWriter.writeCharacters(itemMod.partialresult_string);
                            xmlWriter.writeEndElement();
                        xmlWriter.writeEndElement(); 
                    }   
                    xmlWriter.writeEndElement(); 
                
            }
            xmlWriter.writeEndElement();

            /*QList<BottomEvent>& pBevents = pm->bottomevents();
            xmlWriter.writeStartElement("Event");
            for (BottomEvent& item : pBevents) {
              xmlWriter.writeAttribute("orderInPalette", item.event.orderinpalette);	
                xmlWriter.writeStartElement("EventType");
                    xmlWriter.writeCharacters(QString("%1").arg(item.event.type));	
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("Name");
                    xmlWriter.writeCharacters(item.event.name);	
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("MaxChildDuration");
                    xmlWriter.writeCharacters(item.event.max_child_duration);	
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("EDUPerBeat");
                    xmlWriter.writeCharacters(item.event.edu_perbeat);	
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("TimeSignature");
                    xmlWriter.writeStartElement("Entry1");
                        xmlWriter.writeCharacters(QString("%1").arg(item.event.timesig.bar_value));
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry2");
                        xmlWriter.writeCharacters(QString("%1").arg(item.event.timesig.note_value));
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
                        xmlWriter.writeCharacters(item.event.numchildren.entry_1);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry2");
                        xmlWriter.writeCharacters(item.event.numchildren.entry_2);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry3");
                        xmlWriter.writeCharacters(item.event.numchildren.entry_3);
                    xmlWriter.writeEndElement();
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("ChildEventDefinition");
                    xmlWriter.writeStartElement("Entry1");
                        xmlWriter.writeCharacters(item.event.child_event_def.entry_1);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry2");
                        xmlWriter.writeCharacters(item.event.child_event_def.entry_2);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Entry3");
                        xmlWriter.writeCharacters(item.event.child_event_def.entry_3);
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
                for (Layer itemLayer : item.event.event_layers) {
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
                                    xmlWriter.writeCharacters(QString("%1").arg(layerPkg.event_type));
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
                        xmlWriter.writeCharacters(item.event.spa);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Reverb");
                        xmlWriter.writeCharacters(item.event.reverb);
                    xmlWriter.writeEndElement();
                    xmlWriter.writeStartElement("Filter");
                        xmlWriter.writeCharacters(item.event.filter);
                    xmlWriter.writeEndElement(); 
                    xmlWriter.writeStartElement("Modifiers");
                    xmlWriter.writeCharacters("");
                    for (Modifier itemMod : item.event.modifiers) {
                        xmlWriter.writeStartElement("Modifier");
                            xmlWriter.writeStartElement("Type");
                                xmlWriter.writeCharacters(QString("%1").arg(itemMod.type));
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("ApplyHow");
                                xmlWriter.writeCharacters(itemMod.applyhow_flag ? "True" : "False");
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("Probability");
                                xmlWriter.writeCharacters(itemMod.probability);
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("Amplitude");
                                xmlWriter.writeCharacters(itemMod.amplitude);
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("Rate");
                                xmlWriter.writeCharacters(itemMod.rate);
                            xmlWriter.writeEndElement();
                            xmlWriter.writeStartElement("Width");
                                xmlWriter.writeCharacters(itemMod.width);
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
                                xmlWriter.writeCharacters(itemMod.partialresult_string);
                            xmlWriter.writeEndElement();
                        xmlWriter.writeEndElement(); 
                    }  
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
                                xmlWriter.writeCharacters(item.extra_info.freq_info.entry_1);
                            xmlWriter.writeEndElement(); 
                            xmlWriter.writeStartElement("FrequencyEntry2");
                                xmlWriter.writeCharacters(item.extra_info.freq_info.entry_2);
                            xmlWriter.writeEndElement(); 
                        xmlWriter.writeEndElement(); 
                        xmlWriter.writeStartElement("Loudness");
                            xmlWriter.writeCharacters(item.extra_info.loudness);
                        xmlWriter.writeEndElement(); 
                        xmlWriter.writeStartElement("Spatialization");
                            xmlWriter.writeCharacters(item.extra_info.spa);
                        xmlWriter.writeEndElement();
                        xmlWriter.writeStartElement("Reverb");
                            xmlWriter.writeCharacters(item.extra_info.reverb);
                        xmlWriter.writeEndElement(); 
                        xmlWriter.writeStartElement("Filter");
                            xmlWriter.writeCharacters(item.extra_info.filter);
                        xmlWriter.writeEndElement(); 
                        xmlWriter.writeStartElement("ModifierGroup");
                            xmlWriter.writeCharacters(item.extra_info.modifier_group);
                        xmlWriter.writeEndElement(); 
                        xmlWriter.writeStartElement("Modifiers");
                        xmlWriter.writeCharacters("");
                        for (Modifier itemMod : item.extra_info.modifiers) {
                            xmlWriter.writeStartElement("Modifier");
                                xmlWriter.writeStartElement("Type");
                                    xmlWriter.writeCharacters(QString("%1").arg(itemMod.type));
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("ApplyHow");
                                    xmlWriter.writeCharacters(itemMod.applyhow_flag ? "True" : "False");
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("Probability");
                                    xmlWriter.writeCharacters(itemMod.probability);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("Amplitude");
                                    xmlWriter.writeCharacters(itemMod.amplitude);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("Rate");
                                    xmlWriter.writeCharacters(itemMod.rate);
                                xmlWriter.writeEndElement();
                                xmlWriter.writeStartElement("Width");
                                    xmlWriter.writeCharacters(itemMod.width);
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
                                    xmlWriter.writeCharacters(itemMod.partialresult_string);
                                xmlWriter.writeEndElement();
                            xmlWriter.writeEndElement(); 
                        }  
                        xmlWriter.writeEndElement(); 
                    xmlWriter.writeEndElement(); 
                
            }
            xmlWriter.writeEndElement();*/

        xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    file.close();
}

void ProjectView::setProperties() {
    if (!projectPropertiesDialog) {
        projectPropertiesDialog = new ProjectPropertiesDialog(mainWindow);
        connect(projectPropertiesDialog->ui->insertFunctionButton, &QPushButton::clicked, this, &ProjectView::propertiesInsertFunction);
    }

    ProjectManager *pm = Inst::get_project_manager();

    projectPropertiesDialog->ui->titleEntry->setText(pm->title());
    projectPropertiesDialog->ui->flagEntry->setText(pm->fileflag());
    projectPropertiesDialog->ui->numChannelsEntry->setText(pm->numchannels());
    projectPropertiesDialog->ui->rateEntry->setText(pm->samplerate());
    projectPropertiesDialog->ui->sizeEntry->setText(pm->samplesize());
    projectPropertiesDialog->ui->numThreadsEntry->setText(pm->numthreads());
    projectPropertiesDialog->ui->synthesisCheckBox->setCheckState(pm->synthesis() ? Qt::Checked : Qt::Unchecked);
    projectPropertiesDialog->ui->scoreCheckBox->setCheckState(pm->score() ? Qt::Checked : Qt::Unchecked);
    projectPropertiesDialog->ui->staffCheckBox->setCheckState(pm->grandstaff() ? Qt::Checked : Qt::Unchecked);
    projectPropertiesDialog->ui->numStaffEntry->setText(pm->numstaffs());
    projectPropertiesDialog->ui->particelBox->setCheckState(pm->outputparticel() ? Qt::Checked : Qt::Unchecked);
    projectPropertiesDialog->ui->topEventEntry->setText(pm->topevent());
    projectPropertiesDialog->ui->durationEntry->setText(pm->duration());

 
    if (projectPropertiesDialog->exec() == QDialog::Accepted) {
        QString new_title = projectPropertiesDialog->ui->titleEntry->text();
        pm->fileflag() = projectPropertiesDialog->ui->flagEntry->text();
        pm->numchannels() = projectPropertiesDialog->ui->numChannelsEntry->text();
        pm->samplerate() = projectPropertiesDialog->ui->rateEntry->text();
        pm->samplerate() = projectPropertiesDialog->ui->sizeEntry->text();
        pm->numthreads() = projectPropertiesDialog->ui->numThreadsEntry->text();
        pm->synthesis() = projectPropertiesDialog->ui->synthesisCheckBox->isChecked();
        pm->score() = projectPropertiesDialog->ui->scoreCheckBox->isChecked();
        pm->grandstaff() = projectPropertiesDialog->ui->staffCheckBox->isChecked();
        pm->numstaffs() = projectPropertiesDialog->ui->numStaffEntry->text();
        pm->outputparticel() = projectPropertiesDialog->ui->particelBox->isChecked();
        QString new_topevent = projectPropertiesDialog->ui->topEventEntry->text();
        pm->duration() = projectPropertiesDialog->ui->durationEntry->text();

        if (new_title != pm->title()) {
            QString old_pathAndName = pm->libpath();
            QString new_pathAndName = pm->datpath() + "/" + new_title + ".dissco";
            if (QFile::rename(old_pathAndName, new_pathAndName)) {
                pm->libpath() = new_pathAndName;
                pm->title() = new_title;
            }
        }

        if (new_topevent != pm->topevent()) {
            QList<HEvent>& pHevents = pm->hevents();
            for (HEvent &item : pHevents) {
                if (item.name == pm->topevent() && item.type == top) {
                    item.name = new_topevent;
                    break;
                }
            }
            for (int row = 0; row < paletteView->folderTop->rowCount(); row++) {
                QStandardItem* oldTopName = paletteView->folderTop->child(row, 1);
                if (oldTopName && oldTopName->text() == pm->topevent()) {
                    oldTopName->setText(new_topevent);
                    break;
                }
            }
            pm->topevent() = projectPropertiesDialog->ui->topEventEntry->text();
        }

        MUtilities::modified();
        delete projectPropertiesDialog;
        projectPropertiesDialog = NULL;
    }
}

void ProjectView::propertiesInsertFunction() {
    if (!projectPropertiesDialog) return;
    ProjectManager *pm = Inst::get_project_manager();
    FunctionGenerator* generator = new FunctionGenerator(mainWindow, functionReturnFloat, pm->duration());
    if (generator->exec() == QDialog::Accepted) {
        QString result = generator->getResultString();
        if (!result.isEmpty()) {
            ProjectManager *pm = Inst::get_project_manager();
            pm->duration() = result;
            projectPropertiesDialog->ui->durationEntry->setText(pm->duration());
        }
    }
    delete generator;
}

void ProjectView::insertObject() {
    if (!newObject) {
        newObject = new FileNewObject(mainWindow);
    }

    if (newObject->exec() == QDialog::Accepted) {
        ProjectManager *pm = Inst::get_project_manager();
    
        if (newObject->ui->buttonTop->isChecked()) {
            QList<HEvent>& eventList = pm->hevents();
            HEvent newObj = {};
            newObj.type = top;
            newObj.name = newObject->ui->objNameEntry->text();
            eventList.push_back(newObj);

            QStandardItem* newObjectType = new QStandardItem("Top");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderTop->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonHigh->isChecked()) {
            QList<HEvent>& eventList = pm->hevents();
            HEvent newObj = {};
            newObj.type = high;
            newObj.name = newObject->ui->objNameEntry->text();
            eventList.push_back(newObj);

            QStandardItem* newObjectType = new QStandardItem("High");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderHigh->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonMid->isChecked()) {
            QList<HEvent>& eventList = pm->hevents();
            HEvent newObj = {};
            newObj.type = mid;
            newObj.name = newObject->ui->objNameEntry->text();
            eventList.push_back(newObj);

            QStandardItem* newObjectType = new QStandardItem("Mid");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderMid->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonLow->isChecked()) {
            QList<HEvent>& eventList = pm->hevents();
            HEvent newObj = {};
            newObj.type = low;
            newObj.name = newObject->ui->objNameEntry->text();
            eventList.push_back(newObj);

            QStandardItem* newObjectType = new QStandardItem("Low");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderLow->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonBottom->isChecked()) {
            QList<BottomEvent>& eventList = pm->bottomevents();
            BottomEvent newObj = {};
            newObj.event.type = bottom;
            newObj.event.name = newObject->ui->objNameEntry->text();
            eventList.push_back(newObj);

            QStandardItem* newObjectType = new QStandardItem("Bottom");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderBottom->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonSpectrum->isChecked()) {
            QList<SpectrumEvent>& eventList = pm->spectrumevents();
            SpectrumEvent newObj;
            newObj.orderinpalette = QString::number(eventList.size()+1);;
            newObj.name = newObject->ui->objNameEntry->text();
            eventList.push_back(newObj);

            QStandardItem* newObjectType = new QStandardItem("Spectrum");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderSpectrum->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonNote->isChecked()) {
            QList<NoteEvent>& eventList = pm->noteevents();
            NoteEvent newObj = {};
            newObj.orderinpalette = QString::number(eventList.size()+1);;
            newObj.name = newObject->ui->objNameEntry->text();
            eventList.push_back(newObj);

            QStandardItem* newObjectType = new QStandardItem("Note");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderNote->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonEnv->isChecked()) {
            QList<EnvelopeEvent>& eventList = pm->envelopeevents();
            EnvelopeEvent newObj = {};
            newObj.orderinpalette = QString::number(eventList.size()+1);;
            newObj.name = newObject->ui->objNameEntry->text();
            eventList.push_back(newObj);

            QStandardItem* newObjectType = new QStandardItem("Envelope");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderEnv->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonSiv->isChecked()) {
            QList<SieveEvent>& eventList = pm->sieveevents();
            SieveEvent newObj = {};
            newObj.orderinpalette = QString::number(eventList.size()+1);;
            newObj.name = newObject->ui->objNameEntry->text();
            eventList.push_back(newObj);

            QStandardItem* newObjectType = new QStandardItem("Sieve");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderSiv->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonSpa->isChecked()) {
            QList<SpaEvent>& eventList = pm->spaevents();
            SpaEvent newObj = {};
            newObj.orderinpalette = QString::number(eventList.size()+1);;
            newObj.name = newObject->ui->objNameEntry->text();
            eventList.push_back(newObj);

            QStandardItem* newObjectType = new QStandardItem("Spatialization");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderSpa->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonPat->isChecked()) {
            QList<PatternEvent>& eventList = pm->patternevents();
            PatternEvent newObj = {};
            newObj.orderinpalette = QString::number(eventList.size()+1);;
            newObj.name = newObject->ui->objNameEntry->text();
            eventList.push_back(newObj);

            QStandardItem* newObjectType = new QStandardItem("Pattern");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderPat->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonRev->isChecked()) {
            QList<ReverbEvent>& eventList = pm->reverbevents();
            ReverbEvent newObj = {};
            newObj.orderinpalette = QString::number(eventList.size()+1);;
            newObj.name = newObject->ui->objNameEntry->text();
            eventList.push_back(newObj);

            QStandardItem* newObjectType = new QStandardItem("Reverb");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderRev->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonFil->isChecked()) {
            QList<FilterEvent>& eventList = pm->filterevents();
            FilterEvent newObj = {};
            newObj.orderinpalette = QString::number(eventList.size()+1);;
            newObj.name = newObject->ui->objNameEntry->text();
            eventList.push_back(newObj);

            QStandardItem* newObjectType = new QStandardItem("Filter");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderFil->appendRow({newObjectType, newObjectName});
        }
        // Commented out because no structs for mea event yet
        /* else if (newObject->ui->buttonMea->isChecked()) {
            QStandardItem* newObjectType = new QStandardItem("Measurement");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderMea->appendRow({newObjectType, newObjectName});
        } */
        
        modifiedButNotSaved = true;
        MUtilities::modified();
        delete newObject;
        newObject = NULL;
    }
}

void ProjectView::updatePaletteView() {

    ProjectManager *pm = Inst::get_project_manager();
    
    QList<HEvent>& pHevents = pm->hevents();
    for (HEvent &item : pHevents) {
        QString itemName = item.name;
        QStandardItem* newObjectName = new QStandardItem(itemName);
        if (item.type == top) {
            QStandardItem* newObjectType = new QStandardItem("Top");
            paletteView->folderTop->appendRow({newObjectType, newObjectName});
        }
        else if (item.type == high) {
            QStandardItem* newObjectType = new QStandardItem("High");
            paletteView->folderHigh->appendRow({newObjectType, newObjectName});
        }
        else if (item.type == mid) {
            QStandardItem* newObjectType = new QStandardItem("Mid");
            paletteView->folderMid->appendRow({newObjectType, newObjectName});
        }
        else if (item.type == low) {
            QStandardItem* newObjectType = new QStandardItem("Low");
            paletteView->folderLow->appendRow({newObjectType, newObjectName});
        }
    }

    QList<BottomEvent>& pBevents = pm->bottomevents();
    for (BottomEvent &item : pBevents) {
        QStandardItem* newObjectName = new QStandardItem(item.event.name);
        QStandardItem* newObjectType = new QStandardItem("Bottom");
        paletteView->folderBottom->appendRow({newObjectType, newObjectName});
    }

    QList<SpectrumEvent>& pSpeEvents = pm->spectrumevents();
    for (SpectrumEvent &item : pSpeEvents) {
        QStandardItem* newObjectName = new QStandardItem(item.name);
        QStandardItem* newObjectType = new QStandardItem("Spectrum");
        paletteView->folderSpectrum->appendRow({newObjectType, newObjectName});
    }

    QList<NoteEvent>& pNevents = pm->noteevents();
    for (NoteEvent &item : pNevents) {
        QStandardItem* newObjectName = new QStandardItem(item.name);
        QStandardItem* newObjectType = new QStandardItem("Note");
        paletteView->folderNote->appendRow({newObjectType, newObjectName});
    }

    QList<EnvelopeEvent>& pEevents = pm->envelopeevents();
    for (EnvelopeEvent &item : pEevents) {
        QStandardItem* newObjectName = new QStandardItem(item.name);
        QStandardItem* newObjectType = new QStandardItem("Envelope");
        paletteView->folderEnv->appendRow({newObjectType, newObjectName});
    }

    QList<SieveEvent>& pSivEvents = pm->sieveevents();
    for (SieveEvent &item : pSivEvents) {
        QStandardItem* newObjectName = new QStandardItem(item.name);
        QStandardItem* newObjectType = new QStandardItem("Sieve");
        paletteView->folderSiv->appendRow({newObjectType, newObjectName});
    }

    QList<SpaEvent>& pSpaEvents = pm->spaevents();
    for (SpaEvent &item : pSpaEvents) {
        QStandardItem* newObjectName = new QStandardItem(item.name);
        QStandardItem* newObjectType = new QStandardItem("Spatialization");
        paletteView->folderSpa->appendRow({newObjectType, newObjectName});
    }

    QList<PatternEvent>& pPevents = pm->patternevents();
    for (PatternEvent &item : pPevents) {
        QStandardItem* newObjectName = new QStandardItem(item.name);
        QStandardItem* newObjectType = new QStandardItem("Pattern");
        paletteView->folderPat->appendRow({newObjectType, newObjectName});
    }

    QList<ReverbEvent>& pRevents = pm->reverbevents();
    for (ReverbEvent &item : pRevents) {
        QStandardItem* newObjectName = new QStandardItem(item.name);
        QStandardItem* newObjectType = new QStandardItem("Reverb");
        paletteView->folderRev->appendRow({newObjectType, newObjectName});
    }

    QList<FilterEvent>& pFevents = pm->filterevents();
    for (FilterEvent &item : pFevents) {
        QStandardItem* newObjectName = new QStandardItem(item.name);
        QStandardItem* newObjectType = new QStandardItem("Filter");
        paletteView->folderFil->appendRow({newObjectType, newObjectName});
    }   
}

//nhi: show attributes
// void ProjectView::showAttributes(IEvent* event) { // using QString for testing
void ProjectView::showAttributes(QString eventType, QString eventName) {
    // TODO: Implement event attributes display
    // This would typically show the event in an EventAttributesViewController
    //qDebug() << "Showing attributes for event:" << QString::fromStdString(event->getEventName());

    // Show the ObjectWindows
    if (eventType == "Top" ){ 
        if (eventName == "") { topWindow->show(); }
        else { eventAttributesView->showAttributesOfEvent(top); }
    }
    else if (eventType == "High" ){
        if (eventName == "") { highWindow->show(); }
        else { eventAttributesView->showAttributesOfEvent(high); }
    }
    else if (eventType == "Mid" ){
        if (eventName == "") { midWindow->show(); }
        else { eventAttributesView->showAttributesOfEvent(mid); }
    }
    else if (eventType == "Low" ){ 
        if (eventName == "") { lowWindow->show(); }
        else { eventAttributesView->showAttributesOfEvent(low); }
    }
    else if (eventType == "Bottom" ){ 
        if (eventName == "") { bottomWindow->show(); }
        else { eventAttributesView->showAttributesOfEvent(bottom); }
    }
    else if (eventType == "Spectrum" ){
        if (eventName == "") { spectrumWindow->show(); }
        else { eventAttributesView->showAttributesOfEvent(sound); }
    }
    else if (eventType == "Note" ){
        if (eventName == "") { noteWindow->show(); }
        else { eventAttributesView->showAttributesOfEvent(note); }
    }
    else if (eventType == "Envelope" ){
        if (eventName == "") { envWindow->show(); }
        else { eventAttributesView->showAttributesOfEvent(env); }
    }
    else if (eventType == "Sieve" ){
        if (eventName == "") { sivWindow->show(); }
        else { eventAttributesView->showAttributesOfEvent(sieve); }
    }
    else if (eventType == "Spatialization" ){
       if (eventName == "") { spaWindow->show(); }
       else { eventAttributesView->showAttributesOfEvent(spa); }
    }
    else if (eventType == "Pattern" ){
        if (eventName == "") { patWindow->show(); }
        else { eventAttributesView->showAttributesOfEvent(pattern); }
    }
    else if (eventType == "Reverb" ){
        if (eventName == "") { revWindow->show(); }
        else { eventAttributesView->showAttributesOfEvent(reverb); }
    }
    else if (eventType == "Filter" ){
        if (eventName == "") { filWindow->show(); }
        else { eventAttributesView->showAttributesOfEvent(filter); }
    }
    else if (eventType == "Measurement" ){
        if (eventName == "") { meaWindow->show(); }
        else { eventAttributesView->showAttributesOfEvent(mea); }
    } 

    mainWindow->ui->eventsScrollArea->widget()->adjustSize();
    mainWindow->ui->eventsScrollArea->updateGeometry();
}
