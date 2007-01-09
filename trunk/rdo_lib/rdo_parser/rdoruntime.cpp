#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoruntime.h"
#include "rdopat.h"
#include "rdortp.h"
#include "rdofun.h"
#include "rdofunc.h"
#include "rdopmd.h"
#include "rdodptrtime.h"
#include "rdofrm.h"
#include "rdoparser.h"
#include <rdoprocess.h>

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOResource
// ----------------------------------------------------------------------------
RDOResource::RDOResource( RDORuntime* rt ):
	RDOResourceTrace( rt ),
	number( 0 ),
	type( 0 ),
	referenceCount( 0 )
{
}

RDOResource::~RDOResource()
{
}

// ----------------------------------------------------------------------------
// ---------- RDORuntime
// ----------------------------------------------------------------------------
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

void RDORuntime::onEraseRes( const int res_id, const RDOCalcEraseRes* calc )
{
	RDOResource* res = allResourcesByID.at( res_id );
	if ( !res ) {
		error( rdo::format("Временный ресурс уже удален. Возможно, он удален ранее в этом же образце. Имя релевантного ему ресурса: %s", calc ? calc->getName().c_str() : "неизвестное имя").c_str(), calc );
	}
	if ( res->referenceCount > 0 ) {
		error( "Невозможно удалить ресурс, т.к. он еще используется", calc );
//		error( "Try to erase used resource", fromCalc );
	} else {
		std::for_each( allPokaz.begin(), allPokaz.end(), std::bind2nd(std::mem_fun1(rdoParse::RDOPMDPokaz::checkResourceErased), res) );
		allResourcesByID.at( res_id ) = NULL;
		// Диструктор ресурса вызывается в std::list::erase, который вызывается из std::list::remove
		allResourcesByTime.remove( res );
		delete res;
	}
}

RDOResource* RDORuntime::createNewResource()
{
	RDOResource* res = new RDOResource( this );

	std::vector< RDOResource* >::iterator it = std::find( allResourcesByID.begin(), allResourcesByID.end(), static_cast<RDOResource*>(NULL) );
	// Найшли дырку в последовательности ресурсов
	if ( it != allResourcesByID.end() ) {
		res->number = (it - allResourcesByID.begin());
		(*it) = res;
	} else {
		res->number = allResourcesByID.size();
		allResourcesByID.push_back( res );
	}
	allResourcesByTime.push_back( res );
	return res;
}

RDOResource* RDORuntime::createNewResource( int number, bool isPermanent )
{
	allResourcesByID.resize( number + 1, NULL );
	if ( allResourcesByID.at(number) != NULL ) {
		throw rdoParse::RDOInternalException("internal error N 0010");
	}
	RDOResource* res = new RDOResource( this );
	res->number = number;
	allResourcesByID.at(number) = res;
	allResourcesByTime.push_back( res );
//	if ( isPermanent )
		permanentResources.push_back( res );
	if( !isPermanent ) {
		res->makeTemporary( true );
	}
	return res;
}

void RDORuntime::insertNewResource( RDOResource* res )
{
	res->number = allResourcesByID.size();
	allResourcesByID.push_back( res );
	allResourcesByTime.push_back( res );
}

void RDORuntime::addRuntimePokaz( rdoParse::RDOPMDPokaz* pok )
{ 
	allPokaz.push_back(pok); 
//	addPokaz(pok); 
}

void RDORuntime::addRuntimeFrame( rdoParse::RDOFRMFrame* frm )
{ 
	allFrames.push_back(frm); 
}

bool RDORuntime::keyDown( unsigned int scan_code )
{
	// Если нажаты VK_SHIFT или VK_CONTROL, то сбросим буфер клавиатуры
	if ( scan_code == VK_SHIFT || scan_code == VK_CONTROL ) {
		std::list< unsigned int >::iterator it = config.keyDown.begin();
		while ( it != config.keyDown.end() ) {
			if ( *it != VK_SHIFT && *it != VK_CONTROL ) {
				it = config.keyDown.erase( it );
			} else {
				it++;
			}
		}
	}
	// Подсчитаем сколько раз клавиша уже в буфере
	int cnt = 0;
	std::list< unsigned int >::iterator it = config.keyDown.begin();
	while ( it != config.keyDown.end() ) {
		if ( *it == scan_code ) {
			cnt++;
		}
		it++;
	}
	// Добавим клавишу в буфер
	if ( cnt < 4 ) {
		config.keyDown.push_back( scan_code );
	}
	if ( cnt == 0 ) key_found = true;
	return cnt > 0;
}

