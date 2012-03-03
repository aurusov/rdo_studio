#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdorss.h"
#include "rdortp.h"
#include "rdoparser.h"
#include "rdoparser_lexer.h"
#include <rdocalc.h>

namespace rdoParse 
{

int rsslex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void rsserror( char* mes )
{
}

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
		parser()->error_push_only( param.src_info(), "Слишком много параметров" );
		parser()->error_push_only( getType()->src_info(), "См. тип ресурса" );
		parser()->error_push_done();
	}
	try
	{
		if ( param->getAsString() == "*" )
		{
			m_params.push_back( (*m_currParam)->getType()->getDefaultValue( param ) );
			m_currParam++;
		}
		else
		{
			m_params.push_back( (*m_currParam)->getType()->getValue( param ) );
			m_currParam++;
		}
	}
	catch( RDOSyntaxException& )
	{
		parser()->error_modify( rdo::format("Для параметра '%s': ", (*m_currParam)->name().c_str()) );
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
