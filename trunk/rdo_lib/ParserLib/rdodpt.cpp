#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdodpt.h"
#include "rdoparser.h"
#include "rdopatrtime.h"

namespace rdoParse 
{

/////////////////////////  "SEARCH" DECISION POINT /////////////////////////

RDODPTSearch::RDODPTSearch(string *_name, DPTSearchTrace _trace)
	: name(_name), trace(_trace) 
{
	if(find_if(currParser->allDPTSome.begin(), currParser->allDPTSome.end(), compareName<RDODPTSome>(_name)) != currParser->allDPTSome.end())
		currParser->error("DPT name: " + *_name + " already defined");

	if(find_if(currParser->allDPTSearch.begin(), currParser->allDPTSearch.end(), compareName<RDODPTSearch>(_name)) != currParser->allDPTSearch.end())
		currParser->error("DPT name: " + *_name + " already defined");

	currParser->allDPTSearch.push_back(this);
}

void RDODPTSearch::addNewActivity(string *_name, string *_ruleName)
{
	if(find_if(activities.begin(), activities.end(), compareName<RDODPTSearchActivity>(_name)) != activities.end())
		currParser->error("Activity name: " + *_name + " already defined");

	lastActivity = new RDODPTSearchActivity(_name, _ruleName);
	activities.push_back(lastActivity);
}

void RDODPTSearch::setActivityValue(DPTSearchValue _value, RDOFUNArithm *_ruleCost)
{
	lastActivity->setValue(_value, _ruleCost);
}

void RDODPTSearch::end()
{
	RDOCalc *condCalc;
	if(!conditon)
		condCalc = new RDOCalcConst(1);
	else
		condCalc = conditon->calc;

	RDOSearchRuntime *dpt = new RDOSearchRuntime(currParser->runTime, 
		condCalc, 
		termConditon->calc, 
		evalBy->createCalc(), 
		compTops);

	switch(trace)
	{
	case DPTnotrace:
		dpt->traceFlag = DPT_no_trace;
		break;
	case DPTtracestat:
		dpt->traceFlag = DPT_trace_stat;
		break;
	case DPTtracetops:
		dpt->traceFlag = DPT_trace_tops;
		break;
	case DPTtraceall:
		dpt->traceFlag = DPT_trace_all;
		break;
	};

	currParser->runTime->addDPT(dpt);

	int size = activities.size();
	for(int i = 0; i < size; i++)
	{
		RDODPTSearchActivity *curr = activities.at(i);
		RDOSearchActivityRuntime *act = curr->createActivityRuntime(currParser->runTime);
		dpt->addActivity(act);
	}
}

RDODPTSearchActivity::RDODPTSearchActivity(string *_name, string *_ruleName)
	: name(_name) 
{
	const RDOPATPattern *pat = currParser->findPattern(_ruleName);
	pat->testGoodForSearchActivity();
	rule = (RDOPATPatternRule *)pat;
	currParamNum = 0;
}

void RDODPTSearchActivity::setValue(DPTSearchValue _value, RDOFUNArithm *_ruleCost)
{
	if(currParamNum != rule->params.size())
		currParser->error("Too few parameters for rule: " + *rule->getName());

	value = _value;
	ruleCost = _ruleCost;
}

void RDODPTSearchActivity::addParam(int _param) 
{
	if(currParamNum >= rule->params.size())
		currParser->error("Too many parameters for rule: " + *rule->getName());

	RDOFUNFunctionParam *param = rule->params[currParamNum++];
	RDOValue val = param->getType()->getRSSIntValue(_param);
	params.push_back(val);
}

void RDODPTSearchActivity::addParam(double *_param) 
{
	if(currParamNum >= rule->params.size())
		currParser->error("Too many parameters for rule: " + *rule->getName());

	RDOFUNFunctionParam *param = rule->params[currParamNum++];
	RDOValue val = param->getType()->getRSSRealValue(_param);
	params.push_back(val);
}

void RDODPTSearchActivity::addParam(string *_param) 
{
	if(currParamNum >= rule->params.size())
		currParser->error("Too many parameters for rule: " + *rule->getName());

	RDOFUNFunctionParam *param = rule->params[currParamNum++];
	RDOValue val = param->getType()->getRSSEnumValue(_param);
	params.push_back(val);
}

void RDODPTSearchActivity::addParam() 
{
	if(currParamNum >= rule->params.size())
		currParser->error("Too many parameters for rule: " + *rule->getName());

	RDOFUNFunctionParam *param = rule->params[currParamNum++];
	RDOValue val = param->getType()->getRSSDefaultValue();
	params.push_back(val);
}

RDOSearchActivityRuntime *RDODPTSearchActivity::createActivityRuntime(RDORuntime *sim)
{
	RDOActivityRuleRuntime *activity = new RDOActivityRuleRuntime(sim, rule->patRuntime, true, name);
	RDOSearchActivityRuntime *act = new RDOSearchActivityRuntime(sim, 
		dynamic_cast<RDORule *>(activity),
		(value == DPT_value_after), 
		ruleCost->createCalc());

	int size = params.size();
	for(int i = 0; i < size; i++)
		activity->addParamCalc(new RDOSetPatternParamCalc(i, params.at(i)));

	return act;
}

/////////////////////////  "SOME" DECISION POINT //////////////////////

RDODPTSome::RDODPTSome(string *_name)
	: name(_name)
{
	if(find_if(currParser->allDPTSome.begin(), currParser->allDPTSome.end(), compareName<RDODPTSome>(_name)) != currParser->allDPTSome.end())
		currParser->error("DPT name: " + *_name + " already defined");

	if(find_if(currParser->allDPTSearch.begin(), currParser->allDPTSearch.end(), compareName<RDODPTSearch>(_name)) != currParser->allDPTSearch.end())
		currParser->error("DPT name: " + *_name + " already defined");

	currParser->allDPTSome.push_back(this);
}

void RDODPTSome::addNewActivity(string *_name, string *_patternName)
{
	if(find_if(activities.begin(), activities.end(), compareName<RDODPTSomeActivity>(_name)) != activities.end())
		currParser->error("Activity name: " + *_name + " already defined");

	lastActivity = new RDODPTSomeActivity(_name, _patternName);
	activities.push_back(lastActivity);
}

void RDODPTSome::end()
{
	int size = activities.size();
	for(int i = 0; i < size; i++)
		activities.at(i)->createActivityRuntime(conditon);
}

RDODPTSomeActivity::RDODPTSomeActivity(string *_name, string *_patternName)
	: name(_name) 
{
	pattern = currParser->findPattern(_patternName);
	pattern->testGoodForSomeActivity();
	currParamNum = 0;
}

void RDODPTSomeActivity::addParam(int _param) 
{
	if(currParamNum >= pattern->params.size())
		currParser->error("Too many parameters for pattern: " + *pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	RDOValue val = param->getType()->getRSSIntValue(_param);
	params.push_back(val);
}

void RDODPTSomeActivity::addParam(double *_param) 
{
	if(currParamNum >= pattern->params.size())
		currParser->error("Too many parameters for pattern: " + *pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	RDOValue val = param->getType()->getRSSRealValue(_param);
	params.push_back(val);
}

void RDODPTSomeActivity::addParam(string *_param) 
{
	if(currParamNum >= pattern->params.size())
		currParser->error("Too many parameters for pattern: " + *pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	RDOValue val = param->getType()->getRSSEnumValue(_param);
	params.push_back(val);
}

void RDODPTSomeActivity::addParam() 
{
	if(currParamNum >= pattern->params.size())
		currParser->error("Too many parameters for pattern: " + *pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	RDOValue val = param->getType()->getRSSDefaultValue();
	params.push_back(val);
}

void RDODPTSomeActivity::createActivityRuntime(RDOFUNLogic *conditon)
{
	RDOActivityRuntime *activity;
	if(!conditon)
		activity = pattern->patRuntime->createActivity(name);
	else
		activity = pattern->patRuntime->createActivity(conditon->calc, name);

	int size = params.size();
	for(int i = 0; i < size; i++)
		activity->addParamCalc(new RDOSetPatternParamCalc(i, params.at(i)));
}


/////////////////////////  FREE ACTIVITIES /////////////////////////

RDODPTFreeActivity::RDODPTFreeActivity(string *_name, string *_patternName)
	: name(_name) 
{
	if(find_if(currParser->allFreeActivity.begin(), currParser->allFreeActivity.end(), compareName<RDODPTFreeActivity>(_name)) != currParser->allFreeActivity.end())
		currParser->error("Free activity name: " + *_name + " already defined");

	currParser->allFreeActivity.push_back(this);

	pattern = currParser->findPattern(_patternName);
	pattern->testGoodForFreeActivity();
	currParamNum = 0;
}

void RDODPTFreeActivity::addParam(int _param) 
{
	if(currParamNum >= pattern->params.size())
		currParser->error("Too many parameters for pattern: " + *pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	RDOValue val = param->getType()->getRSSIntValue(_param);
	params.push_back(val);
}

void RDODPTFreeActivity::addParam(double *_param) 
{
	if(currParamNum >= pattern->params.size())
		currParser->error("Too many parameters for pattern: " + *pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	RDOValue val = param->getType()->getRSSRealValue(_param);
	params.push_back(val);
}

void RDODPTFreeActivity::addParam(string *_param) 
{
	if(currParamNum >= pattern->params.size())
		currParser->error("Too many parameters for pattern: " + *pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	RDOValue val = param->getType()->getRSSEnumValue(_param);
	params.push_back(val);
}

void RDODPTFreeActivity::addParam() 
{
	if(currParamNum >= pattern->params.size())
		currParser->error("Too many parameters for pattern: " + *pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	RDOValue val = param->getType()->getRSSDefaultValue();
	params.push_back(val);
}

void RDODPTFreeActivity::end()
{
	RDOActivityRuntime *activity = pattern->patRuntime->createActivity(name);
	int size = params.size();
	for(int i = 0; i < size; i++)
		activity->addParamCalc(new RDOSetPatternParamCalc(i, params.at(i)));

	size = hotKeys.size();
	for(i = 0; i < size; i++)
		activity->addHotKey(hotKeys.at(i));
}

}		// namespace rdoParse 
