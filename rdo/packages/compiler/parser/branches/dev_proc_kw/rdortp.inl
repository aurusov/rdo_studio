/*!
  \copyright (c) RDO-Team, 2012
  \file      rdortp.inl
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      09.03.2012
  \brief     Типы ресурсов
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
#include "simulator/compiler/parser/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDORTPResType
// --------------------------------------------------------------------------------
inline rsint RDORTPResType::getNumber() const
{
	return m_number;
}

inline rbool RDORTPResType::isPermanent() const
{
	return m_permanent;
}

inline rbool RDORTPResType::isTemporary() const
{
	return !m_permanent;
}

inline CREF(RDORTPResType::ParamList) RDORTPResType::getParams() const
{
	return m_params;
}

inline CREF(rdo::runtime::LPIResourceType) RDORTPResType::getRuntimeResType() const
{
	ASSERT(m_pRuntimeResType);
	return m_pRuntimeResType;
}

inline void RDORTPResType::setType(TypeRDOResType type)
{
	//! \todo вывести ошибку вместо ASSERT()
	ASSERT(!(m_type == procRes && type == procTran));
	ASSERT(!(m_type == procTran && type == procRes));
	m_type = type;
}

inline void RDORTPResType::end()
{
	switch (m_type)
	{
	case simple:
		m_pRuntimeResType = rdo::Factory<rdo::runtime::RDOResourceType>::create(m_number).interface_cast<rdo::runtime::IResourceType>();
		break;
	case procRes:
		m_pRuntimeResType = rdo::Factory<rdo::runtime::RDOResourceTypeProccess>::create(m_number).interface_cast<rdo::runtime::IResourceType>();
		break;
	case procTran:
		m_pRuntimeResType = rdo::Factory<rdo::runtime::RDOResourceTypeTransact>::create(m_number).interface_cast<rdo::runtime::IResourceType>();
		break;
	default:
		NEVER_REACH_HERE;
	}
	ASSERT(m_pRuntimeResType);
	m_pType = m_pRuntimeResType;
	ASSERT(m_pType);
}

CLOSE_RDO_PARSER_NAMESPACE
