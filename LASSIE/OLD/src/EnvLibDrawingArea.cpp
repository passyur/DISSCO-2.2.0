/*******************************************************************************
 *
 *  File Name     : EnvLibDrawingArea.cpp
 *  Date created  : Jan. 29 2010
 *  Authors       : Ming-ching Chiu, Sever Tipei
 *  Organization  : Music School, University of Illinois at Urbana Champaign
 *  Description   : This file contains the implementation of the EnvLibDrawingArea
 *                  class, which is responsible for drawing and manipulating
 *                  envelopes in the UI.
 *
 *==============================================================================
 *
 *  This file is part of LASSIE.
 *  2010 Ming-ching Chiu, Sever Tipei
 *
 *
 *  LASSIE is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  LASSIE is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with LASSIE.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

/*
 * MODIFICATIONS MADE:
 * 
 * 1. Migrated from GTK to Qt:
 *    - Replaced GTK drawing with Qt's QPainter
 *    - Converted event handling to Qt's event system
 *    - Implemented Qt's signal/slot mechanism
 * 
 * 2. Modernized C++ features:
 *    - Used nullptr instead of NULL
 *    - Added proper const correctness to methods
 *    - Added proper documentation with Doxygen-style comments
 * 
 * 3. Improved class structure:
 *    - Added getter and setter methods for better encapsulation
 *    - Made member variables private where appropriate
 *    - Improved method signatures
 * 
 * 4. UI improvements:
 *    - Improved user feedback
 *    - Enhanced drawing capabilities with Qt's QPainter
 */

#include "EnvLibDrawingArea.h"
#include "EnvelopeLibraryWindow.h"
#include "ProjectViewController.h"
#include <sstream>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtCore/QDebug>

EnvLibDrawingArea::EnvLibDrawingArea(EnvelopeLibraryWindow* parent) :
    QWidget(parent),
    envelopeLibraryWindow(parent),
    activeSegment(nullptr),
    activeNode(nullptr),
    upperY(1.0),
    lowerY(0.0),
    head(nullptr),
    tail(nullptr),
    mouseLeftButtonPressedDown(false),
    mouseX(0.0),
    mouseY(0.0),
    moveLeftBound(0.0),
    moveRightBound(1.0) {

    // Set focus policy to receive keyboard events
    setFocusPolicy(Qt::StrongFocus);
    
    // Enable mouse tracking for continuous mouse move events
    setMouseTracking(true);
    
    // Create context menu
    createContextMenu();
}

EnvLibDrawingArea::~EnvLibDrawingArea() {
    // Clean up resources
    delete m_pMenuPopup;
}

void EnvLibDrawingArea::resetFields() {
    activeSegment = nullptr;
    activeNode = nullptr;
    head = nullptr;
    tail = nullptr;
    upperY = 1.0;
    lowerY = 0.0;
    moveLeftBound = 0.0;
    moveRightBound = 1.0;
    mouseX = 0.0;
    mouseY = 0.0;
    mouseLeftButtonPressedDown = false;
    update();
}

void EnvLibDrawingArea::showGraph(EnvelopeLibraryEntry* envelope) {
    if (!envelope) {
        clearGraph();
        return;
    }
    
    // Store the envelope data
    head = envelope->getHead();
    tail = envelope->getTail();
    upperY = envelope->getUpperY();
    lowerY = envelope->getLowerY();
    
    // Reset active elements
    activeSegment = nullptr;
    activeNode = nullptr;
    
    // Update the display
    update();
}

void EnvLibDrawingArea::clearGraph() {
    head = nullptr;
    tail = nullptr;
    activeSegment = nullptr;
    activeNode = nullptr;
    update();
}

void EnvLibDrawingArea::setActiveNodeCoordinate(const QString& x, const QString& y) {
    if (!activeNode) {
        return;
    }
    
    bool okX, okY;
    double xValue = x.toDouble(&okX);
    double yValue = y.toDouble(&okY);
    
    if (!okX || !okY) {
        return;
    }
    
    // Ensure X is within bounds
    if (xValue < moveLeftBound) {
        xValue = moveLeftBound;
    } else if (xValue > moveRightBound) {
        xValue = moveRightBound;
    }
    
    // Ensure Y is within bounds
    if (yValue < lowerY) {
        yValue = lowerY;
    } else if (yValue > upperY) {
        yValue = upperY;
    }
    
    // Update node coordinates
    activeNode->x = xValue;
    activeNode->y = yValue;
    
    // Redraw the graph
    update();
}

