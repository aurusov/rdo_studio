#pragma warning(disable : 4786)  

#ifndef WINVER               // Allow use of features specific to Windows XP or later.
#define WINVER 0x0501        // Change this to the appropriate value to target other versions of Windows.
#endif

#ifdef RDO_MT
#include <afxwin.h>
#else
#include <windows.h>
#endif
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
#include <rdocommon.h>
#include <rdotypes.h>
#include <rdomacros.h>
#include <rdodebug.h>
