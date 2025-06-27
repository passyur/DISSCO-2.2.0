#ifndef ENV_LIB_DRAWINGAREA_H
#define ENV_LIB_DRAWINGAREA_H

#include <QWidget>
#include <QMenu>
#include <QAction>

class EnvelopeLibraryEntry;
class EnvLibEntrySeg;
class EnvLibEntryNode;
class EnvelopeLibraryWindow;

class EnvLibDrawingArea : public QWidget
{
    Q_OBJECT

public:
    /// Constructor: set up state, enable mouse tracking, and build context menu
    explicit EnvLibDrawingArea(EnvelopeLibraryWindow* _envelopeLibraryWindow,
                               QWidget* parent = nullptr);

    /// Destructor: Qt cleans up child widgets/actions automatically
    ~EnvLibDrawingArea() override;

    /// Reset all internal fields to their defaults
    void resetFields();

    /// Trigger a redraw of the given envelope
    void showGraph(EnvelopeLibraryEntry* _envelope);

    /// Clear the drawing area (background will repaint white)
    void clearGraph();

    /// Update the active node's coords from external input
    void setActiveNodeCoordinate(const QString& _x, const QString& _y);

    /// Recompute upperY/lowerY based on envelope data
    void adjustBoundary(EnvelopeLibraryEntry* _envelope);

protected:
    /// Paint the envelope graph
    void paintEvent(QPaintEvent* event) override;

    /// Track mouse movement (update coords display & drag)
    void mouseMoveEvent(QMouseEvent* event) override;

    /// Handle presses (select node, start drag, show popup)
    void mousePressEvent(QMouseEvent* event) override;

    /// Handle releases (end drag)
    void mouseReleaseEvent(QMouseEvent* event) override;

    /// Fallback for context menu key events
    void contextMenuEvent(QContextMenuEvent* event) override;

private slots:
    /////// Context-menu actions ///////
    void insertEnvelopeSegment();
    void removeNode();
    void setFlexible();
    void setFixed();
    void setLinear();
    void setExponential();
    void setSpline();

private:
    /////// Internal helpers ///////
    /// Actually move the active node during drag
    void moveNode();
    /// Map true y → [0,1] for drawing
    double getAdjustedY(double y) const;
    /// Map [0,1] mouse y back to true y
    double mouseAdjustY(double y) const;

    EnvelopeLibraryWindow* envelopeLibraryWindow;

    QMenu*   m_pMenuPopup;
    QAction* actionInsert;
    QAction* actionRemove;
    QAction* actionSetFlexible;
    QAction* actionSetFixed;
    QAction* actionSetLinear;
    QAction* actionSetSpline;
    QAction* actionSetExponential;

    EnvLibEntrySeg*    activeSegment;
    EnvLibEntryNode*   activeNode;
    bool               mouseLeftButtonPressedDown;
    double             mouseX, mouseY;

    double moveLeftBound, moveRightBound;
    double upperY, lowerY;

    EnvLibEntrySeg* head;
    EnvLibEntryNode* tail;
};

#endif // ENV_LIB_DRAWINGAREA_H 