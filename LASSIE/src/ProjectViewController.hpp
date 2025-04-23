#ifndef PROJECTVIEWCONTROLLER_HPP
#define PROJECTVIEWCONTROLLER_HPP

#include <QWidget>
#include <QMap>
#include <QSharedPointer>
#include "Project.hpp"

namespace Ui {
class ProjectViewController;
}

class QStandardItemModel;
class QStandardItem;

class ProjectViewController : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectViewController(QWidget *parent = nullptr);
    ~ProjectViewController();

private slots:
    void onNewButtonClicked();
    void onOpenButtonClicked();
    void onSaveButtonClicked();
    void onCloseButtonClicked();
    void onSearchTextChanged(const QString &text);
    void onProjectSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    
    // Project editing
    void onAddModelClicked();
    void onRemoveModelClicked();
    void onEditModelClicked();

private:
    void setupConnections();
    void initializeModel();
    void updateProjectView(const QSharedPointer<Project>& project);
    void clearProjectView();
    void addProjectToTree(const QSharedPointer<Project>& project);
    void removeProjectFromTree(const QString& projectName);
    void filterProjects(const QString& searchText);
    
    Ui::ProjectViewController *ui;
    QStandardItemModel *model;
    QMap<QString, QSharedPointer<Project>> projects;
    QSharedPointer<Project> currentProject;
};

#endif // PROJECTVIEWCONTROLLER_HPP 