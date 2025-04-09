/*******************************************************************************
 *
 *  File Name     : EnvelopeLibraryEntry.cpp
 *  Date created  : May.17 2010
 *  Authors       : Ming-ching Chiu, Sever Tipei
 *  Organization  : Music School, University of Illinois at Urbana Champaign
 *  Description   : This file contains the class "EnvelopeLibraryEntry" of
 *                  LASSIE. Each EnvelopeLibraryEntry holds the information of
 *                  a premade envelope in the envelope library. It is also a
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

#include "EnvelopeLibraryEntry.h"
#include <sstream>

// EnvelopeNode implementation
EnvelopeNode::EnvelopeNode(double _x, double _y) : 
  x(_x), y(_y), leftSeg(nullptr), rightSeg(nullptr) {
}

int EnvelopeNode::countNumOfNodes() const {
  return (rightSeg == nullptr) ? 1 : 1 + rightSeg->rightNode->countNumOfNodes();
}

// EnvelopeSegment implementation
EnvelopeSegment::EnvelopeSegment() :
  leftNode(nullptr),
  rightNode(nullptr),
  segmentType(EnvelopeSegmentType::Linear),
  segmentProperty(EnvelopeSegmentProperty::Flexible) {
}

EnvelopeSegment::~EnvelopeSegment() {
  // No need to delete nodes as they are managed by the envelope
}

// EnvelopeLibraryEntry implementation
EnvelopeLibraryEntry::EnvelopeLibraryEntry(int _number) :
  number(_number),
  next(nullptr),
  prev(nullptr) {
  
  // Create initial envelope with two nodes and one segment
  head = std::make_unique<EnvelopeNode>(0, 0);
  
  auto rightSeg = std::make_unique<EnvelopeSegment>();
  rightSeg->leftNode = head.get();
  head->rightSeg = std::move(rightSeg);
  
  auto rightNode = std::make_unique<EnvelopeNode>(1, 0);
  head->rightSeg->rightNode = rightNode.get();
  rightNode->leftSeg = head->rightSeg.get();
}

EnvelopeLibraryEntry::EnvelopeLibraryEntry(EnvelopeLibraryEntry* _originalEnvelope, int _number) :
  number(_number),
  next(nullptr),
  prev(nullptr) {
  
  if (!_originalEnvelope || !_originalEnvelope->head) {
    // Create a default envelope if the original is invalid
    head = std::make_unique<EnvelopeNode>(0, 0);
    auto rightSeg = std::make_unique<EnvelopeSegment>();
    rightSeg->leftNode = head.get();
    head->rightSeg = std::move(rightSeg);
    
    auto rightNode = std::make_unique<EnvelopeNode>(1, 0);
    head->rightSeg->rightNode = rightNode.get();
    rightNode->leftSeg = head->rightSeg.get();
    return;
  }
  
  // Copy the original envelope
  EnvelopeNode* originalCurrentNode = _originalEnvelope->head.get();
  
  // Create the head node
  head = std::make_unique<EnvelopeNode>(originalCurrentNode->x, originalCurrentNode->y);
  EnvelopeNode* currentNewNode = head.get();
  
  // Copy the rest of the nodes and segments
  while (originalCurrentNode->rightSeg) {
    // Create a new segment
    auto newSegment = std::make_unique<EnvelopeSegment>();
    newSegment->segmentType = originalCurrentNode->rightSeg->segmentType;
    newSegment->segmentProperty = originalCurrentNode->rightSeg->segmentProperty;
    newSegment->leftNode = currentNewNode;
    
    // Link the segment to the current node
    currentNewNode->rightSeg = std::move(newSegment);
    
    // Create a new right node
    auto newNode = std::make_unique<EnvelopeNode>(
      originalCurrentNode->rightSeg->rightNode->x,
      originalCurrentNode->rightSeg->rightNode->y
    );
    
    // Link the segment to the new node
    currentNewNode->rightSeg->rightNode = newNode.get();
    newNode->leftSeg = currentNewNode->rightSeg.get();
    
    // Move to the next node
    originalCurrentNode = originalCurrentNode->rightSeg->rightNode;
    currentNewNode = newNode.get();
  }
}

EnvelopeLibraryEntry::~EnvelopeLibraryEntry() {
  // The unique_ptr will automatically delete the head node and all segments
  // No need to manually delete anything
}

int EnvelopeLibraryEntry::count() const {
  if (next == nullptr) return 1;
  return next->count() + 1;
}

EnvelopeLibraryEntry* EnvelopeLibraryEntry::createNewEnvelope() {
  if (next != nullptr) return next->createNewEnvelope();
  
  next = new EnvelopeLibraryEntry(number + 1);
  next->prev = this;
  return next;
}

EnvelopeLibraryEntry* EnvelopeLibraryEntry::duplicateEnvelope(EnvelopeLibraryEntry* _originalEnvelope) {
  if (next != nullptr) return next->duplicateEnvelope(_originalEnvelope);
  
  next = new EnvelopeLibraryEntry(_originalEnvelope, number + 1);
  next->prev = this;
  return next;
}

Glib::ustring EnvelopeLibraryEntry::getNumberString() const {
  std::ostringstream oss;
  oss << number;
  return oss.str();
}

EnvelopeLibraryEntry::EnvelopeLibraryEntry(Envelope* _envelope, int _number) :
  number(_number),
  next(nullptr),
  prev(nullptr) {
  
  if (!_envelope) {
    // Create a default envelope if the input is invalid
    head = std::make_unique<EnvelopeNode>(0, 0);
    auto rightSeg = std::make_unique<EnvelopeSegment>();
    rightSeg->leftNode = head.get();
    head->rightSeg = std::move(rightSeg);
    
    auto rightNode = std::make_unique<EnvelopeNode>(1, 0);
    head->rightSeg->rightNode = rightNode.get();
    rightNode->leftSeg = head->rightSeg.get();
    return;
  }
  
  Collection<envelope_segment>* segments = _envelope->getSegments();
  if (!segments || segments->size() < 2) {
    // Create a default envelope if there are not enough segments
    head = std::make_unique<EnvelopeNode>(0, 0);
    auto rightSeg = std::make_unique<EnvelopeSegment>();
    rightSeg->leftNode = head.get();
    head->rightSeg = std::move(rightSeg);
    
    auto rightNode = std::make_unique<EnvelopeNode>(1, 0);
    head->rightSeg->rightNode = rightNode.get();
    rightNode->leftSeg = head->rightSeg.get();
    return;
  }
  
  // Create the head node
  head = std::make_unique<EnvelopeNode>(segments->get(0).x, segments->get(0).y);
  EnvelopeNode* currentNode = head.get();
  
  // Create the rest of the nodes and segments
  for (int i = 0; i < segments->size() - 1; i++) {
    // Create a new segment
    auto newSegment = std::make_unique<EnvelopeSegment>();
    
    // Set segment properties
    if (_envelope->getSegmentLengthType(i) == FIXED) {
      newSegment->segmentProperty = EnvelopeSegmentProperty::Fixed;
    } else {
      newSegment->segmentProperty = EnvelopeSegmentProperty::Flexible;
    }
    
    // Set segment type
    if (_envelope->getSegmentInterpolationType(i) == EXPONENTIAL) {
      newSegment->segmentType = EnvelopeSegmentType::Exponential;
    } else if (_envelope->getSegmentInterpolationType(i) == CUBIC_SPLINE) {
      newSegment->segmentType = EnvelopeSegmentType::Spline;
    } else {
      newSegment->segmentType = EnvelopeSegmentType::Linear;
    }
    
    // Link the segment to the current node
    newSegment->leftNode = currentNode;
    currentNode->rightSeg = std::move(newSegment);
    
    // Create a new right node
    auto newNode = std::make_unique<EnvelopeNode>(
      segments->get(i + 1).x,
      segments->get(i + 1).y
    );
    
    // Link the segment to the new node
    currentNode->rightSeg->rightNode = newNode.get();
    newNode->leftSeg = currentNode->rightSeg.get();
    
    // Move to the next node
    currentNode = newNode.get();
  }
}
