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

namespace rdoParse 
{

//////////////////////////// RDOSMR::RDOSMR /////////////////////////////////
RDOSMR::RDOSMR(string *_modelName)
	: modelName(_modelName),
	resourceFileName(NULL),
	oprIevFileName(NULL),
	frameFileName(NULL),
	statisticFileName(NULL),
	resultsFileName(NULL),
	traceFileName(NULL),
	showMode(SM_NoShow),
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

void RDOSMR::setValue(const char *descrName, string* RDOSMR::*pMem, string* newValue)
{
	if(this->*pMem != NULL)
		currParser->error("Second appearence of " + string(descrName) + " descriptor");

	this->*pMem = newValue;
}

void RDOSMR::setValue(const char *descrName, double* RDOSMR::*pMem, double* newValue)
{
	if(this->*pMem != NULL)
		currParser->error("Second appearence of " + string(descrName) + " descriptor");

	this->*pMem = newValue;
}

void RDOSMR::setShowMode(ShowMode sm)
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

void RDOSMR::setConstValue(string *constName, RDOFUNArithm *arithm)
{
	const RDOFUNConstant *cons = currParser->findFUNConst(constName);
	if(!cons)
		currParser->error("Undefined constant: " + *constName);

	RDOCalc *calc = arithm->createCalc(cons->descr->getType());
	currParser->runTime->addInitCalc(new RDOCalcSetConst(cons->number, calc));
}

void RDOSMR::setResParValue(string *resName, string *parName, RDOFUNArithm *arithm)
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

void RDOSMR::setSeed(string *seqName, int _base)
{
	const RDOFUNSequence *seq = currParser->findSequence(seqName);
	if(!seq)
		currParser->error("Undefined sequence: " + *seqName);

	seq->initSeq->setBase(_base);
}


}		// namespace rdoParse 

