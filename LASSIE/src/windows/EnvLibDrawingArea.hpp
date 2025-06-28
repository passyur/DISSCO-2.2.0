#ifndef ENV_LIB_DRAWINGAREA_HPP
#define ENV_LIB_DRAWINGAREA_HPP

#include <QWidget>
#include <QMenu>
#include <QAction>

class EnvelopeLibraryEntry;
class EnvLibEntrySeg;
class EnvLibEntryNode;
class EnvelopeLibraryWindow;

/**
 * @brief Custom widget for drawing and editing envelope graphs
 */
class EnvLibDrawingArea : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor: set up state, enable mouse tracking, and build context menu
     * @param _envelopeLibraryWindow  parent window reference
     * @param parent  parent widget
     */
    explicit EnvLibDrawingArea(EnvelopeLibraryWindow* _envelopeLibraryWindow,
                               QWidget* parent = nullptr);

    /**
     * @brief Destructor: Qt cleans up child widgets/actions automatically
     */
    ~EnvLibDrawingArea() override;

    /**
     * @brief Reset all internal fields to their defaults
     */
    void resetFields();

    /**
     * @brief Trigger a redraw of the given envelope
     * @param _envelope  envelope to display
     */
    void showGraph(EnvelopeLibraryEntry* _envelope);

    /**
     * @brief Clear the drawing area (background will repaint white)
     */
    void clearGraph();

    /**
     * @brief Update the active node's coords from external input
     * @param _x  x-coordinate value
     * @param _y  y-coordinate value
     */
    void setActiveNodeCoordinate(const QString& _x, const QString& _y);

    /**
     * @brief Recompute upperY/lowerY based on envelope data
     * @param _envelope  envelope to analyze
     */
    void adjustBoundary(EnvelopeLibraryEntry* _envelope);

protected:
    /**
     * @brief Paint the envelope graph
     * @param event  paint event
     */
    void paintEvent(QPaintEvent* event) override;

    /**
     * @brief Track mouse movement (update coords display & drag)
     * @param event  mouse move event
     */
    void mouseMoveEvent(QMouseEvent* event) override;

    /**
     * @brief Handle presses (select node, start drag, show popup)
     * @param event  mouse press event
     */
    void mousePressEvent(QMouseEvent* event) override;

    /**
     * @brief Handle releases (end drag)
     * @param event  mouse release event
     */
    void mouseReleaseEvent(QMouseEvent* event) override;

    /**
     * @brief Fallback for context menu key events
     * @param event  context menu event
     */
    void contextMenuEvent(QContextMenuEvent* event) override;

private slots:
    /////// Context-menu actions ///////
    /**
     * @brief Insert a new node at the mouse position
     */
    void insertEnvelopeSegment();
    
    /**
     * @brief Remove the currently active node
     */
    void removeNode();
    
    /**
     * @brief Set the segment at mouse position to flexible length
     */
    void setFlexible();
    
    /**
     * @brief Set the segment at mouse position to fixed length
     */
    void setFixed();
    
    /**
     * @brief Set the segment at mouse position to linear interpolation
     */
    void setLinear();
    
    /**
     * @brief Set the segment at mouse position to spline interpolation
     */
    void setSpline();
    
    /**
     * @brief Set the segment at mouse position to exponential interpolation
     */
    void setExponential();

private:
    /////// Internal helpers ///////
    /**
     * @brief Actually move the active node during drag
     */
    void moveNode();
    
    /**
     * @brief Map true y → [0,1] for drawing
     * @param y  true y-coordinate
     * @return adjusted y-coordinate for drawing
     */
    double getAdjustedY(double y) const;
    
    /**
     * @brief Map [0,1] mouse y back to true y
     * @param y  mouse y-coordinate [0,1]
     * @return true y-coordinate
     */
    double mouseAdjustY(double y) const;

    EnvelopeLibraryWindow* envelopeLibraryWindow; ///< reference to parent window

    QMenu*   m_pMenuPopup;        ///< context menu for right-click actions
    QAction* actionInsert;        ///< insert node action
    QAction* actionRemove;        ///< remove node action
    QAction* actionSetFlexible;   ///< set flexible action
    QAction* actionSetFixed;      ///< set fixed action
    QAction* actionSetLinear;     ///< set linear action
    QAction* actionSetSpline;     ///< set spline action
    QAction* actionSetExponential; ///< set exponential action

    EnvLibEntrySeg*    activeSegment; ///< currently selected segment
    EnvLibEntryNode*   activeNode;    ///< currently selected node
    bool               mouseLeftButtonPressedDown; ///< mouse drag state
    double             mouseX, mouseY; ///< current mouse coordinates

    double moveLeftBound, moveRightBound; ///< drag boundaries
    double upperY, lowerY;                ///< display boundaries

    EnvLibEntrySeg* head;   ///< head of segment list
    EnvLibEntryNode* tail;  ///< tail of node list
};

#endif // ENV_LIB_DRAWINGAREA_HPP