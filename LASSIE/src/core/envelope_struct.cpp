// /*******************************************************************************
//  *
//  *  File Name     : envelope_struct.cpp, originally EnvelopeLibraryEntry.cpp
//  *  Date created  : May.17 2010
//  *  Authors       : Ming-ching Chiu, Sever Tipei
//  *  Organization  : Music School, University of Illinois at Urbana Champaign
//  *  Description   : This file contains the class "EnvelopeLibraryEntry" of
//  *                  LASSIE. Each EnvelopeLibraryEntry holds the information of
//  *                  a premade envelope in the envelope library. It is also a
//  *                  doubly-linked list.
//  *
//  *==============================================================================
//  *
//  *  This file is part of LASSIE.
//  *  2010 Ming-ching Chiu, Sever Tipei
//  *
//  *
//  *  LASSIE is free software: you can redistribute it and/or modify
//  *  it under the terms of the GNU General Public License as published by
//  *  the Free Software Foundation, either version 3 of the License, or
//  *  (at your option) any later version.
//  *
//  *  LASSIE is distributed in the hope that it will be useful,
//  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  *  GNU General Public License for more details.
//  *
//  *  You should have received a copy of the GNU General Public License
//  *  along with LASSIE.  If not, see <http://www.gnu.org/licenses/>.
//  *
//  ******************************************************************************/

//  #include "envelope_struct.hpp"
// //#include <stdlib.h>

// EnvelopeLibraryEntry::EnvelopeLibraryEntry(int _number){
//   number =_number;
//   prev = nullptr;
//   next = nullptr;

//   head = new EnvLibEntryNode(0,0);
//   head->rightSeg = new EnvLibEntrySeg();
//   head->rightSeg->leftNode = head;
//   head->rightSeg->rightNode = new EnvLibEntryNode(1,0);
//   head->rightSeg->rightNode->leftSeg = head->rightSeg;

// }


// EnvelopeLibraryEntry::EnvelopeLibraryEntry(EnvelopeLibraryEntry &other, int _number){
//   number = _number;
//   prev = nullptr;
//   next = nullptr;

//   EnvLibEntryNode* originalEnvelopeCurrentNode = other.head;
//   EnvLibEntryNode* duplicationCurrentNode;


//   head = new EnvLibEntryNode(originalEnvelopeCurrentNode->x,
//                              originalEnvelopeCurrentNode->y);
//   duplicationCurrentNode = head;

//   while (originalEnvelopeCurrentNode->rightSeg != nullptr){
//     EnvLibEntrySeg* newSegment = new EnvLibEntrySeg();
//     newSegment->segmentType =
//       originalEnvelopeCurrentNode->rightSeg->segmentType;
//     newSegment->segmentProperty =
//       originalEnvelopeCurrentNode->rightSeg->segmentProperty;

//     duplicationCurrentNode->rightSeg = newSegment;
//     newSegment->leftNode = duplicationCurrentNode;

//     EnvLibEntryNode* newNode =
//       new EnvLibEntryNode(originalEnvelopeCurrentNode->rightSeg->rightNode->x,
//                           originalEnvelopeCurrentNode->rightSeg->rightNode->y);

//     newSegment->rightNode = newNode;
//     newNode->leftSeg = newSegment;

//     originalEnvelopeCurrentNode =
//       originalEnvelopeCurrentNode->rightSeg->rightNode;

//     duplicationCurrentNode = newNode;

//   }



// }





// EnvelopeLibraryEntry::~EnvelopeLibraryEntry(){//delete segments!}
// //TODO: lassie doesn't support deleting envelope so far so no worry about it.

// }



// int EnvelopeLibraryEntry::count(){
//   if (next == nullptr) return 1;
//   else return next->count() + 1;
// }

// /* this should be converted to tail-recursive form -6/23/25 */
// EnvelopeLibraryEntry* EnvelopeLibraryEntry::create(){
//   if (next != nullptr){
//     return next->create();
//   } else {
//     next = new EnvelopeLibraryEntry (number + 1);
//     next->prev = this;
//     return next;
//   }
// }

// /* same here as above -6/23/25 */
// EnvelopeLibraryEntry* EnvelopeLibraryEntry::duplicate(EnvelopeLibraryEntry &other){
//     if (next != nullptr) {
//         return next->duplicate(other);
//     } else {
//         next = new EnvelopeLibraryEntry (other, number + 1 );
//         next->prev = this;
//         return next;
//     }
// }

// QString EnvelopeLibraryEntry::get_number_as_qstring(){
//   return QString::fromStdString(std::to_string(number));;
// }

// EnvelopeLibraryEntry::EnvelopeLibraryEntry(Envelope* _envelope,int _number){
//   prev =nullptr;
//   next =nullptr;
// 	number = _number;
// 	Collection<envelope_segment>* segments = _envelope->getSegments ();

// 	EnvLibEntryNode* currentNode = nullptr;
// 	// EnvLibEntryNode* prevNode = nullptr;
// 	EnvLibEntrySeg* currentSeg = nullptr;
// 	EnvLibEntrySeg* prevSeg = nullptr;



// 	int i = 0;
// 	for (; i < segments->size()-1; i++){

// 		// prevNode = currentNode;
// 		prevSeg = currentSeg;
// 		currentNode = new EnvLibEntryNode(segments->get(i).x, segments->get(i).y);
// 		currentSeg = new EnvLibEntrySeg();
// 		if (i ==0){
// 			head = currentNode;
// 		}
// 		else{
// 			prevSeg->rightNode = currentNode;
// 		}
// 		currentNode->leftSeg = prevSeg;
// 		currentNode->rightSeg = currentSeg;
// 		currentSeg->leftNode = currentNode;

// 		if (_envelope->getSegmentLengthType(i) == FIXED){


//       currentSeg->segmentProperty = envSegmentPropertyFixed;
//     }
//     else {

//       currentSeg->segmentProperty = envSegmentPropertyFlexible;
//     }


//     if (_envelope->getSegmentInterpolationType(i) == EXPONENTIAL){
//       currentSeg->segmentType = envSegmentTypeExponential;
//     }
//     else if(_envelope->getSegmentInterpolationType(i) == CUBIC_SPLINE){
//       currentSeg->segmentType = envSegmentTypeSpline;
//     }
//     else {
//       currentSeg->segmentType = envSegmentTypeLinear;
//     }

//   }// end of for loop

// 	currentSeg->rightNode = new EnvLibEntryNode(segments->get(i).x,
// 	                                            segments->get(i).y);

// 	currentSeg->rightNode->leftSeg = currentSeg;

// }


// EnvLibEntrySeg::EnvLibEntrySeg():
//   leftNode(nullptr),
//   rightNode(nullptr),
//   segmentType(envSegmentTypeLinear),
//   segmentProperty(envSegmentPropertyFlexible){}




// EnvLibEntryNode::EnvLibEntryNode(double _x, double _y):
//   x(_x), y(_y), leftSeg(nullptr), rightSeg(nullptr){}

// int EnvLibEntryNode::countNumOfNodes(){
//   return (rightSeg==nullptr)?1:1+rightSeg->rightNode->countNumOfNodes();}
