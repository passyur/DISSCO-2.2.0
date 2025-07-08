#ifndef PROJECTVIEW_HPP
#define PROJECTVIEW_HPP

#include "MainWindow.hpp"
#include "ProjectPropertiesDialog.hpp"
#include "FileNewObject.hpp"
#include "../core/project_struct.hpp"
#include "../../LASS/src/LASS.h"
#include "EnvelopeLibraryEntry.hpp"
#include "../../CMOD/src/Markov.h"

#include <QDomDocument>

class MainWindow;
class EnvelopeLibraryEntry;
class EventAttributesViewController;
class ObjectWindow;
class PaletteViewController;

class ProjectView : public QObject {
    Q_OBJECT
    
    public:
        /* init/constructor to spawn a project_view inside of mainwin corresponding to an empty, new project */
        // ProjectView();
        /* init/constructor to spawn a project_view inside of mainwin corresponding to a particular filepath and project_title; i.e., open project */
        ProjectView(Project *proj);
        ProjectView(MainWindow* _mainWindow, QString _pathAndName);
        /* function to return full .dissco file path */
        QString getPathAndName();

        // ProjectView(const project_view& other);
        // ProjectView& operator=(const project_view& rhs);

        // ~ProjectView();
        
        // void showContents();
        // void hideContents();

        /* function to write to the xml .dissco file */
        void save();
        QString inlineXml(QDomDocument& doc);

        /* function called when changes made to project */
        void modified();

        /* functions for modifying the Markov Model list */
        void initializeModifiers();
        std::map<std::string, bool> getDefaultNoteModifiers();
        std::vector<std::string> getCustomNoteModifiers();

        /* set properties pop up function */
        void setProperties();

        /* new object pop up function */
        void insertObject();   

         /* functions for modifying the Envelope list */
        EnvelopeLibraryEntry* getEnvelopeLibraryEntries();
        EnvelopeLibraryEntry* createNewEnvelope();
        EnvelopeLibraryEntry* duplicateEnvelope(EnvelopeLibraryEntry* _originalEnvelope);
        void deleteEnvelope(EnvelopeLibraryEntry* toDelete);

        // nhi: Markov model methods
        std::vector<MarkovModel<float>>& getMarkovModels() { return markovModels; }
        int createNewMarkovModel() { 
            markovModels.push_back(MarkovModel<float>()); 
            return markovModels.size() - 1; 
        }
        int duplicateMarkovModel(int idx) { 
            if (idx >= 0 && idx < markovModels.size()) {
                markovModels.push_back(markovModels[idx]); 
                return markovModels.size() - 1;
            }
            return -1;
        }
        void removeMarkovModel(int idx) { 
            if (idx >= 0 && idx < markovModels.size()) {
                markovModels.erase(markovModels.begin() + idx);
            }
        }

        // ObjectWindow and Palette methods
        PaletteViewController* getPalette() { return paletteView; }
        void showAttributes(QString eventType, QString eventName);
        //void showAttributes(class IEvent* event);

    private:
        /* storing pointers */
        MainWindow* mainWindow;
        ProjectPropertiesDialog* projectPropertiesDialog;
        FileNewObject* newObject;
        
        /* storing saved state of project */
        bool modifiedButNotSaved;

        /* set properties insert function button */
        void propertiesInsertFunction();

        /* the *.dissco file */
        QString pathAndName;
        /* properties */
        QString top_event;
        QString project_title;
        QString file_flag;
        QString duration;
        QString num_channels;
        QString sample_rate;
        QString sample_size;
        QString num_threads;
        QString num_staff;
        QString dat_path;
        QString lib_path;
        QString seed;
        QString measure;

        /* flags for CMOD */
        EnvelopeLibraryEntry* envelopeLibraryEntries;
        bool score;
        bool synthesis;
        bool output_particel;
        bool empty_project;
        bool grandStaff;
        std::map<std::string, bool> defaultNoteModifiers;
        std::vector<std::string> customNoteModifiers;

        /* list of custom note modifiers, per user */
        QList<QString> custom_note_modifiers;

        //nhi: add Markov models vector
        /* Markov models */
        std::vector<MarkovModel<float>> markovModels;

        // ObjectWindow instances
        ObjectWindow* topWindow;
        ObjectWindow* highWindow;
        ObjectWindow* midWindow;
        ObjectWindow* lowWindow;
        ObjectWindow* bottomWindow;
        ObjectWindow* spectrumWindow;
        ObjectWindow* envWindow;
        ObjectWindow* sivWindow;
        ObjectWindow* spaWindow;
        ObjectWindow* patWindow;
        ObjectWindow* revWindow;
        ObjectWindow* filWindow;
        ObjectWindow* noteWindow;
        ObjectWindow* meaWindow;

        // PaletteViewController pointer
        PaletteViewController* paletteView;

        // EventAttributesViewController pointer
        EventAttributesViewController* eventAttributesView;
};

#endif