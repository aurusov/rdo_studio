#include "pch.h"
#include "rdoopr.h"
#include "rdoparser.h"
#include "rdortp.h"
#include "rdofun.h"
#include "rdopat.h"
#include "rdopatrtime.h"
#include "rdoparser_lexer.h"
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

RDOOPROperation::RDOOPROperation( RDOParser* _parser, const std::string& _oprName, const std::string& patName ):
	RDOParserObject( _parser ),
	name( _oprName )
{
	pattern = parser->findPattern( patName );
	if(!pattern)
		parser->error("Undefined pattern name: " + getName());

	activity = pattern->patRuntime->createActivity(_oprName);
	currParam = 0;
}

void RDOOPROperation::addParam(int intParam)
{
	if(pattern->params.size() <= currParam)
		parser->error("Too much parameters for pattern : " + pattern->getName() + " in operation: " + getName());

	RDOFUNFunctionParam *param = pattern->params.at(currParam);
	rdoRuntime::RDOValue val = param->getType()->getRSSIntValue(intParam);
	activity->addParamCalc(new rdoRuntime::RDOSetPatternParamCalc(parser->runTime, currParam, val));
	currParam++;
}

void RDOOPROperation::addParam(double *realParam)
{
	if(pattern->params.size() <= currParam)
		parser->error("Too much parameters for pattern : " + pattern->getName() + " in operation: " + getName());

	RDOFUNFunctionParam *param = pattern->params.at(currParam);
	rdoRuntime::RDOValue val = param->getType()->getRSSRealValue(realParam);
	activity->addParamCalc(new rdoRuntime::RDOSetPatternParamCalc(parser->runTime, currParam, val));
	currParam++;
}

void RDOOPROperation::addParam( const std::string& stringParam )
{
	if(pattern->params.size() <= currParam)
		parser->error("Too much parameters for pattern : " + pattern->getName() + " in operation: " + getName());

	RDOFUNFunctionParam *param = pattern->params.at(currParam);
	rdoRuntime::RDOValue val = param->getType()->getRSSEnumValue(stringParam);
	activity->addParamCalc(new rdoRuntime::RDOSetPatternParamCalc(parser->runTime, currParam, val));
	currParam++;
}

void RDOOPROperation::addHotKey( const std::string& hotKey )
{
	if ( dynamic_cast<rdoRuntime::RDOActivityKeyboardRuntime*>(activity) ) {
		activity->addHotKey( hotKey );
	} else {
		parser->error( "ќбразец не €вл€етс€ клавиатурной операцией" );
	}
}

void RDOOPROperation::addParam()
{
	if(pattern->params.size() <= currParam)
		parser->error("Too much parameters for pattern : " + pattern->getName() + " in operation: " + getName());

	RDOFUNFunctionParam *param = pattern->params.at(currParam);
	rdoRuntime::RDOValue val = param->getType()->getParamDefaultValue();
	activity->addParamCalc(new rdoRuntime::RDOSetPatternParamCalc(parser->runTime, currParam, val));
	currParam++;
}

void RDOOPROperation::endOfDefinition()
{
	if(pattern->params.size() > currParam)
		parser->error("Too few parameters for pattern : " + pattern->getName() + " in operation: " + getName());
}

} // namespace rdoParse 
