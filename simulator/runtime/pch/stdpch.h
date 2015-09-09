// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_VISUAL_STUDIO
    #pragma warning(disable : 4786)
    #ifndef WINVER               // Allow use of features specific to Windows XP or later.
        #define WINVER 0x0501    // Change this to the appropriate value to target other versions of Windows.
    #endif
    #include <windows.h>
#endif // COMPILER_VISUAL_STUDIO

#include <algorithm>
#include <list>
#include <vector>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <map>
#include <functional>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "utils/src/debug/rdodebug.h"
// --------------------------------------------------------------------------------