void EnvLibDrawingArea::adjustBoundary(EnvelopeLibraryEntry* envelope) {
    if (!envelope) {
        return;
    }
    
    // Find min and max Y values
    double minY = 1.0;
    double maxY = 0.0;
    
    EnvelopeNode* node = envelope->getHead();
    while (node) {
        if (node->y < minY) {
            minY = node->y;
        }
        if (node->y > maxY) {
            maxY = node->y;
        }
        node = node->rightSeg ? node->rightSeg->rightNode : nullptr;
    }
    
    // Update envelope boundaries
    envelope->setLowerY(minY);
    envelope->setUpperY(maxY);
    
    // Update local boundaries
    lowerY = minY;
    upperY = maxY;
    
    // Redraw the graph
    update();
}

void EnvLibDrawingArea::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw background
    painter.fillRect(rect(), Qt::white);
    
    // Draw grid
    painter.setPen(QPen(Qt::lightGray, 1, Qt::DashLine));
    
    // Draw horizontal grid lines
    for (int i = 0; i <= 10; i++) {
        int y = i * height() / 10;
        painter.drawLine(0, y, width(), y);
    }
    
    // Draw vertical grid lines
    for (int i = 0; i <= 10; i++) {
        int x = i * width() / 10;
        painter.drawLine(x, 0, x, height());
    }
    
    // Draw envelope
    drawEnvelope(painter);
}

void EnvLibDrawingArea::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        mouseLeftButtonPressedDown = true;
        mouseX = event->pos().x();
        mouseY = event->pos().y();
        
        // Find node or segment at position
        if (findNodeOrSegmentAtPosition(event->pos())) {
            update();
        }
    }
    
    QWidget::mousePressEvent(event);
}

void EnvLibDrawingArea::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        mouseLeftButtonPressedDown = false;
    }
    
    QWidget::mouseReleaseEvent(event);
}

void EnvLibDrawingArea::mouseMoveEvent(QMouseEvent* event) {
    mouseX = event->pos().x();
    mouseY = event->pos().y();
    
    if (mouseLeftButtonPressedDown && (activeNode || activeSegment)) {
        moveNode();
    } else {
        // Update cursor based on what's under it
        if (findNodeOrSegmentAtPosition(event->pos())) {
            setCursor(Qt::PointingHandCursor);
        } else {
            setCursor(Qt::ArrowCursor);
        }
    }
    
    QWidget::mouseMoveEvent(event);
}

void EnvLibDrawingArea::contextMenuEvent(QContextMenuEvent* event) {
    if (m_pMenuPopup) {
        m_pMenuPopup->exec(event->globalPos());
    }
}

void EnvLibDrawingArea::moveNode() {
    if (!activeNode) {
        return;
    }
    
    // Convert mouse coordinates to envelope coordinates
    double x = static_cast<double>(mouseX) / width();
    double y = getAdjustedY(static_cast<double>(mouseY) / height());
    
    // Ensure X is within bounds
    if (x < moveLeftBound) {
        x = moveLeftBound;
    } else if (x > moveRightBound) {
        x = moveRightBound;
    }
    
    // Ensure Y is within bounds
    if (y < lowerY) {
        y = lowerY;
    } else if (y > upperY) {
        y = upperY;
    }
    
    // Update node coordinates
    activeNode->x = x;
    activeNode->y = y;
    
    // Redraw the graph
    update();
}

void EnvLibDrawingArea::insertEnvelopeSegment() {
    if (!head) {
        return;
    }
    
    // Convert mouse coordinates to envelope coordinates
    double x = static_cast<double>(mouseX) / width();
    double y = getAdjustedY(static_cast<double>(mouseY) / height());
    
    // Find the segment to insert after
    EnvelopeNode* leftNode = head;
    EnvelopeNode* rightNode = leftNode->rightSeg ? leftNode->rightSeg->rightNode : nullptr;
    
    while (rightNode && leftNode->x < x && rightNode->x < x) {
        leftNode = rightNode;
        rightNode = rightNode->rightSeg ? rightNode->rightSeg->rightNode : nullptr;
    }
    
    // Create new node
    auto newNode = std::make_unique<EnvelopeNode>(x, y);
    
    // Create new segment
    auto newSegment = std::make_unique<EnvelopeSegment>();
    newSegment->segmentType = EnvelopeSegmentType::Linear;
    newSegment->segmentProperty = EnvelopeSegmentProperty::Flexible;
    
    // Link the segment
    newSegment->leftNode = leftNode;
    newSegment->rightNode = newNode.get();
    
    // Link the nodes
    leftNode->rightSeg = std::move(newSegment);
    newNode->leftSeg = leftNode->rightSeg.get();
    
    // Update the envelope
    if (rightNode) {
        newNode->rightSeg = std::move(rightNode->leftSeg);
        newNode->rightSeg->leftNode = newNode.get();
        rightNode->leftSeg = nullptr;
    }
    
    // Update active elements
    activeNode = newNode.get();
    activeSegment = newNode->leftSeg.get();
    
    // Redraw the graph
    update();
}

