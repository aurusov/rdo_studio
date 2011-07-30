/*
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_enum.inl
 * @authors   Урусов Андрей
 * @date      
 * @brief     
 * @indent    4T
 */

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_exception.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOEnumType
// ********************************************************************************
inline RDOEnumType::RDOEnumType()
	: RDOType(RDOType::t_enum)
{}

inline RDOEnumType::RDOEnumType(CREF(Enums) enums)
	: RDOType(RDOType::t_enum)
	, m_enum (enums          )
{}

inline void RDOEnumType::add(CREF(tstring) next)
{
	ASSERT(findEnum(next) == END);
	m_enum.push_back(next);
}

inline ruint RDOEnumType::findEnum(CREF(tstring) val) const
{
	CIterator it = std::find(m_enum.begin(), m_enum.end(), val);
	return it != m_enum.end() ? it - m_enum.begin() : END;
}

inline rbool RDOEnumType::exist(CREF(tstring) val) const
{
	return findEnum(val) != END;
}

inline rbool                        RDOEnumType::empty    () const { return m_enum.empty(); }
inline const RDOEnumType::CIterator RDOEnumType::begin    () const { return m_enum.begin(); }
inline const RDOEnumType::CIterator RDOEnumType::end      () const { return m_enum.end();   }
inline CREF(RDOEnumType::Enums)     RDOEnumType::getValues() const { return m_enum;         }

CLOSE_RDO_RUNTIME_NAMESPACE
