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
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDORTPEnum
// ----------------------------------------------------------------------------
RDORTPEnum::RDORTPEnum(CREF(RDOValue) first)
	: RDOTypeParam(rdoRuntime::g_unknow, RDOParserSrcInfo())
{
	m_type = new rdoRuntime::RDOEnumType(RDOParser::s_parser()->runtime());
	add(first);
}

RDORTPEnum::~RDORTPEnum()
{}

CPTR(RDOType) RDORTPEnum::cast(CREF(RDOType) toType) const
{
	switch (toType->typeID())
	{
		case rdoRuntime::RDOType__int::t_enum: return operator==(static_cast<CREF(RDORTPEnum)>(toType)) ? this : NULL;
	}
	return NULL;
}

rdoRuntime::RDOValue RDORTPEnum::cast(CREF(rdoRuntime::RDOValue) from) const
{
	return m_type->cast(from);
}

void RDORTPEnum::add(CREF(RDOValue) next)
{
	if (__enum()->findEnum(next->getIdentificator()) != rdoRuntime::RDOEnumType::END)
	{
		RDOParser::s_parser()->error().error(next.src_info(), rdo::format(_T("Значение перечислимого типа уже существует: %s"), next.src_text().c_str()));
	}
	__enum()->add(next->getIdentificator());
}

rdoRuntime::RDOValue RDORTPEnum::findEnumValueWithThrow(CREF(RDOParserSrcInfo) src_info, CREF(tstring) value) const
{
	rdoRuntime::RDOValue result = cast(rdoRuntime::RDOValue(value, rdoRuntime::g_identificator));
	if (result.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		RDOParser::s_parser()->error().push_only(src_info, rdo::format(_T("Неверное значение параметра перечислимого типа: %s"), value.c_str()));
		RDOParser::s_parser()->error().push_only(this->src_info(), rdo::format(_T("Возможные значения: %s"), __enum()->asString().c_str()));
		RDOParser::s_parser()->error().push_done();
	}
	return result;
}

rdoRuntime::RDOValue RDORTPEnum::getFirstValue() const
{
	if (__enum()->getValues().empty())
	{
		RDOParser::s_parser()->error().error(src_info(), _T("Внутренняя ошибка: Пустой перечислимый тип"));
	}
	return rdoRuntime::RDOValue(*__enum());
}

CLOSE_RDO_PARSER_NAMESPACE
