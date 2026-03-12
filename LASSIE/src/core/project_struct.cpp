/* 
The 'project' object that keeps track of project details to deploy
in the associated window (currently, the project view).
*/
#include "project_struct.hpp"
#include "xmlparser.hpp"

#include <QDomDocument>
#include <QTextStream>

#include <type_traits>

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
    auto project = new Project(title, id);
#ifdef TABEDITOR
    project_hash_.insert(project->id, project);
#endif

    return project;
}

Project* ProjectManager::open(const QString& filepath, const QByteArray& id){
    QFileInfo info(filepath);
    const QString cpath = info.canonicalFilePath();
    info.setFile(cpath);

    Project *project = create(info.baseName(), id);
    const QFileInfo fileinfo(filepath);
    project->fileinfo = fileinfo;

    curr_project_ = project;

    qDebug() << "Now parsing " << filepath;
    parse(project, filepath);
    
    return project;
}

Project* ProjectManager::build(const QString& filepath, const QByteArray& id){
    const QFileInfo info(filepath);

    Project *project = create(info.baseName(), id);
    const QFileInfo fileinfo(filepath);
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

void ProjectManager::addEvent(const Eventtype new_type, QString new_name) const {
    auto addHEvent = [&](QList<HEvent>& list, const Eventtype type) {
        HEvent obj = {};
        obj.type = type;
        obj.name = new_name;
        list.push_back(obj);
    };
    auto addSimple = [&](auto& list) {
        using T = typename std::remove_reference_t<decltype(list)>::value_type;
        T obj = {};
        obj.orderinpalette = QString::number(list.size() + 1);
        obj.name = new_name;
        list.push_back(obj);
    };

    switch(new_type) {
        case high:    addHEvent(highevents(), high);  break;
        case mid:     addHEvent(midevents(), mid);    break;
        case low:     addHEvent(lowevents(), low);    break;
        case bottom: {
            BottomEvent obj = {};
            obj.event.type = bottom;
            obj.event.name = new_name;
            bottomevents().push_back(obj);
            break;
        }
        case sound:   addSimple(spectrumevents());  break;
        case env:     addSimple(envelopeevents());  break;
        case sieve:   addSimple(sieveevents());     break;
        case spa:     addSimple(spaevents());       break;
        case pattern: addSimple(patternevents());   break;
        case reverb:  addSimple(reverbevents());    break;
        case note:    addSimple(noteevents());      break;
        case filter:  addSimple(filterevents());    break;
        case folder: case mea: case spec:
        default:
            break;
    }
}

void ProjectManager::writeSeedEntry(const QString& seed) const {
    const QString filepath = curr_project_->fileinfo.absoluteFilePath();

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
        // we have to remove any pre-existing seed elements before writing, since Qt's Xml stuff doesn't support
        // modifying existing elements
        if (const QDomNode text = seedEl.firstChild(); !text.isNull())
            seedEl.removeChild(text);
        seedEl.appendChild(doc.createTextNode(seed));
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
    out << doc.toString();
    file.close();
}
