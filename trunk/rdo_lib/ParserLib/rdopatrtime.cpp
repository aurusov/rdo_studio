#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdopatrtime.h"
#include "rdoruntime.h"
#include "rdoparser.h"
#include "rdohotkey.h"

namespace rdoRuntime
{

void RDOPatternRuntime::addEndCalc(RDOCalc *calc)
{
	throw RDOInternalException("parser internal error 0005");
}

RDOActivityRuntime *RDOPatternRuntime::createActivity(RDOCalc *condition, string *_oprName)
{
	throw RDOInternalException("parser internal error 0501");
}

RDOPatternRuntime::RDOPatternRuntime(RDORuntime *_runtime, bool _trace)
	: runtime(_runtime), trace(_trace) 
{ 
	runtime->addPattern(this); 
}

RDORuleRuntime::RDORuleRuntime(RDORuntime *rTime, bool _trace)
	:RDOPatternRuntime(rTime, _trace) {}

RDOIERuntime::RDOIERuntime(RDORuntime *rTime, bool _trace)
	:RDOPatternRuntime(rTime, _trace) {}

RDOOperationRuntime::RDOOperationRuntime(RDORuntime *rTime, bool _trace)
	:RDOPatternRuntime(rTime, _trace) {}

vector<RDOResourceTrace *> RDOActivityRuntime::getRelevantResources(RDOSimulator *sim)
{
	RDORuntime *runtime = (RDORuntime *)sim;

	vector<RDOResourceTrace *> result;
	int size = relResNumbers.size();
	for(int i = 0; i < size; i++)
		result.push_back(runtime->findResource(relResNumbers[i]));

	return result; 
}

bool RDORuleRuntime::choiceFrom(RDOSimulator *sim)
{
	return RDOPatternRuntime::choiceFrom(sim);
}

void RDORuleRuntime::convertRule(RDOSimulator *sim)
{
	RDORuntime *runtime = (RDORuntime *)sim;
	int size = beginCalcs.size();
	for(int i = 0; i < size; i++)
		beginCalcs.at(i)->calcValueBase(runtime);
}

double RDOIERuntime::getNextTimeInterval(RDOSimulator *sim)
{
	RDORuntime *runtime = (RDORuntime *)sim;
	return timeField->calcValueBase(runtime);
}

void RDOIERuntime::convertEvent(RDOSimulator *sim)
{
	RDORuntime *runtime = (RDORuntime *)sim;
	choiceFrom(sim);	// to set permanent resource relevent numbers
	int size = beginCalcs.size();
	for(int i = 0; i < size; i++)
		beginCalcs.at(i)->calcValueBase(runtime);
}

bool RDOPatternRuntime::choiceFrom(RDOSimulator *sim)
{
	RDORuntime *runtime = (RDORuntime *)sim;
	int size = choiceFromCalcs.size();
	for(int i = 0; i < size; i++)
		if(!choiceFromCalcs[i]->calcValueBase(runtime))
			return 0;

	return 1;
}

bool RDOOperationRuntime::choiceFrom(RDOSimulator *sim)
{
	return RDOPatternRuntime::choiceFrom(sim);
}

bool RDOIERuntime::choiceFrom(RDOSimulator *sim)
{
	return RDOPatternRuntime::choiceFrom(sim);
}

double RDOOperationRuntime::getNextTimeInterval(RDOSimulator *sim)
{
	RDORuntime *runtime = (RDORuntime *)sim;
	return timeField->calcValueBase(runtime);
}

void RDOOperationRuntime::convertBegin(RDOSimulator *sim)
{
	RDORuntime *runtime = (RDORuntime *)sim;
	int size = beginCalcs.size();
	for(int i = 0; i < size; i++)
		beginCalcs.at(i)->calcValueBase(runtime);
}

void RDOOperationRuntime::convertEnd(RDOSimulator *sim)
{
	RDORuntime *runtime = (RDORuntime *)sim;
	int size = endCalcs.size();
	for(int i = 0; i < size; i++)
		endCalcs.at(i)->calcValueBase(runtime);
}

RDOOperationTrace *RDOActivityOperationRuntime::clone2(RDOSimulator *sim) 
{ 
	RDORuntime *runtime = (RDORuntime *)sim;
	RDOActivityOperationRuntime *newActivity = new RDOActivityOperationRuntime(runtime, pattern, trace, oprName);
	newActivity->setParamsCalcs.insert(newActivity->setParamsCalcs.begin(), 
		setParamsCalcs.begin(), setParamsCalcs.end());
	newActivity->relResNumbers.insert(newActivity->relResNumbers.begin(), relResNumbers.begin(), relResNumbers.end());

	return newActivity;
}

RDOActivityRuntime *RDORuleRuntime::createActivity(string *_oprName)
{
	RDOActivityRuleRuntime *rule = new RDOActivityRuleRuntime(runtime, this, trace, _oprName);
	currParser->runTime->addRuntimeRule(rule); 
	return rule;
}

RDOActivityRuntime *RDORuleRuntime::createActivity(RDOCalc *condition, string *_oprName)
{
	RDOActivityRuleRuntime *rule = new RDOActivityRuleRuntime(runtime, this, trace, condition, _oprName);
	currParser->runTime->addRuntimeRule(rule); 
	return rule;
}

RDOActivityRuntime *RDOIERuntime::createActivity(string *_oprName)
{
	RDOActivityIERuntime *ie = new RDOActivityIERuntime(runtime, this, trace, _oprName);
	currParser->runTime->addRuntimeIE(ie); 
	return ie;
}

RDOActivityRuntime *RDOOperationRuntime::createActivity(string *_oprName)
{
	RDOActivityOperationRuntime *oper = new RDOActivityOperationRuntime(runtime, this, trace, _oprName);
	currParser->runTime->addRuntimeOperation(oper); 
	return oper;
}

RDOActivityRuntime *RDOOperationRuntime::createActivity(RDOCalc *condition, string *_oprName)
{
	RDOActivityOperationRuntime *oper = new RDOActivityOperationRuntime(runtime, this, trace, condition, _oprName);
	currParser->runTime->addRuntimeOperation(oper); 
	return oper;
}

RDOActivityRuntime *RDOKeyboardRuntime::createActivity(string *_oprName)
{
	RDOActivityKeyboardRuntime *oper = new RDOActivityKeyboardRuntime(runtime, this, trace, _oprName);
	currParser->runTime->addRuntimeOperation(oper); 
	return oper;
}

RDOActivityRuntime *RDOKeyboardRuntime::createActivity(RDOCalc *condition, string *_oprName)
{
	RDOActivityKeyboardRuntime *oper = new RDOActivityKeyboardRuntime(runtime, this, trace, condition, _oprName);
	currParser->runTime->addRuntimeOperation(oper); 
	return oper;
}

void RDOActivityRuntime::setPatternParameters(RDOSimulator *sim) 
{
	RDORuntime *runtime = (RDORuntime *)sim;
	int size = setParamsCalcs.size();
	for(int i = 0; i < size; i++)
		setParamsCalcs.at(i)->calcValueBase(runtime);
}

void RDOActivityRuntime::addHotKey(string *hotKey)
{
	currParser->error("This pattern is not of keyboard type");
}

RDOActivityRuleRuntime::RDOActivityRuleRuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, string *_oprName)
	: RDORuleTrace(rTime), RDOActivityRuntime(_pattern, _oprName) { trace = _trace; haveAdditionalCondition = false;}