void RDORuntime::keyUp( unsigned int scan_code )
{
	// Если отжаты VK_SHIFT или VK_CONTROL, то сбросим удалим их из буфера
//	if ( scan_code == VK_SHIFT || scan_code == VK_CONTROL ) {
		std::list< unsigned int >::iterator it = config.keyDown.begin();
		while ( it != config.keyDown.end() ) {
			if ( *it == scan_code ) {
				it = config.keyDown.erase( it );
			} else {
				it++;
			}
		}
//	}
}

bool RDORuntime::checkKeyPressed( unsigned int scan_code, bool shift, bool control )
{
	bool shift_found   = false;
	bool control_found = false;
	// Найдем VK_SHIFT и/или VK_CONTROL в буфере
	std::list< unsigned int >::iterator it = config.keyDown.begin();
	while ( it != config.keyDown.end() ) {
		if ( *it == VK_SHIFT ) {
			shift_found = true;
			if ( shift_found && control_found ) break;
		}
		if ( *it == VK_CONTROL ) {
			control_found = true;
			if ( shift_found && control_found ) break;
		}
		it++;
	}
	// Теперь найдем саму клавишу в буфере
	// Удалим её из буфера перед выходом
	if ( shift_found == shift && control_found == control ) {
		std::list< unsigned int >::iterator it = config.keyDown.begin();
		while ( it != config.keyDown.end() ) {
			if ( *it == scan_code ) {
				config.keyDown.erase( it );
				key_found = true;
				return true;
			}
			it++;
		}
	}
	key_found = false;
	return false;
}

bool RDORuntime::checkAreaActivated( const std::string& oprName )
{
	std::vector<std::string>::iterator it = std::find( config.activeAreasMouseClicked.begin(), config.activeAreasMouseClicked.end(), oprName );
	if ( it == config.activeAreasMouseClicked.end() ) {
		return false;
	}
	config.activeAreasMouseClicked.erase( it );
	return true;
}

