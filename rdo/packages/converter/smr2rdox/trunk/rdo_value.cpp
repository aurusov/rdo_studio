/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_value.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_value.h"
#include "converter/smr2rdox/rdo_type.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOValue
// --------------------------------------------------------------------------------
RDOValue::RDOValue(CREF(rdo::runtime::RDOValue) value, CREF(LPRDOType) type, CREF(RDOParserSrcInfo) src_info)
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
	: RDOParserSrcInfo(src_info)
	, m_value         (rdo::runtime::RDOValue(src_info.src_text(), rdo::runtime::g_identificator))
	, m_type          (rdo::Factory<RDOType__identificator>::create())
{}

// Неопределенный тип
RDOValue::RDOValue()
	: RDOParserSrcInfo()
	, m_value         (rdo::runtime::RDOValue(rdo::runtime::g_unknow.object_parent_cast<rdo::runtime::RDOType>()))
	, m_type          (rdo::Factory<RDOType__unknow>::create())
{}

CREF(LPRDOType) RDOValue::type() const
{
	return m_type;
}

rdo::runtime::RDOType::TypeID RDOValue::typeID() const
{
	return m_type->type()->typeID();
}

CREF(rdo::runtime::RDOValue) RDOValue::value() const
{
	return m_value;
}

CPTR(rdo::runtime::RDOValue) RDOValue::operator-> () const
{
	return &m_value;
}

rbool RDOValue::defined() const
{
	return m_value.typeID() != rdo::runtime::RDOType::t_unknow;
}

rbool RDOValue::constant() const
{
	return
		m_value.typeID() == rdo::runtime::RDOType::t_int  ||
		m_value.typeID() == rdo::runtime::RDOType::t_real ||
		m_value.typeID() == rdo::runtime::RDOType::t_bool ||
		m_value.typeID() == rdo::runtime::RDOType::t_string;
}

LPRDOValue RDOValue::getIdentificator(CREF(tstring) identificator)
{
	return rdo::Factory<RDOValue>::create(RDOParserSrcInfo(identificator));
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
