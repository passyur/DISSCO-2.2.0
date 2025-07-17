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
    // pm->libpath() = _pathAndName;
    qDebug() << "pm->title: " << pm->title();
    qDebug() << "pm->libpath: " << pm->libpath();
    qDebug() << "pm->fileinfo: " << pm->fileinfo();

    QList<HEvent> pHevents = pm->hevents();
    HEvent defaultTop; // &defaultTop when modifying
    defaultTop.type = top;
    defaultTop.name = "0";
    pHevents.push_back(defaultTop);
    for (HEvent item : pHevents) {
        qDebug() << "Hevent type:" << item.type;
        qDebug() << "Hevent name:" << item.name;
    }

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

    // Create a default top event
    QStandardItem* defaultObjectType = new QStandardItem("Top");
    QStandardItem* defaultObjectName = new QStandardItem(Inst::get_project_manager()->topevent());
    paletteView->folderTop->appendRow({defaultObjectType, defaultObjectName});
    // HEvent newEvent;
    // newEvent.name = defaultObjectName;
    // newEvent.type = top;
    // project->h_events.append(newEvent);

    // IEvent* newEvent = new IEvent();
    // newEvent->setEventName("0");
    // newEvent->setEventType(eventTop);
    // paletteView->insertEvent(newEvent, "Top");

}

// Function to write XML Formatting
QString ProjectView::inlineXml(QDomDocument& doc) {
    QString str = doc.toString();
    str.remove(QRegularExpression("[\\n\\t\\r]+"));
    str.replace(QRegularExpression(">\\s+<"), "><");
    return str;
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
                QDomDocument tempDoc;
                QString duration = pm->duration();
                if (tempDoc.setContent(duration.trimmed()) && !tempDoc.documentElement().isNull()) {
                    // Write raw XML inside <Duration>
                    QString durationXML = inlineXml(tempDoc);
                    xmlWriter.writeCharacters("");
                    xmlWriter.device()->write(durationXML.toUtf8());
                } else {
                    // String/Number inside <Duration>
                    xmlWriter.writeCharacters(duration);
                }
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
            // xmlWriter.writeStartElement("Event");	
            // xmlWriter.writeAttribute("orderInPalette", "-1");	
            // xmlWriter.writeEndElement();
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
        pm->topevent() = projectPropertiesDialog->ui->topEventEntry->text();
        pm->duration() = projectPropertiesDialog->ui->durationEntry->text();

        if (new_title != pm->title()) {
            QString old_pathAndName = pm->libpath();
            QString new_pathAndName = pm->datpath() + "/" + new_title + ".dissco";
            if (QFile::rename(old_pathAndName, new_pathAndName)) {
                pm->libpath() = new_pathAndName;
                pm->title() = new_title;
            }
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
        if (newObject->ui->buttonTop->isChecked()) {
            QStandardItem* newObjectType = new QStandardItem("Top");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderTop->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonHigh->isChecked()) {
            QStandardItem* newObjectType = new QStandardItem("High");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderHigh->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonMid->isChecked()) {
            QStandardItem* newObjectType = new QStandardItem("Mid");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderMid->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonLow->isChecked()) {
            QStandardItem* newObjectType = new QStandardItem("Low");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderLow->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonBottom->isChecked()) {
            QStandardItem* newObjectType = new QStandardItem("Bottom");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderBottom->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonSpectrum->isChecked()) {
            QStandardItem* newObjectType = new QStandardItem("Spectrum");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderSpectrum->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonNote->isChecked()) {
            QStandardItem* newObjectType = new QStandardItem("Note");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderNote->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonEnv->isChecked()) {
            QStandardItem* newObjectType = new QStandardItem("Envelope");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderEnv->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonSiv->isChecked()) {
            QStandardItem* newObjectType = new QStandardItem("Sieve");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderSiv->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonSpa->isChecked()) {
            QStandardItem* newObjectType = new QStandardItem("Spatialization");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderSpa->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonPat->isChecked()) {
            QStandardItem* newObjectType = new QStandardItem("Pattern");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderPat->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonRev->isChecked()) {
            QStandardItem* newObjectType = new QStandardItem("Reverb");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderRev->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonFil->isChecked()) {
            QStandardItem* newObjectType = new QStandardItem("Filter");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderFil->appendRow({newObjectType, newObjectName});
        }
        else if (newObject->ui->buttonMea->isChecked()) {
            QStandardItem* newObjectType = new QStandardItem("Measurement");
            QStandardItem* newObjectName = new QStandardItem(newObject->ui->objNameEntry->text());
            paletteView->folderMea->appendRow({newObjectType, newObjectName});
        }
        
        modifiedButNotSaved = true;
        MUtilities::modified();
        delete newObject;
        newObject = NULL;
    }
}

/*
// Public function to return the path and name for Main Window 
QString ProjectView::getPathAndName() {
    return pathAndName;
}

// Function to return the default node modifiers 
std::map<std::string, bool> ProjectView::getDefaultNoteModifiers(){
  return defaultNoteModifiers;
}

// Function to return the custom node modifiers 
std::vector<std::string> ProjectView::getCustomNoteModifiers(){
  return customNoteModifiers;
}

// Function called when project modified to change the Main Window title to unsaved version which alerts the user to save the project
void ProjectView::modified(){
  if (modifiedButNotSaved){
    mainWindow->setUnsavedTitle(pathAndName);
  }
}

// Envelope Library Functions 
EnvelopeLibraryEntry* ProjectView::getEnvelopeLibraryEntries(){
	return envelopeLibraryEntries;
}
EnvelopeLibraryEntry* ProjectView::createNewEnvelope(){
    if (envelopeLibraryEntries == NULL) {
        envelopeLibraryEntries = new EnvelopeLibraryEntry(1);
        return envelopeLibraryEntries;
    } else {
        return envelopeLibraryEntries->createNewEnvelope();
    }
}
EnvelopeLibraryEntry* ProjectView::duplicateEnvelope(EnvelopeLibraryEntry* _originalEnvelope) {
    if (envelopeLibraryEntries != NULL) {
        return envelopeLibraryEntries->duplicateEnvelope(_originalEnvelope);
    } else { return NULL; }
}
// nhi: delete envelope
void ProjectView::deleteEnvelope(EnvelopeLibraryEntry* toDelete) {
    if (!envelopeLibraryEntries || !toDelete) return;
    if (envelopeLibraryEntries == toDelete) {
        EnvelopeLibraryEntry* next = envelopeLibraryEntries->next;
        if (next) next->prev = nullptr;
        delete envelopeLibraryEntries;
        envelopeLibraryEntries = next;
        return;
    }
    EnvelopeLibraryEntry* curr = envelopeLibraryEntries;
    while (curr && curr->next) {
        if (curr->next == toDelete) {
            EnvelopeLibraryEntry* del = curr->next;
            curr->next = del->next;
            if (del->next) del->next->prev = curr;
            delete del;
            return;
        }
        curr = curr->next;
    }
}
*/
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
