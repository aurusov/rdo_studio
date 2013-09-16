// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(disable : 4786)  

	#ifndef WINVER               // Allow use of features specific to Windows XP or later.
	#define WINVER 0x0501        // Change this to the appropriate value to target other versions of Windows.
	#endif
#endif // COMPILER_VISUAL_STUDIO

#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <iostream>
#include <sstream>
#include <limits>
#include <float.h>
#include <list>
#include <math.h>
#include <time.h>

#include "utils/src/common/rdocommon.h"
#include "utils/src/common/rdotypes.h"
#include "utils/src/common/rdomacros.h"
#include "utils/src/debug/rdodebug.h"
