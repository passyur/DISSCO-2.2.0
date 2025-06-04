#ifndef ENVELOPE_LIBRARY_ENTRY_HPP
#define ENVELOPE_LIBRARY_ENTRY_HPP

#include <QString>
#include <QDomDocument>
#include <QDomElement>
#include <vector>

enum class EnvelopeSegmentType {
    Linear,
    Exponential,
    Spline
};

enum class EnvelopeSegmentProperty {
    Flexible,
    Fixed
};

class EnvelopeNode {
public:
    double x;
    double y;
    EnvelopeNode(double x, double y);
};

class EnvelopeSegment {
public:
    int startNodeIndex;
    int endNodeIndex;
    EnvelopeSegmentType type;
    EnvelopeSegmentProperty property;
    EnvelopeSegment(int start, int end);
};

class EnvelopeLibraryEntry {
public:
    EnvelopeLibraryEntry(const QString& name);
    EnvelopeLibraryEntry(const EnvelopeLibraryEntry& other);
    ~EnvelopeLibraryEntry();

    QString getName() const { return name; }
    void setName(const QString& newName) { name = newName; }

    int addNode(double x, double y);
    void removeNode(int index);
    void moveNode(int index, double x, double y);
    int findClosestNode(double x, double y, double threshold) const;
    const EnvelopeNode* getNode(int index) const;
    size_t getNodeCount() const { return nodes.size(); }

    void addSegment(int startIndex, int endIndex, EnvelopeSegmentType type = EnvelopeSegmentType::Linear);
    void removeSegment(int startIndex, int endIndex);
    void setSegmentType(int startIndex, int endIndex, EnvelopeSegmentType type);
    void setSegmentProperty(int startIndex, int endIndex, EnvelopeSegmentProperty property);
    const EnvelopeSegment* getSegment(int startIndex, int endIndex) const;

    void saveToXml(QDomDocument& doc, QDomElement& element) const;
    void loadFromXml(const QDomElement& element);

private:
    QString name;
    std::vector<EnvelopeNode*> nodes;
    std::vector<EnvelopeSegment*> segments;
};

#endif // ENVELOPE_LIBRARY_ENTRY_HPP 