RDOActivityRuleRuntime::RDOActivityRuleRuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, RDOCalc *condition, string *_oprName)
	: RDORuleTrace(rTime), RDOActivityRuntime(_pattern, _oprName), additionalCondition(condition) { trace = _trace; haveAdditionalCondition = true;}

RDOActivityIERuntime::RDOActivityIERuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, string *_oprName)
	: RDOIETrace(rTime), RDOActivityRuntime(_pattern, _oprName) { trace = _trace; }

RDOActivityOperationRuntime::RDOActivityOperationRuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, string *_oprName)
	: RDOOperationTrace(rTime), RDOActivityRuntime(_pattern, _oprName) { trace = _trace; haveAdditionalCondition = false;}

RDOActivityOperationRuntime::RDOActivityOperationRuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, RDOCalc *condition, string *_oprName)
	: RDOOperationTrace(rTime), RDOActivityRuntime(_pattern, _oprName), additionalCondition(condition) { trace = _trace; haveAdditionalCondition = true;}

bool RDOActivityRuleRuntime::choiceFrom(RDOSimulator *sim) 
{ 
	((RDORuntime *)sim)->setCurrentActivity(this);
	if(haveAdditionalCondition)
		if(!additionalCondition->calcValueBase(dynamic_cast<RDORuntime *>(sim)))
			return false;

	return ((RDORuleRuntime*)pattern)->choiceFrom(sim); 
}
void RDOActivityRuleRuntime::convertRule(RDOSimulator *sim) 
{ 
	((RDORuntime *)sim)->setCurrentActivity(this);
	((RDORuleRuntime*)pattern)->convertRule(sim); 
}


