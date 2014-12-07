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
	// TODO RDOParserSrcInfo() для TypeInfo реально неопределёно, добавить соответствующий конструктор
	LPRDOValue pValue = getUnknow(RDOParserSrcInfo());
	ASSERT(pValue);

	m_pType = pValue->m_pType;
	m_value = pValue->m_value;
	ASSERT(m_pType);
}

// Для t_identificator известно только имя, но не тип
RDOValue::RDOValue(const RDOParserSrcInfo& src_info)
	: RDOParserSrcInfo(src_info                                                              )
	, m_value         (rdo::runtime::RDOValue(src_info.src_text(), rdo::runtime::g_identificator))
{
	m_pType = rdo::Factory<TypeInfo>::delegate<RDOType__identificator>(src_info);
	ASSERT(m_pType);
}

RDOValue::RDOValue(const LPRDOValue& pValue)
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

RDOValue::RDOValue(const rdo::explicit_value<int>& value, const RDOParserSrcInfo& src_info)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
{
	m_pType = rdo::Factory<TypeInfo>::delegate<RDOType__int>(src_info);
}

RDOValue::RDOValue(const rdo::explicit_value<std::size_t>& value, const RDOParserSrcInfo& src_info)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
{
	m_pType = rdo::Factory<TypeInfo>::delegate<RDOType__int>(src_info);
}

RDOValue::RDOValue(const rdo::explicit_value<double>& value, const RDOParserSrcInfo& src_info)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
{
	m_pType = rdo::Factory<TypeInfo>::delegate<RDOType__real>(src_info);
}

RDOValue::RDOValue(const rdo::explicit_value<std::string>& value, const RDOParserSrcInfo& src_info)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
{
	m_pType = rdo::Factory<TypeInfo>::delegate<RDOType__string>(src_info);
}

RDOValue::RDOValue(const LPTypeInfo& pType)
	: RDOParserSrcInfo(pType->src_info(RDOParserSrcInfo()))
	, m_value         (pType->type())
	, m_pType         (pType)
{
	ASSERT(m_pType);
}

RDOValue::RDOValue(const LPTypeInfo& pType, const RDOParserSrcInfo& src_info)
	: RDOParserSrcInfo(src_info)
	, m_value         (pType->type())
	, m_pType         (pType)
{
	ASSERT(m_pType);
}

RDOValue::RDOValue(const rdo::runtime::RDOValue& value, const RDOParserSrcInfo& src_info, const LPTypeInfo& pType)
	: RDOParserSrcInfo(src_info)
	, m_value         (value)
	, m_pType         (pType)
{
	ASSERT(m_pType);
}

const LPTypeInfo& RDOValue::typeInfo() const
{
	return m_pType;
}

rdo::runtime::RDOType::TypeID RDOValue::typeID() const
{
	return m_pType->typeID();
}

rdo::runtime::RDOValue& RDOValue::value()
{
	return m_value;
}

const rdo::runtime::RDOValue& RDOValue::value() const
{
	return m_value;
}

bool RDOValue::defined() const
{
	return m_value.typeID() != rdo::runtime::RDOType::t_unknow;
}

bool RDOValue::constant() const
{
	if (m_value.typeID() == rdo::runtime::RDOType::t_int     ||
	    m_value.typeID() == rdo::runtime::RDOType::t_real    ||
	    m_value.typeID() == rdo::runtime::RDOType::t_bool    ||
	    m_value.typeID() == rdo::runtime::RDOType::t_string)
	{
		return true;
	}

	if (m_value.typeID() == rdo::runtime::RDOType::t_identificator && m_value.getIdentificator() == "*")
	{
		return true;
	}

	if (m_value.typeID() == rdo::runtime::RDOType::t_pointer)
	{
		return m_value.isType<rdo::runtime::RDOArrayType>();
	}

	return false;
}

LPRDOValue RDOValue::getIdentificator(const std::string& identificator)
{
	return rdo::Factory<RDOValue>::create(RDOParserSrcInfo(identificator));
}

LPRDOValue RDOValue::getUnknow(const RDOParserSrcInfo& src_info)
{
	return rdo::Factory<RDOValue>::create(
		rdo::Factory<TypeInfo>::delegate<RDOType__unknow>(src_info),
		src_info
	);
}

CLOSE_RDO_PARSER_NAMESPACE
