#ifndef PALETTEVIEWCONTROLLER_HPP
#define PALETTEVIEWCONTROLLER_HPP

#include <QWidget>
#include <QListWidget>
#include <QTreeWidget>
#include <QMap>
#include <QSharedPointer>
#include "IEvent.hpp"
#include "Tool.hpp"

namespace Ui {
class PaletteViewController;
}

class PaletteViewController : public QWidget
{
    Q_OBJECT

public:
    explicit PaletteViewController(QWidget *parent = nullptr);
    ~PaletteViewController();

    void addTool(const QSharedPointer<Tool> &tool);
    void removeTool(const QString &toolName);
    void addObject(const QSharedPointer<IEvent> &object);
    void removeObject(const QString &objectName);
    void clearObjects();
    void refreshObjects();

    QSharedPointer<Tool> getSelectedTool() const;
    QSharedPointer<IEvent> getSelectedObject() const;

private slots:
    void onToolSelectionChanged();
    void onObjectSelectionChanged();
    void onToolDragStarted();
    void onObjectDragStarted();

private:
    void setupUi();
    void createConnections();
    void initializeTools();
    void updateToolList();
    void updateObjectTree();

    Ui::PaletteViewController *ui;
    QMap<QString, QSharedPointer<Tool>> tools;
    QMap<QString, QSharedPointer<IEvent>> objects;
};

#endif // PALETTEVIEWCONTROLLER_HPP 