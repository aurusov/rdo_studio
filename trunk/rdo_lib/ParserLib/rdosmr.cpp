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

int smr1lex( int* lpval, void* lexer )
{
	((RDOFlexLexer*)lexer)->m_lpval = lpval;
	return ((RDOFlexLexer*)lexer)->yylex();
}
void smr1error( char* mes )
{
	throw RDOSMR1OkException("");
//	rdoParse::currParser->error( mes );
}

int smr2lex( int* lpval, void* lexer )
{
	((RDOFlexLexer*)lexer)->m_lpval = lpval;
	return ((RDOFlexLexer*)lexer)->yylex();
}
void smr2error( char* mes )
{
	rdoParse::currParser->error( mes );
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
	showMode(RDOSimulatorNS::SM_NoShow),
	frameNumber(1),
	showRate(NULL),
	runStartTime(NULL),
	traceStartTime(NULL),
	traceEndTime(NULL),
	terminateIf(NULL),
	showModeSet(false),
	frameNumberSet(false)
{
	currParser->setSMR(this);
}

void RDOSMR::setValue(const char *descrName, std::string* RDOSMR::*pMem, std::string* newValue)
{
	if(this->*pMem != NULL)
		currParser->error("Second appearence of " + std::string(descrName) + " descriptor");

	this->*pMem = newValue;
}

void RDOSMR::setValue(const char *descrName, double* RDOSMR::*pMem, double* newValue)
{
	if(this->*pMem != NULL)
		currParser->error("Second appearence of " + std::string(descrName) + " descriptor");

	this->*pMem = newValue;
}

void RDOSMR::setShowMode(RDOSimulatorNS::ShowMode sm)
{
	if(showModeSet)
		currParser->error("Second appearence of Show_mode descriptor");

	showModeSet = true;
	showMode = sm;
}

void RDOSMR::setFrameNumber(int fn)
{
	if(frameNumberSet)
		currParser->error("Second appearence of Frame_number descriptor");

	frameNumberSet = true;
	frameNumber = fn;
}

void RDOSMR::setTerminateIf(RDOFUNLogic *logic)
{
	if(!currParser->runTime->setTerminateIf(logic->calc))
		currParser->error("Second Terminate_if entry");
}

void RDOSMR::setConstValue(std::string *constName, RDOFUNArithm *arithm)
{
	const RDOFUNConstant *cons = currParser->findFUNConst(constName);
	if(!cons)
		currParser->error("Undefined constant: " + *constName);

	RDOCalc *calc = arithm->createCalc(cons->descr->getType());
	currParser->runTime->addInitCalc(new RDOCalcSetConst(cons->number, calc));
}

void RDOSMR::setResParValue(std::string *resName, std::string *parName, RDOFUNArithm *arithm)
{
	const RDORSSResource *res = currParser->findRSSResource(resName);
	if(!res)
		currParser->error("Undefined resource name: " + *resName);

	const RDORTPParamDesc *descr = res->getType()->findRTPParam(parName);
	if(!descr)
		currParser->error("Undefined resource parameter name: " + *parName);

	int parNumb = res->getType()->getRTPParamNumber(parName);
	RDOCalc *calc = arithm->createCalc(descr->getType());
	currParser->runTime->addInitCalc(new RDOSetResourceParamCalc(res->getNumber(), parNumb, calc));
}

void RDOSMR::setSeed(std::string *seqName, int _base)
{
	const RDOFUNSequence *seq = currParser->findSequence(seqName);
	if(!seq)
		currParser->error("Undefined sequence: " + *seqName);

	seq->initSeq->setBase(_base);
}


}		// namespace rdoParse 

