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
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/error.h"
#include "simulator/runtime/rdo_exception.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Error
// --------------------------------------------------------------------------------
void Error::push(CREF(RDOSyntaxMessage) error)
{
	m_errorList.push_back(error);
	throw RDORuntimeException(_T(""));
}

void Error::push(CREF(tstring) message, CREF(RDOSrcInfo) srcInfo)
{
	ASSERT(!message.empty());

	RDOSyntaxMessage error(
		message,
		RDOSyntaxMessage::UNKNOWN,
		srcInfo.src_filetype(),
		srcInfo.src_pos().m_last_line,
		srcInfo.src_pos().m_last_pos
	);
	push(error);
}

CREF(Error::ErrorList) Error::list() const
{
	return m_errorList;
}

CLOSE_RDO_RUNTIME_NAMESPACE
