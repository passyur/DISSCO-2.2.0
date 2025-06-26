#ifndef ENVLIBDRAWINGAREA_HPP
#define ENVLIBDRAWINGAREA_HPP

#include <QWidget>
#include <QPointF>
#include <QUndoStack>
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>

class EnvelopeLibraryWindow;
class EnvelopeLibraryEntry;

class EnvLibDrawingArea : public QWidget
{
    Q_OBJECT

public:
    explicit EnvLibDrawingArea(EnvelopeLibraryWindow *parent = nullptr);
    ~EnvLibDrawingArea();

    void setEnvelope(EnvelopeLibraryEntry *envelope);
    void clearEnvelope();
    void updateSelectedNode(double x, double y);
    int getSelectedPointIndex() const { return selectedPoint; }

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawGrid(QPainter &painter);
    void drawEnvelope(QPainter &painter);
    void drawPoints(QPainter &painter);
    int findNearestPoint(const QPointF &pos) const;
    QPointF mapToEnvelope(const QPointF &pos) const;
    QPointF mapFromEnvelope(const QPointF &pos) const;
    void drawSelectedNode(QPainter &painter);
    QPoint worldToScreen(double x, double y) const;
    QPointF screenToWorld(const QPoint &point) const;
    void updateMouseCoordinates(const QPoint &pos);

    EnvelopeLibraryWindow *parentWindow;
    EnvelopeLibraryEntry *currentEnvelope;
    int selectedPoint;
    bool isDragging;
    QPointF dragStartPos;
    QUndoStack *undoStack;

    // Drawing parameters
    static const int GRID_SPACING = 50;
    static const int NODE_RADIUS = 5;
    static const int SELECTED_NODE_RADIUS = 7;
    
    // Coordinate transformation
    double worldXMin, worldXMax;
    double worldYMin, worldYMax;
    int viewportWidth, viewportHeight;
    double scaleX, scaleY;
};

#endif // ENVLIBDRAWINGAREA_H 