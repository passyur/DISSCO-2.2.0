#include "EnvLibDrawingArea.h"
#include "EnvelopeLibraryWindow.h"
#include "EnvelopeLibraryEntry.h"

#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <cmath>

EnvLibDrawingArea::EnvLibDrawingArea(EnvelopeLibraryWindow *parent)
    : QWidget(parent)
    , parentWindow(parent)
    , currentEnvelope(nullptr)
    , selectedNode(-1)
    , isDragging(false)
    , worldXMin(0.0)
    , worldXMax(1.0)
    , worldYMin(0.0)
    , worldYMax(1.0)
    , viewportWidth(0)
    , viewportHeight(0)
    , scaleX(1.0)
    , scaleY(1.0)
{
    setMinimumSize(400, 300);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

EnvLibDrawingArea::~EnvLibDrawingArea()
{
}

void EnvLibDrawingArea::setEnvelope(EnvelopeLibraryEntry *envelope)
{
    currentEnvelope = envelope;
    selectedNode = -1;
    if (envelope) {
        worldYMin = envelope->getMinY();
        worldYMax = envelope->getMaxY();
        scaleY = viewportHeight / (worldYMax - worldYMin);
    }
    update();
}

void EnvLibDrawingArea::clearEnvelope()
{
    currentEnvelope = nullptr;
    selectedNode = -1;
    update();
}

void EnvLibDrawingArea::updateSelectedNode(double x, double y)
{
    if (currentEnvelope && selectedNode >= 0) {
        currentEnvelope->moveNode(selectedNode, x, y);
        update();
    }
}

void EnvLibDrawingArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Fill background
    painter.fillRect(rect(), Qt::white);

    // Draw grid
    drawGrid(painter);

    if (currentEnvelope) {
        drawEnvelope(painter);
        drawNodes(painter);
        if (selectedNode >= 0) {
            drawSelectedNode(painter);
        }
    }
}

void EnvLibDrawingArea::mousePressEvent(QMouseEvent *event)
{
    if (!currentEnvelope)
        return;

    QPointF worldPos = screenToWorld(event->pos());
    
    // Find closest node within threshold distance
    selectedNode = currentEnvelope->findClosestNode(worldPos.x(), worldPos.y(), 0.05);
    
    if (selectedNode >= 0) {
        isDragging = true;
        const EnvelopeNode* node = currentEnvelope->getNode(selectedNode);
        if (node) {
            parentWindow->setEntries(QString::number(node->x), QString::number(node->y));
        }
    } else if (event->button() == Qt::LeftButton) {
        // Add new node
        selectedNode = currentEnvelope->addNode(worldPos.x(), worldPos.y());
        isDragging = true;
        parentWindow->setEntries(QString::number(worldPos.x()), QString::number(worldPos.y()));
    }
    
    update();
}

void EnvLibDrawingArea::mouseMoveEvent(QMouseEvent *event)
{
    updateMouseCoordinates(event->pos());

    if (!currentEnvelope)
        return;

    if (isDragging && selectedNode >= 0) {
        QPointF worldPos = screenToWorld(event->pos());
        currentEnvelope->moveNode(selectedNode, worldPos.x(), worldPos.y());
        parentWindow->setEntries(QString::number(worldPos.x()), QString::number(worldPos.y()));
        update();
    }

    lastMousePos = event->pos();
}

void EnvLibDrawingArea::mouseReleaseEvent(QMouseEvent *event)
{
    isDragging = false;
}

void EnvLibDrawingArea::resizeEvent(QResizeEvent *event)
{
    viewportWidth = event->size().width();
    viewportHeight = event->size().height();
    
    // Update scaling factors
    scaleX = viewportWidth / (worldXMax - worldXMin);
    scaleY = viewportHeight / (worldYMax - worldYMin);

    QWidget::resizeEvent(event);
}

void EnvLibDrawingArea::drawGrid(QPainter &painter)
{
    QPen gridPen(Qt::lightGray, 1, Qt::SolidLine);
    painter.setPen(gridPen);

    // Draw vertical grid lines
    for (int x = 0; x <= viewportWidth; x += GRID_SPACING) {
        painter.drawLine(x, 0, x, viewportHeight);
    }

    // Draw horizontal grid lines
    for (int y = 0; y <= viewportHeight; y += GRID_SPACING) {
        painter.drawLine(0, y, viewportWidth, y);
    }

    // Draw axes
    QPen axisPen(Qt::black, 2, Qt::SolidLine);
    painter.setPen(axisPen);
    
    // X-axis
    QPoint origin = worldToScreen(0, 0);
    QPoint xEnd = worldToScreen(1, 0);
    painter.drawLine(origin, xEnd);
    
    // Y-axis
    QPoint yEnd = worldToScreen(0, 1);
    painter.drawLine(origin, yEnd);
}