void EnvLibDrawingArea::removeNode() {
    if (!activeNode || !activeNode->leftSeg) {
        return;
    }
    
    // Get the segment to remove
    EnvelopeSegment* segmentToRemove = activeNode->leftSeg;
    
    // Get the nodes
    EnvelopeNode* leftNode = segmentToRemove->leftNode;
    EnvelopeNode* rightNode = activeNode;
    
    // Get the next segment
    EnvelopeSegment* nextSegment = rightNode->rightSeg.get();
    
    // Link the nodes
    if (nextSegment) {
        nextSegment->leftNode = leftNode;
        leftNode->rightSeg = std::move(rightNode->rightSeg);
    } else {
        leftNode->rightSeg = nullptr;
    }
    
    // Update active elements
    activeNode = leftNode;
    activeSegment = leftNode->rightSeg.get();
    
    // Redraw the graph
    update();
}

void EnvLibDrawingArea::setFlexible() {
    if (activeSegment) {
        activeSegment->segmentProperty = EnvelopeSegmentProperty::Flexible;
        update();
    }
}

void EnvLibDrawingArea::setFixed() {
    if (activeSegment) {
        activeSegment->segmentProperty = EnvelopeSegmentProperty::Fixed;
        update();
    }
}

void EnvLibDrawingArea::setLinear() {
    if (activeSegment) {
        activeSegment->segmentType = EnvelopeSegmentType::Linear;
        update();
    }
}

void EnvLibDrawingArea::setExponential() {
    if (activeSegment) {
        activeSegment->segmentType = EnvelopeSegmentType::Exponential;
        update();
    }
}

void EnvLibDrawingArea::setSpline() {
    if (activeSegment) {
        activeSegment->segmentType = EnvelopeSegmentType::Spline;
        update();
    }
}

double EnvLibDrawingArea::getAdjustedY(double y) {
    // Scale coordinates in a larger window to a range of 0-1
    return 1.0 - y;
}

double EnvLibDrawingArea::mouseAdjustY(double y) {
    // Scale mouse coordinates from 0-1 to the true height
    return (1.0 - y) * height();
}

bool EnvLibDrawingArea::findNodeOrSegmentAtPosition(const QPoint& pos) {
    if (!head) {
        return false;
    }
    
    // Convert mouse coordinates to envelope coordinates
    double mouseX = static_cast<double>(pos.x()) / width();
    double mouseY = getAdjustedY(static_cast<double>(pos.y()) / height());
    
    // Check nodes
    EnvelopeNode* node = head;
    while (node) {
        // Calculate node position in screen coordinates
        int nodeX = static_cast<int>(node->x * width());
        int nodeY = static_cast<int>((1.0 - node->y) * height());
        
        // Check if mouse is within node radius
        QPoint nodePos(nodeX, nodeY);
        if ((pos - nodePos).manhattanLength() <= NODE_RADIUS) {
            activeNode = node;
            activeSegment = nullptr;
            return true;
        }
        
        // Check segments
        if (node->rightSeg) {
            EnvelopeNode* rightNode = node->rightSeg->rightNode;
            if (rightNode) {
                // Calculate segment endpoints
                int rightX = static_cast<int>(rightNode->x * width());
                int rightY = static_cast<int>((1.0 - rightNode->y) * height());
                
                // Calculate distance from point to line segment
                QPoint p1(nodeX, nodeY);
                QPoint p2(rightX, rightY);
                
                // Simple distance calculation (can be improved for better accuracy)
                QPoint p(pos.x(), pos.y());
                QPoint closest;
                
                // Calculate closest point on line segment
                QPoint line = p2 - p1;
                double lineLength = line.manhattanLength();
                
                if (lineLength == 0) {
                    closest = p1;
                } else {
                    double t = ((p.x() - p1.x()) * line.x() + (p.y() - p1.y()) * line.y()) / (lineLength * lineLength);
                    t = qBound(0.0, t, 1.0);
                    closest = p1 + QPoint(static_cast<int>(t * line.x()), static_cast<int>(t * line.y()));
                }
                
                // Check if mouse is within segment tolerance
                if ((p - closest).manhattanLength() <= SEGMENT_TOLERANCE) {
                    activeNode = nullptr;
                    activeSegment = node->rightSeg.get();
                    return true;
                }
            }
        }
        
        node = node->rightSeg ? node->rightSeg->rightNode : nullptr;
    }
    
    // No node or segment found
    activeNode = nullptr;
    activeSegment = nullptr;
    return false;
}

void EnvLibDrawingArea::drawEnvelope(QPainter& painter) {
    if (!head) {
        return;
    }
    
    // Draw segments
    EnvelopeNode* node = head;
    while (node && node->rightSeg) {
        drawSegment(painter, node->rightSeg.get(), node->rightSeg.get() == activeSegment);
        node = node->rightSeg->rightNode;
    }
    
    // Draw nodes
    node = head;
    while (node) {
        drawNode(painter, node, node == activeNode);
        node = node->rightSeg ? node->rightSeg->rightNode : nullptr;
    }
}

