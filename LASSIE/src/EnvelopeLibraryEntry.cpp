#include "EnvelopeLibraryEntry.hpp"
#include <QDomDocument>
#include <QDomElement>
#include <cmath>
#include <stdexcept>

EnvelopeNode::EnvelopeNode(double x, double y)
    : x(x)
    , y(y)
{
}

EnvelopeSegment::EnvelopeSegment(int start, int end)
    : startNodeIndex(start)
    , endNodeIndex(end)
    , type(EnvelopeSegmentType::Linear)
    , property(EnvelopeSegmentProperty::Flexible)
{
}

EnvelopeLibraryEntry::EnvelopeLibraryEntry(const QString& name)
    : name(name)
{
}

EnvelopeLibraryEntry::EnvelopeLibraryEntry(const EnvelopeLibraryEntry& other)
    : name(other.name)
{
    // Copy nodes
    for (const auto* node : other.nodes) {
        nodes.push_back(new EnvelopeNode(node->x, node->y));
    }
    
    // Copy segments
    for (const auto* segment : other.segments) {
        auto* newSegment = new EnvelopeSegment(segment->startNodeIndex, segment->endNodeIndex);
        newSegment->type = segment->type;
        newSegment->property = segment->property;
        segments.push_back(newSegment);
    }
}

EnvelopeLibraryEntry::~EnvelopeLibraryEntry()
{
    for (auto* node : nodes) {
        delete node;
    }
    nodes.clear();
    
    for (auto* segment : segments) {
        delete segment;
    }
    segments.clear();
}

int EnvelopeLibraryEntry::addNode(double x, double y)
{
    auto* node = new EnvelopeNode(x, y);
    nodes.push_back(node);
    return nodes.size() - 1;
}

void EnvelopeLibraryEntry::removeNode(int index)
{
    if (index >= 0 && index < nodes.size()) {
        delete nodes[index];
        nodes.erase(nodes.begin() + index);
        
        // Remove any segments connected to this node
        auto it = segments.begin();
        while (it != segments.end()) {
            if ((*it)->startNodeIndex == index || (*it)->endNodeIndex == index) {
                delete *it;
                it = segments.erase(it);
            } else {
                // Update indices of segments after the removed node
                if ((*it)->startNodeIndex > index) (*it)->startNodeIndex--;
                if ((*it)->endNodeIndex > index) (*it)->endNodeIndex--;
                ++it;
            }
        }
    }
}

void EnvelopeLibraryEntry::moveNode(int index, double x, double y)
{
    if (index >= 0 && index < nodes.size()) {
        nodes[index]->x = x;
        nodes[index]->y = y;
    }
}

int EnvelopeLibraryEntry::findClosestNode(double x, double y, double threshold) const
{
    int closestIndex = -1;
    double minDistance = threshold;
    
    for (size_t i = 0; i < nodes.size(); ++i) {
        double dx = nodes[i]->x - x;
        double dy = nodes[i]->y - y;
        double distance = std::sqrt(dx * dx + dy * dy);
        
        if (distance < minDistance) {
            minDistance = distance;
            closestIndex = i;
        }
    }
    
    return closestIndex;
}

const EnvelopeNode* EnvelopeLibraryEntry::getNode(int index) const
{
    if (index >= 0 && index < nodes.size()) {
        return nodes[index];
    }
    return nullptr;
}

void EnvelopeLibraryEntry::addSegment(int startIndex, int endIndex, EnvelopeSegmentType type)
{
    if (startIndex >= 0 && startIndex < nodes.size() &&
        endIndex >= 0 && endIndex < nodes.size() &&
        startIndex != endIndex) {
        auto* segment = new EnvelopeSegment(startIndex, endIndex);
        segment->type = type;
        segments.push_back(segment);
    }
}

void EnvelopeLibraryEntry::removeSegment(int startIndex, int endIndex)
{
    auto it = segments.begin();
    while (it != segments.end()) {
        if ((*it)->startNodeIndex == startIndex && (*it)->endNodeIndex == endIndex) {
            delete *it;
            segments.erase(it);
            break;
        }
        ++it;
    }
}

void EnvelopeLibraryEntry::setSegmentType(int startIndex, int endIndex, EnvelopeSegmentType type)
{
    for (auto* segment : segments) {
        if (segment->startNodeIndex == startIndex && segment->endNodeIndex == endIndex) {
            segment->type = type;
            break;
        }
    }
}

void EnvelopeLibraryEntry::setSegmentProperty(int startIndex, int endIndex, EnvelopeSegmentProperty property)
{
    for (auto* segment : segments) {
        if (segment->startNodeIndex == startIndex && segment->endNodeIndex == endIndex) {
            segment->property = property;
            break;
        }
    }
}

const EnvelopeSegment* EnvelopeLibraryEntry::getSegment(int startIndex, int endIndex) const
{
    for (const auto* segment : segments) {
        if (segment->startNodeIndex == startIndex && segment->endNodeIndex == endIndex) {
            return segment;
        }
    }
    return nullptr;
}

void EnvelopeLibraryEntry::saveToXml(QDomDocument& doc, QDomElement& element) const
{
    element.setAttribute("name", name);
    
    // Save nodes
    QDomElement nodesElement = doc.createElement("nodes");
    for (const auto* node : nodes) {
        QDomElement nodeElement = doc.createElement("node");
        nodeElement.setAttribute("x", QString::number(node->x));
        nodeElement.setAttribute("y", QString::number(node->y));
        nodesElement.appendChild(nodeElement);
    }
    element.appendChild(nodesElement);
    
    // Save segments
    QDomElement segmentsElement = doc.createElement("segments");
    for (const auto* segment : segments) {
        QDomElement segmentElement = doc.createElement("segment");
        segmentElement.setAttribute("startIndex", QString::number(segment->startNodeIndex));
        segmentElement.setAttribute("endIndex", QString::number(segment->endNodeIndex));
        segmentElement.setAttribute("type", static_cast<int>(segment->type));
        segmentElement.setAttribute("property", static_cast<int>(segment->property));
        segmentsElement.appendChild(segmentElement);
    }
    element.appendChild(segmentsElement);
}

void EnvelopeLibraryEntry::loadFromXml(const QDomElement& element)
{
    name = element.attribute("name");
    
    // Clear existing data
    for (auto* node : nodes) delete node;
    nodes.clear();
    for (auto* segment : segments) delete segment;
    segments.clear();
    
    // Load nodes
    QDomElement nodesElement = element.firstChildElement("nodes");
    QDomElement nodeElement = nodesElement.firstChildElement("node");
    while (!nodeElement.isNull()) {
        double x = nodeElement.attribute("x").toDouble();
        double y = nodeElement.attribute("y").toDouble();
        nodes.push_back(new EnvelopeNode(x, y));
        nodeElement = nodeElement.nextSiblingElement("node");
    }
    
    // Load segments
    QDomElement segmentsElement = element.firstChildElement("segments");
    QDomElement segmentElement = segmentsElement.firstChildElement("segment");
    while (!segmentElement.isNull()) {
        int startIndex = segmentElement.attribute("startIndex").toInt();
        int endIndex = segmentElement.attribute("endIndex").toInt();
        auto* segment = new EnvelopeSegment(startIndex, endIndex);
        segment->type = static_cast<EnvelopeSegmentType>(segmentElement.attribute("type").toInt());
        segment->property = static_cast<EnvelopeSegmentProperty>(segmentElement.attribute("property").toInt());
        segments.push_back(segment);
        segmentElement = segmentElement.nextSiblingElement("segment");
    }
} 