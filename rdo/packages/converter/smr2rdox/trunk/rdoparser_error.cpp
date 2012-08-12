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
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdoparser_error.h"
#include "converter/smr2rdox/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Error
// --------------------------------------------------------------------------------
Error::Error()
	: m_blocked(false)
{}

//! 1
void Error::error(CREF(RDOParserSrcInfo) src_info, rdo::service::simulation::RDOSyntaxMessage::ErrorCode error_code)
{
	if (blocked())
		return;

	error(src_info, _T(""), error_code);
}

void Error::push_only(CREF(RDOParserSrcInfo) src_info, rdo::service::simulation::RDOSyntaxMessage::ErrorCode error_code)
{
	if (blocked())
		return;

	push_only(src_info, _T(""), error_code);
}

//! 2
void Error::error(CREF(RDOParserSrcInfo) src_info, CREF(tstring) message, rdo::service::simulation::RDOSyntaxMessage::ErrorCode error_code)
{
	if (blocked())
		return;

	push_only(src_info, message, error_code);
	throw RDOSyntaxException(m_errors.back().m_message);
}

void Error::warning(CREF(RDOParserSrcInfo) src_info, CREF(tstring) message, rdo::service::simulation::RDOSyntaxMessage::ErrorCode error_code) 
{
	if (blocked())
		return;

	m_errors.push_back(rdo::service::simulation::RDOSyntaxMessage(message, error_code, src_info.src_filetype(), src_info.src_pos().m_last_line, src_info.src_pos().m_last_pos, rdo::service::simulation::RDOSyntaxMessage::MESSAGE_ERROR));
}

void Error::push_only(CREF(RDOParserSrcInfo) src_info, CREF(tstring) message, rdo::service::simulation::RDOSyntaxMessage::ErrorCode error_code)
{
	if (blocked())
		return;

	if (src_info.src_pos().m_last_line != rdo::runtime::RDOSrcInfo::Position::UNDEFINE_LINE && src_info.src_pos().m_last_pos != rdo::runtime::RDOSrcInfo::Position::UNDEFINE_POS)
	{
		m_errors.push_back(rdo::service::simulation::RDOSyntaxMessage(message, error_code, src_info.src_filetype(), src_info.src_pos().m_last_line, src_info.src_pos().m_last_pos));
	}
}

//! 3
void Error::error(CREF(RDOParserSrcInfo) src_info1, CREF(RDOParserSrcInfo) src_info2, CREF(tstring) message, rdo::service::simulation::RDOSyntaxMessage::ErrorCode error_code)
{
	if (blocked())
		return;

	push_only(src_info1.src_pos().m_last_line != src_info2.src_pos().m_last_line ? src_info1 : src_info2, message, error_code);
	throw RDOSyntaxException(m_errors.back().m_message);
}

//! misc
void Error::push_done()
{
	if (blocked())
		return;

	if (!m_errors.empty())
	{
		throw rdo::converter::smr2rdox::RDOSyntaxException(m_errors.back().m_message);
	}
}

void Error::modify(CREF(tstring) message)
{
	if (blocked())
		return;

	if (!m_errors.empty())
	{
		m_errors.front().m_message = message + m_errors.front().m_message;
		throw rdo::converter::smr2rdox::RDOSyntaxException(_T(""));
	}
}

void Error::clear()
{
	if (blocked())
		return;

	m_errors.clear();
}

CREF(Error::ErrorList) Error::getList() const
{
	return m_errors;
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
	return Converter::s_converter()->error();
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
