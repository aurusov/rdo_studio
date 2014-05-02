/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_exception.cpp
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      06.08.2011
  \brief     Определение исключений
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_exception.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOException
// --------------------------------------------------------------------------------
RDOException::RDOException(const std::string& message)
	: m_message(message)
{}

const std::string& RDOException::message() const
{
	return m_message;
}

// --------------------------------------------------------------------------------
// -------------------- RDORuntimeException
// --------------------------------------------------------------------------------
RDORuntimeException::RDORuntimeException(const std::string& message)
	: RDOException(message)
{}

std::string RDORuntimeException::getType() const
{
	return "RDO Runtime Error";
}

// --------------------------------------------------------------------------------
// -------------------- RDOTypeException
// --------------------------------------------------------------------------------
RDOTypeException::RDOTypeException(const std::string& message)
	: RDORuntimeException(message)
{}

std::string RDOTypeException::getType() const
{
	return "RDOType Error";
}

// --------------------------------------------------------------------------------
// -------------------- RDOValueException
// --------------------------------------------------------------------------------
RDOValueException::RDOValueException(const std::string& message)
	: RDORuntimeException(message)
{}

std::string RDOValueException::getType() const
{
	return "RDOValue Error";
}

// --------------------------------------------------------------------------------
// -------------------- RDOUndefinedException
// --------------------------------------------------------------------------------
RDOUndefinedException::RDOUndefinedException()
	: RDORuntimeException("")
{}

std::string RDOUndefinedException::getType() const
{
	return "RDOValue Undefined Error";
}

// --------------------------------------------------------------------------------
// -------------------- RDOInternalException
// --------------------------------------------------------------------------------
RDOInternalException::RDOInternalException(const std::string& message)
	: RDORuntimeException(message)
{}

std::string RDOInternalException::getType() const
{
	return "RDO Internal Error";
}

CLOSE_RDO_RUNTIME_NAMESPACE
