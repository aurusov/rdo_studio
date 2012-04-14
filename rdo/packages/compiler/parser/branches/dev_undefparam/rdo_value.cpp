/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/compiler/parser/rdo_value.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/rdo_array.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOValue
// --------------------------------------------------------------------------------
RDOValue::RDOValue()
	: RDOParserSrcInfo()
	, m_value         (rdo::runtime::RDOValue(rdo::runtime::g_unknow.object_parent_cast<rdo::runtime::RDOType>()))
{
	//! @todo RDOParserSrcInfo() для TypeInfo реально неопределёно, добавить соответствующий конструктор
	LPRDOValue pValue = getUnknow(RDOParserSrcInfo());
	ASSERT(pValue);

	m_pType = pValue->m_pType;
	m_value = pValue->m_value;
	ASSERT(m_pType);
}

// Для t_identificator известно только имя, но не тип
RDOValue::RDOValue(CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info                                                              )
	, m_value         (rdo::runtime::RDOValue(src_info.src_text(), rdo::runtime::g_identificator))
{
	m_pType = rdo::Factory<TypeInfo>::delegate<RDOType__identificator>(src_info);
	ASSERT(m_pType);
}

RDOValue::RDOValue(CREF(LPRDOValue) pValue)
	: RDOParserSrcInfo(pValue->src_info())
	, m_value         (pValue->m_value   )
	, m_pType         (pValue->m_pType   )
{
	ASSERT(m_pType);

	memcpy(&m_buffer, &pValue->m_buffer, sizeof(m_buffer));

	switch (typeID())
	{
	case rdo::runtime::RDOType::t_pointer:
		reinterpret_cast<rdo::LPIRefCounter>(&m_buffer)->addref();
		break;

	default:
		break;
	}
}

RDOValue::RDOValue(CREF(rdo::explicit_value<rsint>) value, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
{
	m_pType = rdo::Factory<TypeInfo>::delegate<RDOType__int>(src_info);
}

RDOValue::RDOValue(CREF(rdo::explicit_value<ruint>) value, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
{
	m_pType = rdo::Factory<TypeInfo>::delegate<RDOType__int>(src_info);
}

RDOValue::RDOValue(CREF(rdo::explicit_value<double>) value, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
{
	m_pType = rdo::Factory<TypeInfo>::delegate<RDOType__real>(src_info);
}

RDOValue::RDOValue(CREF(rdo::explicit_value<tstring>) value, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
{
	m_pType = rdo::Factory<TypeInfo>::delegate<RDOType__string>(src_info);
}

RDOValue::RDOValue(CREF(LPTypeInfo) pType)
	: RDOParserSrcInfo(pType->src_info(RDOParserSrcInfo()))
	, m_value         (pType->type()->type()              )
	, m_pType         (pType                              )
{
	ASSERT(m_pType);
}

RDOValue::RDOValue(CREF(LPTypeInfo) pType, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info             )
	, m_value         (pType->type()->type())
	, m_pType         (pType                )
{
	ASSERT(m_pType);
}

RDOValue::RDOValue(CREF(rdo::runtime::RDOValue) value, CREF(RDOParserSrcInfo) src_info, CREF(LPTypeInfo) pType)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
	, m_pType         (pType   )
{
	ASSERT(m_pType);
}

CREF(LPTypeInfo) RDOValue::typeInfo() const
{
	return m_pType;
}

rdo::runtime::RDOType::TypeID RDOValue::typeID() const
{
	return m_pType->type()->typeID();
}

REF(rdo::runtime::RDOValue) RDOValue::value()
{
	return m_value;
}

CREF(rdo::runtime::RDOValue) RDOValue::value() const
{
	return m_value;
}

rbool RDOValue::defined() const
{
	return m_value.typeID() != rdo::runtime::RDOType::t_unknow;
}

rbool RDOValue::constant() const
{
	if (m_value.typeID() == rdo::runtime::RDOType::t_int     ||
	    m_value.typeID() == rdo::runtime::RDOType::t_real    ||
	    m_value.typeID() == rdo::runtime::RDOType::t_bool    ||
	    m_value.typeID() == rdo::runtime::RDOType::t_string)
	{
		return true;
	}

	if (m_value.typeID() == rdo::runtime::RDOType::t_identificator && m_value.getIdentificator() == _T("*"))
	{
		return true;
	}

	if (m_value.typeID() == rdo::runtime::RDOType::t_pointer)
	{
		return m_value.isType<rdo::runtime::RDOArrayType>();
	}

	return false;
}

LPRDOValue RDOValue::getIdentificator(CREF(tstring) identificator)
{
	return rdo::Factory<RDOValue>::create(RDOParserSrcInfo(identificator));
}

LPRDOValue RDOValue::getUnknow(CREF(RDOParserSrcInfo) src_info)
{
	return rdo::Factory<RDOValue>::create(
		rdo::Factory<TypeInfo>::delegate<RDOType__unknow>(src_info),
		src_info
	);
}

CLOSE_RDO_PARSER_NAMESPACE
