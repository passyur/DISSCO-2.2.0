#ifndef MARKOVMODELLIBRARYWINDOW_H
#define MARKOVMODELLIBRARYWINDOW_H

#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MarkovModelLibraryWindow; }
QT_END_NAMESPACE

class MarkovModelLibraryWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MarkovModelLibraryWindow(QWidget *parent = nullptr);
    ~MarkovModelLibraryWindow();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

    std::unique_ptr<Ui::MarkovModelLibraryWindow> ui;

    // Menus
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    // Toolbars
    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    // Actions
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *closeAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
};

#endif // MARKOVMODELLIBRARYWINDOW_H 