#ifndef PROJECTSTRUCT_HPP
#define PROJECTSTRUCT_HPP

#include <QString>
#include <QList>
#include <QMap>
#include <QHash>
#include <QObject>

/*
the model: all transactions dealing with Projects must go through the ProjectManager to do so.
              the PM will simply handle all the project requests and will not expose just anything
              unnecessary to the caller. 
*/

// QMap<QString, bool> default_note_modifiers;

class ProjectManager;

class Project : public QObject {
    friend class ProjectManager;

    Q_OBJECT
    
    public: 
        /* to be called by ProjectView */
        void showContents();
        void hideContents();
    private:
        /* init/constructor to spawn a project with a given title and id; TO BE CALLED BY PROJECT MANAGER (hence why it's private) */
        Project(const QString& _title = QString(), const QByteArray& _id = QByteArray());

        /* the *.dissco file */
        QString filepath;
        QByteArray id;

        /* properties */

        QString top_event;
        QString title;
        QString file_flag;
        QString duration;
        QString num_channels;
        QString sample_rate;
        QString sample_size;
        QString num_threads;
        QString dat_path;
        QString lib_path;
        QString seed;
        QString measure;

        /* flags for CMOD */

        bool score;
        bool synthesis;
        bool output_particel;
        bool empty_project;

        /* list of custom note modifiers, per user */
        QList<QString> custom_note_modifiers;
        /* dict of known default modifiers */
};

class ProjectManager : public QObject {
    Q_OBJECT

    public:
        ProjectManager(){
            curr_project_ = nullptr;
        }
        
        Project* create(const QString& title = QString(), const QByteArray& id = QByteArray());
        Project* open(const QString& filepath, const QByteArray& id = QByteArray());
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