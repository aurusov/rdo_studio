/*!
  \copyright (c) RDO-Team, 2012
  \file      error.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.01.2012
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/error.h"
#include "simulator/runtime/rdo_exception.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Error
// --------------------------------------------------------------------------------
void Error::push(CREF(rdo::service::simulation::RDOSyntaxError) error)
{
	m_errorList.push_back(error);
	throw RDORuntimeException(_T(""));
}

void Error::push(CREF(tstring) message, CREF(RDOSrcInfo) srcInfo)
{
	ASSERT(!message.empty());

	rdo::service::simulation::RDOSyntaxError error(
		rdo::service::simulation::RDOSyntaxError::UNKNOWN,
		message,
		srcInfo.src_pos().m_last_line,
		srcInfo.src_pos().m_last_pos,
		srcInfo.src_filetype()
	);
	push(error);
}

CREF(Error::ErrorList) Error::list() const
{
	return m_errorList;
}

CLOSE_RDO_RUNTIME_NAMESPACE
