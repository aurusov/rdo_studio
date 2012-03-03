#include "pch.h"
#include "rdoparser_enum.h"
#include "rdoparser.h"

namespace rdoParse 
{

// ----------------------------------------------------------------------------
// ---------- RDORTPEnum
// ----------------------------------------------------------------------------
RDORTPEnum::RDORTPEnum(CPTR(RDOParserObject) _parent, CREF(RDOValue) first)
	: RDOType        (g_unknow)
	, RDOParserObject(_parent )
{
	m_type = new rdoRuntime::RDOEnumType(parser()->runtime());
	add(first);
}

RDORTPEnum::~RDORTPEnum()
{
}

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

void RDORTPEnum::add( const RDOValue& next )
{
	if ( __enum()->findEnum( next->getIdentificator() ) != rdoRuntime::RDOEnumType::END )
	{
		parser()->error( next.src_info(), rdo::format("Значение перечислимого типа уже существует: %s", next.src_text().c_str()) );
	}
	__enum()->add( next->getIdentificator() );
}

rdoRuntime::RDOValue RDORTPEnum::findEnumValueWithThrow( const RDOParserSrcInfo& val_src_info, const std::string& val ) const
{
	rdoRuntime::RDOValue result = cast(rdoRuntime::RDOValue(val, rdoRuntime::g_identificator));
	if (result.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		parser()->error_push_only( val_src_info, rdo::format("Неверное значение параметра перечислимого типа: %s", val.c_str()) );
		parser()->error_push_only( src_info(), rdo::format("Возможные значения: %s", __enum()->asString().c_str()) );
		parser()->error_push_done();
	}
	return result;
}

rdoRuntime::RDOValue RDORTPEnum::getFirstValue() const
{
	if ( __enum()->getValues().empty() )
	{
		parser()->error( src_info(), "Внутренняя ошибка: Пустой перечислимый тип" );
	}
	return rdoRuntime::RDOValue( *__enum() );
}

} // namespace rdoParse
