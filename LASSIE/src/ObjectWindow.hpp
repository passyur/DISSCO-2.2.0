#ifndef OBJECTWINDOW_HPP
#define OBJECTWINDOW_HPP

#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QSharedPointer>
#include "ProjectViewController.hpp"
#include "IEvent.hpp"

class ObjectWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ObjectWindow(EventType type, QWidget *parent = nullptr);
    ~ObjectWindow();

    void setActiveProject(ProjectViewController* project);
    void refresh();
    QString getSelectedFolder() const;
    QSharedPointer<IEvent> getCurrentSelectedEvent() const;

private slots:
    void onAddClicked();
    void onRemoveClicked();
    void onEditClicked();
    void onSearchTextChanged(const QString &text);
    void onSelectionChanged();

private:
    void setupUi();
    void createConnections();
    void updateObjectView();
    void clearObjectView();
    void addObjectToView(const QSharedPointer<IEvent> &event);
    void removeObjectFromView(const QSharedPointer<IEvent> &event);

    Ui::ObjectWindow *ui;
    EventType eventType;
    ProjectViewController *activeProject;
    QStandardItemModel *objectModel;
    QMap<QString, QSharedPointer<IEvent>> objects;
};

#endif // OBJECTWINDOW_HPP 