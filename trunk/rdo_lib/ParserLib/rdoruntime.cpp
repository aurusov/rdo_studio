#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoruntime.h"
#include "rdopatrtime.h"
#include "rdopat.h"
#include "rdortp.h"
#include "rdofun.h"
#include "RdoFunc.h"
#include "rdopmd.h"
#include "rdodptrtime.h"
#include "rdofrm.h"

namespace rdoRuntime
{

void RDORuntime::setConstValue(int numberOfConst, RDOValue value)
{
	if(allConstants.size() <= numberOfConst)
		allConstants.resize(numberOfConst + 1);

	allConstants.at(numberOfConst) = value;
}

RDOValue RDORuntime::getConstValue(int numberOfConst)
{
	return allConstants.at(numberOfConst);
}

RDOValue RDORuntime::getResParamVal(const int nRes, const int nParam) const
{
   RDOResource *res = findResource(nRes);
   return res->params.at(nParam);
}

void RDORuntime::setResParamVal(const int nRes, const int nParam, RDOValue val)
{
   RDOResource *res = findResource(nRes);
	if(res->params.size() <= nParam)
		res->params.resize(nParam + 1);

   res->params.at(nParam) = val;
}

RDOValue RDORuntime::eraseRes(const int resNumb)
{
	RDOResource *res = allResources.at(resNumb);
	for_each(allPokaz.begin(), allPokaz.end(), bind2nd(mem_fun1(RDOPMDPokaz::checkResourceErased), res));

	delete res;
	allResources.at(resNumb) = NULL;
	return 1;
}

int RDORuntime::getRelResNumber(const int nRelRes) const
{
	return currActivity->getRelResNumber(nRelRes);
}

RDOResource *RDORuntime::findResource(const int num) const
{
	return allResources.at(num);
}

RDOResource *RDORuntime::createNewResource()
{
	vector<RDOResource *>::iterator it = find(allResources.begin(), allResources.end(), (RDOResource *)NULL);
	if(it != allResources.end())
	{
		RDOResource *newRes = new RDOResource(this);
		newRes->number = (it - allResources.begin());
		(*it) = newRes;
		return newRes;
	}

	RDOResource *newRes = new RDOResource(this);
	newRes->number = allResources.size();
	allResources.push_back(newRes);
	return newRes;
}

RDOResource *RDORuntime::createNewResource(int number, bool isPermanent)
{
	allResources.resize(number + 1, NULL);
	if(allResources.at(number) != NULL)
		throw RDORuntimeException("internal error N 0010");

	RDOResource *newRes = new RDOResource(this);
	newRes->number = number;
	allResources.at(number) = newRes;
//	if(isPermanent)
		permanentResources.push_back(newRes);
	if(!isPermanent)
		newRes->makeTemporary(true);
	return newRes;
}

void RDORuntime::addRuntimePokaz(RDOPMDPokaz *pok)
{ 
	allPokaz.push_back(pok); 
	addPokaz(pok); 
}

void RDORuntime::addRuntimeFrame(RDOFRMFrame *frm)
{ 
	allFrames.push_back(frm); 
}

bool RDORuntime::checkKeyPressed(int scanCode)
{
	vector<int>::iterator it = find(config.keysPressed.begin(), config.keysPressed.end(), scanCode);
	if(it == config.keysPressed.end())
		return false;	

	config.keysPressed.erase(it);
	return true;
}

bool RDORuntime::checkAreaActivated(string *oprName)
{
	vector<string>::iterator it = find(config.activeAreasMouseClicked.begin(), config.activeAreasMouseClicked.end(), *oprName);
	if(it == config.activeAreasMouseClicked.end())
		return false;	

	config.activeAreasMouseClicked.erase(it);
	return true;
}

RDOValue RDOCalcCreateResource::calcValue(RDORuntime *sim) const
{
	RDOResource *res = sim->createNewResource();
	res->type = type;
	res->trace = traceFlag;
	res->params.insert(res->params.begin(), paramsCalcs.begin(), paramsCalcs.end());
	return RDOValue(1);		// just to return something
}

RDOCalcCreateResource::RDOCalcCreateResource(int _type, bool _traceFlag, const vector<RDOValue> &_paramsCalcs):
	type(_type), traceFlag(_traceFlag)
{
	paramsCalcs.insert(paramsCalcs.begin(), _paramsCalcs.begin(), _paramsCalcs.end());
}

RDOValue RDOCalcCreateNumberedResource::calcValue(RDORuntime *sim) const
{
	RDOResource *res = sim->createNewResource(number, isPermanent);
	res->type = type;
	res->trace = traceFlag;
	res->params.insert(res->params.begin(), paramsCalcs.begin(), paramsCalcs.end());
	return RDOValue(1);		// just to return something
}

RDOCalcCreateNumberedResource::RDOCalcCreateNumberedResource(int _type, bool _traceFlag, const vector<RDOValue> &_paramsCalcs, int _number, bool _isPermanent):
	type(_type), traceFlag(_traceFlag), number(_number), isPermanent(_isPermanent)
{
	paramsCalcs.insert(paramsCalcs.begin(), _paramsCalcs.begin(), _paramsCalcs.end());
}

RDOValue RDOCalcCreateEmptyResource::calcValue(RDORuntime *sim) const
{
	RDOResource *res = sim->createNewResource();
	sim->selectRelResource(relResNumber, res->number);
	res->type = type;
	res->trace = traceFlag;
	res->params.insert(res->params.begin(), 0);
	return RDOValue(1);		// just to return something
}

RDOCalcCreateEmptyResource::RDOCalcCreateEmptyResource(int _type, bool _traceFlag, int _relResNumber):
	type(_type), traceFlag(_traceFlag), relResNumber(_relResNumber)
{
}

RDORuntime::RDORuntime(): tracer(NULL), result(NULL)
{
	terminateIfCalc = NULL;
}

int a = 0;
bool RDORuntime::endCondition()
{
	if(!terminateIfCalc)
		return false;	// forever

	a++;
	if(a == 30)
		int b = 0;
	return fabs(terminateIfCalc->calcValue(this)) > DBL_EPSILON; 
}

bool RDORuntime::setTerminateIf(RDOCalc *_terminateIfCalc)
{
	if(terminateIfCalc)
		return false;

	terminateIfCalc = _terminateIfCalc;
	return true;
}

RDORuntime::~RDORuntime()
{
	rdoDestroy();
	DeleteAllObjects(allCalcs);
	DeleteAllObjects(allPatterns);
	DeleteAllObjects(rules);
	DeleteAllObjects(ies);
	DeleteAllObjects(operations);
	DeleteAllObjects(allPokaz);
	DeleteAllObjects(allDPTs);
	DeleteAllObjects(allResources);
	DeleteAllObjects(allFrames);
}

string RDOResource::getTypeId()
{
	ostringstream str;
	str << type;
	return str.str();
}

string RDOResource::traceParametersValue()
{
   ostringstream str;
	if(params.size() > 0)
	{
		vector<RDOValue>::iterator end = params.end();
		for(vector<RDOValue>::iterator it = params.begin();;)
		{
			str << (*it);
			if(++it == end)
				break;
			str << " ";
		}
	}
   return str.str();
}

vector<RDOResourceTrace *> RDORuntime::getPermanentResources()
{
	return permanentResources;
}

void RDORuntime::rdoInit(RDOTrace *customTracer, RDOResult *customResult)
{
	tracer = customTracer;
	result = customResult;
	currFuncTop = 0;
	RDOSimulatorTrace::rdoInit();
}

void RDORuntime::onInit()
{
	for_each(initCalcs.begin(), initCalcs.end(), bind2nd(mem_fun1(&RDOCalc::calcValue), this));
	
	int size = rules.size();
	for(int i = 0; i < size; i++)
		addTemplateRule(rules.at(i));

	size = ies.size();
	for(i = 0; i < size; i++)
		addTemplateIrregularEvent(ies.at(i));

	size = operations.size();
	for(i = 0; i < size; i++)
		addTemplateOperation(operations.at(i));

	size = allDPTs.size();
	for(i = 0; i < size; i++)
		addTemplateDecisionPoint(allDPTs.at(i));
}

RDOTrace *RDORuntime::getTracer()
{
	return tracer;
}

void RDORuntime::onDestroy()
{                    
	DeleteAllObjects(allResources);
   if(tracer) 
      delete tracer;
	tracer = NULL;

	if(result)
      delete result;
	result = NULL;
}

RDOValue RDOCalcFuncParam::calcValue(RDORuntime *sim) const
{ 
	return sim->getFuncArgument(numberOfParam); 
}

RDOValue RDORuntime::getFuncArgument(int numberOfParam)
{
	return funcStack.at(currFuncTop + numberOfParam);
}


RDOValue RDOFunCalcExist::calcValue(RDORuntime *sim) const
{
	bool res = false;
	vector<RDOResource *>::iterator end = sim->allResources.end();
	for(vector<RDOResource *>::iterator it = sim->allResources.begin(); 
								it != end && !res; it++)
	{
		if(*it == NULL)
			continue;

		if((*it)->type != nTempResType)
			continue;

		sim->pushGroupFunc(*it);
		if(condition->calcValue(sim))
			res = true;
		sim->popGroupFunc();
	}
	return res;
}

RDOValue RDOFunCalcNotExist::calcValue(RDORuntime *sim) const
{
	bool res = true;
	vector<RDOResource *>::iterator end = sim->allResources.end();
	for(vector<RDOResource *>::iterator it = sim->allResources.begin(); 
							it != end && res; it++)
	{
		if(*it == NULL)
			continue;

		if((*it)->type != nTempResType)
			continue;

		sim->pushGroupFunc(*it);
		if(condition->calcValue(sim))
			res = false;
		sim->popGroupFunc();
	}
	return res;
}

RDOValue RDOFunCalcForAll::calcValue(RDORuntime *sim) const
{
	bool res = true;
	vector<RDOResource *>::iterator end = sim->allResources.end();
	for(vector<RDOResource *>::iterator it = sim->allResources.begin(); 
							it != end && res; it++)
	{
		if(*it == NULL)
			continue;

		if((*it)->type != nTempResType)
			continue;

		sim->pushGroupFunc(*it);
		if(!condition->calcValue(sim))
			res = false;
		sim->popGroupFunc();
	}
	return res;
}

RDOValue RDOFunCalcNotForAll::calcValue(RDORuntime *sim) const
{
	bool res = false;
	vector<RDOResource *>::iterator end = sim->allResources.end();
	for(vector<RDOResource *>::iterator it = sim->allResources.begin(); 
							it != end && !res; it++)
	{
		if(*it == NULL)
			continue;

		if((*it)->type != nTempResType)
			continue;

		sim->pushGroupFunc(*it);
		if(!condition->calcValue(sim))
			res = true;
		sim->popGroupFunc();
	}
	return res;
}

RDOSelectResourceCalc::RDOSelectResourceCalc(int _relNumb, RDOPATFirst *_first, RDOPATChoice *_choice): 
	relNumb(_relNumb)
{
	if(_first == NULL || _first->isEmpty || _first->isFirst)
		isFirst = true;
	else
	{
		isFirst = false;
		isWithMin = _first->isWithMin;
		first = _first->arithm->createCalc(NULL);
	}

	if(_choice == NULL || _choice->isEmpty || _choice->isNoCheck)
		hasChoice = false;
	else
	{
		hasChoice = true;
		choice = _choice->logic->calc;
	}
}

RDOValue RDOSelectResourceDirectCalc::calcValue(RDORuntime *sim) const
{
	sim->selectRelResource(relNumb, resNumb);
	if(hasChoice)
		if(!choice->calcValue(sim))
			return 0;

	return 1;
}

RDOValue RDOSelectResourceByTypeCalc::calcValue(RDORuntime *sim) const
{
	RDOValue maxVal = -DBL_MAX;
	RDOValue minVal = DBL_MAX;
	int nResMin = -1;
	int nResMax = -1;
	vector<RDOResource *>::iterator end = sim->allResources.end();
	for(vector<RDOResource *>::iterator it = sim->allResources.begin(); 
													it != end; it++)
	{
		if(*it == NULL)
			continue;

		if((*it)->type != resType)
			continue;

		sim->selectRelResource(relNumb, (*it)->number);
		if(hasChoice)
			if(!choice->calcValue(sim))
				continue;

		if(isFirst)
			return 1;

		RDOValue tmp = first->calcValue(sim);
		if(tmp > maxVal)
		{
			maxVal = tmp;
			nResMax = (*it)->number;
		}

		if(tmp < minVal)
		{
			minVal = tmp;
			nResMin = (*it)->number;
		}
	}

	if(isWithMin && nResMin != -1)
	{
		sim->selectRelResource(relNumb, nResMin);
		return 1;
	}

	if(!isWithMin && nResMax != -1)
	{
		sim->selectRelResource(relNumb, nResMax);
		return 1;
	}

	return 0;
}

RDOValue RDOSetRelParamCalc::calcValue(RDORuntime *sim) const
{
	sim->setResParamVal(sim->getRelResNumber(relNumb), parNumb, calc->calcValue(sim));
	return 1;
}						 

RDOValue RDOSetResourceParamCalc::calcValue(RDORuntime *sim) const
{
	sim->setResParamVal(resNumb, parNumb, calc->calcValue(sim));
	return 1;
}						 

RDOValue RDOCalcPatParam::calcValue(RDORuntime *sim) const
{ 
	return sim->getPatternParameter(numberOfParam); 
}

void RDORuntime::selectRelResource(int relNumb, int resNumb) 
{ 
	currActivity->selectRelResource(relNumb, resNumb); 
}

RDOValue RDOCalcSeqInit::calcValue(RDORuntime *sim) const	
{
	gen->setSeed(base);
	return RDOValue(0);
}

RDOCalcSeqInit::~RDOCalcSeqInit() 
{ 
	delete gen; 
}

RDOValue RDOCalcSeqNextUniform::calcValue(RDORuntime *sim) const	
{
	return RDOValue(gen->next(sim->getFuncArgument(0), sim->getFuncArgument(1)));
}

RDOValue RDOCalcSeqNextExponential::calcValue(RDORuntime *sim) const	
{
	return RDOValue(gen->next(sim->getFuncArgument(0)));
}

RDOValue RDOCalcSeqNextNormal::calcValue(RDORuntime *sim) const	
{
	return RDOValue(gen->next(sim->getFuncArgument(0), sim->getFuncArgument(1)));
}

RDOValue RDOCalcSeqNextByHist::calcValue(RDORuntime *sim) const	
{
	return RDOValue(gen->next());
}

RDOCalc::RDOCalc() 
{	
	rdoParse::addCalcToRuntime(this); 
}


RDOSimulator *RDORuntime::clone()
{
	RDORuntime *other = new RDORuntime();
	int size = allResources.size();
	for(int i = 0; i < size; i++)
	{
		if(allResources.at(i) == NULL)
			other->allResources.push_back(NULL);
		else
		{
			RDOResource *newRes = new RDOResource(*allResources.at(i));
			other->allResources.push_back(newRes);
			other->permanentResources.push_back(newRes);
		}
	}

	return other;
}

bool RDORuntime::operator == (RDOSimulator &other)
{
	RDORuntime *otherRuntime = dynamic_cast<RDORuntime *>(&other);
	if(otherRuntime->allResources.size() != allResources.size())
		return false;

	int size = allResources.size();
	for(int i = 0; i < size; i++)
	{
		if(allResources.at(i) == NULL && otherRuntime->allResources.at(i) != NULL)
			return false;

		if(allResources.at(i) != NULL && otherRuntime->allResources.at(i) == NULL)
			return false;

		if(allResources.at(i) == NULL && otherRuntime->allResources.at(i) == NULL)
			continue;

		if(*otherRuntime->allResources.at(i) != *allResources.at(i))
			return false;
	}

	return true;
}


bool RDOResource::operator != (RDOResource &other)
{
	if(type != other.type)
		return true;

	if(params.size() != other.params.size())
		return true;

	int size = params.size();
	for(int i = 0; i < size; i++)
	{
		if(params.at(i) != other.params.at(i))
			return true;
	}

	return false;
}

void RDORuntime::rdoDelay(double fromTime, double toTime)
{
	config.currTime = fromTime;
	config.newTime = toTime;
	config.realTimeDelay = (toTime - fromTime)/config.showRate * 3600 * 1000;

//	config.frame = NULL;
	config.frames.clear();

	if(config.showAnimation == RDOSimulatorNS::SM_Animation)
	{
		/*
		RDOFRMFrame *frame = allFrames.at(config.currFrameToShow);
		if(frame->checkCondition(this))
			config.frame = frame->createFrame(this);
		else
		{
			int size = allFrames.size();
			for(int i = 0; i < size; i++)
			{
				frame = allFrames.at(i);
				if(frame->checkCondition(this))
				{
					config.frame = frame->createFrame(this);
					config.currFrameToShow = i;
					break;
				}
			}
		}
		*/

		int size = allFrames.size();
		for(int i = 0; i < size; i++)
		{
			RDOFRMFrame *frame = allFrames.at(i);
			if(frame->checkCondition(this))
				config.frames.push_back(frame->createFrame(this));
			else
				config.frames.push_back(NULL);
		}
	}

	if(frameCallBack)
		(*frameCallBack)(&config, param);
	else
	{								// normally  frameCallBack deletes config.frame
		for(int i = 0; i < config.frames.size(); i++)
			delete config.frames.at(i);
		config.frames.clear();
	}
}

string RDORuntime::writeActivitiesStructure()
{
	stringstream stream;
	int counter = 1;
	int size = rules.size();
	for(int i = 0; i < size; i++)
	{
		stream << counter++ << " ";
		rules.at(i)->writeModelStructure(stream);
	}

	size = operations.size();
	for(i = 0; i < size; i++)
	{
		stream << counter++ << " ";
		operations.at(i)->writeModelStructure(stream);
	}

	stream << endl;

	counter = 1;
	size = ies.size();
	for(i = 0; i < size; i++)
	{
		stream << counter++ << " ";
		ies.at(i)->writeModelStructure(stream);
	}
/*
	counter = 1;
	size = allDPTs.size();
	for(i = 0; i < size; i++)
	{
		allDPTs.at(i)->writeModelStructure(stream);
	}
  */
	return stream.str();
}


}	// namespace rdoRuntime