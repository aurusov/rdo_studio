/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_enum.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      18.04.2009
  \brief     Перечисления
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
#include <algorithm>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/rdo_exception.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOEnumType
// --------------------------------------------------------------------------------
inline RDOEnumType::RDOEnumType()
	: RDOType(RDOType::t_enum)
{}

inline RDOEnumType::RDOEnumType(const Enums& enums)
	: RDOType(RDOType::t_enum)
	, m_enum (enums          )
{}

inline void RDOEnumType::add(const std::string& next)
{
	ASSERT(findEnum(next) == END);
	m_enum.push_back(next);
}

inline std::size_t RDOEnumType::findEnum(const std::string& val) const
{
	CIterator it = std::find(m_enum.begin(), m_enum.end(), val);
	return it != m_enum.end() ? it - m_enum.begin() : END;
}

inline bool RDOEnumType::exist(const std::string& val) const
{
	return findEnum(val) != END;
}

inline bool RDOEnumType::empty() const
{
	return m_enum.empty();
}

inline const RDOEnumType::CIterator RDOEnumType::begin() const
{
	return m_enum.begin();
}

inline const RDOEnumType::CIterator RDOEnumType::end() const
{
	return m_enum.end();
}

inline const RDOEnumType::Enums& RDOEnumType::getValues() const
{
	return m_enum;
}

CLOSE_RDO_RUNTIME_NAMESPACE
