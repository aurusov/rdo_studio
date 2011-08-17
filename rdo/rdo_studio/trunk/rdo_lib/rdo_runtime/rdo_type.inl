/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_type.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      18.04.2009
  \brief     Базовый тип данных
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOType
// --------------------------------------------------------------------------------
inline RDOType::RDOType(TypeID typeID)
	: m_typeID(typeID)
{}

inline RDOType::TypeID RDOType::typeID() const
{
	return m_typeID;
}

CLOSE_RDO_RUNTIME_NAMESPACE
