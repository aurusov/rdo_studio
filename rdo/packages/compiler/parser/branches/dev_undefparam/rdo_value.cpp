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
	, m_value         (rdoRuntime::RDOValue(rdoRuntime::g_unknow.object_parent_cast<rdoRuntime::RDOType>()))
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
	, m_value         (rdoRuntime::RDOValue(src_info.src_text(), rdoRuntime::g_identificator))
{
	m_pType = rdo::Factory<TypeInfo>::create(
		rdo::Factory<RDOType__identificator>::create(),
		src_info
	);
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
	case rdoRuntime::RDOType::t_pointer:
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
	m_pType = rdo::Factory<TypeInfo>::create(
		rdo::Factory<RDOType__int>::create(),
		src_info
	);
}

RDOValue::RDOValue(CREF(rdo::explicit_value<ruint>) value, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
{
	m_pType = rdo::Factory<TypeInfo>::create(
		rdo::Factory<RDOType__int>::create(),
		src_info
	);
}

RDOValue::RDOValue(CREF(rdo::explicit_value<double>) value, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
{
	m_pType = rdo::Factory<TypeInfo>::create(
		rdo::Factory<RDOType__real>::create(),
		src_info
	);
}

RDOValue::RDOValue(CREF(rdo::explicit_value<tstring>) value, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
{
	m_pType = rdo::Factory<TypeInfo>::create(
		rdo::Factory<RDOType__string>::create(),
		src_info
	);
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

RDOValue::RDOValue(CREF(rdoRuntime::RDOValue) value, CREF(RDOParserSrcInfo) src_info, CREF(LPTypeInfo) pType)
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

rdoRuntime::RDOType::TypeID RDOValue::typeID() const
{
	return m_pType->type()->typeID();
}
REF(rdoRuntime::RDOValue) RDOValue::value()
{
	return m_value;
}

CREF(rdoRuntime::RDOValue) RDOValue::value() const
{
	return m_value;
}

rbool RDOValue::defined() const
{
	return m_value.typeID() != rdoRuntime::RDOType::t_unknow;
}

rbool RDOValue::constant() const
{
	if (m_value.typeID() == rdoRuntime::RDOType::t_int     ||
	    m_value.typeID() == rdoRuntime::RDOType::t_real    ||
	    m_value.typeID() == rdoRuntime::RDOType::t_bool    ||
	    m_value.typeID() == rdoRuntime::RDOType::t_string)
	{
		return true;
	}

	if (m_value.typeID() == rdoRuntime::RDOType::t_identificator && m_value.getIdentificator() == _T("*"))
	{
		return true;
	}

	if (m_value.typeID() == rdoRuntime::RDOType::t_pointer)
	{
		return m_value.isType<rdoRuntime::RDOArrayType>();
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
		rdo::Factory<TypeInfo>::create(
			rdo::Factory<RDOType__unknow>::create(),
			src_info
		),
		src_info
	);
}

CLOSE_RDO_PARSER_NAMESPACE
