#include "EnvLibDrawingArea.hpp"
#include "EnvelopeLibraryWindow.hpp"
#include "EnvelopeLibraryEntry.hpp"

#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <cmath>

EnvLibDrawingArea::EnvLibDrawingArea(QWidget *parent)
    : QWidget(parent)
    , parentWindow(qobject_cast<EnvelopeLibraryWindow*>(parent))
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
        // Calculate min/max Y values from envelope nodes
        double minY = 0.0, maxY = 1.0;
        for (size_t i = 0; i < envelope->getNodeCount(); ++i) {
            const EnvelopeNode* node = envelope->getNode(i);
            if (node) {
                minY = std::min(minY, node->y);
                maxY = std::max(maxY, node->y);
            }
        }
        worldYMin = minY;
        worldYMax = maxY;
        scaleY = viewportHeight / (worldYMax - worldYMin);
    }
    update();
}

void EnvLibDrawingArea::updateNodePosition(double x, double y)
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
    for (size_t i = 0; i < currentEnvelope->getNodeCount() - 1; ++i) {
        const EnvelopeNode* node1 = currentEnvelope->getNode(i);
        const EnvelopeNode* node2 = currentEnvelope->getNode(i + 1);
        if (node1 && node2) {
            QPoint p1 = worldToScreen(node1->x, node1->y);
            QPoint p2 = worldToScreen(node2->x, node2->y);
            painter.drawLine(p1, p2);
        }
    }
}

void EnvLibDrawingArea::drawNodes(QPainter &painter)
{
    if (!currentEnvelope)
        return;

    QPen nodePen(Qt::black, 1, Qt::SolidLine);
    QBrush nodeBrush(Qt::white);
    painter.setPen(nodePen);
    painter.setBrush(nodeBrush);

    // Draw all nodes
    for (size_t i = 0; i < currentEnvelope->getNodeCount(); ++i) {
        const EnvelopeNode* node = currentEnvelope->getNode(i);
        if (node) {
            QPoint center = worldToScreen(node->x, node->y);
            painter.drawEllipse(center, 4, 4);
        }
    }
}

void EnvLibDrawingArea::drawSelectedNode(QPainter &painter)
{
    if (!currentEnvelope || selectedNode < 0)
        return;

    const EnvelopeNode* node = currentEnvelope->getNode(selectedNode);
    if (node) {
        QPen selectedPen(Qt::red, 2, Qt::SolidLine);
        QBrush selectedBrush(Qt::red);
        painter.setPen(selectedPen);
        painter.setBrush(selectedBrush);

        QPoint center = worldToScreen(node->x, node->y);
        painter.drawEllipse(center, 6, 6);
    }
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