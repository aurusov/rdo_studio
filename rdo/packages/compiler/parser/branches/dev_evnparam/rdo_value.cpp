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
	m_pType = rdo::Factory<TypeInfo>::create(
		rdo::Factory<RDOType__unknow>::create(),
		RDOParserSrcInfo() /// @todo TypeInfo реально неопределён, добавить соответствующий конструктор
	);
}

RDOValue::RDOValue(CREF(LPRDOValue) pValue)
	: RDOParserSrcInfo(pValue->src_info())
	, m_value         (pValue->m_value   )
	, m_pType         (pValue->m_pType   )
{
	memcpy(&m_buffer, &pValue->m_buffer, sizeof(m_buffer));

	switch (typeID())
	{
	case rdoRuntime::RDOType::t_string       :
	case rdoRuntime::RDOType::t_identificator:
	case rdoRuntime::RDOType::t_array        :
	case rdoRuntime::RDOType::t_arrayIterator:
	case rdoRuntime::RDOType::t_pointer      :
		{
			reinterpret_cast<rdo::LPIRefCounter>(&m_buffer)->addref();
			break;
		}
	default:
		{
			break;
		}
	}
}

RDOValue::RDOValue(CREF(rdoRuntime::RDOValue) value, CREF(RDOParserSrcInfo) src_info, CREF(LPTypeInfo) pType)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
	, m_pType         (pType   )
{}

RDOValue::RDOValue(CREF(LPTypeInfo) pType)
	: RDOParserSrcInfo(pType->src_info(RDOParserSrcInfo()))
	, m_value         (pType->type()->type()              )
	, m_pType         (pType                              )
{}

// Для t_identificator известно только имя, но не тип
RDOValue::RDOValue(CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info                                                              )
	, m_value         (rdoRuntime::RDOValue(src_info.src_text(), rdoRuntime::g_identificator))
{
	m_pType = rdo::Factory<TypeInfo>::create(
		rdo::Factory<RDOType__identificator>::create(),
		src_info
	);
}

CREF(LPTypeInfo) RDOValue::typeInfo() const
{
	return m_pType;
}

rdoRuntime::RDOType::TypeID RDOValue::typeID() const
{
	return m_pType->type()->typeID();
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
	    m_value.typeID() == rdoRuntime::RDOType::t_array   ||
	    m_value.typeID() == rdoRuntime::RDOType::t_string)
	{
		return true;
	}

	if (m_value.typeID() == rdoRuntime::RDOType::t_identificator && m_value.getIdentificator() == _T("*"))
	{
		return true;
	}

	return false;
}

LPRDOValue RDOValue::getIdentificator(CREF(tstring) identificator)
{
	return rdo::Factory<RDOValue>::create(RDOParserSrcInfo(identificator));
}

CLOSE_RDO_PARSER_NAMESPACE
