#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoopr.h"
#include "rdoparser.h"
#include "rdortp.h"
#include "rdofun.h"
#include "rdopat.h"
#include "rdopatrtime.h"
#include "rdoruntime.h"
#include "rdoparser_lexer.h"

namespace rdoParse 
{

int oprlex( int* lpval, void* lexer )
{
	((RDOFlexLexer*)lexer)->m_lpval = lpval;
	return ((RDOFlexLexer*)lexer)->yylex();
}
void oprerror( char* mes )
{
	rdoParse::currParser->error( mes );
}

RDOOPROperation::RDOOPROperation(std::string *_oprName, std::string *patName)
:name(_oprName)
{
	pattern = currParser->findPattern(patName);
	if(!pattern)
		currParser->error("Undefined pattern name: " + *getName());

	activity = pattern->patRuntime->createActivity(_oprName);
	currParam = 0;
}

void RDOOPROperation::addParam(int intParam)
{
	if(pattern->params.size() <= currParam)
		currParser->error("Too much parameters for pattern : " + *pattern->getName() + " in operation: " + *getName());

	RDOFUNFunctionParam *param = pattern->params.at(currParam);
	RDOValue val = param->getType()->getRSSIntValue(intParam);
	activity->addParamCalc(new RDOSetPatternParamCalc(currParam, val));
	currParam++;
}

void RDOOPROperation::addParam(double *realParam)
{
	if(pattern->params.size() <= currParam)
		currParser->error("Too much parameters for pattern : " + *pattern->getName() + " in operation: " + *getName());

	RDOFUNFunctionParam *param = pattern->params.at(currParam);
	RDOValue val = param->getType()->getRSSRealValue(realParam);
	activity->addParamCalc(new RDOSetPatternParamCalc(currParam, val));
	currParam++;
}

void RDOOPROperation::addParam(std::string *stringParam)
{
	if(pattern->params.size() <= currParam)
		currParser->error("Too much parameters for pattern : " + *pattern->getName() + " in operation: " + *getName());

	RDOFUNFunctionParam *param = pattern->params.at(currParam);
	RDOValue val = param->getType()->getRSSEnumValue(stringParam);
	activity->addParamCalc(new RDOSetPatternParamCalc(currParam, val));
	currParam++;
}

void RDOOPROperation::addHotKey(std::string *hotKey)
{
	activity->addHotKey(hotKey);
}

void RDOOPROperation::addParam()
{
	if(pattern->params.size() <= currParam)
		currParser->error("Too much parameters for pattern : " + *pattern->getName() + " in operation: " + *getName());

	RDOFUNFunctionParam *param = pattern->params.at(currParam);
	RDOValue val = param->getType()->getRSSDefaultValue();
	activity->addParamCalc(new RDOSetPatternParamCalc(currParam, val));
	currParam++;
}

void RDOOPROperation::endOfDefinition()
{
	if(pattern->params.size() > currParam)
		currParser->error("Too few parameters for pattern : " + *pattern->getName() + " in operation: " + *getName());
}

}		// namespace rdoParse 
