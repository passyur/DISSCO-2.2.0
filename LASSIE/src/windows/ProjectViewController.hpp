#ifndef PROJECT_VIEW_CONTROLLER_H
#define PROJECT_VIEW_CONTROLLER_H

#include <QVBoxLayout>
#include <QString>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class ProjectViewController; }
QT_END_NAMESPACE

class MainWindow;
class EnvelopeLibraryEntry;

class ProjectViewController : public QVBoxLayout
{

    public:
        // Constructors and deconstructors
        ProjectViewController(MainWindow* _mainWindow);
        ProjectViewController(MainWindow* _mainWindow, QString _pathAndName);
        ProjectViewController(MainWindow* _mainWindow, QString _pathAndName, QString _projectTitle);
        ~ProjectViewController();

        // Modifying Envelope List Functions

        // Modifying Markov Model List Functions
        

        // Save Function
        void save();

    private:
        bool modifiedButNotSaved;

        QString pathAndName;
        QString projectTitle;
        QString fileFlag;
        QString duration;
        QString numOfChannels;
        QString sampleRate;
        QString sampleSize;
        QString numOfThreads;
        QString numOfStaff;
        QString topEvent;
        bool synthesis;
        bool score;
        bool grandStaff;
        bool outputParticel;
        QString seed;


};

#endif