bool RDORuntime::isKeyDown()
{
	return key_found || !config.activeAreasMouseClicked.empty();
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateNumberedResource
// ----------------------------------------------------------------------------
RDOCalcCreateNumberedResource::RDOCalcCreateNumberedResource( int _type, bool _traceFlag, const std::vector< RDOValue >& _paramsCalcs, int _number, bool _isPermanent ):
	type( _type ),
	traceFlag( _traceFlag ),
	number( _number ),
	isPermanent( _isPermanent )
{
	paramsCalcs.insert( paramsCalcs.begin(), _paramsCalcs.begin(), _paramsCalcs.end() );
}

RDOValue RDOCalcCreateNumberedResource::calcValue( RDORuntime* sim ) const
{
	RDOResource* res = sim->createNewResource( number, isPermanent );
	res->type  = type;
	res->trace = traceFlag;
	res->params.insert( res->params.begin(), paramsCalcs.begin(), paramsCalcs.end() );
	return RDOValue(1);		// just to return something
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateEmptyResource
// ----------------------------------------------------------------------------
RDOCalcCreateEmptyResource::RDOCalcCreateEmptyResource( int _type, bool _traceFlag, int _rel_res_id, int _numParameters ):
	type( _type ),
	traceFlag( _traceFlag ),
	rel_res_id( _rel_res_id ),
	numParameters( _numParameters )
{
}

RDOValue RDOCalcCreateEmptyResource::calcValue( RDORuntime* sim ) const
{
	RDOResource* res = sim->createNewResource();
	sim->setRelRes( rel_res_id, res->number );
	res->type  = type;
	res->trace = traceFlag;
	res->params.insert( res->params.begin(), numParameters, 0 );
	return RDOValue(1);		// just to return something
}

RDORuntime::RDORuntime():
	tracer( NULL ),
	result( NULL ),
	whyStop( rdoSimulator::EC_OK ),
	key_found( false )
{
	terminateIfCalc = NULL;
}

bool RDORuntime::endCondition()
{
	if ( !terminateIfCalc ) {
		return false;	// forever
	}
	return fabs( terminateIfCalc->calcValueBase(this) ) > DBL_EPSILON;
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
	DeleteAllObjects( allCalcs, true );
	DeleteAllObjects( allPatterns );
	DeleteAllObjects( rules );
	DeleteAllObjects( ies );
	DeleteAllObjects( operations );
	DeleteAllObjects( allPokaz );
	DeleteAllObjects( allDPTs );
	DeleteAllObjects( allResourcesByID );
	DeleteAllObjects( allFrames );
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

std::list< RDOResourceTrace* > RDORuntime::getPermanentResources()
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

void RDORuntime::addRuntimeOperation( RDOActivityOperationRuntime* oper )
{ 
	operations.push_back( oper ); 
	addTemplateBaseOperation( oper ); 
}

void RDORuntime::addRuntimeRule( RDOActivityRuleRuntime* rule )
{ 
	rules.push_back( rule );
	addTemplateBaseOperation( rule );
}

void RDORuntime::addRuntimeIE( RDOActivityIERuntime* ie )
{ 
	ies.push_back( ie ); 
	addTemplateBaseOperation( ie ); 
}

void RDORuntime::addRuntimeProcess( rdoRuntime::RDOPROCProcess* _process )
{
	process.push_back( _process );
	addTemplateBaseOperation( _process ); 
}

void RDORuntime::addDPT( RDOSearchRuntime* dpt )
{ 
	allDPTs.push_back( dpt );
	addTemplateBaseOperation( dpt );
}

void RDORuntime::onInit()
{
	std::for_each( initCalcs.begin(), initCalcs.end(), std::bind2nd(std::mem_fun1(&RDOCalc::calcValueBase), this) );
}

RDOTrace *RDORuntime::getTracer()
{
	return tracer;
}

void RDORuntime::onDestroy()
{                    
	DeleteAllObjects( allResourcesByID );

	if ( tracer ) {
		delete tracer;
		tracer = NULL;
	}

	if ( result ) {
		delete result;
		result = NULL;
	}
}

RDOValue RDOCalcFuncParam::calcValue(RDORuntime *sim) const
{ 
	return sim->getFuncArgument(numberOfParam); 
}

RDOValue RDORuntime::getFuncArgument(int numberOfParam)
{
	return funcStack.at(currFuncTop + numberOfParam);
}

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcGroup
// ----------------------------------------------------------------------------
RDOValue RDOFunCalcExist::calcValue( RDORuntime* sim ) const
{
	bool res = false;
	std::list< RDOResource* >::iterator end = sim->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = sim->allResourcesByTime.begin(); it != end && !res; it++ ) {
		if ( *it == NULL ) continue;
		if ( (*it)->type != nResType ) continue;
		sim->pushGroupFunc( *it );
		if ( condition->calcValueBase(sim) ) res = true;
		sim->popGroupFunc();
	}
	return res;
}

RDOValue RDOFunCalcNotExist::calcValue( RDORuntime* sim ) const
{
	bool res = true;
	std::list< RDOResource* >::iterator end = sim->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = sim->allResourcesByTime.begin(); it != end && res; it++ ) {
		if ( *it == NULL ) continue;
		if ( (*it)->type != nResType ) continue;
		sim->pushGroupFunc( *it );
		if ( condition->calcValueBase(sim) ) res = false;
		sim->popGroupFunc();
	}
	return res;
}

RDOValue RDOFunCalcForAll::calcValue( RDORuntime* sim ) const
{
	bool first_found = false;
	bool res = true;
	std::list< RDOResource* >::iterator end = sim->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = sim->allResourcesByTime.begin(); it != end && res; it++ ) {
		if ( *it == NULL ) continue;
		if ( (*it)->type != nResType ) continue;
		sim->pushGroupFunc( *it );
		if ( !condition->calcValueBase(sim) ) {
			res = false;
		} else if ( !first_found ) {
			first_found = true;
		}
		sim->popGroupFunc();
	}
	return first_found ? res : false;
}

