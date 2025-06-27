#include "EnvelopeLibraryEntry.h"
#include <cstdio>

//------------------------------------------------------------------------------
// EnvLibEntrySeg
//------------------------------------------------------------------------------
EnvLibEntrySeg::EnvLibEntrySeg()
  : leftNode(nullptr),
    rightNode(nullptr),
    segmentType(envSegmentTypeLinear),
    segmentProperty(envSegmentPropertyFlexible)
{}

//------------------------------------------------------------------------------
// EnvLibEntryNode
//------------------------------------------------------------------------------
EnvLibEntryNode::EnvLibEntryNode(double _x, double _y)
  : x(_x),
    y(_y),
    leftSeg(nullptr),
    rightSeg(nullptr)
{}

int EnvLibEntryNode::countNumOfNodes() {
  // If no outgoing segment, just this one; otherwise 1 + nodes to the right
  return (rightSeg == nullptr)
         ? 1
         : 1 + rightSeg->rightNode->countNumOfNodes();
}

//------------------------------------------------------------------------------
// EnvelopeLibraryEntry
//------------------------------------------------------------------------------
EnvelopeLibraryEntry::EnvelopeLibraryEntry(int _number)
  : number(_number),
    prev(nullptr),
    next(nullptr)
{
  // initialize a simple two-node envelope: (0,0) → (1,0)
  head = new EnvLibEntryNode(0.0, 0.0);
  head->rightSeg = new EnvLibEntrySeg();
  head->rightSeg->leftNode = head;

  EnvLibEntryNode* tail = new EnvLibEntryNode(1.0, 0.0);
  head->rightSeg->rightNode = tail;
  tail->leftSeg = head->rightSeg;
}

EnvelopeLibraryEntry::EnvelopeLibraryEntry(EnvelopeLibraryEntry* _originalEnvelope, int _number)
  : number(_number),
    prev(nullptr),
    next(nullptr)
{
  // duplicate linked nodes/segments from original
  EnvLibEntryNode* origNode = _originalEnvelope->head;
  head = new EnvLibEntryNode(origNode->x, origNode->y);
  EnvLibEntryNode* dupNode = head;

  while (origNode->rightSeg != nullptr) {
    // copy segment properties
    EnvLibEntrySeg* newSeg = new EnvLibEntrySeg();
    newSeg->segmentType     = origNode->rightSeg->segmentType;
    newSeg->segmentProperty = origNode->rightSeg->segmentProperty;

    dupNode->rightSeg = newSeg;
    newSeg->leftNode  = dupNode;

    // step forward in original
    origNode = origNode->rightSeg->rightNode;

    // create new node
    EnvLibEntryNode* newNode = new EnvLibEntryNode(origNode->x, origNode->y);
    newSeg->rightNode = newNode;
    newNode->leftSeg  = newSeg;
    dupNode = newNode;
  }
}

EnvelopeLibraryEntry::EnvelopeLibraryEntry(Envelope* _envelope, int _number)
  : number(_number),
    prev(nullptr),
    next(nullptr)
{
  auto segments = _envelope->getSegments();
  EnvLibEntryNode* prevNode = nullptr;
  EnvLibEntrySeg*  prevSeg  = nullptr;

  // build all but last segment
  int lastIndex = segments->size() - 1;
  for (int i = 0; i < lastIndex; ++i) {
    // create node
    EnvLibEntryNode* node = new EnvLibEntryNode(segments->get(i).x, segments->get(i).y);
    if (i == 0) head = node;
    if (prevSeg) {
      prevSeg->rightNode = node;
      node->leftSeg      = prevSeg;
    }

    // create segment
    EnvLibEntrySeg* seg = new EnvLibEntrySeg();
    node->rightSeg = seg;
    seg->leftNode  = node;

    // set fixed/flexible
    seg->segmentProperty = 
        (_envelope->getSegmentLengthType(i) == FIXED)
        ? envSegmentPropertyFixed
        : envSegmentPropertyFlexible;

    // set interpolation type
    switch (_envelope->getSegmentInterpolationType(i)) {
      case EXPONENTIAL:  seg->segmentType = envSegmentTypeExponential; break;
      case CUBIC_SPLINE: seg->segmentType = envSegmentTypeSpline;      break;
      default:            seg->segmentType = envSegmentTypeLinear;      break;
    }

    prevNode = node;
    prevSeg  = seg;
  }

  // final node
  EnvLibEntryNode* finalNode = new EnvLibEntryNode(
    segments->get(lastIndex).x,
    segments->get(lastIndex).y
  );
  prevSeg->rightNode = finalNode;
  finalNode->leftSeg = prevSeg;
}

EnvelopeLibraryEntry::~EnvelopeLibraryEntry() {
  // any cleanup logic would go here (delete nodes/segments) if needed
}

int EnvelopeLibraryEntry::count() {
  return (next == nullptr) ? 1 : 1 + next->count();
}

EnvelopeLibraryEntry* EnvelopeLibraryEntry::createNewEnvelope() {
  if (next)
    return next->createNewEnvelope();

  next       = new EnvelopeLibraryEntry(number + 1);
  next->prev = this;
  return next;
}

EnvelopeLibraryEntry* EnvelopeLibraryEntry::duplicateEnvelope(EnvelopeLibraryEntry* _originalEnvelope) {
  if (next)
    return next->duplicateEnvelope(_originalEnvelope);

  next       = new EnvelopeLibraryEntry(_originalEnvelope, number + 1);
  next->prev = this;
  return next;
}

QString EnvelopeLibraryEntry::getNumberString() const {
  return QString::number(number);
} 