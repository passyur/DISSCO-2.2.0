#ifndef PROJECTVIEWCONTROLLER_HPP
#define PROJECTVIEWCONTROLLER_HPP

#include <QWidget>
#include <QTreeWidget>
#include <QString>
#include <QStandardItemModel>
#include <QPushButton>
#include <QLineEdit>

namespace Ui {
class ProjectViewController;
}

class ProjectViewController : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectViewController(QTreeWidget *treeWidget, QWidget *parent = nullptr);
    ~ProjectViewController();

    int getProjectCount() const;
    void addProject(const QString &filePath);

private slots:
    void onNewButtonClicked();
    void onOpenButtonClicked();
    void onSaveButtonClicked();
    void onCloseButtonClicked();
    void onSearchTextChanged(const QString &text);
    void onProjectSelectionChanged();

private:
    void setupConnections();
    void initializeModel();

    QTreeWidget *treeWidget;
    QStandardItemModel *model;
    Ui::ProjectViewController *ui;
};

#endif // PROJECTVIEWCONTROLLER_HPP 