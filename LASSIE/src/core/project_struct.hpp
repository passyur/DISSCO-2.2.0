#ifndef PROJECTSTRUCT_HPP
#define PROJECTSTRUCT_HPP

#include <QString>
#include <QList>
#include <QMap>
#include <QHash>
#include <QObject>

#ifdef MARKOV
#include "MarkovModel.h"
#endif

#include "../windows/EnvelopeLibraryEntry.hpp"
// #include "IEvent.h"

/*
the model: all transactions dealing with Projects must go through the ProjectManager to do so.
              the PM will simply handle all the project requests and will not expose just anything
              unnecessary to the caller. 
*/

// QMap<QString, bool> default_note_modifiers;

class ProjectManager;

// class IEvent;

class Project : public QObject {
    friend class ProjectManager;

    Q_OBJECT
    
    public: 
        /* to be called by ProjectView */
        void showContents();
        void hideContents();
    private:
        /**
         *  This function constructs a Project object that will represent the data contained in a DISSCO project
         *	\param _title The title of the project, default initialization to empty QString
         *  \param _id The UUID corresponding to this project, default initialization to empty QByteArray
         *  \returns 
        **/
        Project(const QString& _title = QString(), const QByteArray& _id = QByteArray());

        /* the *.dissco file */
        QString filepath;
        QByteArray id;

        /* properties */

        QString top_event;
        QString title;
        QString file_flag;
        QString duration;
        QString num_channels = "2";
        QString sample_rate = "44100";
        QString sample_size = "16";
        QString num_threads = "1";
        QString num_staffs;
        QString dat_path;
        QString lib_path;
        QString seed;
        QString measure;

        /* flags for CMOD */
        bool grand_staff = false;
        bool score = false;
        bool synthesis = true;
        bool output_particel = false;
        bool empty_project = false;

        EnvelopeLibraryEntry *elentry = nullptr;
#ifdef MARKOV
        QList<MarkovModel<float>*> markovModels;
#endif
        // QList<IEvent*> events;
        /* list of custom note modifiers, per user */
        QList<QString> custom_note_modifiers;
        /* dict of known default modifiers */
        QMap<QString, bool> default_note_modifiers{{"+8va", true}, {"-8va", true}, {"accent", true}, {"coda", true}, {"downbow", true}, {"downmordent", true}, {"downprall", true}, {"espressivo", true}, {"fermata", true}, {"flageolet", true}, {"halfopen", true}, {"lheel", true}, {"lineprall", true}, {"longfermata", true}, {"ltoe", true}, {"marcato", true}, {"mordent", true}, {"null", true}, {"open", true}, {"portato", true}, {"prall", true}, {"pralldown", true}, {"prallmordent", true}, {"prallprall", true}, {"prallup", true}, {"reverseturn", true}, {"rheel", true}, {"rtoe", true}, {"segno", true}, {"shortfermata", true}, {"snappizzicato", true}, {"staccatissimo", true}, {"staccato", true}, {"stopped", true}, {"tenuto", true}, {"trill", true}, {"turn", true}, {"upbow", true}, {"upmordent", true}, {"upprall", true}, {"varcoda", true}, {"verylongfermata", true}};
};

class ProjectManager : public QObject {
    Q_OBJECT

    public:
        ProjectManager(){
            curr_project_ = nullptr;
        }
        
        Project* create(const QString& title = QString(), const QByteArray& id = QByteArray());
        /* validates and, if successful, opens the file and creates a Project from that file */
        Project* open(const QString& filepath, const QByteArray& id = QByteArray());
        void parse(Project *p, const QString& filepath);
        void close(Project*);
        int save(Project*);
        int saveAs(Project*);
        
        void set_curr_project(class Project*);
        void send_curr_project_to_main(class Project*);
#ifdef TABEDITOR
        QList<Project*> get_projects() { return project_hash_.values(); }
        QList<QByteArray> get_project_IDs() { return project_hash_.keys(); }
#endif
    private:
#ifdef TABEDITOR
        QHash<QByteArray, Project*> project_hash_;
#endif
        /* the project to be displayed by the bound ProjectView */
        Project *curr_project_;
};

#endif