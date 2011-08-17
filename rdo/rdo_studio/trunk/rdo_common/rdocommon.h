/*
  \copyright (c) RDO-Team, 2011
  \file      rdocommon.h
  \author    Барс Александр, Урусов Андрей
  \date      13.06.2009
  \brief     
  \indent    4T
 */

#ifndef _RDOCOMMON_H_
#define _RDOCOMMON_H_

#pragma warning(disable : 4786)

// ====================================================================== INCLUDES
#include <vector>
#include <sstream>
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdotypes.h"
#include "rdo_common/rdomacros.h"
#include "rdo_common/model_objects.h"
// ===============================================================================

OPEN_RDO_NAMESPACE
	template< class T >
	T rmin(T v1, T v2)
	{
		return v1 <= v2 ? v1 : v2;
	}

	template< class T >
	T rmax(T v1, T v2)
	{
		return v1 >= v2 ? v1 : v2;
	}

	tstring format(CPTR(tchar) str, ...);
	tstring format(CPTR(tchar) str, REF(va_list) params);
	tstring format(ruint resource, ...);
	tstring format(ruint resource, REF(va_list) params);

	void toLower  (REF(tstring) str);
	void trim     (REF(tstring) str);
	void trimLeft (REF(tstring) str);
	void trimRight(REF(tstring) str);

	tstring extractFilePath(CREF(tstring) fileName);

	template< class T >
	inline tstring toString(T value)
	{
		std::ostringstream str;
		str << value;
		return str.str();
	}

	template<class T>
	void deleteAllObjects(REF(T) container)
	{
		T::reverse_iterator it = container.rbegin();
		while (it != container.rend())
		{
			delete *it;
			++it;
		}
		container.clear();
	}

	// ----------------------------------------------------------------------------
	// ---------- vector
	// ----------------------------------------------------------------------------
	template <class T>
	class vector: public std::vector<T>
	{
	public:
		vector()
		{}
		vector(CREF(T) item)
		{
			push_back(item);
		}
		REF(vector) operator() (CREF(T) item)
		{
			push_back(item);
			return *this;
		}
	};
CLOSE_RDO_NAMESPACE

OPEN_RDO_RUNTIME_NAMESPACE
	enum RunTimeMode
	{
		RTM_MaxSpeed,
		RTM_Jump,
		RTM_Sync,
		RTM_Pause,
		RTM_BreakPoint
	};
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_SIMULATOR_NAMESPACE
	enum ShowMode
	{
		SM_NoShow,
		SM_Animation,
		SM_Monitor
	};

	enum RDOExitCode
	{
		EC_OK = 0,
		EC_ParserError,
		EC_RunTimeError,
		EC_UserBreak,
		EC_ModelNotFound,
		EC_NoMoreEvents
	};

	struct RDOSyntaxError
	{
		enum ErrorCode
		{
			UNKNOWN = 1,
		};

		ErrorCode                     error_code;
		tstring                       message;
		int                           error_line;
		int                           error_pos;
		rbool                         warning;
		rdoModelObjects::RDOFileType  file;

		RDOSyntaxError(ErrorCode _error_code, CREF(tstring) _message, int _error_line, int _error_pos, rdoModelObjects::RDOFileType _file, rbool _warning = false)
			: error_code(_error_code)
			, message   (_message   )
			, error_line(_error_line)
			, error_pos (_error_pos )
			, file      (_file      )
			, warning   (_warning   )
		{}
	};
CLOSE_RDO_SIMULATOR_NAMESPACE

#include "rdo_common/rdocommon.inl"

#endif //! _RDOCOMMON_H_
