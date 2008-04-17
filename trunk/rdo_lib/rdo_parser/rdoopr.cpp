#include "pch.h"
#include "rdoopr.h"
#include "rdoparser.h"
#include "rdortp.h"
#include "rdofun.h"
#include "rdopat.h"
#include "rdoparser_lexer.h"
#include <rdo_pattern.h>
#include <rdo_ie.h>
#include <rdo_rule.h>
#include <rdo_operation.h>
#include <rdo_keyboard.h>
#include <rdocalc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoParse 
{

int oprlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void oprerror( char* mes )
{
}

// ----------------------------------------------------------------------------
// ---------- RDOOPROperation
// ----------------------------------------------------------------------------
RDOOPROperation::RDOOPROperation( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivityHotKey( _parser, _src_info, _pattern_src_info )
{
	switch ( m_pattern->getType() )
	{
		case RDOPATPattern::PT_IE:
		{
			m_activity = static_cast<rdoRuntime::RDOPatternIrregEvent*>(m_pattern->getPatRuntime())->createActivity( parser()->runtime(), getName() );
			break;
		}
		case RDOPATPattern::PT_Rule:
		{
			m_activity = static_cast<rdoRuntime::RDOPatternRule*>(m_pattern->getPatRuntime())->createActivity( parser()->runtime(), getName() );
			break;
		}
		case RDOPATPattern::PT_Operation:
		{
			m_activity = static_cast<rdoRuntime::RDOPatternOperation*>(m_pattern->getPatRuntime())->createActivity( parser()->runtime(), getName() );
			break;
		}
		case RDOPATPattern::PT_Keyboard:
		{
			m_activity = static_cast<rdoRuntime::RDOPatternKeyboard*>(m_pattern->getPatRuntime())->createActivity( parser()->runtime(), getName() );
			break;
		}
		default:
		{
			parser()->error_push_only( src_info(), "Неизвестный тип образца" );
			parser()->error_push_only( m_pattern->src_info(), "См. образец" );
			parser()->error_push_done();
		}
	}
	parser()->insertOPROperation( this );
}

} // namespace rdoParse 