RDOValue RDOFunCalcNotForAll::calcValue( RDORuntime* sim ) const
{
	bool res = false;
	std::list< RDOResource* >::iterator end = sim->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = sim->allResourcesByTime.begin(); it != end && !res; it++ ) {
		if ( *it == NULL ) continue;
		if ( (*it)->type != nResType ) continue;
		sim->pushGroupFunc( *it );
		if( !condition->calcValueBase(sim) ) res = true;
		sim->popGroupFunc();
	}
	return res;
}

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcSelect
// ----------------------------------------------------------------------------
void RDOFunCalcSelect::prepare( RDORuntime* sim ) const
{
	res_list.clear();
	std::list< RDOResource* >::iterator end = sim->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = sim->allResourcesByTime.begin(); it != end; it++ ) {
		if ( *it == NULL ) continue;
		if ( (*it)->type != nResType ) continue;
		sim->pushGroupFunc( *it );
		if ( condition->calcValueBase(sim) ) {
			res_list.push_back( *it );
		}
		sim->popGroupFunc();
	}
}

RDOValue RDOFunCalcSelect::calcValue( RDORuntime* sim ) const
{
	prepare( sim );
	return res_list.empty();
}

RDOValue RDOFunCalcSelectSize::calcValue( RDORuntime* sim ) const
{
	select->prepare( sim );
	return select->res_list.size();
}

RDOValue RDOFunCalcSelectExist::calcValue( RDORuntime* sim ) const
{
	select->prepare( sim );
	bool res = false;
	std::list< RDOResource* >::iterator it  = select->res_list.begin();
	std::list< RDOResource* >::iterator end = select->res_list.end();
	while ( it != end && !res ) {
		sim->pushGroupFunc( *it );
		if ( condition->calcValueBase(sim) ) res = true;
		sim->popGroupFunc();
		it++;
	}
	return res;
}

RDOValue RDOFunCalcSelectNotExist::calcValue( RDORuntime* sim ) const
{
	select->prepare( sim );
	bool res = true;
	std::list< RDOResource* >::iterator it  = select->res_list.begin();
	std::list< RDOResource* >::iterator end = select->res_list.end();
	while ( it != end && res ) {
		sim->pushGroupFunc( *it );
		if ( condition->calcValueBase(sim) ) res = false;
		sim->popGroupFunc();
		it++;
	}
	return res;
}

RDOValue RDOFunCalcSelectForAll::calcValue( RDORuntime* sim ) const
{
	select->prepare( sim );
	if ( select->res_list.empty() ) return false;
	bool res = true;
	std::list< RDOResource* >::iterator it  = select->res_list.begin();
	std::list< RDOResource* >::iterator end = select->res_list.end();
	while ( it != end && res ) {
		sim->pushGroupFunc( *it );
		if ( !condition->calcValueBase(sim) ) res = false;
		sim->popGroupFunc();
		it++;
	}
	return res;
}

RDOValue RDOFunCalcSelectNotForAll::calcValue( RDORuntime* sim ) const
{
	select->prepare( sim );
	bool res = false;
	std::list< RDOResource* >::iterator it  = select->res_list.begin();
	std::list< RDOResource* >::iterator end = select->res_list.end();
	while ( it != end && !res ) {
		sim->pushGroupFunc( *it );
		if ( !condition->calcValueBase(sim) ) res = true;
		sim->popGroupFunc();
		it++;
	}
	return res;
}

