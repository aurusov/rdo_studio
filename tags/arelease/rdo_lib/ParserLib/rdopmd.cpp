#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

#include "rdopmd.h"
#include "rdoparser.h"
#include "rdorss.h"
#include "rdortp.h"
#include "rdofun.h"
#include "rdocalcconst.h"
#include "rdoruntime.h"


namespace rdoParse 
{

//////////////////////////// RDOPMDPokaz::RDOPMDPokaz /////////////////////////////////
RDOPMDPokaz::RDOPMDPokaz(const string *const _name, bool _trace)
	: name(*_name), RDOPokazTrace(currParser->runTime) 
{
	trace = _trace; 
	currParser->runTime->addRuntimePokaz(this);
	id = currParser->pokazCounter++;
}

//////////////////////////// RDOPMDWatchPar::RDOPMDWatchPar /////////////////////////////////
RDOPMDWatchPar::RDOPMDWatchPar(string *_name, bool _trace, string *_resName, string *_parName)
	: RDOPMDPokaz(_name, _trace) 
{
	const RDORSSResource *const res = currParser->findRSSResource(_resName);
	if(!res)
		currParser->error("Undefined resource name: " + *_resName);

	if(!res->getType()->isPerm())
		currParser->error("Resource must be of permanent type: " + *_resName);

	const RDORTPParamDesc *const par = res->getType()->findRTPParam(_parName);
	if(!par)
		currParser->error("Undefined parameter name: " + *_parName + " for resource " + *_resName);

	int type = par->getType()->getType();
	if((type != 0) && (type != 1))
		currParser->error("Enumerative parameter: " + *_resName + "." + *_parName + " not allowed in watch_par statement");

	resNumber = res->getNumber();
	parNumber = res->getType()->getRTPParamNumber(_parName);
}

string RDOPMDWatchPar::traceValue()
{
	return toString(currValue);
}

bool RDOPMDWatchPar::resetPokaz(RDOSimulator *sim)
{
	RDORuntime *runtime = dynamic_cast<RDORuntime *>(sim);

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
	RDORuntime *runtime = dynamic_cast<RDORuntime *>(sim);
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
	RDORuntime *runtime = dynamic_cast<RDORuntime *>(sim);

	double currTime = runtime->getCurrentTime();
	double val = currValue * (currTime - timePrev);
	sum	+= val;
	sumSqr += val * val;

	double average = sum / (currTime - timeBegin);

	runtime->getResult().width(30);
	runtime->getResult() << left << name 
		<< "\t" << traceValue()
		<< "\t" << watchNumber
		<< "\t" << average 
		<< "\t" << sumSqr 
		<< "\t" << minValue 
		<< "\t" << maxValue << '\n'; 

	return true;
}

//////////////////////////// RDOPMDWatchState::RDOPMDWatchState /////////////////////////////////
RDOPMDWatchState::RDOPMDWatchState(string *_name, bool _trace, RDOFUNLogic *_logic)
	: RDOPMDPokaz(_name, _trace), logicCalc(_logic->calc)
{
}

string RDOPMDWatchState::traceValue()
{
	return currValue?"TRUE":"FALSE";
}

bool RDOPMDWatchState::resetPokaz(RDOSimulator *sim)
{
	RDORuntime *runtime = dynamic_cast<RDORuntime *>(sim);

	watchNumber = 0;
	currValue = false;
	sum = 0;
	sumSqr = 0;
	minValue = DBL_MAX;
	maxValue = DBL_MIN;

	timePrev = timeBegin = runtime->getCurrentTime();
	return true;
}

bool RDOPMDWatchState::checkPokaz(RDOSimulator *sim)
{
	RDORuntime *runtime = dynamic_cast<RDORuntime *>(sim);
	bool newValue = fabs(logicCalc->calcValue(runtime)) > DBL_EPSILON;
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
}

bool RDOPMDWatchState::calcStat(RDOSimulator *sim)
{
	RDORuntime *runtime = dynamic_cast<RDORuntime *>(sim);

	double currTime = runtime->getCurrentTime();
	double val = currValue * (currTime - timePrev);
	sum	+= val;
	sumSqr += val * val;

	double average = sum / (currTime - timeBegin);

	runtime->getResult().width(30);
	runtime->getResult() << left << name 
		<< "\t" << traceValue() 
		<< "\t" << watchNumber
		<< "\t" << average 
		<< "\t" << sumSqr 
		<< "\t" << minValue 
		<< "\t" << maxValue << '\n'; 

	return true;
}

//////////////////////////// RDOPMDWatchQuant::RDOPMDWatchQuant /////////////////////////////////
RDOPMDWatchQuant::RDOPMDWatchQuant(string *_name, bool _trace, string *_resTypeName)
	: RDOPMDPokaz(_name, _trace)
{
	funGroup = new RDOFUNGroup(5, _resTypeName);
}

void RDOPMDWatchQuant::setLogic(RDOFUNLogic *_logic)
{
	logicCalc = _logic->calc;
	currParser->fUNGroupStack.pop_back();
}

void RDOPMDWatchQuant::setLogicNoCheck()
{
	logicCalc = new RDOCalcConst(1);
	currParser->fUNGroupStack.pop_back();
}

string RDOPMDWatchQuant::traceValue()
{
	return toString(currValue);
}

bool RDOPMDWatchQuant::resetPokaz(RDOSimulator *sim)
{
	RDORuntime *runtime = dynamic_cast<RDORuntime *>(sim);

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
	RDORuntime *runtime = dynamic_cast<RDORuntime *>(sim);

	int newValue = 0;
	for(vector<RDOResource *>::iterator it = runtime->allResources.begin(); 
													it != runtime->allResources.end(); it++)
	{
		if(*it == NULL)
			continue;

		if((*it)->type != funGroup->resType->getType())
			continue;

		runtime->pushGroupFunc(*it);
		if(logicCalc->calcValue(runtime))
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
	RDORuntime *runtime = dynamic_cast<RDORuntime *>(sim);

	double currTime = runtime->getCurrentTime();
	double val = currValue * (currTime - timePrev);
	sum	+= val;
	sumSqr += val * val;

	double average = sum / (currTime - timeBegin);

	runtime->getResult().width(30);
	runtime->getResult() << left << name 
		<< "\t" << traceValue()
		<< "\t" << watchNumber
		<< "\t" << average 
		<< "\t" << sumSqr 
		<< "\t" << minValue 
		<< "\t" << maxValue << '\n'; 

	return true;
}

//////////////////////////// RDOPMDWatchValue::RDOPMDWatchValue /////////////////////////////////
RDOPMDWatchValue::RDOPMDWatchValue(string *_name, bool _trace, string *_resTypeName)
	: RDOPMDPokaz(_name, _trace)
{
	funGroup = new RDOFUNGroup(5, _resTypeName);
	wasChanged = false;
}

void RDOPMDWatchValue::setLogic(RDOFUNLogic *_logic, RDOFUNArithm *_arithm)
{
	logicCalc = _logic->calc;
	arithmCalc = _arithm->createCalc();
	currParser->fUNGroupStack.pop_back();
}

void RDOPMDWatchValue::setLogicNoCheck(RDOFUNArithm *_arithm)
{
	logicCalc = new RDOCalcConst(1);
	arithmCalc = _arithm->createCalc();
	currParser->fUNGroupStack.pop_back();
}

string RDOPMDWatchValue::traceValue()
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
	RDORuntime *runtime = dynamic_cast<RDORuntime *>(sim);

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
	runtime->getResult() << left << name 
		<< "\t" << watchNumber
		<< "\t" << average 
		<< "\t" << averageSqr
		<< "\t" << deviation
		<< "\t" << minValue 
		<< "\t" << maxValue << '\n'; 

	return true;
}

bool RDOPMDWatchValue::checkResourceErased(RDOResource *res)
{
	RDORuntime *runtime = dynamic_cast<RDORuntime *>(sim);

	if(res->type != funGroup->resType->getType())
		return false;

	runtime->pushGroupFunc(res);
	if(logicCalc->calcValue(runtime))
	{
		currValue = arithmCalc->calcValue(runtime);
		runtime->getTracer()->writePokaz(runtime, this);

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
RDOPMDGetValue::RDOPMDGetValue(string *_name, RDOFUNArithm *_arithm)
	: RDOPMDPokaz(_name, false), arithmCalc(_arithm->createCalc())
{
}

string RDOPMDGetValue::traceValue()
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
	RDORuntime *runtime = dynamic_cast<RDORuntime *>(sim);

	runtime->getResult().width(30);
	runtime->getResult() << left << name 
		<< "\t" << arithmCalc->calcValue(runtime) << '\n';

	return true;
}

}		// namespace rdoParse 
