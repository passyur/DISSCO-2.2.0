#ifndef LASSIE_H
#define LASSIE_H

#include "../core/IEvent.h"

// Qt core and widget modules (replaces GTKmm)
#include <QtCore/QtCore>     // Core non-GUI classes: QString, QVector, QMap, etc.
#include <QtWidgets/QtWidgets> // All widget classes: QApplication, QWidget, etc.

// System headers
#include <sys/types.h>
#include <unistd.h>

// LASS library (logic and music synthesis core)
#include "LASS.h"

// CMOD library (common utilities and helper routines)
#include "CMOD.h"
#include "Utilities.h"

// Other standard headers used across LASSIE
#include <algorithm>
#include <cmath>
#include <cstdarg>
#include <cstring>
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

#endif // LASSIE_H