// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdoparser_error.h"
#include "converter/smr2rdox/rdoparser.h"
// --------------------------------------------------------------------------------

using namespace rdo::simulation::report;

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

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
    throw RDOSyntaxException(m_errors.back().getText());
}

void Error::warning(const RDOParserSrcInfo& src_info, const std::string& message) 
{
    if (blocked())
        return;

    m_errors.push_back(rdo::simulation::report::FileMessage(message, src_info.src_filetype(), src_info.src_pos().m_last_line, src_info.src_pos().m_last_pos, FileMessage::Type::MESSAGE_WARNING));
}

void Error::push_only(const RDOParserSrcInfo& src_info, const std::string& message)
{
    if (blocked())
        return;

    if (src_info.src_pos().m_last_line != rdo::runtime::RDOSrcInfo::Position::UNDEFINE_LINE && src_info.src_pos().m_last_pos != rdo::runtime::RDOSrcInfo::Position::UNDEFINE_POS)
    {
        m_errors.push_back(rdo::simulation::report::FileMessage(message, src_info.src_filetype(), src_info.src_pos().m_last_line, src_info.src_pos().m_last_pos));
    }
}

// 2
void Error::error(const RDOParserSrcInfo& src_info1, const RDOParserSrcInfo& src_info2, const std::string& message)
{
    if (blocked())
        return;

    push_only(src_info1.src_pos().m_last_line != src_info2.src_pos().m_last_line ? src_info1 : src_info2, message);
    throw RDOSyntaxException(m_errors.back().getText());
}

// misc
void Error::push_done()
{
    if (blocked())
        return;

    if (!m_errors.empty())
    {
        throw rdo::converter::smr2rdox::RDOSyntaxException(m_errors.back().getText());
    }
}

void Error::modify(const std::string& message)
{
    if (blocked())
        return;

    if (!m_errors.empty())
    {
        std::string new_text = message + m_errors.front().getText();
        m_errors.front().setText(new_text);
        throw rdo::converter::smr2rdox::RDOSyntaxException("");
    }
}

void Error::clear()
{
    if (blocked())
        return;

    m_errors.clear();
}

const Error::ErrorList& Error::getList() const
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

bool Error::blocked() const
{
    return m_blocked;
}

Error& g_error()
{
    return Converter::s_converter()->error();
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
