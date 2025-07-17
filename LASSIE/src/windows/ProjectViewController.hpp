#ifndef PROJECTVIEW_HPP
#define PROJECTVIEW_HPP

#include "MainWindow.hpp"
#include "ProjectPropertiesDialog.hpp"
#include "FileNewObject.hpp"
#include "../core/project_struct.hpp"
#include "../../LASS/src/LASS.h"
#include "../core/EnvelopeLibraryEntry.hpp"
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
        ProjectView(MainWindow* _mainWindow, QString _pathAndName);

        /* function to write to the xml .dissco file */
        void save();
        QString inlineXml(QDomDocument& doc);

        /* set properties pop up function */
        void setProperties();

        /* new object pop up function */
        void insertObject();   

         /* functions for modifying the Envelope list */
        EnvelopeLibraryEntry* getEnvelopeLibraryEntries();
        EnvelopeLibraryEntry* createNewEnvelope();
        EnvelopeLibraryEntry* duplicateEnvelope(EnvelopeLibraryEntry* _originalEnvelope);
        void deleteEnvelope(EnvelopeLibraryEntry* toDelete);
        /*
        // nhi: Markov model methods
        std::vector<MarkovModel<float>>& getMarkovModels() { return markovModels; }
        int createNewMarkovModel() { 
            markovModels.push_back(MarkovModel<float>()); 
            return markovModels.size() - 1; 
        }
        int duplicateMarkovModel(int idx) { 
            if (idx >= 0 && idx < (int) markovModels.size()) {
                markovModels.push_back(markovModels[idx]); 
                return markovModels.size() - 1;
            }
            return -1;
        }
        void removeMarkovModel(int idx) { 
            if (idx >= 0 && idx < (int) markovModels.size()) {
                markovModels.erase(markovModels.begin() + idx);
            }
        }*/

        // ObjectWindow and Palette methods
        PaletteViewController* getPalette() { return paletteView; }
        void showAttributes(QString eventType, QString eventName);
        //void showAttributes(class IEvent* event);

    private:
        /* storing pointers */
        MainWindow* mainWindow;
        ProjectPropertiesDialog* projectPropertiesDialog;
        FileNewObject* newObject;
        PaletteViewController* paletteView;
        EventAttributesViewController* eventAttributesView;
        
        /* storing saved state of project */
        bool modifiedButNotSaved;

        /* set properties insert function button */
        void propertiesInsertFunction();

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

        QMap<QString, bool> default_note_modifiers{
            {"+8va", true},          {"-8va", true},
            {"accent", true},        {"coda", true},
            {"downbow", true},       {"downmordent", true},
            {"downprall", true},     {"espressivo", true},
            {"fermata", true},       {"flageolet", true},
            {"halfopen", true},      {"lheel", true},
            {"lineprall", true},     {"longfermata", true},
            {"ltoe", true},          {"marcato", true},
            {"mordent", true},       {"null", true},
            {"open", true},          {"portato", true},
            {"prall", true},         {"pralldown", true},
            {"prallmordent", true},  {"prallprall", true},
            {"prallup", true},       {"reverseturn", true},
            {"rheel", true},         {"rtoe", true},
            {"segno", true},         {"shortfermata", true},
            {"snappizzicato", true}, {"staccatissimo", true},
            {"staccato", true},      {"stopped", true},
            {"tenuto", true},        {"trill", true},
            {"turn", true},          {"upbow", true},
            {"upmordent", true},     {"upprall", true},
            {"varcoda", true},       {"verylongfermata", true}};
};

#endif