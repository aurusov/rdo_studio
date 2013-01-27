// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#if _MSC_VER > 1200
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif
#endif

#pragma warning(push)
#pragma warning(disable: 4512)
#include <boost/signal.hpp>
#pragma warning(pop)

#pragma warning(disable: 4786)

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxole.h>         // для rdostudioapp
#include <afxpriv.h>        // для rdostudioapp
#include <afxmt.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <math.h>
#include <map>
#include <vector>
#include <list>
#include <algorithm>

#ifdef ASSERT
#undef ASSERT
#endif

#ifdef TRACE
#undef TRACE
#endif
#ifdef TRACE1
#undef TRACE1
#endif
#ifdef TRACE2
#undef TRACE2
#endif
#ifdef TRACE3
#undef TRACE3
#endif

#include "utils/rdodebug.h"
#include "utils/rdocommon.h"
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_string.h"
