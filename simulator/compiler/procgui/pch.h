#ifndef _PROCGUI_PCH_H_
#define _PROCGUI_PCH_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"

#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(disable: 4786) 
#endif // COMPILER_VISUAL_STUDIO

#ifndef WINVER               // Allow use of features specific to Windows XP or later.
	#define WINVER 0x0501    // Change this to the appropriate value to target other versions of Windows.
#endif
// ----------------------------------------------------------------------- INCLUDES
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
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "utils/src/debug/rdodebug.h"
// --------------------------------------------------------------------------------

#endif // _PROCGUI_PCH_H_
