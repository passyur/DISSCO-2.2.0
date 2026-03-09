/* 
The 'project' object that keeps track of project details to deploy
in the associated window (currently, the project view).
*/
#include "project_struct.hpp"
#include "xmlparser.hpp"

#include <QDomDocument>
#include <QTextStream>

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

    Project *project = create(info.baseName(), id);
    QFileInfo fileinfo(filepath);
    project->fileinfo = fileinfo;

    curr_project_ = project;

    qDebug() << "Now parsing " << filepath;
    parse(project, filepath);
    
    return project;
}

Project* ProjectManager::build(const QString& filepath, const QByteArray& id){
    QFileInfo info(filepath);

    Project *project = create(info.baseName(), id);
    QFileInfo fileinfo(filepath);
    project->fileinfo = fileinfo;
    project->dat_path = fileinfo.absolutePath();
    project->lib_path = fileinfo.absoluteFilePath();

    curr_project_ = project;  

    // Create a default top event
    HEvent& topevent = this->topevent();
    HEvent defaultTop;
    defaultTop.type = top;
    defaultTop.name = "0";
    defaultTop.orderinpalette = "-1";
    topevent = defaultTop;

    return project;
}

void ProjectManager::addEvent(Eventtype newEvent, QString eventName) {
    switch(newEvent) {
        case high: {
            QList<HEvent>& eventList = highevents();
            HEvent newObj = {};
            newObj.type = high;
            newObj.name = eventName;
            eventList.push_back(newObj);
            break;
        }
        case mid: {
            QList<HEvent>& eventList = midevents();
            HEvent newObj = {};
            newObj.type = mid;
            newObj.name = eventName;
            eventList.push_back(newObj);
            break;
        }
        case low: {
            QList<HEvent>& eventList = lowevents();
            HEvent newObj = {};
            newObj.type = low;
            newObj.name = eventName;
            eventList.push_back(newObj);
            break;
        }
        case bottom: {
            QList<BottomEvent>& eventList = bottomevents();
            BottomEvent newObj = {};
            newObj.event.type = bottom;
            newObj.event.name = eventName;
            eventList.push_back(newObj);
            break;
        }
        case sound: {
            QList<SpectrumEvent>& eventList = spectrumevents();
            SpectrumEvent newObj;
            newObj.orderinpalette = QString::number(eventList.size()+1);;
            newObj.name = eventName;
            eventList.push_back(newObj);
            break;
        }
        case env: {
            QList<EnvelopeEvent>& eventList = envelopeevents();
            EnvelopeEvent newObj = {};
            newObj.orderinpalette = QString::number(eventList.size()+1);;
            newObj.name = eventName;
            eventList.push_back(newObj);
            break;
        }
        case sieve: {
            QList<SieveEvent>& eventList = sieveevents();
            SieveEvent newObj = {};
            newObj.orderinpalette = QString::number(eventList.size()+1);;
            newObj.name = eventName;
            eventList.push_back(newObj);
            break;
        }
        case spa: {
            QList<SpaEvent>& eventList = spaevents();
            SpaEvent newObj = {};
            newObj.orderinpalette = QString::number(eventList.size()+1);;
            newObj.name = eventName;
            eventList.push_back(newObj);
            break;
        }
        case pattern: {
            QList<PatternEvent>& eventList = patternevents();
            PatternEvent newObj = {};
            newObj.orderinpalette = QString::number(eventList.size()+1);;
            newObj.name = eventName;
            eventList.push_back(newObj);
            break;
        }
        case reverb: {
            QList<ReverbEvent>& eventList = reverbevents();
            ReverbEvent newObj = {};
            newObj.orderinpalette = QString::number(eventList.size()+1);;
            newObj.name = eventName;
            eventList.push_back(newObj);
            break;
        }
        case note: {
            QList<NoteEvent>& eventList = noteevents();
            NoteEvent newObj = {};
            newObj.orderinpalette = QString::number(eventList.size()+1);;
            newObj.name = eventName;
            eventList.push_back(newObj);
            break;
        }
        case filter: {
            QList<FilterEvent>& eventList = filterevents();
            FilterEvent newObj = {};
            newObj.orderinpalette = QString::number(eventList.size()+1);;
            newObj.name = eventName;
            eventList.push_back(newObj);
            break;
        }
        case folder: case mea: case spec:
            break;
        default:
            break;
    }

}

void ProjectManager::writeSeedEntry(const QString& seed) const {
    QString filepath = curr_project_->fileinfo.absoluteFilePath();

    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QDomDocument doc;
    doc.setContent(&file);
    file.close();

    QDomElement seedEl = doc.documentElement()
        .firstChildElement("ProjectConfiguration")
        .firstChildElement("Seed");

    if (!seedEl.isNull()) {
        QDomNode text = seedEl.firstChild();
        if (!text.isNull())
            seedEl.removeChild(text);
        seedEl.appendChild(doc.createTextNode(seed));
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
    out << doc.toString();
    file.close();
}
