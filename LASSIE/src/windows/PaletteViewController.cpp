#include "PaletteViewController.hpp"
#include "ProjectViewController.hpp"
#include "ObjectWindow.hpp"
#include "../core/IEvent.h"

PaletteViewController::PaletteViewController(ProjectView* projectView)
    : QWidget(nullptr), projectView(projectView)
{
    // Initialize empty vectors for each event type
    eventsByType["Top"] = std::vector<IEvent*>();
    eventsByType["High"] = std::vector<IEvent*>();
    eventsByType["Mid"] = std::vector<IEvent*>();
    eventsByType["Low"] = std::vector<IEvent*>();
    eventsByType["Bottom"] = std::vector<IEvent*>();
    eventsByType["Spectrum"] = std::vector<IEvent*>();
    eventsByType["Envelope"] = std::vector<IEvent*>();
    eventsByType["Sieve"] = std::vector<IEvent*>();
    eventsByType["Spatialization"] = std::vector<IEvent*>();
    eventsByType["Pattern"] = std::vector<IEvent*>();
    eventsByType["Reverb"] = std::vector<IEvent*>();
    eventsByType["Note"] = std::vector<IEvent*>();
    eventsByType["Filter"] = std::vector<IEvent*>();
    eventsByType["Measurement"] = std::vector<IEvent*>();
}

PaletteViewController::~PaletteViewController() = default;

ObjectWindowObjectPackage* PaletteViewController::getObjectsLinkedList(const QString& type)
{
    if (!eventsByType.contains(type) || eventsByType[type].empty()) {
        return nullptr;
    }

    // Create the first package
    ObjectWindowObjectPackage* head = new ObjectWindowObjectPackage(projectView);
    head->ievent = eventsByType[type][0];
    head->prev = nullptr;
    head->next = nullptr;

    // Create packages for remaining events
    ObjectWindowObjectPackage* current = head;
    for (size_t i = 1; i < eventsByType[type].size(); ++i) {
        ObjectWindowObjectPackage* next = new ObjectWindowObjectPackage(projectView);
        next->ievent = eventsByType[type][i];
        next->prev = current;
        next->next = nullptr;
        
        current->next = next;
        current = next;
    }

    return head;
}

void PaletteViewController::insertEvent(IEvent* event, const QString& type)
{
    if (eventsByType.contains(type)) {
        eventsByType[type].push_back(event);
    }
}

void PaletteViewController::removeEvent(IEvent* event, const QString& type)
{
    if (eventsByType.contains(type)) {
        auto& events = eventsByType[type];
        events.erase(std::remove(events.begin(), events.end(), event), events.end());
    }
}

int PaletteViewController::getEventTypeFromString(const QString& type)
{
    if (type == "Top") return 0;
    if (type == "High") return 1;
    if (type == "Mid") return 2;
    if (type == "Low") return 3;
    if (type == "Bottom") return 4;
    if (type == "Spectrum") return 5;
    if (type == "Envelope") return 6;
    if (type == "Sieve") return 7;
    if (type == "Spatialization") return 8;
    if (type == "Pattern") return 9;
    if (type == "Reverb") return 10;
    if (type == "Note") return 12;
    if (type == "Filter") return 13;
    if (type == "Measurement") return 14;
    return -1;
} 