#include "ProjectViewController.hpp"
#include "mainwindow.hpp"

#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QXmlStreamWriter>
#include <QStatusBar>
#include<QDebug>
#include <QTextStream>

ProjectViewController::ProjectViewController(MainWindow *_mainWindow) {
    QString topName = "";
    
    pathAndName   = "";
    projectTitle  = "";
    fileFlag      = ""; 
    duration      = ""; 
    numOfChannels = "2";
    sampleRate    = "44100";
    sampleSize    = "16";
    numOfThreads  = "1";
    numOfStaff    = "1";
    topEvent      = "0";
    synthesis     = true;
	score 		  = false;
    grandStaff    = false;
    qDebug() << "Constructor 1";
}

ProjectViewController::ProjectViewController(MainWindow *_mainWindow, QString _pathAndName) {
    modifiedButNotSaved = true;
    seed = "";

    QString topName = "0";
    QFileInfo fileInfo(_pathAndName);
    projectTitle  = fileInfo.fileName();
    fileFlag      = "THMLBsnv";
    duration      = "";
    numOfChannels = "2";
    sampleRate    = "44100";
    sampleSize    = "16";
    numOfThreads  = "1";
    numOfStaff    = "1";
    topEvent      = "0";
    synthesis     = true;
	score 		  = false;
    grandStaff    = false;
    outputParticel= false;

    pathAndName = _pathAndName;
    qDebug() << "Constructor 2";
}

ProjectViewController::ProjectViewController(MainWindow* _mainWindow, QString _pathAndName, QString _projectTitle) {
    seed = "";
    modifiedButNotSaved = false;

    pathAndName = _pathAndName;
    projectTitle  = _projectTitle;

    QString disscoFile = pathAndName +"/"+_projectTitle;
    qDebug() << "Constructor 3";
}


ProjectViewController::~ProjectViewController(){
}

void ProjectViewController::save(){
    qDebug() << "In PVC Save Function";
    modifiedButNotSaved = false;

    QString fileName = pathAndName + "/" + projectTitle + ".dissco";
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
                xmlWriter.writeCharacters(projectTitle);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("FileFlag");	
                xmlWriter.writeCharacters(fileFlag);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("TopEvent");	
                xmlWriter.writeCharacters(topEvent);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("PieceStartTime");	
                xmlWriter.writeCharacters("0");
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("Duration");	
                xmlWriter.writeCharacters(duration);
            xmlWriter.writeEndElement();

            if (synthesis) {
                xmlWriter.writeStartElement("Synthesis");
                    xmlWriter.writeCharacters("True");
                xmlWriter.writeEndElement();
            }

            if (score) {
                xmlWriter.writeStartElement("Score");
                    xmlWriter.writeCharacters("True");
                xmlWriter.writeEndElement();
            }

            if (grandStaff) {
                xmlWriter.writeStartElement("GrandStaff");
                    xmlWriter.writeCharacters("True");
                xmlWriter.writeEndElement();
            }

            xmlWriter.writeStartElement("NumberOfStaff");	
                xmlWriter.writeCharacters(numOfStaff);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("NumberOfChannels");	
                xmlWriter.writeCharacters(numOfChannels);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("SampleRate");	
                xmlWriter.writeCharacters(sampleRate);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("SampleSize");	
                xmlWriter.writeCharacters(sampleSize);
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("NumberOfThreads");	
                xmlWriter.writeCharacters(numOfThreads);
            xmlWriter.writeEndElement();

            if (outputParticel) {
                xmlWriter.writeStartElement("OutputParticel");
                    xmlWriter.writeCharacters("True");
                xmlWriter.writeEndElement();
            }

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
        xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    file.close();
    qDebug() << "File written to:" << fileName;

}

