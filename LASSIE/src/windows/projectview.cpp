/* The 'project' object that keeps track of project details to deploy
 * in the associated window (currently, the project view).
 *
 * Copyright (c) 2025, DISSCO authors */

#include <QFile>
#include <QString>
#include <QFileInfo>
#include <QIODevice>
#include <QXmlStreamWriter>
#include<QDebug>
#include <QTextStream>

// #include "Define.h"
#include "projectview.hpp"
#include "mainwindow.hpp"
#include "inst.hpp"
// #include "muParser.h"

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

ProjectView::ProjectView(Project *proj){
    Inst::instance()->get_project_manager();

    empty_project = false;

    seed = "";

    // initializeModifiers();
    // add(leftTwoPlusAttributes);
}

ProjectView::ProjectView(MainWindow* _mainWindow, QString _pathAndName) {

    QFileInfo fileInfo(_pathAndName);
    QString dirPath = fileInfo.absoluteFilePath();
    QString pathOnly = fileInfo.absolutePath();
    QString nameOnly = fileInfo.completeBaseName();

    dat_path = pathOnly;
    lib_path = dirPath;
    project_title = nameOnly;
    file_flag = ""; 
    duration = ""; 
    num_channels = "2";
    sample_rate = "44100";
    sample_size = "16";
    num_threads = "1";
    num_staff = "1";
    top_event = "0";
    seed = "";
    synthesis = true;
	score = false;
    grandStaff = false;
}


// ProjectView::~ProjectView(){}

void ProjectView::save(){
    qDebug() << "In PVC Save Function";

    QString fileName = dat_path + "/" + project_title + ".dissco";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << file.fileName();
        qDebug() << "Error reason:" << file.errorString();
        return;
    }
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument("1.0");
    xmlWriter.writeStartElement("ProjectRoot");
        xmlWriter.writeStartElement("ProjectConfiguration");

            xmlWriter.writeStartElement("Title");	
                xmlWriter.writeCharacters(project_title);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("FileFlag");	
                xmlWriter.writeCharacters(file_flag);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("TopEvent");	
                xmlWriter.writeCharacters(top_event);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("PieceStartTime");	
                xmlWriter.writeCharacters("0");
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("Duration");	
                xmlWriter.writeCharacters(duration);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("Synthesis");
            if (synthesis) { xmlWriter.writeCharacters("True"); }
            else { xmlWriter.writeCharacters("False"); }
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("Score");
            if (score) { xmlWriter.writeCharacters("True"); }
            else { xmlWriter.writeCharacters("False"); }
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("GrandStaff");
            if (grandStaff) { xmlWriter.writeCharacters("True"); }
            else { xmlWriter.writeCharacters("False"); }
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("NumberOfStaff");	
                xmlWriter.writeCharacters(num_staff);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("NumberOfChannels");	
                xmlWriter.writeCharacters(num_channels);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("SampleRate");	
                xmlWriter.writeCharacters(sample_rate);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("SampleSize");	
                xmlWriter.writeCharacters(sample_size);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("NumberOfThreads");	
                xmlWriter.writeCharacters(num_threads);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("OutputParticel");
            if (output_particel) { xmlWriter.writeCharacters("True"); }
            else { xmlWriter.writeCharacters("False"); }
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("Seed");	
                xmlWriter.writeCharacters(seed);
            xmlWriter.writeEndElement();

        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("NoteModifiers");
            xmlWriter.writeStartElement("DefaultModifiers");	
                // to do
            xmlWriter.writeEndElement();	
            xmlWriter.writeStartElement("CustomModifiers");	
                // to do
            xmlWriter.writeEndElement();       
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("EnvelopeLibrary");	
            // to do
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("MarkovModelLibrary");	
            // to do
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("Events");	
            // to do
            // xmlWriter.writeStartElement("Event");	
            // xmlWriter.writeAttribute("orderInPalette", "-1");	
            // xmlWriter.writeEndElement();
        xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    file.close();
    qDebug() << "File written to:" << fileName;

}
