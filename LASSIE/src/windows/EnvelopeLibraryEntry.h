#ifndef ENVELOPE_LIBRARY_ENTRY_H
#define ENVELOPE_LIBRARY_ENTRY_H

#include <QString>
#include "LASSIE.h"
#include "../../LASS/src/Envelope.h"

/**
 * @brief Types of interpolation between nodes
 */
enum envSegmentType {
  envSegmentTypeLinear,      ///< Linear interpolation
  envSegmentTypeExponential, ///< Exponential interpolation
  envSegmentTypeSpline       ///< Cubic spline interpolation
};

/**
 * @brief Whether segment length is fixed or flexible
 */
enum envSegmentProperty {
  envSegmentPropertyFlexible, ///< Flexible segment length
  envSegmentPropertyFixed     ///< Fixed segment length
};

class EnvLibEntryNode;

/**
 * @brief Represents a segment between two nodes in an envelope
 */
class EnvLibEntrySeg {
public:
  EnvLibEntryNode* leftNode;   ///< pointer to node at start of this segment
  EnvLibEntryNode* rightNode;  ///< pointer to node at end of this segment
  envSegmentType segmentType;      ///< interpolation type
  envSegmentProperty segmentProperty;  ///< length property

  /** 
   * @brief Default constructor: sets up a linear, flexible segment 
   */
  EnvLibEntrySeg();

  /** @brief Default destructor */
  ~EnvLibEntrySeg() {}
};

/**
 * @brief Represents a node (control point) in an envelope
 */
class EnvLibEntryNode {
public:
  double x;                 ///< time or parameter value
  double y;                 ///< amplitude or value
  EnvLibEntrySeg* leftSeg;  ///< segment coming into this node
  EnvLibEntrySeg* rightSeg; ///< segment leaving this node

  /**
   * @brief Construct a node at coordinates (_x, _y)
   * @param _x  x-coordinate
   * @param _y  y-coordinate
   */
  EnvLibEntryNode(double _x, double _y);

  /** @brief Default destructor */
  ~EnvLibEntryNode() {}

  /**
   * @brief Count this node plus all nodes reachable to the right
   * @return number of nodes from this to list end
   */
  int countNumOfNodes();
};

/**
 * @brief Represents an envelope entry in the library
 */
class EnvelopeLibraryEntry {
public:
  EnvLibEntryNode* head;          ///< head of the node/segment list
  int number;                     ///< envelope ID
  EnvelopeLibraryEntry* next;     ///< next in the library list
  EnvelopeLibraryEntry* prev;     ///< previous in the library list

  /**
   * @brief Create a fresh envelope with given index
   * @param _number  index for this entry
   */
  EnvelopeLibraryEntry(int _number);

  /**
   * @brief Copy-construct an entry from an existing one
   * @param _originalEnvelope  entry to duplicate
   * @param _number            new index
   */
  EnvelopeLibraryEntry(EnvelopeLibraryEntry* _originalEnvelope, int _number);

  /**
   * @brief Build from an LASSIE Envelope object
   * @param _envelope  source Envelope
   * @param _number    new index
   */
  EnvelopeLibraryEntry(Envelope* _envelope, int _number);

  /** @brief Destructor (cleanup segments/nodes if desired) */
  ~EnvelopeLibraryEntry();

  /**
   * @brief Count how many entries in the doubly-linked list
   * @return total entries including this
   */
  int count();

  /**
   * @brief Append a new, empty envelope entry after this one
   * @return pointer to the new entry
   */
  EnvelopeLibraryEntry* createNewEnvelope();

  /**
   * @brief Append a duplicated envelope entry after this one
   * @param _originalEnvelope  which entry to duplicate
   * @return pointer to the new duplicate
   */
  EnvelopeLibraryEntry* duplicateEnvelope(EnvelopeLibraryEntry* _originalEnvelope);

  /**
   * @brief Get this entry's number as a QString
   * @return number converted to QString
   */
  QString getNumberString() const;
};

#endif // ENVELOPE_LIBRARY_ENTRY_H 