RDOSelectResourceCalc::RDOSelectResourceCalc( int _rel_res_id, rdoParse::RDOPATChoice* _choice, rdoParse::RDOPATSelectType* _selection_type ):
	rel_res_id( _rel_res_id ),
	choice( NULL ),
	selection_calc( NULL ),
	selection_type( rdoParse::RDOPATSelectType::st_empty )
{
	if ( _choice != NULL && _choice->type == rdoParse::RDOPATChoice::ch_from ) {
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
	sim->setRelRes( rel_res_id, res_id );
	if ( choice && !choice->calcValueBase( sim ) ) {
		sim->setRelRes( rel_res_id, -1 );
		return 0;
	}
	return 1;
}

RDOValue RDOSelectResourceByTypeCalc::calcValue( RDORuntime* sim ) const
{
	RDOValue maxVal   = -DBL_MAX;
	RDOValue minVal   = DBL_MAX;
	int res_minmax_id = -1;
	std::list< RDOResource* >::iterator end = sim->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = sim->allResourcesByTime.begin(); it != end; it++ ) {

		if ( *it && (*it)->type == resType ) {

			int res_id = (*it)->number;

			switch ( selection_type ) {
				case rdoParse::RDOPATSelectType::st_empty: {
					return 1;
				}
				case rdoParse::RDOPATSelectType::st_first: {
					sim->setRelRes( rel_res_id, res_id );
					if ( choice && !choice->calcValueBase( sim ) ) {
						sim->setRelRes( rel_res_id, -1 );
						continue;
					}
					return 1;
				}
				case rdoParse::RDOPATSelectType::st_with_min: {
					sim->setRelRes( rel_res_id, res_id );
					if ( choice && !choice->calcValueBase( sim ) ) {
						sim->setRelRes( rel_res_id, -1 );
						continue;
					}
					RDOValue tmp = selection_calc->calcValueBase( sim );
					if ( tmp < minVal ) {
						minVal        = tmp;
						res_minmax_id = res_id;
					}
					break;
				}
				case rdoParse::RDOPATSelectType::st_with_max: {
					sim->setRelRes( rel_res_id, res_id );
					if ( choice && !choice->calcValueBase( sim ) ) {
						sim->setRelRes( rel_res_id, -1 );
						continue;
					}
					RDOValue tmp = selection_calc->calcValueBase( sim );
					if ( tmp > maxVal ) {
						maxVal        = tmp;
						res_minmax_id = res_id;
					}
					break;
				}
			}
		}
	}

	if ( res_minmax_id != -1 ) {
		sim->setRelRes( rel_res_id, res_minmax_id );
		return 1;
	}

	return 0;
}

std::vector< int > RDOSelectResourceDirectCommonCalc::getPossibleNumbers( RDORuntime* sim ) const
{
	std::vector< int > res;	
	res.push_back( rel_res_id );
	return res;
}

std::vector< int > RDOSelectResourceByTypeCommonCalc::getPossibleNumbers( RDORuntime* sim ) const
{
	std::vector< int > res;	
	std::list< RDOResource* >::iterator end = sim->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = sim->allResourcesByTime.begin(); it != end; it++ ) {
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
		res.push_back(sim->getResByRelRes(i));
	}

	RDOValue bestVal = 0;
	bool hasBest = false;
	getBest(allNumbs, 0, res, bestVal, sim, hasBest);

	if(!hasBest)
		return false;

	for(i = 0; i < res.size(); i++)
		sim->setRelRes(i, res.at(i));

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
				res.at(i) = sim->getResByRelRes(i);
			bestVal = newVal;
			hasBest = true;
		}

		return;
	}

	std::vector<int> &ourLevel = allNumbs.at(level);
	for(int i = 0; i < ourLevel.size(); i++)
	{
		sim->setRelRes(level, ourLevel.at(i));
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
	sim->setResParamVal(sim->getResByRelRes(relNumb), parNumb, calc->calcValueBase(sim));
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
	RDOValue res = gen->next( sim->getFuncArgument(0), sim->getFuncArgument(1) );
	if ( diap ) {
		if ( res < diap_min ) return res_real ? diap_min : static_cast<int>(diap_min > 0 ? diap_min + 0.5 : diap_min - 0.5);
		if ( res > diap_max ) return res_real ? diap_max : static_cast<int>(diap_max > 0 ? diap_max + 0.5 : diap_max - 0.5);
		return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
	} else {
		return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
	}
}

RDOValue RDOCalcSeqNextExponential::calcValue(RDORuntime *sim) const	
{
	RDOValue res = gen->next( sim->getFuncArgument(0) );
	if ( diap ) {
		if ( res < diap_min ) return res_real ? diap_min : static_cast<int>(diap_min > 0 ? diap_min + 0.5 : diap_min - 0.5);
		if ( res > diap_max ) return res_real ? diap_max : static_cast<int>(diap_max > 0 ? diap_max + 0.5 : diap_max - 0.5);
		return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
	} else {
		return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
	}
}

