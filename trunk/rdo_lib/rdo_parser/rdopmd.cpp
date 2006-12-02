#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdopmd.h"
#include "rdoparser.h"
#include "rdorss.h"
#include "rdortp.h"
#include "rdofun.h"
#include "rdocalcconst.h"
#include "rdoruntime.h"
#include "rdoparser_lexer.h"

namespace rdoParse 
{

int pmdlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void pmderror( char* mes )
{
	rdoParse::parser->error( mes );
}

//////////////////////////// RDOPMDPokaz::RDOPMDPokaz /////////////////////////////////
RDOPMDPokaz::RDOPMDPokaz(const std::string *const _name, bool _trace)
	: name(*_name), RDOPokazTrace(parser->runTime) 
{
	trace = _trace; 
}

void RDOPMDPokaz::endOfCreation()
{
	id = parser->getPMD_id();
	parser->insertPMDPokaz( this );
	parser->runTime->addRuntimePokaz(this);
}

//////////////////////////// RDOPMDWatchPar::RDOPMDWatchPar /////////////////////////////////
RDOPMDWatchPar::RDOPMDWatchPar(std::string *_name, bool _trace, std::string *_resName, std::string *_parName)
	: RDOPMDPokaz(_name, _trace) 
{
	const RDORSSResource *const res = parser->findRSSResource(_resName);
	if(!res)
		parser->error("Undefined resource name: " + *_resName);

	if( !res->getType()->isPermanent() )
		parser->error("Resource must be of permanent type: " + *_resName);

	const RDORTPParamDesc *const par = res->getType()->findRTPParam(_parName);
	if(!par)
		parser->error("Undefined parameter name: " + *_parName + " for resource " + *_resName);

	int type = par->getType()->getType();
	if ( type != RDORTPResParam::pt_int && type != RDORTPResParam::pt_real ) {
		parser->error("Enumerative parameter: " + *_resName + "." + *_parName + " not allowed in watch_par statement");
	}

	resNumber = res->getNumber();
	parNumber = res->getType()->getRTPParamNumber(_parName);
	endOfCreation();
}

std::string RDOPMDWatchPar::traceValue()
{
	return toString(currValue);
}

bool RDOPMDWatchPar::resetPokaz(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	watchNumber = 0;
	currValue = 0;
	sum = 0;
	sumSqr = 0;
	minValue = DBL_MAX;
	maxValue = DBL_MIN;

	timePrev = timeBegin = runtime->getCurrentTime();
	return true;
}

bool RDOPMDWatchPar::checkPokaz(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);
	double newValue = runtime->getResParamVal(resNumber, parNumber);
	if(newValue != currValue)
	{
		double currTime = runtime->getCurrentTime();
		double val = currValue * (currTime - timePrev);
		sum	+= val;
		sumSqr += val * val;
		timePrev = currTime;

		currValue = newValue;
		wasChanged = true;
		watchNumber ++;
		if(minValue > currValue)
			minValue = currValue;

		if(maxValue < currValue)
			maxValue = currValue;

		return true;
	}
	return false;
}

bool RDOPMDWatchPar::calcStat(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	double currTime = runtime->getCurrentTime();
	double val = currValue * (currTime - timePrev);
	sum	+= val;
	sumSqr += val * val;

	double average = sum / (currTime - timeBegin);

	runtime->getResult().width(30);
	runtime->getResult() << std::left << name 
		<< "\t" << traceValue()
		<< "\t" << watchNumber
		<< "\t" << average 
		<< "\t" << sumSqr 
		<< "\t" << minValue 
		<< "\t" << maxValue << '\n'; 

	return true;
}

//////////////////////////// RDOPMDWatchState::RDOPMDWatchState /////////////////////////////////
RDOPMDWatchState::RDOPMDWatchState(std::string *_name, bool _trace, RDOFUNLogic *_logic)
	: RDOPMDPokaz(_name, _trace), logicCalc(_logic->calc)
{
	endOfCreation();
}

std::string RDOPMDWatchState::traceValue()
{
	return currValue?"TRUE":"FALSE";
}

bool RDOPMDWatchState::resetPokaz(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	watchNumber = 0;
	currValue = fabs(logicCalc->calcValueBase(runtime)) > DBL_EPSILON;
	sum = 0;
	sumSqr = 0;
	minValue = DBL_MAX;
	maxValue = DBL_MIN;

	timePrev = timeBegin = runtime->getCurrentTime();
	return true;
}

