#pragma once

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_VISUAL_STUDIO
	#ifdef RDO_MT
		#include <afxwin.h>
	#else
		#include <windows.h>
	#endif
#endif // COMPILER_VISUAL_STUDIO
#ifdef COMPILER_GCC
	#include <assert.h>
#endif // COMPILER_GCC

// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

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
#ifdef ASSERT
#undef ASSERT
#endif

#ifdef _DEBUG

void StdOutput(const std::string& message);
void StdError(const std::string& message);

#define TRACE(A)                   StdOutput(A);
#define TRACE1(A, P1)              StdOutput(rdo::format(A, P1));
#define TRACE2(A, P1, P2)          StdOutput(rdo::format(A, P1, P2));
#define TRACE3(A, P1, P2, P3)      StdOutput(rdo::format(A, P1, P2, P3));
#define TRACE4(A, P1, P2, P3, P4)  StdOutput(rdo::format(A, P1, P2, P3, P4));

#ifdef COMPILER_VISUAL_STUDIO

#define ASSERT(A) \
	if (!(A)) \
	{ \
		StdError(rdo::format("ASSERT: line %d, source '%s', condition '%s'\n", __LINE__, __FILE__, #A)); \
		::DebugBreak(); \
	}

template <typename Type>
Type IdentityNeverReachHere(Type value)
{
	return value;
}

#define NEVER_REACH_HERE ASSERT(IdentityNeverReachHere(false))

#else // COMPILER_VISUAL_STUDIO

#define ASSERT(A) assert(A);
#define NEVER_REACH_HERE ASSERT(false)

#endif // COMPILER_VISUAL_STUDIO

#else // _DEBUG

#define TRACE(A)
#define TRACE1(A, P1)
#define TRACE2(A, P1, P2)
#define TRACE3(A, P1, P2, P3)
#define TRACE4(A, P1, P2, P3, P4)
#define ASSERT(A)
#define NEVER_REACH_HERE

#endif // _DEBUG