void EnvLibDrawingArea::drawEnvelope(QPainter &painter)
{
    if (!currentEnvelope)
        return;

    QPen envelopePen(Qt::blue, 2, Qt::SolidLine);
    painter.setPen(envelopePen);

    // Draw segments between nodes
    const EnvelopeNode* node = currentEnvelope->getNode(0);
    while (node && node->next) {
        QPoint start = worldToScreen(node->x, node->y);
        QPoint end = worldToScreen(node->next->x, node->next->y);
        
        // Find segment type
        const EnvelopeSegment* segment = currentEnvelope->getSegment(0, 1); // TODO: Fix indices
        if (segment) {
            switch (segment->type) {
                case EnvelopeSegmentType::Linear:
                    painter.drawLine(start, end);
                    break;
                    
                case EnvelopeSegmentType::Exponential: {
                    QPainterPath path;
                    path.moveTo(start);
                    
                    // Draw exponential curve using multiple line segments
                    const int steps = 20;
                    for (int i = 1; i <= steps; ++i) {
                        double t = i / static_cast<double>(steps);
                        double x = node->x + t * (node->next->x - node->x);
                        double y = node->y * std::pow(node->next->y / node->y, t);
                        QPoint p = worldToScreen(x, y);
                        path.lineTo(p);
                    }
                    
                    painter.drawPath(path);
                    break;
                }
                
                case EnvelopeSegmentType::Spline: {
                    QPainterPath path;
                    path.moveTo(start);
                    
                    // Calculate control points for cubic Bezier curve
                    double dx = node->next->x - node->x;
                    double dy = node->next->y - node->y;
                    QPoint c1 = worldToScreen(node->x + dx/3, node->y + dy/3);
                    QPoint c2 = worldToScreen(node->x + 2*dx/3, node->y + 2*dy/3);
                    
                    path.cubicTo(c1, c2, end);
                    painter.drawPath(path);
                    break;
                }
            }
        } else {
            // Default to linear if no segment found
            painter.drawLine(start, end);
        }
        
        node = node->next;
    }
}

void EnvLibDrawingArea::drawNodes(QPainter &painter)
{
    if (!currentEnvelope)
        return;

    QPen nodePen(Qt::blue, 1, Qt::SolidLine);
    QBrush nodeBrush(Qt::white);
    painter.setPen(nodePen);
    painter.setBrush(nodeBrush);

    // Draw all nodes except selected node
    const EnvelopeNode* node = currentEnvelope->getNode(0);
    int index = 0;
    while (node) {
        if (index != selectedNode) {
            QPoint screenPos = worldToScreen(node->x, node->y);
            painter.drawEllipse(screenPos, NODE_RADIUS, NODE_RADIUS);
        }
        node = node->next;
        index++;
    }
}

void EnvLibDrawingArea::drawSelectedNode(QPainter &painter)
{
    if (!currentEnvelope || selectedNode < 0)
        return;

    const EnvelopeNode* node = currentEnvelope->getNode(selectedNode);
    if (!node)
        return;

    QPen selectedPen(Qt::red, 2, Qt::SolidLine);
    QBrush selectedBrush(Qt::white);
    painter.setPen(selectedPen);
    painter.setBrush(selectedBrush);

    QPoint screenPos = worldToScreen(node->x, node->y);
    painter.drawEllipse(screenPos, SELECTED_NODE_RADIUS, SELECTED_NODE_RADIUS);
}

QPoint EnvLibDrawingArea::worldToScreen(double x, double y) const
{
    int screenX = static_cast<int>((x - worldXMin) * scaleX);
    int screenY = static_cast<int>(viewportHeight - (y - worldYMin) * scaleY);
    return QPoint(screenX, screenY);
}

QPointF EnvLibDrawingArea::screenToWorld(const QPoint &point) const
{
    double worldX = point.x() / scaleX + worldXMin;
    double worldY = (viewportHeight - point.y()) / scaleY + worldYMin;
    return QPointF(worldX, worldY);
}

void EnvLibDrawingArea::updateMouseCoordinates(const QPoint &pos)
{
    QPointF worldPos = screenToWorld(pos);
    parentWindow->setEntries(QString::number(worldPos.x()), QString::number(worldPos.y()));
} 