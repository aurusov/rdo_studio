/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_value.cpp
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/rdo_type.h"
#include "rdo_lib/rdo_parser/rdo_array.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOValue
// ----------------------------------------------------------------------------
RDOValue::RDOValue()
	: RDOParserSrcInfo()
	, m_value         (rdoRuntime::RDOValue(rdoRuntime::g_unknow.object_parent_cast<rdoRuntime::RDOType>()))
	, m_type          (rdo::Factory<RDOType__unknow>::create())
{}

RDOValue::RDOValue(CREF(LPRDOArrayValue) pValue)
	: RDOParserSrcInfo(pValue->src_info()    )
	, m_pArray        (pValue                )
	, m_type          (pValue->getArrayType())
{
	m_value = m_pArray->getRArray();
}

RDOValue::RDOValue(CREF(rdoRuntime::RDOValue) value, CREF(LPRDOType) type, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
	, m_type          (type    )
{}

RDOValue::RDOValue(CREF(LPRDOType) type, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info    )
	, m_value         (type->type())
	, m_type          (type        )
{}

// Для t_identificator известно только имя, но не тип
RDOValue::RDOValue(CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info                                                              )
	, m_value         (rdoRuntime::RDOValue(src_info.src_text(), rdoRuntime::g_identificator))
	, m_type          (rdo::Factory<RDOType__identificator>::create()                        )
{}

void RDOValue::operator= (CREF(RDOValue) value)
{
	m_value  = value.m_value;
	m_type   = value.m_type;
	m_pArray = value.m_pArray;
	setSrcInfo(value.src_info());
}

CREF(LPRDOType) RDOValue::type() const
{
	return m_type;
}

rdoRuntime::RDOType::TypeID RDOValue::typeID() const
{
	return m_type->type()->typeID();
}

CREF(rdoRuntime::RDOValue) RDOValue::value() const
{
	return m_value;
}

CPTR(rdoRuntime::RDOValue) RDOValue::operator-> () const
{
	return &m_value;
}

CREF(LPRDOArrayValue) RDOValue::getArray() const
{
	return m_pArray;
}

rbool RDOValue::defined() const
{
	return m_value.typeID() != rdoRuntime::RDOType::t_unknow;
}

rbool RDOValue::constant() const
{
	return
		m_value.typeID() == rdoRuntime::RDOType::t_int     ||
		m_value.typeID() == rdoRuntime::RDOType::t_real    ||
		m_value.typeID() == rdoRuntime::RDOType::t_bool    ||
		m_value.typeID() == rdoRuntime::RDOType::t_array   ||
		m_value.typeID() == rdoRuntime::RDOType::t_string;
}

RDOValue RDOValue::getIdentificator(CREF(tstring) identificator)
{
	return RDOValue(RDOParserSrcInfo(identificator));
}

CLOSE_RDO_PARSER_NAMESPACE
