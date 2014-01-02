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
#include "utils/src/common/rdotypes.h"
#include "utils/src/common/rdomacros.h"
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

inline void RDORTPResType::setSubtype(Subtype type)
{
	//! \todo вывести ошибку вместо ASSERT()
	ASSERT(!(m_subtype == RT_PROCESS_RESOURCE && type == RT_PROCESS_TRANSACT));
	ASSERT(!(m_subtype == RT_PROCESS_TRANSACT && type == RT_PROCESS_RESOURCE));
	m_subtype = type;
}

CLOSE_RDO_PARSER_NAMESPACE
