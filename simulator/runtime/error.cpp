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
void Error::push(const SyntaxMessage& message)
{
    m_errorList.push_back(message);
    throw RDORuntimeException("");
}

void Error::push(const std::string& message, const RDOSrcInfo& srcInfo)
{
    ASSERT(!message.empty());

    SyntaxMessage error(
        message,
        srcInfo.src_filetype(),
        srcInfo.src_pos().m_last_line,
        srcInfo.src_pos().m_last_pos
    );
    push(error);
}

const Error::ErrorList& Error::list() const
{
    return m_errorList;
}

CLOSE_RDO_RUNTIME_NAMESPACE
