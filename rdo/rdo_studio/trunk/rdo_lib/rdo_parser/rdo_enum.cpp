/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_enum.h
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
RDOEnumType::RDOEnumType(CREF(RDOValue) first)
	: RDOType(&rdoRuntime::g_unknow)
{
	m_type = new rdoRuntime::RDOEnumType(RDOParser::s_parser()->runtime());
	add(first);
}

RDOEnumType::~RDOEnumType()
{
	delete m_type;
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

CPTR(RDOType) RDOEnumType::type_cast(CREF(RDOType) from) const
{
	switch (from->typeID())
	{
		case rdoRuntime::RDOType__int::t_enum: return operator==(static_cast<CREF(RDOEnumType)>(from)) ? this : NULL;
	}
	return NULL;
}

rdoRuntime::RDOValue RDOEnumType::value_cast(CREF(rdoRuntime::RDOValue) from) const
{
	switch (from.typeID())
	{
		case rdoRuntime::RDOType::t_identificator: {
			return (getEnums().findEnum(from.getIdentificator()) != rdoRuntime::RDOEnumType::END) ?
				rdoRuntime::RDOValue(getEnums(), from.getIdentificator()) :
				rdoRuntime::RDOValue(rdoRuntime::g_unknow);
			break;
		}
		case rdoRuntime::RDOType::t_enum: {
			if (m_type == &from.type())
				return from;
			break;
		}
	}
	throw rdoRuntime::RDOTypeException();
}

void RDOEnumType::add(CREF(RDOValue) next)
{
	if (getEnums().findEnum(next->getIdentificator()) != rdoRuntime::RDOEnumType::END)
	{
		rdoParse::g_error().error(next.src_info(), rdo::format(_T("Значение перечислимого типа уже существует: %s"), next.src_text().c_str()));
	}
	__enum()->add(next->getIdentificator());
}

//rdoRuntime::RDOValue RDOEnumType::findEnumValueWithThrow(CREF(RDOParserSrcInfo) src_info, CREF(tstring) value) const
//{
//	rdoRuntime::RDOValue result = value_cast(rdoRuntime::RDOValue(value, rdoRuntime::g_identificator));
//	if (result.typeID() == rdoRuntime::RDOType::t_unknow)
//	{
//		rdoParse::g_error().push_only(src_info, rdo::format(_T("Неверное значение параметра перечислимого типа: %s"), value.c_str()));
//		rdoParse::g_error().push_only(this->src_info(), rdo::format(_T("Возможные значения: %s"), getEnums().asString().c_str()));
//		rdoParse::g_error().push_done();
//	}
//	return result;
//}

//rdoRuntime::RDOValue RDOEnumType::getFirstValue() const
//{
//	if (getEnums().getValues().empty())
//	{
//		rdoParse::g_error().error(src_info(), _T("Внутренняя ошибка: Пустой перечислимый тип"));
//	}
//	return rdoRuntime::RDOValue(getEnums());
//}

CLOSE_RDO_PARSER_NAMESPACE