RDOValue RDOCalcSeqNextNormal::calcValue( RDORuntime* sim ) const
{
	RDOValue res = gen->next( sim->getFuncArgument(0), sim->getFuncArgument(1) );
	if ( diap ) {
		// В старом РДО при выходе за допустимый диапазон просто присваиваются значения диапазона, без выбора нового случайного числа. Вот этот алгоритм.
		if ( res < diap_min ) return res_real ? diap_min : static_cast<int>(diap_min > 0 ? diap_min + 0.5 : diap_min - 0.5);
		if ( res > diap_max ) return res_real ? diap_max : static_cast<int>(diap_max > 0 ? diap_max + 0.5 : diap_max - 0.5);
		return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
		// В новом РДО была сделана попытка выбирать новое случайное число, если ткущее вышло за диапазон. Но при этом смешается среднее (оно и в другом случае может смещаться imho). Для совместимости оставим первый вариант.
//		for ( int i = 0; i < 1000; i++ ) {
//			if ( res >= diap_min && res <= diap_max ) return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
//			res = gen->next( sim->getFuncArgument(0), sim->getFuncArgument(1) );
//		}
//		sim->error( "Не удается получить значение, попадающее в назначенный диапазон", this );
//		return res_real ? diap_min : static_cast<int>(diap_min);
	} else {
		return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
	}
}

RDOValue RDOCalcSeqNextByHist::calcValue( RDORuntime* sim ) const
{
	RDOValue res = gen->next();
	return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
}

RDOCalc::RDOCalc()
{
	fileToParse         = rdoParse::parser->getFileToParse();
	YYLTYPE error_pos   = error();
	error_pos.last_line = rdoParse::parser->lexer_loc_line();
	setErrorPos( error_pos );
	rdoParse::addCalcToRuntime( this ); 
}

RDOCalc::~RDOCalc()
{
	rdoParse::removeCalcToRuntime( this ); 
}

RDOValue RDOCalc::calcValueBase( RDORuntime* sim ) const
{
	try {
		return calcValue( sim );
	} catch ( RDOException& ) {
		if ( sim->errors.empty() ) {
			sim->error( "ошибка в", this );
//			sim->error( "in", this );
		} else {
			sim->error( NULL, this );
		}
	}
	return 0; // unreachable code
}

RDOSimulator *RDORuntime::clone()
{
	RDORuntime* other = new RDORuntime();
	other->sizeof_sim = sizeof( RDORuntime );
	int size = allResourcesByID.size();
	for ( int i = 0; i < size; i++ ) {
		if ( allResourcesByID.at(i) == NULL ) {
			other->allResourcesByID.push_back( NULL );
			other->allResourcesByTime.push_back( NULL );
		} else {
			RDOResource* res = new RDOResource( *allResourcesByID.at(i) );
			other->sizeof_sim += sizeof( RDOResource ) + sizeof( void* ) * 2;
			other->allResourcesByID.push_back( res );
			other->allResourcesByID.push_back( res );
			other->permanentResources.push_back( res );
		}
	}
	other->allConstants = allConstants;

	return other;
}

