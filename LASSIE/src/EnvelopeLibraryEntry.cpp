#include "EnvelopeLibraryEntry.h"
#include <QDomDocument>
#include <QDomElement>
#include <cmath>
#include <stdexcept>

EnvelopeLibraryEntry::EnvelopeLibraryEntry(const QString& name)
    : name(name)
    , minY(0.0)
    , maxY(1.0)
    , firstNode(nullptr)
{
}

EnvelopeLibraryEntry::EnvelopeLibraryEntry(const EnvelopeLibraryEntry& other)
    : name(other.name)
    , minY(other.minY)
    , maxY(other.maxY)
    , firstNode(nullptr)
{
    // Deep copy nodes
    EnvelopeNode* otherNode = other.firstNode;
    while (otherNode) {
        addNode(otherNode->x, otherNode->y);
        otherNode = otherNode->next;
    }

    // Deep copy segments
    for (const auto& segment : other.segments) {
        int startIndex = 0, endIndex = 0;
        EnvelopeNode* node = firstNode;
        
        // Find start node index
        while (node && node != segment->startNode) {
            startIndex++;
            node = node->next;
        }
        
        // Find end node index
        node = firstNode;
        while (node && node != segment->endNode) {
            endIndex++;
            node = node->next;
        }
        
        addSegment(startIndex, endIndex, segment->type, segment->property);
    }
}

EnvelopeLibraryEntry::~EnvelopeLibraryEntry()
{
    // Delete all nodes
    EnvelopeNode* current = firstNode;
    while (current) {
        EnvelopeNode* next = current->next;
        delete current;
        current = next;
    }
}

int EnvelopeLibraryEntry::addNode(double x, double y)
{
    EnvelopeNode* newNode = new EnvelopeNode(x, y);
    
    if (!firstNode || x < firstNode->x) {
        // Insert at beginning
        newNode->next = firstNode;
        if (firstNode) {
            firstNode->prev = newNode;
        }
        firstNode = newNode;
        return 0;
    }
    
    // Find insertion point
    EnvelopeNode* current = firstNode;
    int index = 0;
    
    while (current->next && current->next->x < x) {
        current = current->next;
        index++;
    }
    
    // Insert after current
    newNode->next = current->next;
    newNode->prev = current;
    if (current->next) {
        current->next->prev = newNode;
    }
    current->next = newNode;
    
    return index + 1;
}

void EnvelopeLibraryEntry::removeNode(int index)
{
    if (!firstNode) return;
    
    EnvelopeNode* nodeToRemove = firstNode;
    for (int i = 0; i < index && nodeToRemove; i++) {
        nodeToRemove = nodeToRemove->next;
    }
    
    if (!nodeToRemove) return;
    
    // Remove any segments connected to this node
    segments.erase(
        std::remove_if(segments.begin(), segments.end(),
            [nodeToRemove](const std::unique_ptr<EnvelopeSegment>& segment) {
                return segment->startNode == nodeToRemove || segment->endNode == nodeToRemove;
            }),
        segments.end()
    );
    
    // Update node links
    if (nodeToRemove->prev) {
        nodeToRemove->prev->next = nodeToRemove->next;
    } else {
        firstNode = nodeToRemove->next;
    }
    
    if (nodeToRemove->next) {
        nodeToRemove->next->prev = nodeToRemove->prev;
    }
    
    delete nodeToRemove;
}

void EnvelopeLibraryEntry::moveNode(int index, double x, double y)
{
    EnvelopeNode* node = firstNode;
    for (int i = 0; i < index && node; i++) {
        node = node->next;
    }
    
    if (node) {
        node->x = x;
        node->y = y;
    }
}

int EnvelopeLibraryEntry::findClosestNode(double x, double y, double threshold) const
{
    if (!firstNode) return -1;
    
    int closestIndex = -1;
    double minDistance = threshold;
    EnvelopeNode* current = firstNode;
    int index = 0;
    
    while (current) {
        double dx = current->x - x;
        double dy = current->y - y;
        double distance = std::sqrt(dx * dx + dy * dy);
        
        if (distance < minDistance) {
            minDistance = distance;
            closestIndex = index;
        }
        
        current = current->next;
        index++;
    }
    
    return closestIndex;
}

const EnvelopeNode* EnvelopeLibraryEntry::getNode(int index) const
{
    const EnvelopeNode* node = firstNode;
    for (int i = 0; i < index && node; i++) {
        node = node->next;
    }
    return node;
}

void EnvelopeLibraryEntry::addSegment(int startIndex, int endIndex,
                                     EnvelopeSegmentType type,
                                     EnvelopeSegmentProperty property)
{
    EnvelopeNode* startNode = firstNode;
    for (int i = 0; i < startIndex && startNode; i++) {
        startNode = startNode->next;
    }
    
    EnvelopeNode* endNode = firstNode;
    for (int i = 0; i < endIndex && endNode; i++) {
        endNode = endNode->next;
    }
    
    if (!startNode || !endNode || startNode == endNode) return;
    
    // Check if segment already exists
    for (const auto& segment : segments) {
        if ((segment->startNode == startNode && segment->endNode == endNode) ||
            (segment->startNode == endNode && segment->endNode == startNode)) {
            return;
        }
    }
    
    segments.push_back(std::make_unique<EnvelopeSegment>(startNode, endNode, type, property));
}

