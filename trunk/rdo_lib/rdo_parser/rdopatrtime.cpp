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
	throw rdoParse::RDOInternalException("parser internal error 0005");
}

RDOActivityRuntime *RDOPatternRuntime::createActivity(RDOCalc *condition, std::string *_oprName)
{
	throw rdoParse::RDOInternalException("parser internal error 0501");
}

RDOPatternRuntime::RDOPatternRuntime( RDORuntime* _runtime, bool _trace ):
	timeField( NULL ),
	runtime( _runtime ),
	trace( _trace )
{
	runtime->addPattern(this); 
}

RDORuleRuntime::RDORuleRuntime(RDORuntime *rTime, bool _trace)
	:RDOPatternRuntime(rTime, _trace) {}

RDOIERuntime::RDOIERuntime(RDORuntime *rTime, bool _trace)
	:RDOPatternRuntime(rTime, _trace) {}

RDOOperationRuntime::RDOOperationRuntime(RDORuntime *rTime, bool _trace)
	:RDOPatternRuntime(rTime, _trace) {}

std::vector<RDOResourceTrace *> RDOActivityRuntime::getRelevantResources(RDOSimulator *sim)
{
	RDORuntime *runtime = (RDORuntime *)sim;

	std::vector<RDOResourceTrace *> result;
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

double RDOIERuntime::getNextTimeInterval( RDOSimulator* sim )
{
	RDORuntime* runtime = static_cast<RDORuntime*>(sim);
	double time_next = timeField->calcValueBase(runtime);
	if ( time_next >= 0 ) return time_next;
	runtime->error( rdo::format("ѕопытка запланировать событие в прошлом. ¬ыражение времени дл€ $Time имеет отрицательное значение: %f", time_next).c_str(), timeField );
	return 0;
}

void RDOIERuntime::convertEvent(RDOSimulator *sim)
{
	RDORuntime *runtime = (RDORuntime *)sim;
	choiceFrom(sim);	// to set permanent resource relevent numbers
	int size = beginCalcs.size();
	for(int i = 0; i < size; i++)
		beginCalcs.at(i)->calcValueBase(runtime);
}

bool RDOPatternRuntime::choiceFrom( RDOSimulator* sim )
{
	RDORuntime* runtime = (RDORuntime*)sim;
//	runtime->allResourcesChoiced.clear();
	int size = choiceFromCalcs.size();
	for ( int i = 0; i < size; i++ ) {
		if ( !choiceFromCalcs[i]->calcValueBase(runtime) ) return false;
	}
	return true;
}

bool RDOOperationRuntime::choiceFrom(RDOSimulator *sim)
{
	return RDOPatternRuntime::choiceFrom(sim);
}

bool RDOIERuntime::choiceFrom(RDOSimulator *sim)
{
	return RDOPatternRuntime::choiceFrom(sim);
}

double RDOOperationRuntime::getNextTimeInterval( RDOSimulator* sim )
{
	RDORuntime* runtime = static_cast<RDORuntime*>(sim);
	double time_next = timeField->calcValueBase(runtime);
	if ( time_next >= 0 ) return time_next;
	runtime->error( rdo::format("ѕопытка запланировать окончание операции в прошлом. ¬ыражение времени дл€ $Time имеет отрицательное значение: %f", time_next).c_str(), timeField );
	return 0;
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

RDOActivityRuntime *RDORuleRuntime::createActivity(std::string *_oprName)
{
	RDOActivityRuleRuntime *rule = new RDOActivityRuleRuntime(runtime, this, trace, _oprName);
	rdoParse::parser->runTime->addRuntimeRule(rule); 
	return rule;
}

RDOActivityRuntime *RDORuleRuntime::createActivity(RDOCalc *condition, std::string *_oprName)
{
	RDOActivityRuleRuntime *rule = new RDOActivityRuleRuntime(runtime, this, trace, condition, _oprName);
	rdoParse::parser->runTime->addRuntimeRule(rule); 
	return rule;
}

RDOActivityRuntime *RDOIERuntime::createActivity(std::string *_oprName)
{
	RDOActivityIERuntime *ie = new RDOActivityIERuntime(runtime, this, trace, _oprName);
	rdoParse::parser->runTime->addRuntimeIE(ie); 
	return ie;
}

RDOActivityRuntime *RDOOperationRuntime::createActivity(std::string *_oprName)
{
	RDOActivityOperationRuntime *oper = new RDOActivityOperationRuntime(runtime, this, trace, _oprName);
	rdoParse::parser->runTime->addRuntimeOperation(oper); 
	return oper;
}

RDOActivityRuntime *RDOOperationRuntime::createActivity(RDOCalc *condition, std::string *_oprName)
{
	RDOActivityOperationRuntime *oper = new RDOActivityOperationRuntime(runtime, this, trace, condition, _oprName);
	rdoParse::parser->runTime->addRuntimeOperation(oper); 
	return oper;
}

RDOActivityRuntime *RDOKeyboardRuntime::createActivity(std::string *_oprName)
{
	RDOActivityKeyboardRuntime *oper = new RDOActivityKeyboardRuntime(runtime, this, trace, _oprName);
	rdoParse::parser->runTime->addRuntimeOperation(oper); 
	return oper;
}

RDOActivityRuntime *RDOKeyboardRuntime::createActivity(RDOCalc *condition, std::string *_oprName)
{
	RDOActivityKeyboardRuntime *oper = new RDOActivityKeyboardRuntime(runtime, this, trace, condition, _oprName);
	rdoParse::parser->runTime->addRuntimeOperation(oper); 
	return oper;
}

void RDOActivityRuntime::setPatternParameters(RDOSimulator *sim) 
{
	RDORuntime *runtime = (RDORuntime *)sim;
	int size = setParamsCalcs.size();
	for(int i = 0; i < size; i++)
		setParamsCalcs.at(i)->calcValueBase(runtime);
}

void RDOActivityRuntime::addHotKey( std::string* hotKey )
{
	rdoParse::parser->error( "This pattern is not of keyboard type" );
}

RDOActivityRuleRuntime::RDOActivityRuleRuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, std::string *_oprName)
	: RDORuleTrace(rTime), RDOActivityRuntime(_pattern, _oprName) { trace = _trace; haveAdditionalCondition = false;}

RDOActivityRuleRuntime::RDOActivityRuleRuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, RDOCalc *condition, std::string *_oprName)
	: RDORuleTrace(rTime), RDOActivityRuntime(_pattern, _oprName), additionalCondition(condition) { trace = _trace; haveAdditionalCondition = true;}

