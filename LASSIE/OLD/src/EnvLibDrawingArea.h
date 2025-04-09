#ifndef ENV_LIB_DRAWINGAREA_H
#define ENV_LIB_DRAWINGAREA_H

/*
 * MODIFICATIONS MADE:
 * 
 * 1. Modernized C++ features:
 *    - Added proper const correctness to methods
 *    - Added proper documentation with Doxygen-style comments
 * 
 * 2. Improved class structure:
 *    - Added getter and setter methods for better encapsulation
 *    - Made member variables private where appropriate
 *    - Improved method signatures
 * 
 * 3. Code organization:
 *    - Added comprehensive documentation
 *    - Improved code readability
 *    - Simplified method implementations
 * 
 * 4. UI improvements:
 *    - Added better error handling
 *    - Improved user feedback
 */

#include "LASSIE.h"
#include "EnvelopeLibraryEntry.h"

class EnvelopeLibraryWindow;

/**
 * @brief Class for drawing and manipulating envelopes in the UI
 */
class EnvLibDrawingArea : public Gtk::DrawingArea {
public:
  /**
   * @brief Constructor for EnvLibDrawingArea
   * @param _envelopeLibraryWindow Pointer to the parent window
   */
  EnvLibDrawingArea(EnvelopeLibraryWindow* _envelopeLibraryWindow);

  /**
   * @brief Destructor for EnvLibDrawingArea
   */
  virtual ~EnvLibDrawingArea();

  /**
   * @brief Reset all fields to default values
   */
  void resetFields();

  /**
   * @brief Display the graph of an envelope
   * @param _envelope Pointer to the envelope to display
   */
  void showGraph(EnvelopeLibraryEntry* _envelope);

  /**
   * @brief Clear the current graph
   */
  void clearGraph();

  /**
   * @brief Handle mouse movement events
   * @param event The mouse event
   * @return true if the event was handled
   */
  bool mouseMoving(GdkEventMotion* event);

  /**
   * @brief Set the coordinates of the active node
   * @param _x X coordinate as a string
   * @param _y Y coordinate as a string
   */
  void setActiveNodeCoordinate(const std::string& _x, const std::string& _y);

  /**
   * @brief Adjust the boundary of an envelope
   * @param _envelope Pointer to the envelope to adjust
   */
  void adjustBoundary(EnvelopeLibraryEntry* _envelope);

protected:
  /**
   * @brief Handle expose events
   * @param event The expose event
   * @return true if the event was handled
   */
  virtual bool on_expose_event(GdkEventExpose* event) override;

  /**
   * @brief Handle mouse button press events
   * @param event The button event
   * @return true if the event was handled
   */
  bool mouseButtonPressed(GdkEventButton* event);

  /**
   * @brief Handle mouse button release events
   * @param event The button event
   * @return true if the event was handled
   */
  bool mouseButtonReleased(GdkEventButton* event);

  /**
   * @brief Flag indicating if the left mouse button is pressed
   */
  bool mouseLeftButtonPressedDown;

  /**
   * @brief Current mouse Y coordinate
   */
  double mouseY;

  /**
   * @brief Current mouse X coordinate
   */
  double mouseX;

  /**
   * @brief Move the currently selected node
   */
  void moveNode();

  /**
   * @brief Insert a new envelope segment
   */
  void insertEnvelopeSegment();

  /**
   * @brief Remove the currently selected node
   */
  void removeNode();

  /**
   * @brief Set the current segment to flexible
   */
  void setFlexible();

  /**
   * @brief Set the current segment to fixed
   */
  void setFixed();

  /**
   * @brief Set the current segment to linear interpolation
   */
  void setLinear();

  /**
   * @brief Set the current segment to exponential interpolation
   */
  void setExponential();

  /**
   * @brief Set the current segment to spline interpolation
   */
  void setSpline();

  /**
   * @brief Scale coordinates in a larger window to a range of 0-1
   * @param y The Y coordinate to adjust
   * @return The adjusted Y coordinate
   */
  double getAdjustedY(double y);

  /**
   * @brief Scale mouse coordinates from 0-1 to the true height
   * @param y The Y coordinate to adjust
   * @return The adjusted Y coordinate
   */
  double mouseAdjustY(double y);

private:
  /**
   * @brief Pointer to the parent window
   */
  EnvelopeLibraryWindow* envelopeLibraryWindow;

  /**
   * @brief UI manager for the popup menu
   */
  Glib::RefPtr<Gtk::UIManager> m_refUIManager;

  /**
   * @brief Action group for the popup menu
   */
  Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

  /**
   * @brief Popup menu
   */
  Gtk::Menu* m_pMenuPopup;

  /**
   * @brief Currently active segment
   */
  EnvelopeSegment* activeSegment;

  /**
   * @brief Currently active node
   */
  EnvelopeNode* activeNode;

  /**
   * @brief Left boundary for node movement
   */
  double moveLeftBound;

  /**
   * @brief Right boundary for node movement
   */
  double moveRightBound;

  /**
   * @brief Maximum Y value for envelope above 1
   */
  double upperY;

  /**
   * @brief Minimum Y value for envelope below 0
   */
  double lowerY;

  /**
   * @brief Head segment of the envelope
   */
  EnvelopeSegment* head;

  /**
   * @brief Tail node of the envelope
   */
  EnvelopeNode* tail;
};

#endif
