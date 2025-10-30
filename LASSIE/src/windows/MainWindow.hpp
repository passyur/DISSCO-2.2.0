#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <memory>
#include <QStatusBar>

#include "../inst.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class EnvelopeLibraryWindow;
class MarkovModelLibraryWindow; //nhi: more descriptive than MarkovWindow
class ProjectView;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        //MainWindow(QWidget *parent = nullptr);
        MainWindow(Inst*);
        static MainWindow* instance() { return instance_; }
        ~MainWindow();
        void setUnsavedTitle(QString unsavedFile);

        std::unique_ptr<Ui::MainWindow> ui;
        std::unique_ptr<EnvelopeLibraryWindow> envelopeLibraryWindow;
        std::unique_ptr<MarkovModelLibraryWindow> markovWindow;

    public slots:
        void showStatusMessage(const QString& message) { statusBar()->showMessage(message, 5000); }

    private slots:
        // File operations
        void newFile();
        void openFile();
        void saveFile();
        void saveFileAs();

        // Edit operations
         /* TODO: implement undo /edo */
        void undo() { statusBar()->showMessage(tr("Undo"), 2000); }
        void redo() { statusBar()->showMessage(tr("Redo"), 2000); }

        // Window operations
        void showEnvelopeLibraryWindow();
        void showMarkovWindow();
        void showPropertiesDialog();
        void showFileNewObjectDialog();

    protected:
        void closeEvent(QCloseEvent *event) override;
        void readSettings();
        void writeSettings();

    private:
        void createActions();
        void enableProjectActions(bool enabled);
        void createMenus();
        void createToolBars();
        void createStatusBar() { statusBar()->showMessage(tr("Ready")); }

        void showFile();

        QString currentFile;
        
        // Actions
        QAction *newAct;
        QAction *openAct;
        QAction *saveAct;
        QAction *saveAsAct;
        QAction *exitAct;

        QAction *undoAct;
        QAction *redoAct;
        QAction *cutAct;
        QAction *copyAct;
        QAction *pasteAct;

        QAction *newObjAct;
        QAction *setPropAct;
        QAction *runAct;
        QAction *configNoteModAct;

        QAction *showEnvelopeLibraryAct;
        QAction *showMarkovAct;
        QAction *aboutAct;
        QAction *aboutQtAct;

        // Menus
        QMenu *fileMenu;
        QMenu *editMenu;
        QMenu *projectMenu;
        QMenu *viewMenu;
        QMenu *helpMenu;

        // Toolbars
        QToolBar *fileToolBar;
        QToolBar *projectToolBar;
        QToolBar *editToolBar;

        QStatusBar *statusbar_;
        
        // projectView pointer for testing
        ProjectView* projectView;
        
        Inst *main_;
        static MainWindow *instance_;
};

#endif // MAINWINDOW_HPP