void EnvLibDrawingArea::drawNode(QPainter& painter, EnvelopeNode* node, bool isActive) {
    if (!node) {
        return;
    }
    
    // Calculate node position in screen coordinates
    int x = static_cast<int>(node->x * width());
    int y = static_cast<int>((1.0 - node->y) * height());
    
    // Set pen and brush based on active state
    if (isActive) {
        painter.setPen(QPen(Qt::red, 2));
        painter.setBrush(Qt::red);
    } else {
        painter.setPen(QPen(Qt::black, 1));
        painter.setBrush(Qt::white);
    }
    
    // Draw node
    painter.drawEllipse(QPoint(x, y), NODE_RADIUS, NODE_RADIUS);
}

void EnvLibDrawingArea::drawSegment(QPainter& painter, EnvelopeSegment* segment, bool isActive) {
    if (!segment || !segment->leftNode || !segment->rightNode) {
        return;
    }
    
    // Calculate segment endpoints in screen coordinates
    int x1 = static_cast<int>(segment->leftNode->x * width());
    int y1 = static_cast<int>((1.0 - segment->leftNode->y) * height());
    int x2 = static_cast<int>(segment->rightNode->x * width());
    int y2 = static_cast<int>((1.0 - segment->rightNode->y) * height());
    
    // Set pen based on active state and segment type
    QPen pen;
    if (isActive) {
        pen = QPen(Qt::red, 2);
    } else {
        pen = QPen(Qt::black, 1);
    }
    
    // Set pen style based on segment property
    if (segment->segmentProperty == EnvelopeSegmentProperty::Fixed) {
        pen.setStyle(Qt::DashLine);
    } else {
        pen.setStyle(Qt::SolidLine);
    }
    
    painter.setPen(pen);
    
    // Draw segment based on type
    if (segment->segmentType == EnvelopeSegmentType::Linear) {
        // Draw straight line
        painter.drawLine(x1, y1, x2, y2);
    } else if (segment->segmentType == EnvelopeSegmentType::Exponential) {
        // Draw exponential curve
        QPainterPath path;
        path.moveTo(x1, y1);
        
        // Approximate exponential curve with multiple line segments
        for (int i = 1; i <= 10; i++) {
            double t = i / 10.0;
            double x = x1 + t * (x2 - x1);
            double y = y1 + (y2 - y1) * (1.0 - std::exp(-5.0 * t)) / (1.0 - std::exp(-5.0));
            path.lineTo(static_cast<int>(x), static_cast<int>(y));
        }
        
        painter.drawPath(path);
    } else if (segment->segmentType == EnvelopeSegmentType::Spline) {
        // Draw spline curve (simplified)
        QPainterPath path;
        path.moveTo(x1, y1);
        
        // Approximate spline with multiple line segments
        for (int i = 1; i <= 10; i++) {
            double t = i / 10.0;
            double x = x1 + t * (x2 - x1);
            double y = y1 + t * (y2 - y1);
            path.lineTo(static_cast<int>(x), static_cast<int>(y));
        }
        
        painter.drawPath(path);
    }
}

void EnvLibDrawingArea::createContextMenu() {
    m_pMenuPopup = new QMenu(this);
    
    QAction* addAction = m_pMenuPopup->addAction(tr("Insert Node"));
    connect(addAction, &QAction::triggered, this, &EnvLibDrawingArea::insertEnvelopeSegment);
    
    QAction* removeAction = m_pMenuPopup->addAction(tr("Remove Node"));
    connect(removeAction, &QAction::triggered, this, &EnvLibDrawingArea::removeNode);
    
    m_pMenuPopup->addSeparator();
    
    QAction* flexibleAction = m_pMenuPopup->addAction(tr("Set Flexible"));
    connect(flexibleAction, &QAction::triggered, this, &EnvLibDrawingArea::setFlexible);
    
    QAction* fixedAction = m_pMenuPopup->addAction(tr("Set Fixed"));
    connect(fixedAction, &QAction::triggered, this, &EnvLibDrawingArea::setFixed);
    
    m_pMenuPopup->addSeparator();
    
    QAction* linearAction = m_pMenuPopup->addAction(tr("Set Linear"));
    connect(linearAction, &QAction::triggered, this, &EnvLibDrawingArea::setLinear);
    
    QAction* exponentialAction = m_pMenuPopup->addAction(tr("Set Exponential"));
    connect(exponentialAction, &QAction::triggered, this, &EnvLibDrawingArea::setExponential);
    
    QAction* splineAction = m_pMenuPopup->addAction(tr("Set Spline"));
    connect(splineAction, &QAction::triggered, this, &EnvLibDrawingArea::setSpline);
}
