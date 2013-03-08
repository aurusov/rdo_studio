/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_exception.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      06.08.2011
  \brief     Определение исключений
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdomacros.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOException
// --------------------------------------------------------------------------------
inline RDOException::RDOException(CREF(tstring) message)
	: m_message(message)
{}

inline CREF(tstring) RDOException::message() const
{
	return m_message;
}

// --------------------------------------------------------------------------------
// -------------------- RDORuntimeException
// --------------------------------------------------------------------------------
inline RDORuntimeException::RDORuntimeException(CREF(tstring) message)
	: RDOException(message)
{}

inline tstring RDORuntimeException::getType() const
{
	return _T("RDO Runtime Error");
}

// --------------------------------------------------------------------------------
// -------------------- RDOTypeException
// --------------------------------------------------------------------------------
inline RDOTypeException::RDOTypeException(CREF(tstring) message)
	: RDORuntimeException(message)
{}

inline tstring RDOTypeException::getType() const
{
	return _T("RDOType Error");
}

// --------------------------------------------------------------------------------
// -------------------- RDOValueException
// --------------------------------------------------------------------------------
inline RDOValueException::RDOValueException(CREF(tstring) message)
	: RDORuntimeException(message)
{}

inline tstring RDOValueException::getType() const
{
	return _T("RDOValue Error");
}

// --------------------------------------------------------------------------------
// -------------------- RDOUndefinedException
// --------------------------------------------------------------------------------
inline RDOUndefinedException::RDOUndefinedException()
	: RDORuntimeException(_T(""))
{}

inline tstring RDOUndefinedException::getType() const
{
	return _T("RDOValue Undefined Error");
}

// --------------------------------------------------------------------------------
// -------------------- RDOInternalException
// --------------------------------------------------------------------------------
inline RDOInternalException::RDOInternalException(CREF(tstring) message)
	: RDORuntimeException(message)
{}

inline tstring RDOInternalException::getType() const
{
	return _T("RDO Internal Error");
}

CLOSE_RDO_RUNTIME_NAMESPACE
