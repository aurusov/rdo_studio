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
#include "utils/src/common/rdomacros.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOException
// --------------------------------------------------------------------------------
inline RDOException::RDOException(const std::string& message)
	: m_message(message)
{}

inline const std::string& RDOException::message() const
{
	return m_message;
}

// --------------------------------------------------------------------------------
// -------------------- RDORuntimeException
// --------------------------------------------------------------------------------
inline RDORuntimeException::RDORuntimeException(const std::string& message)
	: RDOException(message)
{}

inline std::string RDORuntimeException::getType() const
{
	return "RDO Runtime Error";
}

// --------------------------------------------------------------------------------
// -------------------- RDOTypeException
// --------------------------------------------------------------------------------
inline RDOTypeException::RDOTypeException(const std::string& message)
	: RDORuntimeException(message)
{}

inline std::string RDOTypeException::getType() const
{
	return "RDOType Error";
}

// --------------------------------------------------------------------------------
// -------------------- RDOValueException
// --------------------------------------------------------------------------------
inline RDOValueException::RDOValueException(const std::string& message)
	: RDORuntimeException(message)
{}

inline std::string RDOValueException::getType() const
{
	return "RDOValue Error";
}

// --------------------------------------------------------------------------------
// -------------------- RDOUndefinedException
// --------------------------------------------------------------------------------
inline RDOUndefinedException::RDOUndefinedException()
	: RDORuntimeException("")
{}

inline std::string RDOUndefinedException::getType() const
{
	return "RDOValue Undefined Error";
}

// --------------------------------------------------------------------------------
// -------------------- RDOInternalException
// --------------------------------------------------------------------------------
inline RDOInternalException::RDOInternalException(const std::string& message)
	: RDORuntimeException(message)
{}

inline std::string RDOInternalException::getType() const
{
	return "RDO Internal Error";
}

CLOSE_RDO_RUNTIME_NAMESPACE