bool RDOPMDWatchState::checkPokaz(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);
	bool newValue = fabs(logicCalc->calcValueBase(runtime)) > DBL_EPSILON;
	if(newValue && !currValue)	// from FALSE to TRUE
	{
		timePrev = runtime->getCurrentTime();
	}
	else if(!newValue && currValue)	// from TRUE to FALSE
	{
		double currTime = runtime->getCurrentTime();
		double val = currTime - timePrev;
		sum	+= val;
		sumSqr += val * val;

		wasChanged = true;
		watchNumber ++;

		if(minValue > val)
			minValue = val;

		if(maxValue < val)
			maxValue = val;
	}

	currValue = newValue;
	return true;

/*
	if(newValue != currValue)
	{
		double currTime = runtime->getCurrentTime();
		double val = currValue * (currTime - timePrev);
		sum	+= val;
		sumSqr += val * val;
		timePrev = currTime;

		currValue = newValue;
		wasChanged = true;
		if(!newValue)
			watchNumber ++;

		if(minValue > val)
			minValue = val;

		if(maxValue < val)
			maxValue = val;

		return true;
	}
	return false;
*/
}

bool RDOPMDWatchState::calcStat(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	double currTime = runtime->getCurrentTime();
	double val = currValue * (currTime - timePrev);
	sum	+= val;
	sumSqr += val * val;

	double average = sum / (currTime - timeBegin);

	runtime->getResult().width(30);
	runtime->getResult() << std::left << name 
		<< "\t" << traceValue() 
		<< "\t" << watchNumber
		<< "\t" << average 
		<< "\t" << sumSqr 
		<< "\t" << minValue 
		<< "\t" << maxValue << '\n'; 

	return true;
}

//////////////////////////// RDOPMDWatchQuant::RDOPMDWatchQuant /////////////////////////////////
RDOPMDWatchQuant::RDOPMDWatchQuant(std::string *_name, bool _trace, std::string *_resTypeName)
	: RDOPMDPokaz(_name, _trace)
{
	funGroup = new RDOFUNGroupLogic(5, _resTypeName);
	endOfCreation();
}

void RDOPMDWatchQuant::setLogic(RDOFUNLogic *_logic)
{
	logicCalc = _logic->calc;
	parser->getFUNGroupStack().pop_back();
}

void RDOPMDWatchQuant::setLogicNoCheck()
{
	logicCalc = new rdoRuntime::RDOCalcConst(1);
	parser->getFUNGroupStack().pop_back();
}

std::string RDOPMDWatchQuant::traceValue()
{
	return toString(currValue);
}

bool RDOPMDWatchQuant::resetPokaz(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	watchNumber = 0;
	currValue = 0;
	sum = 0;
	sumSqr = 0;
	minValue = DBL_MAX;
	maxValue = DBL_MIN;

	timePrev = timeBegin = runtime->getCurrentTime();
	return true;
}

bool RDOPMDWatchQuant::checkPokaz(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	int newValue = 0;
	for(std::vector< rdoRuntime::RDOResource* >::iterator it = runtime->allResources.begin(); 
													it != runtime->allResources.end(); it++)
	{
		if(*it == NULL)
			continue;

		if((*it)->type != funGroup->resType->getNumber())
			continue;

		runtime->pushGroupFunc(*it);
		if(logicCalc->calcValueBase(runtime))
			newValue++;

		runtime->popGroupFunc();
	}

	if(newValue != currValue)
	{
		double currTime = runtime->getCurrentTime();
		double val = currValue * (currTime - timePrev);
		sum	+= val;
		sumSqr += val * val;
		timePrev = currTime;

		currValue = newValue;
		wasChanged = true;
		watchNumber ++;
		if(minValue > currValue)
			minValue = currValue;

		if(maxValue < currValue)
			maxValue = currValue;

		return true;
	}
	return false;
}

bool RDOPMDWatchQuant::calcStat(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	double currTime = runtime->getCurrentTime();
	double val = currValue * (currTime - timePrev);
	sum	+= val;
	sumSqr += val * val;

	double average = sum / (currTime - timeBegin);

	runtime->getResult().width(30);
	runtime->getResult() << std::left << name 
		<< "\t" << traceValue()
		<< "\t" << watchNumber
		<< "\t" << average 
		<< "\t" << sumSqr 
		<< "\t" << minValue 
		<< "\t" << maxValue << '\n'; 

	return true;
}

//////////////////////////// RDOPMDWatchValue::RDOPMDWatchValue /////////////////////////////////
RDOPMDWatchValue::RDOPMDWatchValue(std::string *_name, bool _trace, std::string *_resTypeName)
	: RDOPMDPokaz(_name, _trace)
{
	funGroup = new RDOFUNGroupLogic(5, _resTypeName);
	wasChanged = false;
	endOfCreation();
}

