#ifndef ENVELOPELIBRARYENTRY_HPP
#define ENVELOPELIBRARYENTRY_HPP

#include <QString>
#include <vector>
#include <memory>
#include <QPointF>

// Forward declarations
class QDomDocument;
class QDomElement;

// Enums for envelope segment types and properties
enum class EnvelopeSegmentType {
    Linear,
    Exponential,
    Spline
};

enum class EnvelopeSegmentProperty {
    None,
    Fixed,
    Tied
};

// Structure to represent a node in the envelope
struct EnvelopeNode {
    double x;
    double y;
    EnvelopeNode* next;
    EnvelopeNode* prev;

    EnvelopeNode(double x = 0.0, double y = 0.0)
        : x(x), y(y), next(nullptr), prev(nullptr) {}
};

// Structure to represent a segment in the envelope
struct EnvelopeSegment {
    EnvelopeNode* startNode;
    EnvelopeNode* endNode;
    EnvelopeSegmentType type;
    EnvelopeSegmentProperty property;

    EnvelopeSegment(EnvelopeNode* start = nullptr, EnvelopeNode* end = nullptr,
                   EnvelopeSegmentType t = EnvelopeSegmentType::Linear,
                   EnvelopeSegmentProperty p = EnvelopeSegmentProperty::None)
        : startNode(start), endNode(end), type(t), property(p) {}
};

class EnvelopeLibraryEntry {
public:
    // Constructors and destructor
    explicit EnvelopeLibraryEntry(const QString& name = QString());
    EnvelopeLibraryEntry(const EnvelopeLibraryEntry& other);
    ~EnvelopeLibraryEntry();

    // Node operations
    int addNode(double x, double y);
    void removeNode(int index);
    void moveNode(int index, double x, double y);
    int findClosestNode(double x, double y, double threshold) const;
    const EnvelopeNode* getNode(int index) const;

    // Point operations for EnvLibDrawingArea
    void addPoint(const QPointF& point);
    void removePoint(int index);
    void movePoint(int index, const QPointF& point);
    void insertPoint(int index, const QPointF& point);
    QPointF getPoint(int index) const;
    int findPoint(const QPointF& point) const;
    int getPointCount() const;

    // Segment operations
    void addSegment(int startIndex, int endIndex,
                   EnvelopeSegmentType type = EnvelopeSegmentType::Linear,
                   EnvelopeSegmentProperty property = EnvelopeSegmentProperty::None);
    void removeSegment(int startIndex, int endIndex);
    void setSegmentType(int startIndex, int endIndex, EnvelopeSegmentType type);
    void setSegmentProperty(int startIndex, int endIndex, EnvelopeSegmentProperty property);
    const EnvelopeSegment* getSegment(int startIndex, int endIndex) const;

    // File operations
    void saveToXml(QDomDocument& doc, QDomElement& element) const;
    void loadFromXml(const QDomElement& element);

    // Getters and setters
    QString getName() const { return name; }
    void setName(const QString& newName) { name = newName; }
    double getMinY() const { return minY; }
    void setMinY(double value) { minY = value; }
    double getMaxY() const { return maxY; }
    void setMaxY(double value) { maxY = value; }

private:
    QString name;
    double minY;
    double maxY;
    EnvelopeNode* firstNode;
    std::vector<std::unique_ptr<EnvelopeSegment>> segments;
};

// Register this type with Qt's meta-object system
#include <QMetaType>
Q_DECLARE_METATYPE(std::shared_ptr<EnvelopeLibraryEntry>)
Q_DECLARE_METATYPE(EnvelopeLibraryEntry*)

#endif // ENVELOPELIBRARYENTRY_H 