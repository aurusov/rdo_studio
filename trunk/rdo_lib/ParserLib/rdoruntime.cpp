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
#include "rdoparser.h"

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

RDOValue RDORuntime::eraseRes(const int resNumb, const RDOCalc *fromCalc)
{
	RDOResource *res = allResources.at(resNumb);
	for_each(allPokaz.begin(), allPokaz.end(), bind2nd(mem_fun1(RDOPMDPokaz::checkResourceErased), res));

	if(res->referenceCount > 0)
	{
		error("Try to erase used resource", fromCalc);
	}
	else
	{
		delete res;
		allResources.at(resNumb) = NULL;
	}
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
		throw RDOInternalException("internal error N 0010");

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

//	config.keysPressed.erase(it);
	return true;
}

void RDORuntime::eraseKeyPressed(int scanCode)
{
	config.keysPressed.erase(remove(config.keysPressed.begin(), config.keysPressed.end(), scanCode), config.keysPressed.end());
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
	res->params.insert(res->params.begin(), numParameters, 0);
	return RDOValue(1);		// just to return something
}

RDOCalcCreateEmptyResource::RDOCalcCreateEmptyResource(int _type, bool _traceFlag, int _relResNumber, int _numParameters):
	type(_type), traceFlag(_traceFlag), relResNumber(_relResNumber), numParameters(_numParameters)
{
}

RDORuntime::RDORuntime(): tracer(NULL), result(NULL)
{
	terminateIfCalc = NULL;
}

