#ifndef _RDO_STUDIO_STDAFX_H_
#define _RDO_STUDIO_STDAFX_H_

#ifndef WINVER               // Allow use of features specific to Windows XP or later.
#define WINVER 0x0501        // Change this to the appropriate value to target other versions of Windows.
#endif

#pragma warning(push)
#pragma warning(disable: 4512)
#include <boost/signal.hpp>
#pragma warning(pop)

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxole.h>
#include <afxpriv.h>    // for CDockContext
#include <afxtempl.h>   // for CTypedPtrArray
#include <afxmt.h>
#include <afxpriv.h>    // for CDockContext
#include <afxtempl.h>   // for CTypedPtrArray

#pragma warning( disable : 4786 )

#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <list>
#include <algorithm>

#include "utils/rdodebug.h"
#include "utils/rdocommon.h"
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
#include "utils/rdodebug.h"

void appendMenu (PTR(CMenu) pFrom, const int from_index, PTR(CMenu) pTo);
void eraseMenu  (PTR(CMenu) pFrom, const int from_index = -1 );
int  roundDouble(const double val);

#endif // _RDO_STUDIO_STDAFX_H_
