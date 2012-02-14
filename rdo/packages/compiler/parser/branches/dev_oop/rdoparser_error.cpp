/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser_error.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.02.2010
  \brief     основан на rdoparser.h/cpp
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser_error.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Error
// --------------------------------------------------------------------------------
Error::Error()
	: m_blocked(false)
{}

//! 1
void Error::error(CREF(RDOParserSrcInfo) src_info, rdoSimulator::RDOSyntaxError::ErrorCode error_code)
{
	if (blocked())
		return;

	error(src_info, _T(""), error_code);
}

void Error::push_only(CREF(RDOParserSrcInfo) src_info, rdoSimulator::RDOSyntaxError::ErrorCode error_code)
{
	if (blocked())
		return;

	push_only(src_info, _T(""), error_code);
}

//! 2
void Error::error(CREF(RDOParserSrcInfo) src_info, CREF(tstring) message, rdoSimulator::RDOSyntaxError::ErrorCode error_code)
{
	if (blocked())
		return;

	push_only(src_info, message, error_code);
	throw RDOSyntaxException(m_errorList.back().m_message);
}

void Error::warning(CREF(RDOParserSrcInfo) src_info, CREF(tstring) message, rdoSimulator::RDOSyntaxError::ErrorCode error_code) 
{
	if (blocked())
		return;

	m_errorList.push_back(rdoSimulator::RDOSyntaxError(error_code, message, src_info.src_pos().m_last_line, src_info.src_pos().m_last_pos, src_info.src_filetype(), true));
}

void Error::push_only(CREF(RDOParserSrcInfo) src_info, CREF(tstring) message, rdoSimulator::RDOSyntaxError::ErrorCode error_code)
{
	if (blocked())
		return;

	if (src_info.src_pos().m_last_line != rdoRuntime::RDOSrcInfo::Position::UNDEFINE_LINE && src_info.src_pos().m_last_pos != rdoRuntime::RDOSrcInfo::Position::UNDEFINE_POS)
	{
		m_errorList.push_back(rdoSimulator::RDOSyntaxError(error_code, message, src_info.src_pos().m_last_line, src_info.src_pos().m_last_pos, src_info.src_filetype()));
	}
}

//! 3
void Error::error(CREF(RDOParserSrcInfo) src_info1, CREF(RDOParserSrcInfo) src_info2, CREF(tstring) message, rdoSimulator::RDOSyntaxError::ErrorCode error_code)
{
	if (blocked())
		return;

	push_only(src_info1.src_pos().m_last_line != src_info2.src_pos().m_last_line ? src_info1 : src_info2, message, error_code);
	throw RDOSyntaxException(m_errorList.back().m_message);
}

//! misc
void Error::push_done()
{
	if (blocked())
		return;

	if (!m_errorList.empty())
	{
		throw rdoParse::RDOSyntaxException(m_errorList.back().m_message);
	}
}

void Error::modify(CREF(tstring) message)
{
	if (blocked())
		return;

	if (!m_errorList.empty())
	{
		m_errorList.front().m_message = message + m_errorList.front().m_message;
		throw rdoParse::RDOSyntaxException(_T(""));
	}
}

void Error::clear()
{
	if (blocked())
		return;

	m_errorList.clear();
}

CREF(Error::ErrorList) Error::getList() const
{
	return m_errorList;
}

void Error::block()
{
	m_blocked = true;
}

void Error::unblock()
{
	m_blocked = false;
}

rbool Error::blocked() const
{
	return m_blocked;
}

REF(Error) g_error()
{
	return RDOParser::s_parser()->error();
}

CLOSE_RDO_PARSER_NAMESPACE
