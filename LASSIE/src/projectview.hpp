#ifndef PROJECTVIEW_HPP
#define PROJECTVIEW_HPP

#include "mainwindow.hpp"
#include "core/project_struct.hpp"
#include "../../LASS/src/LASS.h"

class ProjectView : public QObject {
    Q_OBJECT
    
    public:
        /* init/constructor to spawn a project_view inside of mainwin corresponding to an empty, new project */
        // ProjectView();
        /* init/constructor to spawn a project_view inside of mainwin corresponding to a particular filepath and project_title; i.e., open project */
        ProjectView(Project *proj);

        // ProjectView(const project_view& other);
        // ProjectView& operator=(const project_view& rhs);

        // ~ProjectView();
        
        // void showContents();
        // void hideContents();

    private:
        /* the *.dissco file */

        /* properties */

        QString top_event;
        QString project_title;
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
};

#endif