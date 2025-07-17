#ifndef ENVELOPE_LIBRARY_ENTRY_HPP
#define ENVELOPE_LIBRARY_ENTRY_HPP

#include <QString>
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

namespace EnvelopeUtilities {
    inline EnvelopeLibraryEntry* createNewEnvelopeHelper(EnvelopeLibraryEntry *entries){
        // ProjectManager *pm = Inst::get_project_manager();
        // EnvelopeLibraryEntry *entries = pm->envlibentries();
        if(entries == nullptr){
            entries = new EnvelopeLibraryEntry(1);
            return entries;
        }else{
            return entries->EnvelopeLibraryEntry::createNewEnvelope();
        }
    }

    /// \todo should be devolved to EnvelopeLibraryEntry instead of this deceptively named helper function
    inline EnvelopeLibraryEntry* duplicateEnvelopeHelper(EnvelopeLibraryEntry *entries, EnvelopeLibraryEntry* _originalEnvelope) {
        // ProjectManager *pm = Inst::get_project_manager();
        // EnvelopeLibraryEntry *entries = pm->envlibentries();
        if (entries != NULL)
            return entries->duplicateEnvelope(_originalEnvelope);
        
        return nullptr;
    }

    // nhi: delete envelope
    inline void deleteEnvelope(EnvelopeLibraryEntry *entries, EnvelopeLibraryEntry* toDelete) {
        // ProjectManager *pm = Inst::get_project_manager();
        // EnvelopeLibraryEntry *entries = pm->envlibentries();
        if (!entries || !toDelete) return;
        if (entries == toDelete) {
            EnvelopeLibraryEntry* next = entries->next;
            if (next) next->prev = nullptr;
            delete entries;
            entries = next;
            return;
        }
        EnvelopeLibraryEntry* curr = entries;
        while (curr && curr->next) {
            if (curr->next == toDelete) {
                EnvelopeLibraryEntry* del = curr->next;
                curr->next = del->next;
                if (del->next) del->next->prev = curr;
                delete del;
                return;
            }
            curr = curr->next;
        }
    }
}

#endif // ENVELOPE_LIBRARY_ENTRY_HPP