RDOActivityIERuntime::RDOActivityIERuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, std::string *_oprName)
	: RDOIETrace(rTime), RDOActivityRuntime(_pattern, _oprName) { trace = _trace; }

RDOActivityOperationRuntime::RDOActivityOperationRuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, std::string *_oprName)
	: RDOOperationTrace(rTime), RDOActivityRuntime(_pattern, _oprName) { trace = _trace; haveAdditionalCondition = false;}

RDOActivityOperationRuntime::RDOActivityOperationRuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, RDOCalc *condition, std::string *_oprName)
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

void RDOActivityKeyboardRuntime::addHotKey( std::string* hotKey )
{
	unsigned int _scan_code = rdoParse::rdoHotKeyToolkit.codeFromString( hotKey );
	switch ( _scan_code ) {
		case VK_SHIFT  : shift     = true;       break;
		case VK_CONTROL: control   = true;       break;
		default        : scan_code = _scan_code; break;
	}
}

bool RDOActivityKeyboardRuntime::choiceFrom( RDOSimulator *sim )
{
	RDORuntime* runtime = static_cast<RDORuntime*>(sim);
	runtime->setCurrentActivity( this );

	if ( !runtime->checkAreaActivated( oprName ) ) {
		if ( !runtime->checkKeyPressed( scan_code, shift, control ) ) return false;
	}

	return RDOActivityOperationRuntime::choiceFrom( sim ); 
}

void RDOActivityRuntime::writeModelStructure(std::stringstream &stream)
{
	stream << *oprName << " " << pattern->getPatternId() << std::endl;
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