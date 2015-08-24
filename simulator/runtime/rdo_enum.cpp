// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <algorithm>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "simulator/runtime/rdo_enum.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/rdo_exception.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOEnumType
// --------------------------------------------------------------------------------
RDOEnumType::RDOEnumType()
    : RDOType(RDOType::Type::ENUM)
{}

RDOEnumType::RDOEnumType(const Enums& enums)
    : RDOType(RDOType::Type::ENUM)
    , m_enum (enums          )
{}

void RDOEnumType::add(const std::string& next)
{
    ASSERT(findEnum(next) == END);
    m_enum.push_back(next);
}

std::size_t RDOEnumType::findEnum(const std::string& val) const
{
    CIterator it = std::find(m_enum.begin(), m_enum.end(), val);
    return it != m_enum.end() ? it - m_enum.begin() : END;
}

bool RDOEnumType::exist(const std::string& val) const
{
    return findEnum(val) != END;
}

bool RDOEnumType::empty() const
{
    return m_enum.empty();
}

const RDOEnumType::CIterator RDOEnumType::begin() const
{
    return m_enum.begin();
}

const RDOEnumType::CIterator RDOEnumType::end() const
{
    return m_enum.end();
}

const RDOEnumType::Enums& RDOEnumType::getValues() const
{
    return m_enum;
}

CLOSE_RDO_RUNTIME_NAMESPACE
