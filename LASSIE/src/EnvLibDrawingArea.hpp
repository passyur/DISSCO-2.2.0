#ifndef ENV_LIB_DRAWING_AREA_H
#define ENV_LIB_DRAWING_AREA_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "EnvelopeLibraryEntry.hpp"

class EnvelopeLibraryWindow;

class EnvLibDrawingArea : public QWidget {
    Q_OBJECT

public:
    explicit EnvLibDrawingArea(QWidget *parent = nullptr);
    ~EnvLibDrawingArea();

    void setEnvelope(EnvelopeLibraryEntry *envelope);
    void updateNodePosition(double x, double y);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawGrid(QPainter &painter);
    void drawEnvelope(QPainter &painter);
    void drawNodes(QPainter &painter);
    void drawSelectedNode(QPainter &painter);
    QPoint worldToScreen(double x, double y) const;
    QPointF screenToWorld(const QPoint &point) const;
    void updateMouseCoordinates(const QPoint &pos);

    EnvelopeLibraryWindow *parentWindow;
    EnvelopeLibraryEntry *currentEnvelope;
    int selectedNode;
    bool isDragging;
    QPoint lastMousePos;

    // Drawing area settings
    double worldXMin;
    double worldXMax;
    double worldYMin;
    double worldYMax;
    int viewportWidth;
    int viewportHeight;
    double scaleX;
    double scaleY;

    static const int GRID_SPACING = 20;
};

#endif // ENV_LIB_DRAWING_AREA_H 