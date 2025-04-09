/*******************************************************************************
 *
 *  File Name     : EnvelopeLibraryEntry.h
 *  Date created  : May.17 2010
 *  Authors       : Ming-ching Chiu, Sever Tipei
 *  Organization  : Music School, University of Illinois at Urbana Champaign
 *  Description   : This file contains the class "EnvelopeLibraryEntry" of 
 *                  LASSIE. Each EnvelopeLibraryEntry holds the information of
                    a premade envelope in the envelope library.It is also a 
 *                  doubly-linked list.                  
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
 * 1. Modernized C++ features:
 *    - Replaced C-style enums with enum class for better type safety
 *    - Added proper const correctness to methods
 *    - Added proper documentation with Doxygen-style comments
 * 
 * 2. Improved class structure:
 *    - Renamed classes to be more descriptive (EnvLibEntryNode → EnvelopeNode, 
 *      EnvLibEntrySeg → EnvelopeSegment)
 *    - Added getter and setter methods for better encapsulation
 *    - Made member variables private where appropriate
 * 
 * 3. Memory management:
 *    - Used std::unique_ptr for automatic memory management (better than new and delete)
 *    - Added proper destructor implementation (~EnvelopeLibraryEntry) 
 
 */

#ifndef ENVELOPE_LIBRARY_ENTRY_H
#define ENVELOPE_LIBRARY_ENTRY_H

#include "LASSIE.h"
#include <memory>
#include <string>

/**
 * @brief Type of envelope segment interpolation
 */
enum class EnvelopeSegmentType {
  Linear,
  Exponential,
  Spline
};

/**
 * @brief Property of envelope segment (fixed or flexible)
 */
enum class EnvelopeSegmentProperty {
  Flexible,
  Fixed
};

/**
 * @brief Class representing a node in the envelope
 */
class EnvelopeNode {
public:
  double x;
  double y;
  std::unique_ptr<EnvelopeSegment> leftSeg;
  std::unique_ptr<EnvelopeSegment> rightSeg;
  
  /**
   * @brief Constructor for EnvelopeNode
   * @param _x X coordinate
   * @param _y Y coordinate
   */
  EnvelopeNode(double _x, double _y);
  
  /**
   * @brief Count the number of nodes in the envelope
   * @return Number of nodes
   */
  int countNumOfNodes() const;
};

/**
 * @brief Class representing a segment in the envelope
 */
class EnvelopeSegment {
public:
  EnvelopeNode* leftNode;
  EnvelopeNode* rightNode;
  
  EnvelopeSegmentType segmentType;
  EnvelopeSegmentProperty segmentProperty;
  
  /**
   * @brief Constructor for EnvelopeSegment
   */
  EnvelopeSegment();
  
  /**
   * @brief Destructor for EnvelopeSegment
   */
  ~EnvelopeSegment();
};

/**
 * @brief Class representing an envelope in the library
 */
class EnvelopeLibraryEntry {
public:
  /**
   * @brief Constructor for EnvelopeLibraryEntry with a number
   * @param _number The number of the envelope
   */
  EnvelopeLibraryEntry(int _number);
  
  /**
   * @brief Constructor for EnvelopeLibraryEntry from an existing envelope
   * @param _envelope The envelope to copy
   * @param _number The number of the envelope
   */
  EnvelopeLibraryEntry(Envelope* _envelope, int _number);
  
  /**
   * @brief Constructor for EnvelopeLibraryEntry from an existing EnvelopeLibraryEntry
   * @param _originalEnvelope The envelope to copy
   * @param _number The number of the envelope
   */
  EnvelopeLibraryEntry(EnvelopeLibraryEntry* _originalEnvelope, int _number);
  
  /**
   * @brief Destructor for EnvelopeLibraryEntry
   */
  ~EnvelopeLibraryEntry();
  
  /**
   * @brief Count the number of envelopes in the list
   * @return Number of envelopes
   */
  int count() const;
  
  /**
   * @brief Create a new envelope
   * @return Pointer to the new envelope
   */
  EnvelopeLibraryEntry* createNewEnvelope();
  
  /**
   * @brief Duplicate an existing envelope
   * @param _originalEnvelope The envelope to duplicate
   * @return Pointer to the duplicated envelope
   */
  EnvelopeLibraryEntry* duplicateEnvelope(EnvelopeLibraryEntry* _originalEnvelope);
  
  /**
   * @brief Get the number of the envelope as a string
   * @return The number as a string
   */
  Glib::ustring getNumberString() const;
  
  /**
   * @brief Get the head node of the envelope
   * @return Pointer to the head node
   */
  EnvelopeNode* getHead() const { return head.get(); }
  
  /**
   * @brief Get the number of the envelope
   * @return The number of the envelope
   */
  int getNumber() const { return number; }
  
  /**
   * @brief Get the next envelope in the list
   * @return Pointer to the next envelope
   */
  EnvelopeLibraryEntry* getNext() const { return next; }
  
  /**
   * @brief Get the previous envelope in the list
   * @return Pointer to the previous envelope
   */
  EnvelopeLibraryEntry* getPrev() const { return prev; }
  
  /**
   * @brief Set the next envelope in the list
   * @param _next Pointer to the next envelope
   */
  void setNext(EnvelopeLibraryEntry* _next) { next = _next; }
  
  /**
   * @brief Set the previous envelope in the list
   * @param _prev Pointer to the previous envelope
   */
  void setPrev(EnvelopeLibraryEntry* _prev) { prev = _prev; }

private:
  std::unique_ptr<EnvelopeNode> head;
  int number;
  EnvelopeLibraryEntry* next;
  EnvelopeLibraryEntry* prev;
};

#endif