bool RDORuntime::operator == (RDOSimulator &other)
{
	RDORuntime *otherRuntime = dynamic_cast<RDORuntime *>(&other);
	if(otherRuntime->allResourcesByID.size() != allResourcesByID.size())
		return false;

	int size = allResourcesByID.size();
	for(int i = 0; i < size; i++)
	{
		if(allResourcesByID.at(i) == NULL && otherRuntime->allResourcesByID.at(i) != NULL)
			return false;

		if(allResourcesByID.at(i) != NULL && otherRuntime->allResourcesByID.at(i) == NULL)
			return false;

		if(allResourcesByID.at(i) == NULL && otherRuntime->allResourcesByID.at(i) == NULL)
			continue;

		if(*otherRuntime->allResourcesByID.at(i) != *allResourcesByID.at(i))
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

void RDORuntime::rdoDelay( double fromTime, double toTime )
{
/*
	config.currTime      = fromTime;
	config.newTime       = toTime;
	config.realTimeDelay = (toTime - fromTime) * 3600 * 1000 / config.showRate;

	for ( int i = 0; i < config.frames.size(); i++ ) {
		delete config.frames.at(i);
	}
	config.frames.clear();

	if ( config.showAnimation == rdoSimulator::SM_Animation ) {
		int size = allFrames.size();
		for ( int i = 0; i < size; i++ ) {
			rdoParse::RDOFRMFrame* frame = allFrames.at(i);
			if ( frame->checkCondition(this) ) {
				config.frames.push_back( frame->createFrame(this) );
			} else {
				config.frames.push_back( NULL );
			}
		}
	}

	if ( frameCallBack ) {
		(*frameCallBack)( &config, param );
	} else {								// normally  frameCallBack deletes config.frame
		for ( int i = 0; i < config.frames.size(); i++ ) {
			delete config.frames.at(i);
		}
		config.frames.clear();
	}
*/
}

void RDORuntime::onResetPokaz()
{
	std::for_each( allPokaz.begin(), allPokaz.end(), std::bind2nd(std::mem_fun1(&RDOPokaz::resetPokaz), this) );
}

void RDORuntime::onCheckPokaz()
{
	std::for_each( allPokaz.begin(), allPokaz.end(), std::bind2nd(std::mem_fun1(&RDOPokaz::checkPokaz), this) );
}

void RDORuntime::onAfterCheckPokaz()
{
	std::for_each( allPokaz.rbegin(), allPokaz.rend(), std::mem_fun(&RDOPokazTrace::tracePokaz) );
}

std::string RDORuntime::writePokazStructure()
{
	std::stringstream stream;
	for(int i = 0; i < allPokaz.size(); i++)
	{
		rdoParse::RDOPMDPokaz *curr = allPokaz.at(i);
		if(curr->trace)
			curr->writePokazStructure(stream);
	}

	return stream.str();
}

std::string RDORuntime::writeActivitiesStructure( int& counter )
{
	std::stringstream stream;
	std::vector< RDOBaseOperation* >::const_iterator it = haveBaseOperations.begin();
	while ( it != haveBaseOperations.end() ) {
		RDOActivityRuleRuntime* rule = dynamic_cast<RDOActivityRuleRuntime*>(*it);
		if ( rule ) {
			stream << counter++ << " ";
			rule->writeModelStructure( stream );
		} else {
			RDOActivityOperationRuntime* opr = dynamic_cast<RDOActivityOperationRuntime*>(*it);
			if ( opr ) {
				stream << counter++ << " ";
				opr->writeModelStructure( stream );
			}
		}
		it++;
	}
	stream << std::endl;

	int _counter = 1;
	int size = ies.size();
	for ( int i = 0; i < size; i++ ) {
		stream << _counter++ << " ";
		counter++;
		ies.at(i)->writeModelStructure(stream);
	}

	return stream.str();
}

RDOValue RDOCalcDiv::calcValue(RDORuntime *sim) const
{
	RDOValue rVal = right->calcValueBase(sim);
	if ( rVal == 0 ) {
		sim->error( "Деление на ноль", this );
//		sim->error("Division by zero", this);
	}
	return RDOValue( left->calcValueBase(sim) / rVal );
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
	if ( message ) {
		errors.push_back( rdoSimulator::RDOSyntaxError( rdoSimulator::RDOSyntaxError::UNKNOWN, message, calc->error().last_line, calc->error().last_column, calc->getFileType() ) );
	}
	throw rdoParse::RDOSyntaxException("");
}

void RDORuntime::onPutToTreeNode()
{
	// when create TreeNode with new RDOSimulator,
	// make all resources permanent, to avoid trace their
	// erase when delete TreeNode
	for(int i = 0; i < allResourcesByID.size(); i++)
	{
		if(allResourcesByID.at(i))
			allResourcesByID.at(i)->makeTemporary(false);
	}

}

void RDORuntime::writeExitCode()
{
	switch ( whyStop ) {
		case rdoSimulator::EC_OK:
			getTracer()->writeStatus( this, "NORMAL_TERMINATION" );
			break;
		case rdoSimulator::EC_NoMoreEvents:
			getTracer()->writeStatus( this, "NO_MORE_EVENTS" );
			break;
		case rdoSimulator::EC_RunTimeError:
			getTracer()->writeStatus( this, "RUN_TIME_ERROR" );
			break;
		case rdoSimulator::EC_UserBreak:
			getTracer()->writeStatus( this, "USER_BREAK" );
			break;
	}
	getTracer()->stopWriting();
}

void RDORuntime::postProcess()
{
	getTracer()->startWriting();
	try {
		std::for_each( allPokaz.begin(), allPokaz.end(), std::bind2nd(std::mem_fun1(&RDOPokaz::calcStat), this) );
		RDOSimulatorTrace::postProcess();
		writeExitCode();
	} catch ( rdoParse::RDOSyntaxException& e ) {
		writeExitCode();
		throw e;
	}
}

} // namespace rdoRuntime
