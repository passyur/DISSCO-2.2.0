#ifndef PROJECTVIEWCONTROLLER_H
#define PROJECTVIEWCONTROLLER_H

#include <QWidget>
#include <QTabWidget>
#include <memory>

class ProjectViewController : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectViewController(QWidget *parent = nullptr);
    ~ProjectViewController();

    void setUnsaved(bool unsaved);
    bool isUnsaved() const;
    void setProjectName(const QString& name);
    QString projectName() const;

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void setupUi();

    QTabWidget* mainTabWidget;
    bool unsavedChanges;
    QString currentProjectName;

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

#endif // PROJECTVIEWCONTROLLER_H 