double RDOActivityIERuntime::getNextTimeInterval(RDOSimulator *sim) 
{ 
	((RDORuntime *)sim)->setCurrentActivity(this);
	return ((RDOIERuntime*)pattern)->getNextTimeInterval(sim); 
}
void RDOActivityIERuntime::convertEvent(RDOSimulator *sim) 
{ 
	((RDORuntime *)sim)->setCurrentActivity(this);
	((RDOIERuntime*)pattern)->convertEvent(sim); 
}
bool RDOActivityIERuntime::choiceFrom(RDOSimulator *sim) 
{ 
	((RDORuntime *)sim)->setCurrentActivity(this);
	return ((RDOIERuntime*)pattern)->choiceFrom(sim); 
}



double RDOActivityOperationRuntime::getNextTimeInterval(RDOSimulator *sim) 
{ 
	((RDORuntime *)sim)->setCurrentActivity(this);
	return ((RDOOperationRuntime*)pattern)->getNextTimeInterval(sim); 
}
void RDOActivityOperationRuntime::convertBegin(RDOSimulator *sim) 
{ 
	((RDORuntime *)sim)->setCurrentActivity(this);
	((RDOOperationRuntime*)pattern)->convertBegin(sim); 
}
void RDOActivityOperationRuntime::convertEnd(RDOSimulator *sim) 
{ 
	((RDORuntime *)sim)->setCurrentActivity(this);
	((RDOOperationRuntime*)pattern)->convertEnd(sim); 
}
bool RDOActivityOperationRuntime::choiceFrom(RDOSimulator *sim) 
{ 
	((RDORuntime *)sim)->setCurrentActivity(this);
	if(haveAdditionalCondition)
		if(!additionalCondition->calcValueBase(dynamic_cast<RDORuntime *>(sim)))
			return false;

	return ((RDOOperationRuntime*)pattern)->choiceFrom(sim); 
}

void RDOActivityKeyboardRuntime::addHotKey(string *hotKey)
{
	keyScanCodes.push_back(rdoHotKeyToolkit.codeFromString(hotKey));
}

bool RDOActivityKeyboardRuntime::choiceFrom(RDOSimulator *sim)
{
	RDORuntime *runtime = (RDORuntime *)sim;
	runtime->setCurrentActivity(this);

	if(!runtime->checkAreaActivated(oprName))
	{
		int size = keyScanCodes.size();
		for(int i = 0; i < size; i++)
			if(!runtime->checkKeyPressed(keyScanCodes.at(i)))
				return false;

		for(i = 0; i < size; i++)
			runtime->eraseKeyPressed(keyScanCodes.at(i));
	}

	return RDOActivityOperationRuntime::choiceFrom(sim); 
}

void RDOActivityRuntime::writeModelStructure(stringstream &stream)
{
	stream << *oprName << " " << pattern->getPatternId() << endl;
}

void RDOActivityRuntime::incrementRelevantResourceReference(RDOSimulator *sim)
{
	RDORuntime *runtime = dynamic_cast<RDORuntime *>(sim);
	for(int i = 0; i < relResNumbers.size(); i++)
	{
		RDOResource *res = runtime->findResource(relResNumbers.at(i));
		res->referenceCount++;
	}
}

void RDOActivityRuntime::decrementRelevantResourceReference(RDOSimulator *sim)
{
	RDORuntime *runtime = dynamic_cast<RDORuntime *>(sim);
	for(int i = 0; i < relResNumbers.size(); i++)
	{
		RDOResource *res = runtime->findResource(relResNumbers.at(i));
		res->referenceCount--;
	}
}


}	// namespace rdoRuntime