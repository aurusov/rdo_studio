#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdosmr.h"
#include "rdoparser.h"
#include "rdoruntime.h"
#include "rdofun.h"
#include "rdorss.h"
#include "rdoparser_lexer.h"
#include "rdoparser_rdo.h"

namespace rdoParse 
{

int smr1lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void smr1error( char* mes )
{
//	throw RDOSMR1OkException("");
//	rdoParse::parser->error( mes );
}

int smr2lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void smr2error( char* mes )
{
	rdoParse::parser->error( mes );
}

//////////////////////////// RDOSMR::RDOSMR /////////////////////////////////
RDOSMR::RDOSMR(std::string *_modelName)
	: modelName(_modelName),
	resourceFileName(NULL),
	oprIevFileName(NULL),
	frameFileName(NULL),
	statisticFileName(NULL),
	resultsFileName(NULL),
	traceFileName(NULL),
	showMode(rdoSimulator::SM_NoShow),
	frameNumber(1),
	showRate(NULL),
	runStartTime(NULL),
	traceStartTime(NULL),
	traceEndTime(NULL),
	terminateIf(NULL),
	showModeSet(false),
	frameNumberSet(false)
{
	parser->setSMR(this);
}

void RDOSMR::setValue(const char *descrName, std::string* RDOSMR::*pMem, std::string* newValue)
{
	if(this->*pMem != NULL)
		parser->error("Second appearence of " + std::string(descrName) + " descriptor");

	this->*pMem = newValue;
}

void RDOSMR::setValue(const char *descrName, double* RDOSMR::*pMem, double* newValue)
{
	if(this->*pMem != NULL)
		parser->error("Second appearence of " + std::string(descrName) + " descriptor");

	this->*pMem = newValue;
}

void RDOSMR::setShowMode(rdoSimulator::ShowMode sm)
{
	if(showModeSet)
		parser->error("Second appearence of Show_mode descriptor");

	showModeSet = true;
	showMode = sm;
}

void RDOSMR::setFrameNumber(int fn)
{
	if(frameNumberSet)
		parser->error("Second appearence of Frame_number descriptor");

	frameNumberSet = true;
	frameNumber = fn;
}

void RDOSMR::setTerminateIf(RDOFUNLogic *logic)
{
	if(!parser->runTime->setTerminateIf(logic->calc))
		parser->error("Second Terminate_if entry");
}

void RDOSMR::setConstValue(std::string *constName, RDOFUNArithm *arithm)
{
	const RDOFUNConstant *cons = parser->findFUNConst(constName);
	if(!cons)
		parser->error("Undefined constant: " + *constName);

	rdoRuntime::RDOCalc *calc = arithm->createCalc(cons->descr->getType());
	parser->runTime->addInitCalc(new rdoRuntime::RDOCalcSetConst(cons->number, calc));
}

void RDOSMR::setResParValue(std::string *resName, std::string *parName, RDOFUNArithm *arithm)
{
	const RDORSSResource *res = parser->findRSSResource(resName);
	if(!res)
		parser->error("Undefined resource name: " + *resName);

	const RDORTPParamDesc *descr = res->getType()->findRTPParam(parName);
	if(!descr)
		parser->error("Undefined resource parameter name: " + *parName);

	int parNumb = res->getType()->getRTPParamNumber(parName);
	rdoRuntime::RDOCalc *calc = arithm->createCalc(descr->getType());
	parser->runTime->addInitCalc(new rdoRuntime::RDOSetResourceParamCalc(res->getNumber(), parNumb, calc));
}

void RDOSMR::setSeed(std::string *seqName, int _base)
{
	const RDOFUNSequence *seq = parser->findSequence(seqName);
	if(!seq)
		parser->error("Undefined sequence: " + *seqName);

	seq->initSeq->setBase(_base);
}

} // namespace rdoParse 
