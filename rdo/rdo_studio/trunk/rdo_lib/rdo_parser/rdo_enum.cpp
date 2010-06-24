/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_enum.cpp
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_enum.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_error.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOEnumType
// ----------------------------------------------------------------------------
RDOEnumType::RDOEnumType()
	: RDOType(&rdoRuntime::g_unknow)
{
	m_type = new rdoRuntime::RDOEnumType(RDOParser::s_parser()->runtime());
}

RDOEnumType::RDOEnumType(CREF(rdoRuntime::RDOEnumType) enumType)
	: RDOType(&rdoRuntime::g_unknow)
{
	m_type = new rdoRuntime::RDOEnumType(RDOParser::s_parser()->runtime());
	STL_FOR_ALL_CONST(rdoRuntime::RDOEnumType::Enums, enumType, it)
	{
		__enum()->add(*it);
	}
}

RDOEnumType::~RDOEnumType()
{
//	delete m_type;
}

inline tstring RDOEnumType::name() const
{
	tstring str = _T("(");
	rdoRuntime::RDOEnumType::const_iterator it = getEnums().begin();
	while (it != getEnums().end())
	{
		str += *it;
		it++;
		if (it != getEnums().end())
		{
			str += _T(", ");
		}
	}
	str += _T(")");
	return str;
}

LPRDOType RDOEnumType::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->type().typeID())
	{
		case rdoRuntime::RDOType__int::t_enum:
		{
			LPRDOEnumType pEnum(const_cast<PTR(RDOEnumType)>(this));
			//! Это один и тот же тип
			if (pEnum == from)
				return pEnum;

			//! Типы разные, сгенерим ошибку
			if (pEnum.compare(from.lp_cast<LPRDOEnumType>()))
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
			if (getEnums().exist(from_src_info.src_text()))
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
	return rdo::smart_ptr_null();
}

RDOValue RDOEnumType::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	LPRDOEnumType pEnum(const_cast<PTR(RDOEnumType)>(this));
	try
	{
		switch (from.typeID())
		{
		case rdoRuntime::RDOType::t_identificator:
			toValue = (getEnums().findEnum(from->getIdentificator()) != rdoRuntime::RDOEnumType::END) ?
				RDOValue(rdoRuntime::RDOValue(getEnums(), from->getIdentificator()), pEnum, from.src_info()) :
				RDOValue(g_unknow, from.src_info());
			break;

		case rdoRuntime::RDOType::t_string:
			toValue = (getEnums().findEnum(from->getAsString()) != rdoRuntime::RDOEnumType::END) ?
				RDOValue(rdoRuntime::RDOValue(getEnums(), from->getAsString()), pEnum, from.src_info()) :
				RDOValue(g_unknow, from.src_info());
			break;

		case rdoRuntime::RDOType::t_enum:
			if (m_type == &from.type()->type())
				toValue = from;
			break;
		}
	}
	catch (CREF(rdoRuntime::RDOValueException))
	{}

	if (toValue.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoParse::g_error().push_only(src_info,    rdo::format(_T("Неверное значение параметра перечислимого типа: %s"), from.src_info().src_text().c_str()));
		rdoParse::g_error().push_only(to_src_info, rdo::format(_T("Возможные значения: %s"), name().c_str()));
		rdoParse::g_error().push_done();
	}
	return toValue;
}

rdoRuntime::LPRDOCalc RDOEnumType::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return RDOType::calc_cast(pCalc, pType);
}

RDOValue RDOEnumType::get_default() const
{
	LPRDOEnumType pEnum(const_cast<PTR(RDOEnumType)>(this));
	return RDOValue(rdoRuntime::RDOValue(getEnums(), 0), pEnum, RDOParserSrcInfo());
}

void RDOEnumType::writeModelStructure(REF(std::ostream) stream) const
{
	stream << "E " << getEnums().getValues().size() << std::endl;
	for (ruint i = 0; i < getEnums().getValues().size(); i++)
	{
		stream << "    " << i << " " << getEnums().getValues().at(i) << std::endl;
	}
}

void RDOEnumType::add(CREF(RDOValue) next)
{
	if (getEnums().findEnum(next->getAsString()) != rdoRuntime::RDOEnumType::END)
	{
		rdoParse::g_error().error(next.src_info(), rdo::format(_T("Значение перечислимого типа уже существует: %s"), next.src_text().c_str()));
	}
	__enum()->add(next->getAsString());
}

CLOSE_RDO_PARSER_NAMESPACE
