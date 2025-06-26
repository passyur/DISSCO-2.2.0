#ifndef LASSIE_H
#define LASSIE_H

//LASS library
#include "../../../LASS/src/LASS.h"

/* Also includes
#include <algorithm>
#include <cmath>
#include <cstdarg>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>
*/

//CMOD library
#include "../../../CMOD/src/CMOD.h"
#include "../../../CMOD/src/Utilities.h"

/* Also includes:
#include <algorithm>
#include <cmath>
#include <cstdarg>
#include <ctime>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>
*/



//other
#include <sstream>

//using namespace xercesc;


typedef enum {
  functionReturnInt,
  functionReturnFloat,
  functionReturnSPA,
  functionReturnREV,
  functionReturnENV,
  functionReturnSIV,
  functionReturnPAT,
  funcitonReturnMEA,
  functionReturnFIL, // added for filter object
  functionReturnMGP, // ZIYUAN CHEN, July 2023 - added for "Modifier Group"
  functionReturnSPE, //added for generating spectrum from distance
  functionReturnIntList,
  functionReturnFloatList,
  functionReturnEnvelopeList,
  functionReturnString, //used to parse FileValue
  functionReturnList,
  functionReturnMakeListFun

} FunctionReturnType;



typedef enum{
  modifierTremolo = 0,
  modifierVibrato = 1,
  modifierGlissando = 2,
  modifierDetune = 3,
  modifierAmptrans = 4,
  modifierFreqtrans = 5,
  modifierWave_type = 6
} ModifierType;



#endif //LASSIE_H
