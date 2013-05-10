/*!
  \copyright (c) RDO-Team, 2011
  \file      pch.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     Включение стандартных заголовочных файлов C++ и РДО
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(disable : 4786)
	#ifndef WINVER               // Allow use of features specific to Windows XP or later.
		#define WINVER 0x0501    // Change this to the appropriate value to target other versions of Windows.
	#endif

	#ifdef RDO_MT
		#include <afxwin.h>
	#else
		#include <windows.h>
	#endif
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
#include "utils/rdocommon.h"
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
#include "utils/rdodebug.h"
// --------------------------------------------------------------------------------