void EnvelopeLibraryEntry::removeSegment(int startIndex, int endIndex)
{
    EnvelopeNode* startNode = firstNode;
    for (int i = 0; i < startIndex && startNode; i++) {
        startNode = startNode->next;
    }
    
    EnvelopeNode* endNode = firstNode;
    for (int i = 0; i < endIndex && endNode; i++) {
        endNode = endNode->next;
    }
    
    if (!startNode || !endNode) return;
    
    segments.erase(
        std::remove_if(segments.begin(), segments.end(),
            [startNode, endNode](const std::unique_ptr<EnvelopeSegment>& segment) {
                return (segment->startNode == startNode && segment->endNode == endNode) ||
                       (segment->startNode == endNode && segment->endNode == startNode);
            }),
        segments.end()
    );
}

void EnvelopeLibraryEntry::setSegmentType(int startIndex, int endIndex, EnvelopeSegmentType type)
{
    EnvelopeNode* startNode = firstNode;
    for (int i = 0; i < startIndex && startNode; i++) {
        startNode = startNode->next;
    }
    
    EnvelopeNode* endNode = firstNode;
    for (int i = 0; i < endIndex && endNode; i++) {
        endNode = endNode->next;
    }
    
    if (!startNode || !endNode) return;
    
    for (auto& segment : segments) {
        if ((segment->startNode == startNode && segment->endNode == endNode) ||
            (segment->startNode == endNode && segment->endNode == startNode)) {
            segment->type = type;
            break;
        }
    }
}

void EnvelopeLibraryEntry::setSegmentProperty(int startIndex, int endIndex, EnvelopeSegmentProperty property)
{
    EnvelopeNode* startNode = firstNode;
    for (int i = 0; i < startIndex && startNode; i++) {
        startNode = startNode->next;
    }
    
    EnvelopeNode* endNode = firstNode;
    for (int i = 0; i < endIndex && endNode; i++) {
        endNode = endNode->next;
    }
    
    if (!startNode || !endNode) return;
    
    for (auto& segment : segments) {
        if ((segment->startNode == startNode && segment->endNode == endNode) ||
            (segment->startNode == endNode && segment->endNode == startNode)) {
            segment->property = property;
            break;
        }
    }
}

const EnvelopeSegment* EnvelopeLibraryEntry::getSegment(int startIndex, int endIndex) const
{
    EnvelopeNode* startNode = firstNode;
    for (int i = 0; i < startIndex && startNode; i++) {
        startNode = startNode->next;
    }
    
    EnvelopeNode* endNode = firstNode;
    for (int i = 0; i < endIndex && endNode; i++) {
        endNode = endNode->next;
    }
    
    if (!startNode || !endNode) return nullptr;
    
    for (const auto& segment : segments) {
        if ((segment->startNode == startNode && segment->endNode == endNode) ||
            (segment->startNode == endNode && segment->endNode == startNode)) {
            return segment.get();
        }
    }
    
    return nullptr;
}

void EnvelopeLibraryEntry::saveToXml(QDomDocument& doc, QDomElement& element) const
{
    element.setAttribute("name", name);
    element.setAttribute("minY", QString::number(minY));
    element.setAttribute("maxY", QString::number(maxY));
    
    // Save nodes
    QDomElement nodesElement = doc.createElement("nodes");
    EnvelopeNode* current = firstNode;
    while (current) {
        QDomElement nodeElement = doc.createElement("node");
        nodeElement.setAttribute("x", QString::number(current->x));
        nodeElement.setAttribute("y", QString::number(current->y));
        nodesElement.appendChild(nodeElement);
        current = current->next;
    }
    element.appendChild(nodesElement);
    
    // Save segments
    QDomElement segmentsElement = doc.createElement("segments");
    for (const auto& segment : segments) {
        QDomElement segmentElement = doc.createElement("segment");
        
        // Find indices of start and end nodes
        int startIndex = 0;
        int endIndex = 0;
        current = firstNode;
        
        while (current && current != segment->startNode) {
            startIndex++;
            current = current->next;
        }
        
        current = firstNode;
        while (current && current != segment->endNode) {
            endIndex++;
            current = current->next;
        }
        
        segmentElement.setAttribute("startIndex", QString::number(startIndex));
        segmentElement.setAttribute("endIndex", QString::number(endIndex));
        segmentElement.setAttribute("type", static_cast<int>(segment->type));
        segmentElement.setAttribute("property", static_cast<int>(segment->property));
        
        segmentsElement.appendChild(segmentElement);
    }
    element.appendChild(segmentsElement);
}

void EnvelopeLibraryEntry::loadFromXml(const QDomElement& element)
{
    // Clear existing data
    while (firstNode) {
        EnvelopeNode* next = firstNode->next;
        delete firstNode;
        firstNode = next;
    }
    segments.clear();
    
    // Load basic properties
    name = element.attribute("name");
    minY = element.attribute("minY").toDouble();
    maxY = element.attribute("maxY").toDouble();
    
    // Load nodes
    QDomElement nodesElement = element.firstChildElement("nodes");
    QDomElement nodeElement = nodesElement.firstChildElement("node");
    while (!nodeElement.isNull()) {
        double x = nodeElement.attribute("x").toDouble();
        double y = nodeElement.attribute("y").toDouble();
        addNode(x, y);
        nodeElement = nodeElement.nextSiblingElement("node");
    }
    
    // Load segments
    QDomElement segmentsElement = element.firstChildElement("segments");
    QDomElement segmentElement = segmentsElement.firstChildElement("segment");
    while (!segmentElement.isNull()) {
        int startIndex = segmentElement.attribute("startIndex").toInt();
        int endIndex = segmentElement.attribute("endIndex").toInt();
        EnvelopeSegmentType type = static_cast<EnvelopeSegmentType>(segmentElement.attribute("type").toInt());
        EnvelopeSegmentProperty property = static_cast<EnvelopeSegmentProperty>(segmentElement.attribute("property").toInt());
        
        addSegment(startIndex, endIndex, type, property);
        segmentElement = segmentElement.nextSiblingElement("segment");
    }
} 