// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser_error.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

using namespace rdo::simulation::report;

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Error
// --------------------------------------------------------------------------------
Error::Error()
	: m_blocked(false)
{}

// 1
void Error::error(const RDOParserSrcInfo& src_info, const std::string& message)
{
	if (blocked())
		return;

	push_only(src_info, message);
	throw RDOSyntaxException(m_errorList.back().getText());
}

void Error::warning(const RDOParserSrcInfo& src_info, const std::string& message) 
{
	if (blocked())
		return;

	m_errorList.push_back(rdo::simulation::report::FileMessage(message, src_info.src_filetype(), src_info.src_pos().m_last_line, src_info.src_pos().m_last_pos, FileMessage::MT_WARNING));
}

void Error::push_only(const RDOParserSrcInfo& src_info, const std::string& message)
{
	if (blocked())
		return;

	if (src_info.src_pos().m_last_line != rdo::runtime::RDOSrcInfo::Position::UNDEFINE_LINE && src_info.src_pos().m_last_pos != rdo::runtime::RDOSrcInfo::Position::UNDEFINE_POS)
	{
		m_errorList.push_back(rdo::simulation::report::FileMessage(message, src_info.src_filetype(), src_info.src_pos().m_last_line, src_info.src_pos().m_last_pos));
	}
}

// 2
void Error::error(const RDOParserSrcInfo& src_info1, const RDOParserSrcInfo& src_info2, const std::string& message)
{
	if (blocked())
		return;

	push_only(src_info1.src_pos().m_last_line != src_info2.src_pos().m_last_line ? src_info1 : src_info2, message);
	throw RDOSyntaxException(m_errorList.back().getText());
}

// misc
void Error::push_done()
{
	if (blocked())
		return;

	if (!m_errorList.empty())
	{
		throw RDOSyntaxException(m_errorList.back().getText());
	}
}

void Error::modify(const std::string& message)
{
	if (blocked())
		return;

	if (!m_errorList.empty())
	{
		const std::string new_text = message + m_errorList.front().getText();;
		m_errorList.front().setText(new_text); 
		throw RDOSyntaxException("");
	}
}

void Error::clear()
{
	if (blocked())
		return;

	m_errorList.clear();
}

const Error::ErrorList& Error::getList() const
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

bool Error::blocked() const
{
	return m_blocked;
}

Error& g_error()
{
	return RDOParser::s_parser()->error();
}

CLOSE_RDO_PARSER_NAMESPACE