bool RDORuntime::endCondition()
{
	if(!terminateIfCalc)
		return false;	// forever

	return fabs(terminateIfCalc->calcValueBase(this)) > DBL_EPSILON; 
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

void RDORuntime::addRuntimeOperation(RDOActivityOperationRuntime *oper) 
{ 
	operations.push_back(oper); 
	allBaseOperations.push_back(oper); 
}

void RDORuntime::addRuntimeRule(RDOActivityRuleRuntime *rule) 
{ 
	rules.push_back(rule); 
	allBaseOperations.push_back(rule); 
}

void RDORuntime::addRuntimeIE(RDOActivityIERuntime *ie) 
{ 
	ies.push_back(ie); 
	allBaseOperations.push_back(ie); 
}

void RDORuntime::addDPT(RDOSearchRuntime *dpt) 
{ 
	allDPTs.push_back(dpt); 
	allBaseOperations.push_back(dpt); 
}

void RDORuntime::onInit()
{
	for_each(initCalcs.begin(), initCalcs.end(), bind2nd(mem_fun1(&RDOCalc::calcValueBase), this));

	int size = allBaseOperations.size();
	for(int i = 0; i < size; i++)
		addTemplateBaseOperation(allBaseOperations.at(i));

/*	
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
*/
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
		if(condition->calcValueBase(sim))
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
		if(condition->calcValueBase(sim))
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
		if(!condition->calcValueBase(sim))
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
		if(!condition->calcValueBase(sim))
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
		if(!choice->calcValueBase(sim))
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
			if(!choice->calcValueBase(sim))
				continue;

		if(isFirst)
			return 1;

		RDOValue tmp = first->calcValueBase(sim);
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

vector<int> RDOSelectResourceDirectCommonCalc::getPossibleNumbers(RDORuntime *sim) const
{
	vector<int> res;	
	res.push_back(resNumb);
	return res;
}

vector<int> RDOSelectResourceByTypeCommonCalc::getPossibleNumbers(RDORuntime *sim) const
{
	vector<int> res;	
	vector<RDOResource *>::iterator end = sim->allResources.end();
	for(vector<RDOResource *>::iterator it = sim->allResources.begin(); 
													it != end; it++)
	{
		if(*it == NULL)
			continue;

		if((*it)->type != resType)
			continue;

		res.push_back((*it)->number);
	}

	return res;
}

RDOValue RDOSelectResourceCommonCalc::calcValue(RDORuntime *sim) const
{
	vector<vector<int> > allNumbs;
	vector<int> res;
	for(int i = 0; i < resSelectors.size(); i++)
	{
		allNumbs.push_back(resSelectors.at(i)->getPossibleNumbers(sim));
		res.push_back(sim->getRelResNumber(i));
	}

	RDOValue bestVal = 0;
	bool hasBest = false;
	getBest(allNumbs, 0, res, bestVal, sim, hasBest);

	if(!hasBest)
		return false;

	for(i = 0; i < res.size(); i++)
		sim->selectRelResource(i, res.at(i));

	return true;
}

void RDOSelectResourceCommonCalc::getBest(vector<vector<int> > &allNumbs, int level, vector<int> &res, RDOValue &bestVal, RDORuntime *sim, bool &hasBest) const
{
	if(level >= allNumbs.size())
	{
		for(int i = 0; i < resSelectors.size(); i++)
		{
			if(!resSelectors.at(i)->callChoice(sim))
				return;	// state not valid
		}

		RDOValue newVal = choice->calcValueBase(sim);
		if(!hasBest || (useCommonWithMax && (newVal > bestVal)) ||
			(!useCommonWithMax && (newVal < bestVal)))	// found better value
		{
			for(int i = 0; i < resSelectors.size(); i++)
				res.at(i) = sim->getRelResNumber(i);
			bestVal = newVal;
			hasBest = true;
		}

		return;
	}

	vector<int> &ourLevel = allNumbs.at(level);
	for(int i = 0; i < ourLevel.size(); i++)
	{
		sim->selectRelResource(level, ourLevel.at(i));
		getBest(allNumbs, level+1, res, bestVal, sim, hasBest);
	}
}

bool RDOSelectResourceDirectCommonCalc::callChoice(RDORuntime *sim) const
{
	if(hasChoice)
		if(!choice->calcValueBase(sim))
			return 0;

	return 1;
}

bool RDOSelectResourceByTypeCommonCalc::callChoice(RDORuntime *sim) const
{
	if(hasChoice)
		if(!choice->calcValueBase(sim))
			return 0;

	return 1;
}


RDOValue RDOSetRelParamCalc::calcValue(RDORuntime *sim) const
{
	sim->setResParamVal(sim->getRelResNumber(relNumb), parNumb, calc->calcValueBase(sim));
	return 1;
}						 

RDOValue RDOSetResourceParamCalc::calcValue(RDORuntime *sim) const
{
	sim->setResParamVal(resNumb, parNumb, calc->calcValueBase(sim));
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
	fileToParse = currParser->fileToParse;
	lineno = rdoLineno();
	rdoParse::addCalcToRuntime(this); 
}

RDOValue RDOCalc::calcValueBase(RDORuntime *sim) const
{
	try
	{
		return calcValue(sim);
	}
	catch(RDOException &)
	{
		sim->error("in", this);
	}

	return 0; // unreachable code
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

	other->allConstants = allConstants;

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

string RDORuntime::writePokazStructure()
{
	stringstream stream;
	for(int i = 0; i < allPokaz.size(); i++)
	{
		RDOPMDPokaz *curr = allPokaz.at(i);
		curr->writePokazStructure(stream);
	}

	return stream.str();
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

RDOValue RDOCalcDiv::calcValue(RDORuntime *sim) const 
{ 
   RDOValue rVal = right->calcValueBase(sim);
   if(rVal == 0)
		sim->error("Division by zero", this);
//			throw RDORuntimeException("Division by zero");

   return RDOValue(left->calcValueBase(sim) / rVal);
}

RDOValue RDOCalcCheckDiap::calcValue(RDORuntime *sim) const 
{ 
   RDOValue val = oper->calcValueBase(sim);
   if(val < minVal || val > maxVal)
		sim->error(("Parameter out of range: " + toString(val)).c_str(), this);
//			throw RDORuntimeException("parameter out of range");

   return val; 
}


void RDORuntime::error( const char *mes, const RDOCalc *calc )
{
	rdoModelObjects::RDOFileType ft = rdoModelObjects::PAT;
	switch(calc->fileToParse)
	{
	case RTP_FILE: ft = rdoModelObjects::RTP; break;
	case RSS_FILE: ft = rdoModelObjects::RSS; break;
	case FUN_FILE: ft = rdoModelObjects::FUN; break;
	case PAT_FILE: ft = rdoModelObjects::PAT; break;
	case OPR_FILE: ft = rdoModelObjects::OPR; break;
	case DPT_FILE: ft = rdoModelObjects::DPT; break;
	case PMD_FILE: ft = rdoModelObjects::PMD; break;
	case SMR1_FILE:
	case SMR2_FILE: ft = rdoModelObjects::SMR; break;
	case FRM_FILE: ft = rdoModelObjects::FRM; break;
	}
	errors.push_back(RDOSimulatorNS::RDOSyntaxError(mes, calc->lineno, ft));
	throw rdoParse::RDOSyntaxException("");
}

void RDORuntime::onPutToTreeNode()
{
	// when create TreeNode with new RDOSimulator,
	// make all resources permanent, to avoid trace their
	// erase when delete TreeNode
	for(int i = 0; i < allResources.size(); i++)
	{
		if(allResources.at(i))
			allResources.at(i)->makeTemporary(false);
	}

}


}	// namespace rdoRuntime