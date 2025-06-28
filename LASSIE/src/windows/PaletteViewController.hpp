#ifndef PALETTEVIEWCONTROLLER_HPP
#define PALETTEVIEWCONTROLLER_HPP

#include <QWidget>
#include <QMap>
#include <QString>
#include <vector>

class ProjectView;
class IEvent;
class ObjectWindowObjectPackage;

class PaletteViewController : public QWidget {
    Q_OBJECT

public:
    explicit PaletteViewController(ProjectView* projectView);
    ~PaletteViewController();

    // Get the linked list of objects for a specific type
    ObjectWindowObjectPackage* getObjectsLinkedList(const QString& type);
    
    // Insert a new event into the palette
    void insertEvent(IEvent* event, const QString& type);
    
    // Remove an event from the palette
    void removeEvent(IEvent* event, const QString& type);

private:
    ProjectView* projectView;
    
    // Store events by type
    QMap<QString, std::vector<IEvent*>> eventsByType;
    
    // Convert event type string to EventType enum
    int getEventTypeFromString(const QString& type);
};

#endif // PALETTEVIEWCONTROLLER_HPP