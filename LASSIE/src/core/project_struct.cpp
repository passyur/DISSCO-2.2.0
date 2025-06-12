/* 
The 'project' object that keeps track of project details to deploy
in the associated window (currently, the project view).
*/
#include "project_struct.hpp"

#include "../mainwindow.hpp"

#include <QFile>
#include <QFileInfo>
#include <QtLogging>
#include <xercesc/dom/DOMDocument.hpp>

// #include "muParser.h"

// QString modifiers[] = {
//     "-8va", 
//     "+8va",
//     "accent",
//     "espressivo",
//     "marcato",
//     "portato",
//     "staccatissimo",
//     "staccato",
//     "tenuto",
//     "prall",
//     "prallup",
//     "pralldown",
//     "upprall",
//     "downprall",
//     "prallprall",
//     "lineprall",
//     "prallmordent",
//     "mordent",
//     "upmordent",
//     "downmordent",
//     "trill",
//     "turn",
//     "reverseturn",
//     "shortfermata",
//     "fermata",
//     "longfermata",
//     "verylongfermata",
//     "upbow",
//     "downbow",
//     "flageolet",
//     "open",
//     "halfopen",
//     "lheel",
//     "rheel",
//     "ltoe",
//     "rtoe",
//     "snappizzicato",
//     "stopped",
//     "segno",
//     "coda",
//     "varcoda",
//     "null"
// };
  
// namespace ProjectHelper {
//     constexpr void initializeModifiers(std::map<QString, bool>& default_note_modifiers) {
//         for(unsigned int i = 0; i < sizeof(modifiers) / sizeof(QString); ++i){
//             default_note_modifiers.insert(pair<QString, bool>(modifiers[i], true));
//         }

//         std::map<QString, bool>::iterator iter = default_note_modifiers.begin();
//         while (iter != default_note_modifiers.end()) {
//             (*iter).second = true;
//             iter++;
//         }
//     }

//     /* called before appearing (see immediately succeeding f'n) the checkboxes; init'd when a user brings up the configure note modifier menu */
//     inline void initializeNoteModifierButton(std::map<QString, bool>& default_note_modifiers, GTK::CheckButton& checkbox, std::string key_name, std::string button_name){
//         note_modifiers_configuration_dialog_ref_builder->get_widget(button_name, checkbox);
//         /* let the compiler do this, branchless */
//         checkbox->set_active(default_note_modifiers[key_name]);
//     }

//     inline void appearNoteModifierButton(std::map<QString, bool>& default_note_modifiers, GTK::CheckButton& checkbox, std::string key_name, std::string button_name){
//         note_modifiers_configuration_dialog_ref_builder->get_widget(button_name, checkbox);
//         default_note_modifiers[key_name] = checkbox->get_active();
//     }
// }

unsigned int counter = 1;
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
    if(_title.isEmpty())
        title = tr("Untitled");
    else
        title = _title;
    
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
    
    return project;
}

