#include "rdo_lib/rdo_parser/pch.h"

#include "rdo_lib/rdo_parser/rdorss.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"

namespace rdoParse 
{

int rsslex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void rsserror(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDORSSResource
// ----------------------------------------------------------------------------
RDORSSResource::RDORSSResource( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPResType* const _resType, int id )
	: RDOParserObject (_parser                                       )
	, RDOParserSrcInfo(_src_info                                     )
	, resType         (_resType                                      )
	, m_id            (id == UNDEFINED_ID ? _parser->getRSS_id() : id)
	, trace           (false                                         )
{
	parser()->insertRSSResource( this );
	m_currParam = resType->getParams().begin();
}

void RDORSSResource::writeModelStructure( std::ostream& stream ) const
{
	stream << (getID() + 1) << " " << name() << " " << getType()->getNumber() << std::endl;
}

void RDORSSResource::addParam( const RDOValue& param )
{
	if ( m_currParam == getType()->getParams().end() )
	{
		parser()->error().push_only( param.src_info(), "Слишком много параметров" );
		parser()->error().push_only( getType()->src_info(), "См. тип ресурса" );
		parser()->error().push_done();
	}
	try
	{
		if ( param->getAsString() == "*" )
		{
			if (!(*m_currParam)->getDefault().defined())
			{
				parser()->error().push_only(param.src_info(), _T("Невозможно использовать '*', к.т. отсутствует значение по-умолчанию"));
				parser()->error().push_only((*m_currParam)->getParamType()->src_info(), _T("См. описание параметра"));
				parser()->error().push_done();
			}
			m_params.push_back((*m_currParam)->getDefault().value());
			m_currParam++;
		}
		else
		{
			m_params.push_back((*m_currParam)->getParamType()->value_cast(param).value());
			m_currParam++;
		}
	}
	catch( RDOSyntaxException& )
	{
		parser()->error().modify( rdo::format("Для параметра '%s': ", (*m_currParam)->name().c_str()) );
	}
}

bool RDORSSResource::defined() const
{
	return m_currParam == getType()->getParams().end();
}

rdoRuntime::RDOCalc* RDORSSResource::createCalc()
{
	rdoRuntime::RDOCalc* calc = new rdoRuntime::RDOCalcCreateNumberedResource( parser()->runtime(), getType()->getNumber(), getTrace(), params(), getID(), getType()->isPermanent() );
	calc->setSrcInfo( src_info() );
	calc->setSrcText( "Создание ресурса " + calc->src_text() );
	return calc;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCResource
// ----------------------------------------------------------------------------
RDOPROCResource::RDOPROCResource( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPResType* const _resType, int id ):
	RDORSSResource( _parser, _src_info, _resType, id )
{
}

rdoRuntime::RDOCalc* RDOPROCResource::createCalc()
{
	rdoRuntime::RDOCalc* calc = new rdoRuntime::RDOCalcCreateProcessResource( parser()->runtime(), getType()->getNumber(), getTrace(), params(), getID(), getType()->isPermanent() );
	calc->setSrcInfo( src_info() );
	calc->setSrcText( "Создание ресурса " + calc->src_text() );
	return calc;
}

} // namespace rdoParse 
