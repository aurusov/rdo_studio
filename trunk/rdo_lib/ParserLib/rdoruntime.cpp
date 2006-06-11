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
	std::for_each(allPokaz.begin(), allPokaz.end(), std::bind2nd(std::mem_fun1(RDOPMDPokaz::checkResourceErased), res));

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
	std::vector<RDOResource *>::iterator it = std::find(allResources.begin(), allResources.end(), (RDOResource *)NULL);
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
	std::vector<int>::iterator it = std::find(config.keysPressed.begin(), config.keysPressed.end(), scanCode);
	if(it == config.keysPressed.end())
		return false;	

//	config.keysPressed.erase(it);
	return true;
}

void RDORuntime::eraseKeyPressed(int scanCode)
{
	config.keysPressed.erase(std::remove(config.keysPressed.begin(), config.keysPressed.end(), scanCode), config.keysPressed.end());
}

bool RDORuntime::checkAreaActivated(std::string *oprName)
{
	std::vector<std::string>::iterator it = std::find(config.activeAreasMouseClicked.begin(), config.activeAreasMouseClicked.end(), *oprName);
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

RDOCalcCreateResource::RDOCalcCreateResource(int _type, bool _traceFlag, const std::vector<RDOValue> &_paramsCalcs):
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

RDOCalcCreateNumberedResource::RDOCalcCreateNumberedResource(int _type, bool _traceFlag, const std::vector<RDOValue> &_paramsCalcs, int _number, bool _isPermanent):
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

std::string RDOResource::getTypeId()
{
	std::ostringstream str;
	str << type;
	return str.str();
}

std::string RDOResource::traceParametersValue()
{
	std::ostringstream str;
	if(params.size() > 0)
	{
		std::vector<RDOValue>::iterator end = params.end();
		for(std::vector<RDOValue>::iterator it = params.begin();;)
		{
			str << (*it);
			if(++it == end)
				break;
			str << " ";
		}
	}
	return str.str();
}

std::vector<RDOResourceTrace *> RDORuntime::getPermanentResources()
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
	std::for_each(initCalcs.begin(), initCalcs.end(), std::bind2nd(std::mem_fun1(&RDOCalc::calcValueBase), this));

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
	std::vector<RDOResource *>::iterator end = sim->allResources.end();
	for(std::vector<RDOResource *>::iterator it = sim->allResources.begin(); 
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
	std::vector<RDOResource *>::iterator end = sim->allResources.end();
	for(std::vector<RDOResource *>::iterator it = sim->allResources.begin(); 
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
	std::vector<RDOResource *>::iterator end = sim->allResources.end();
	for(std::vector<RDOResource *>::iterator it = sim->allResources.begin(); 
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
	std::vector<RDOResource *>::iterator end = sim->allResources.end();
	for(std::vector<RDOResource *>::iterator it = sim->allResources.begin(); 
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

RDOSelectResourceCalc::RDOSelectResourceCalc( int _relNumb, RDOPATChoice* _choice, RDOPATSelectType* _selection_type ):
	relNumb( _relNumb ),
	choice( NULL ),
	selection_calc( NULL ),
	selection_type( RDOPATSelectType::st_empty )
{
	if ( _choice != NULL && _choice->type == RDOPATChoice::ch_from ) {
		choice = _choice->logic->calc;
	}
	if ( _selection_type ) {
		selection_type = _selection_type->type;
		if ( _selection_type->arithm ) {
			selection_calc = _selection_type->arithm->createCalc( NULL );
		}
	}
}

RDOValue RDOSelectResourceDirectCalc::calcValue( RDORuntime* sim ) const
{
	sim->selectRelResource( relNumb, resNumb );
	if ( choice && !choice->calcValueBase( sim ) ) return 0;
	return 1;
}

RDOValue RDOSelectResourceByTypeCalc::calcValue( RDORuntime* sim ) const
{
	RDOValue maxVal = -DBL_MAX;
	RDOValue minVal = DBL_MAX;
	int nResMinMax  = -1;
	std::vector< RDOResource* >::iterator end = sim->allResources.end();
	for ( std::vector< RDOResource* >::iterator it = sim->allResources.begin(); it != end; it++ ) {

		if ( *it && (*it)->type == resType ) {

			int res_number = (*it)->number;

			switch ( selection_type ) {
				case RDOPATSelectType::st_empty: {
//					sim->selectRelResource( relNumb, 0 );
//					if ( std::find( sim->allResourcesEmptyChoiced.begin(), sim->allResourcesEmptyChoiced.end(), res_number ) != sim->allResourcesEmptyChoiced.end() ) {
//						continue;
//					}
//					sim->selectRelResource( relNumb, res_number );
//					sim->allResourcesEmptyChoiced.push_back( res_number );
					return 1;
				}
				case RDOPATSelectType::st_first: {
					if ( std::find( sim->allResourcesChoiced.begin(), sim->allResourcesChoiced.end(), res_number ) != sim->allResourcesChoiced.end() ) {
						continue;
					}
					sim->selectRelResource( relNumb, res_number );
					if ( choice && !choice->calcValueBase( sim ) ) {
						continue;
					}
					sim->allResourcesChoiced.push_back( res_number );
					return 1;
				}
				case RDOPATSelectType::st_with_min: {
					if ( std::find( sim->allResourcesChoiced.begin(), sim->allResourcesChoiced.end(), res_number ) != sim->allResourcesChoiced.end() ) {
						continue;
					}
					sim->selectRelResource( relNumb, res_number );
					if ( choice && !choice->calcValueBase( sim ) ) {
						continue;
					}
					RDOValue tmp = selection_calc->calcValueBase( sim );
					if ( tmp < minVal ) {
						minVal     = tmp;
						nResMinMax = res_number;
					}
					break;
				}
				case RDOPATSelectType::st_with_max: {
					if ( std::find( sim->allResourcesChoiced.begin(), sim->allResourcesChoiced.end(), res_number ) != sim->allResourcesChoiced.end() ) {
						continue;
					}
					sim->selectRelResource( relNumb, res_number );
					if ( choice && !choice->calcValueBase( sim ) ) {
						continue;
					}
					RDOValue tmp = selection_calc->calcValueBase( sim );
					if ( tmp > maxVal ) {
						maxVal     = tmp;
						nResMinMax = res_number;
					}
					break;
				}
			}
		}
	}

	if ( nResMinMax != -1 ) {
		sim->selectRelResource( relNumb, nResMinMax );
		sim->allResourcesChoiced.push_back( nResMinMax );
		return 1;
	}

	return 0;
}

std::vector<int> RDOSelectResourceDirectCommonCalc::getPossibleNumbers(RDORuntime *sim) const
{
	std::vector<int> res;	
	res.push_back(resNumb);
	return res;
}

std::vector<int> RDOSelectResourceByTypeCommonCalc::getPossibleNumbers(RDORuntime *sim) const
{
	std::vector<int> res;	
	std::vector<RDOResource *>::iterator end = sim->allResources.end();
	for(std::vector<RDOResource *>::iterator it = sim->allResources.begin(); 
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
	std::vector<std::vector<int> > allNumbs;
	std::vector<int> res;
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

void RDOSelectResourceCommonCalc::getBest(std::vector<std::vector<int> > &allNumbs, int level, std::vector<int> &res, RDOValue &bestVal, RDORuntime *sim, bool &hasBest) const
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

	std::vector<int> &ourLevel = allNumbs.at(level);
	for(int i = 0; i < ourLevel.size(); i++)
	{
		sim->selectRelResource(level, ourLevel.at(i));
		getBest(allNumbs, level+1, res, bestVal, sim, hasBest);
	}
}

bool RDOSelectResourceDirectCommonCalc::callChoice(RDORuntime *sim) const
{
	if ( choice && !choice->calcValueBase(sim) ) return 0;
	return 1;
}

bool RDOSelectResourceByTypeCommonCalc::callChoice(RDORuntime *sim) const
{
	if ( choice && !choice->calcValueBase(sim) ) return 0;
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
	fileToParse = parser->getFileToParse();
	lineno      = parser->lexer_loc_line();
	rdoParse::addCalcToRuntime(this); 
}

RDOValue RDOCalc::calcValueBase( RDORuntime* sim ) const
{
	try {
		return calcValue( sim );
	} catch( RDOException& ) {
		sim->error( "in", this );
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

	if(config.showAnimation == rdosim::SM_Animation)
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

std::string RDORuntime::writePokazStructure()
{
	std::stringstream stream;
	for(int i = 0; i < allPokaz.size(); i++)
	{
		RDOPMDPokaz *curr = allPokaz.at(i);
		if(curr->trace)
			curr->writePokazStructure(stream);
	}

	return stream.str();
}

std::string RDORuntime::writeActivitiesStructure()
{
	std::stringstream stream;
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

	stream << std::endl;

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


void RDORuntime::error( const char* message, const RDOCalc* calc )
{
	errors.push_back( rdosim::RDOSyntaxError( rdosim::RDOSyntaxError::UNKNOWN, message, calc->lineno, -1, calc->fileToParse ) );
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

void RDORuntime::postProcess()
{
	getTracer()->startWriting();
   RDOSimulatorTrace::postProcess();
	switch(whyStop)
	{
	case rdoModel::EC_NoMoreEvents:
		getTracer()->writeStatus(this, "NO_MORE_EVENTS");
		break;
	case rdoModel::EC_OK:
		getTracer()->writeStatus(this, "NORMAL_TERMINATION");
		break;
	}
	getTracer()->stopWriting();
}



}	// namespace rdoRuntime