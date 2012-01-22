/*!
  \copyright (c) RDO-Team, 2011
  \file      enum.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/enum.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_error.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOEnumType
// --------------------------------------------------------------------------------
RDOEnumType::RDOEnumType()
	: RDOType(rdo::Factory<rdoRuntime::RDOEnumType>::create())
{
	ASSERT(m_pType);
}

RDOEnumType::RDOEnumType(CREF(rdoRuntime::LPRDOEnumType) pEnumType)
	: RDOType(rdoRuntime::g_unknow)
{
	rdoRuntime::LPRDOEnumType pType = rdo::Factory<rdoRuntime::RDOEnumType>::create();
	m_pType = pType;
	ASSERT(m_pType);

	rdoRuntime::RDOEnumType::Enums::const_iterator it = pEnumType->begin();
	while (it != pEnumType->end())
	{
		pType->add(*it);
	}
}

RDOEnumType::~RDOEnumType()
{}

tstring RDOEnumType::name() const
{
	tstring str = _T("(");
	rdoRuntime::RDOEnumType::const_iterator it = getEnums()->begin();
	while (it != getEnums()->end())
	{
		str += *it;
		it++;
		if (it != getEnums()->end())
		{
			str += _T(", ");
		}
	}
	str += _T(")");
	return str;
}

LPRDOType RDOEnumType::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->type()->typeID())
	{
		case rdoRuntime::RDOType__int::t_enum:
		{
			LPRDOEnumType pEnum(const_cast<PTR(RDOEnumType)>(this));
			//! Это один и тот же тип
			if (pEnum == from)
				return pEnum;

			//! Типы разные, сгенерим ошибку
			if (pEnum.compare(from.object_static_cast<RDOEnumType>()))
			{
				rdoParse::g_error().push_only(src_info,     _T("Используются различные перечислимые типы с одинаковыми значениями"));
				rdoParse::g_error().push_only(to_src_info,   to_src_info.src_text());
				rdoParse::g_error().push_only(src_info,     _T("и"));
				rdoParse::g_error().push_only(from_src_info, from_src_info.src_text());
				rdoParse::g_error().push_only(src_info,     _T("Возможно, удобнее использовать первый из них как перечислимый, а второй как such_as на него, тогда параметры можно будет сравнивать и присваивать"));
			}
			else
			{
				rdoParse::g_error().push_only(src_info,     _T("Несоответствие перечислимых типов"));
				rdoParse::g_error().push_only(to_src_info,   to_src_info.src_text());
				rdoParse::g_error().push_only(src_info,     _T("и"));
				rdoParse::g_error().push_only(from_src_info, from_src_info.src_text());
			}
			rdoParse::g_error().push_done();
			break;
		}
		case rdoRuntime::RDOType::t_string       :
		case rdoRuntime::RDOType::t_identificator:
		{
			if (getEnums()->exist(from_src_info.src_text()))
			{
				LPRDOEnumType pEnum(const_cast<PTR(RDOEnumType)>(this));
				return pEnum;
			}
			rdoParse::g_error().error(src_info, rdo::format(_T("Значение '%s' не является элементом перечислимого типа %s"), from_src_info.src_text().c_str(), to_src_info.src_text().c_str()));
			break;
		}
		default:
			rdoParse::g_error().push_only(src_info,    rdo::format(_T("Ожидается значение перечислимого типа, найдено: %s"), from_src_info.src_text().c_str()));
			rdoParse::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
			rdoParse::g_error().push_done();
			break;
	}
	return NULL;
}

LPRDOValue RDOEnumType::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	LPRDOValue pToValue;
	LPRDOEnumType pEnum(const_cast<PTR(RDOEnumType)>(this));
	try
	{
		switch (pFrom->typeID())
		{
		case rdoRuntime::RDOType::t_identificator:
			if (getEnums()->findEnum(pFrom->value().getIdentificator()) != rdoRuntime::RDOEnumType::END)
			{
				LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pEnum, to_src_info);
				ASSERT(pType);
				pToValue = rdo::Factory<RDOValue>::create(rdoRuntime::RDOValue(getEnums(), pFrom->value().getIdentificator()), pFrom->src_info(), pType);
				ASSERT(pToValue);
			}
			else
			{
				pToValue = RDOValue::getUnknow(pFrom->src_info());
				ASSERT(pToValue);
			}
			break;

		case rdoRuntime::RDOType::t_string:
			if (getEnums()->findEnum(pFrom->value().getAsString()) != rdoRuntime::RDOEnumType::END)
			{
				LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pEnum, to_src_info);
				ASSERT(pType);
				pToValue = rdo::Factory<RDOValue>::create(rdoRuntime::RDOValue(getEnums(), pFrom->value().getAsString()), pFrom->src_info(), pType);
				ASSERT(pToValue);
			}
			else
			{
				pToValue = RDOValue::getUnknow(pFrom->src_info());
				ASSERT(pToValue);
			}
			break;

		case rdoRuntime::RDOType::t_enum:
			if (m_pType == pFrom->typeInfo()->type()->type())
				pToValue = rdo::Factory<RDOValue>::create(pFrom);
			break;
		}
	}
	catch (CREF(rdoRuntime::RDOValueException))
	{}

	if (!pToValue || pToValue->typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoParse::g_error().push_only(src_info,    rdo::format(_T("Неверное значение параметра перечислимого типа: %s"), pFrom->src_info().src_text().c_str()));
		rdoParse::g_error().push_only(to_src_info, rdo::format(_T("Возможные значения: %s"), name().c_str()));
		rdoParse::g_error().push_done();
	}
	return pToValue;
}

rdoRuntime::LPRDOCalc RDOEnumType::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return RDOType::calc_cast(pCalc, pType);
}

rdoRuntime::RDOValue RDOEnumType::get_default() const
{
	return rdoRuntime::RDOValue(getEnums(), 0);
}

void RDOEnumType::writeModelStructure(REF(std::ostream) stream) const
{
	stream << "E " << getEnums()->getValues().size() << std::endl;
	for (ruint i = 0; i < getEnums()->getValues().size(); i++)
	{
		stream << "    " << i << " " << getEnums()->getValues().at(i) << std::endl;
	}
}

void RDOEnumType::add(CREF(LPRDOValue) pNext)
{
	ASSERT(pNext);

	if (getEnums()->findEnum(pNext->value().getAsString()) != rdoRuntime::RDOEnumType::END)
	{
		rdoParse::g_error().error(pNext->src_info(), rdo::format(_T("Значение перечислимого типа уже существует: %s"), pNext->src_text().c_str()));
	}
	getEnums()->add(pNext->value().getAsString());
}

CLOSE_RDO_PARSER_NAMESPACE
