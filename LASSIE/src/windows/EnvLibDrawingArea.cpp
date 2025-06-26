#include "EnvLibDrawingArea.hpp"
#include "EnvelopeLibraryWindow.hpp"
#include "EnvelopeLibraryEntry.hpp"
#include "EnvelopeCommand.hpp"

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
    , selectedPoint(-1)
    , isDragging(false)
    , dragStartPos()
    , undoStack(parent->getUndoStack())
    , worldXMin(0.0)
    , worldXMax(1.0)
    , worldYMin(0.0)
    , worldYMax(1.0)
    , viewportWidth(width())
    , viewportHeight(height())
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

void EnvLibDrawingArea::setEnvelope(EnvelopeLibraryEntry *env)
{
    currentEnvelope = env;
    selectedPoint = -1;
    isDragging = false;
    if (env) {
        worldYMin = env->getMinY();
        worldYMax = env->getMaxY();
        scaleY = viewportHeight / (worldYMax - worldYMin);
    }
    update();
}

void EnvLibDrawingArea::clearEnvelope()
{
    currentEnvelope = nullptr;
    selectedPoint = -1;
    isDragging = false;
    update();
}

void EnvLibDrawingArea::updateSelectedNode(double x, double y)
{
    if (currentEnvelope && selectedPoint >= 0) {
        QPointF newPos(x, y);
        undoStack->push(new MovePointCommand(currentEnvelope, selectedPoint, newPos));
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
        drawPoints(painter);
    }
}

void EnvLibDrawingArea::mousePressEvent(QMouseEvent *event)
{
    if (!currentEnvelope)
        return;

    QPointF pos = mapToEnvelope(event->pos());

    if (event->button() == Qt::LeftButton) {
        if (event->modifiers() & Qt::ShiftModifier) {
            // Add new point
            undoStack->push(new AddPointCommand(currentEnvelope, pos));
            selectedPoint = currentEnvelope->findPoint(pos);
            isDragging = true;
            dragStartPos = pos;
        } else {
            // Select or start dragging existing point
            selectedPoint = findNearestPoint(pos);
            if (selectedPoint >= 0) {
                isDragging = true;
                dragStartPos = currentEnvelope->getPoint(selectedPoint);
            }
        }
    } else if (event->button() == Qt::RightButton) {
        // Remove point
        selectedPoint = findNearestPoint(pos);
        if (selectedPoint >= 0) {
            undoStack->push(new RemovePointCommand(currentEnvelope, selectedPoint));
            selectedPoint = -1;
        }
    }

    update();
}

void EnvLibDrawingArea::mouseMoveEvent(QMouseEvent *event)
{
    if (!currentEnvelope || !isDragging || selectedPoint < 0)
        return;

    QPointF pos = mapToEnvelope(event->pos());
    if (pos != currentEnvelope->getPoint(selectedPoint)) {
        undoStack->push(new MovePointCommand(currentEnvelope, selectedPoint, pos));
        update();
    }
}

void EnvLibDrawingArea::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    isDragging = false;
}

void EnvLibDrawingArea::resizeEvent(QResizeEvent *event)
{
    viewportWidth = width();
    viewportHeight = height();
    
    // Update scaling factors
    scaleX = viewportWidth / (worldXMax - worldXMin);
    scaleY = viewportHeight / (worldYMax - worldYMin);

    QWidget::resizeEvent(event);
}

void EnvLibDrawingArea::drawGrid(QPainter &painter)
{
    QPen gridPen(Qt::lightGray, 1, Qt::DotLine);
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
    if (!currentEnvelope || currentEnvelope->getPointCount() < 2)
        return;

    painter.setPen(QPen(Qt::blue, 2));
    
    QPointF prevPoint = mapFromEnvelope(currentEnvelope->getPoint(0));
    for (int i = 1; i < currentEnvelope->getPointCount(); ++i) {
        QPointF currentPoint = mapFromEnvelope(currentEnvelope->getPoint(i));
        painter.drawLine(prevPoint, currentPoint);
        prevPoint = currentPoint;
    }
}

void EnvLibDrawingArea::drawPoints(QPainter &painter)
{
    QPen pointPen(Qt::red);
    painter.setPen(pointPen);
    painter.setBrush(Qt::white);

    for (int i = 0; i < currentEnvelope->getPointCount(); ++i) {
        QPointF point = mapFromEnvelope(currentEnvelope->getPoint(i));
        if (i == selectedPoint) {
            painter.drawEllipse(point, SELECTED_NODE_RADIUS, SELECTED_NODE_RADIUS);
        } else {
            painter.drawEllipse(point, NODE_RADIUS, NODE_RADIUS);
        }
    }
}

int EnvLibDrawingArea::findNearestPoint(const QPointF &pos) const
{
    const int CLICK_THRESHOLD = 10;
    int nearest = -1;
    double minDist = CLICK_THRESHOLD;

    for (int i = 0; i < currentEnvelope->getPointCount(); ++i) {
        QPointF point = mapFromEnvelope(currentEnvelope->getPoint(i));
        double dist = QLineF(point, pos).length();
        if (dist < minDist) {
            minDist = dist;
            nearest = i;
        }
    }

    return nearest;
}

QPointF EnvLibDrawingArea::mapToEnvelope(const QPointF &pos) const
{
    return QPointF(
        pos.x() / width(),
        1.0 - pos.y() / height()
    );
}

QPointF EnvLibDrawingArea::mapFromEnvelope(const QPointF &pos) const
{
    return QPointF(
        pos.x() * width(),
        (1.0 - pos.y()) * height()
    );
}

QPoint EnvLibDrawingArea::worldToScreen(double x, double y) const
{
    int screenX = static_cast<int>((x - worldXMin) * scaleX);
    int screenY = viewportHeight - static_cast<int>((y - worldYMin) * scaleY);
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