void RDOPMDWatchValue::setLogic(RDOFUNLogic *_logic, RDOFUNArithm *_arithm)
{
	logicCalc  = _logic->calc;
	arithmCalc = _arithm->createCalc();
	parser->getFUNGroupStack().pop_back();
}

void RDOPMDWatchValue::setLogicNoCheck(RDOFUNArithm *_arithm)
{
	logicCalc = new rdoRuntime::RDOCalcConst(1);
	arithmCalc = _arithm->createCalc();
	parser->getFUNGroupStack().pop_back();
}

std::string RDOPMDWatchValue::traceValue()
{
	return toString(currValue);
}

bool RDOPMDWatchValue::resetPokaz(RDOSimulator *sim)
{
	watchNumber = 0;
	currValue = 0;
	sum = 0;
	sumSqr = 0;
	minValue = DBL_MAX;
	maxValue = DBL_MIN;

	return true;
}

bool RDOPMDWatchValue::checkPokaz(RDOSimulator *sim)
{
	return false;
}

bool RDOPMDWatchValue::calcStat(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	double average, averageSqr, deviation;
	if(watchNumber < 2)
		average = averageSqr = deviation = 0;
	else
	{
		average = sum / watchNumber;
		averageSqr = sumSqr - 2 * average * sum + watchNumber * average * average;
		averageSqr = sqrt(averageSqr / (watchNumber - 1));
		deviation = averageSqr / sqrt(watchNumber);
	}

	runtime->getResult().width(30);
	runtime->getResult() << std::left << name 
		<< "\t" << watchNumber
		<< "\t" << average 
		<< "\t" << averageSqr
		<< "\t" << deviation
		<< "\t" << minValue 
		<< "\t" << maxValue << '\n'; 

	return true;
}

bool RDOPMDWatchValue::checkResourceErased( rdoRuntime::RDOResource* res )
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	if(res->type != funGroup->resType->getNumber())
		return false;

	runtime->pushGroupFunc(res);
	if(logicCalc->calcValueBase(runtime))
	{
		currValue = arithmCalc->calcValueBase(runtime);
		tracePokaz();
//		runtime->getTracer()->writePokaz(runtime, this);

		sum	+= currValue;
		sumSqr += currValue * currValue;
		watchNumber++;

		if(minValue > currValue)
			minValue = currValue;

		if(maxValue < currValue)
			maxValue = currValue;

		return true;
	}

	runtime->popGroupFunc();
	return true;
}

//////////////////////////// RDOPMDGetValue::RDOPMDGetValue /////////////////////////////////
RDOPMDGetValue::RDOPMDGetValue(std::string *_name, RDOFUNArithm *_arithm)
	: RDOPMDPokaz(_name, false), arithmCalc(_arithm->createCalc())
{
	endOfCreation();
}

std::string RDOPMDGetValue::traceValue()
{
	return "ERROR";
}

bool RDOPMDGetValue::resetPokaz(RDOSimulator *sim)
{
	return false;
}

bool RDOPMDGetValue::checkPokaz(RDOSimulator *sim)
{
	return false;
}

bool RDOPMDGetValue::calcStat(RDOSimulator *sim)
{
	rdoRuntime::RDORuntime* runtime = dynamic_cast< rdoRuntime::RDORuntime* >(sim);

	runtime->getResult().width(30);
	runtime->getResult() << std::left << name 
		<< "\t" << arithmCalc->calcValueBase(runtime) << '\n';

	return true;
}

void RDOPMDWatchPar::writePokazStructure(std::ostream &stream) const
{
	stream << "\t" << name << "\t" << traceId() << "\twatch_par" << std::endl;
}

void RDOPMDWatchState::writePokazStructure(std::ostream &stream) const
{
	stream << "\t" << name << "\t" << traceId() << "\twatch_state" << std::endl;
}

void RDOPMDWatchQuant::writePokazStructure(std::ostream &stream) const
{
	stream << "\t" << name << "\t" << traceId() << "\twatch_quant" << std::endl;
}

void RDOPMDWatchValue::writePokazStructure(std::ostream &stream) const
{
	stream << "\t" << name << "\t" << traceId() << "\twatch_value" << std::endl;
}

void RDOPMDGetValue::writePokazStructure(std::ostream &stream) const
{
	stream << "\t" << name << "\t" << traceId() << "\tget_value" << std::endl;
}

}		// namespace rdoParse 
