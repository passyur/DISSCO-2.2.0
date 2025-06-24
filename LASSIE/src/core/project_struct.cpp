/* 
The 'project' object that keeps track of project details to deploy
in the associated window (currently, the project view).
*/
#include "project_struct.hpp"
// #include "IEvent.h"

#include "../mainwindow.hpp"

#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QtLogging>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>

namespace XercesParser {
    using namespace xercesc;
    std::string getFunctionString(DOMElement* _thisFunctionElement){
        char* charBuffer;

        DOMCharacterData* textData;
        string returnString;
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
        return returnString;
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
    p->file_flag = QString::fromStdString(XercesParser::getFunctionString(element));
    //topEvent
    element = element->getNextElementSibling();
    p->top_event = QString::fromStdString(XercesParser::getFunctionString(element));

    // pieceStartTime
    element = element->getNextElementSibling(); //skipped

    //duration
    element = element->getNextElementSibling();
    p->duration = QString::fromStdString(XercesParser::getFunctionString(element));

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
    p->num_staffs = QString::fromStdString(XercesParser::getFunctionString(element));

    //numOfChannels
    element = element->getNextElementSibling();
    p->num_channels = QString::fromStdString(XercesParser::getFunctionString(element));

    //sampleRate
    element = element->getNextElementSibling();
    p->sample_rate = QString::fromStdString(XercesParser::getFunctionString(element));

    //sampleSize
    element = element->getNextElementSibling();
    p->sample_size = QString::fromStdString(XercesParser::getFunctionString(element));

    //numOfThreads
    element = element->getNextElementSibling();
    p->num_threads = QString::fromStdString(XercesParser::getFunctionString(element));

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

    DOMElement *envlibelement = noteModifiers->getNextElementSibling();
    char_data = ( DOMCharacterData*) envlibelement->getFirstChild();

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

    DOMElement* currentElement = envlibelement;

    DOMElement* markovModelLibraryElement = envlibelement->getNextElementSibling();
    std::string tagName = buffer = XMLString::transcode(markovModelLibraryElement->getTagName());
    XMLString::release(&buffer);
    if (tagName == "MarkovModelLibrary") {
        currentElement = markovModelLibraryElement;
        DOMText* text = dynamic_cast<DOMText*>(markovModelLibraryElement->getFirstChild());
        std::string data = buffer = XMLString::transcode(text->getWholeText());
        XMLString::release(&buffer);

        // QTextStream ts(QString::fromStdString(data));

        // // read the number of models
        // int size;
        // ts >> size;
        // p->markovModels.resize(size);

        // // read individual models
        // std::string modelText;
        // QString line;
        // getline(ts, line, '\n');
        // for (int i = 0; i < size; i++) {
        //     line = ts.readLine();
        //     modelText = line + '\n';
        //     for (int j = 0; j < 1; j++){
        //         line = ts.readLine();
        //         modelText += line + '\n';
        //     }
        //     line = ts.readLine();
        //     modelText += line;
        std::stringstream ss(data);

        // read the number of models
        long long size;
        ss >> size;
        p->markovModels.resize(size);

        // read individual models
        std::string modelText, line;
        getline(ss, line, '\n');
        for (int i = 0; i < size; i++) {
            getline(ss, line, '\n');
            modelText = line + '\n';
            getline(ss, line, '\n');
            modelText += line + '\n';
            getline(ss, line, '\n');
            modelText += line + '\n';
            getline(ss, line, '\n');
            modelText += line;

            (p->markovModels[i])->from_str(modelText);
        }
    }

    DOMElement *domEvents = currentElement->getNextElementSibling();
    DOMElement *eventElement = domEvents->getFirstElementChild();

    while (eventElement != NULL){
        IEvent* newEvent = new IEvent(eventElement);
        // paletteView->insertEvent(newEvent, newEvent->getEventTypeString());
        p->events.push_back(newEvent);
        eventElement = eventElement->getNextElementSibling();
    }

    auto eventsIter = p->events.begin();
    for (; eventsIter != p->events.end(); ++eventsIter){
        (*eventsIter)->link(p);
    }
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
    project->filepath = filepath;

    parse(project, filepath);
    
    return project;
}

