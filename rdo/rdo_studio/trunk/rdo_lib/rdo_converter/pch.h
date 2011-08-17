/*
 * copyright: (c) RDO-Team, 2011
 * filename : pch.h
 * author   : Урусов Андрей
 * date     : 15.08.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONVERTOR_PCH_H_
#define _CONVERTOR_PCH_H_

#pragma warning(disable: 4786)  

#ifndef WINVER               //! Allow use of features specific to Windows XP or later.
#define WINVER 0x0501        //! Change this to the appropriate value to target other versions of Windows.
#endif

// ====================================================================== INCLUDES
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
// ====================================================================== SYNOPSIS
#include "rdo_common/rdocommon.h"
#include "rdo_common/rdotypes.h"
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdodebug.h"
// ===============================================================================

#endif //! _CONVERTOR_PCH_H_
