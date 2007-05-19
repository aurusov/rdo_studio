#include "pch.h"
#include "rdocalc.h"
#include <limits>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime
{
// ----------------------------------------------------------------------------
// ---------- RDOCalc
// ----------------------------------------------------------------------------
RDOCalc::RDOCalc( RDORuntimeParent* _parent ):
	RDORuntimeObject( _parent )
{
}

RDOCalc::~RDOCalc()
{
}

RDOValue RDOCalc::calcValueBase( RDORuntime* runtime ) const
{
	try {
		return calcValue( runtime );
	} catch ( RDORuntimeException& ) {
		if ( runtime->errors.empty() ) {
			runtime->error( "ошибка в", this );
//			runtime->error( "in", this );
		} else {
			runtime->error( NULL, this );
		}
	}
	return 0; // unreachable code
}

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcGroup
// ----------------------------------------------------------------------------
RDOValue RDOFunCalcExist::calcValue( RDORuntime* runtime ) const
{
	bool res = false;
	std::list< RDOResource* >::iterator end = runtime->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = runtime->allResourcesByTime.begin(); it != end && !res; it++ ) {
		if ( *it == NULL ) continue;
		if ( (*it)->type != nResType ) continue;
		runtime->pushGroupFunc( *it );
		if ( condition->calcValueBase( runtime ) ) res = true;
		runtime->popGroupFunc();
	}
	return res;
}

RDOValue RDOFunCalcNotExist::calcValue( RDORuntime* runtime ) const
{
	bool res = true;
	std::list< RDOResource* >::iterator end = runtime->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = runtime->allResourcesByTime.begin(); it != end && res; it++ ) {
		if ( *it == NULL ) continue;
		if ( (*it)->type != nResType ) continue;
		runtime->pushGroupFunc( *it );
		if ( condition->calcValueBase( runtime ) ) res = false;
		runtime->popGroupFunc();
	}
	return res;
}

RDOValue RDOFunCalcForAll::calcValue( RDORuntime* runtime ) const
{
	bool first_found = false;
	bool res = true;
	std::list< RDOResource* >::iterator end = runtime->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = runtime->allResourcesByTime.begin(); it != end && res; it++ ) {
		if ( *it == NULL ) continue;
		if ( (*it)->type != nResType ) continue;
		runtime->pushGroupFunc( *it );
		if ( !condition->calcValueBase( runtime ) ) {
			res = false;
		} else if ( !first_found ) {
			first_found = true;
		}
		runtime->popGroupFunc();
	}
	return first_found ? res : false;
}

RDOValue RDOFunCalcNotForAll::calcValue( RDORuntime* runtime ) const
{
	bool res = false;
	std::list< RDOResource* >::iterator end = runtime->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = runtime->allResourcesByTime.begin(); it != end && !res; it++ ) {
		if ( *it == NULL ) continue;
		if ( (*it)->type != nResType ) continue;
		runtime->pushGroupFunc( *it );
		if( !condition->calcValueBase( runtime ) ) res = true;
		runtime->popGroupFunc();
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
		if ( condition->calcValueBase( sim ) ) {
			res_list.push_back( *it );
		}
		sim->popGroupFunc();
	}
}

RDOValue RDOFunCalcSelect::calcValue( RDORuntime* runtime ) const
{
	prepare( runtime );
	return res_list.empty();
}

RDOValue RDOFunCalcSelectSize::calcValue( RDORuntime* runtime ) const
{
	select->prepare( runtime );
	return select->res_list.size();
}

RDOValue RDOFunCalcSelectExist::calcValue( RDORuntime* runtime ) const
{
	select->prepare( runtime );
	bool res = false;
	std::list< RDOResource* >::iterator it  = select->res_list.begin();
	std::list< RDOResource* >::iterator end = select->res_list.end();
	while ( it != end && !res ) {
		runtime->pushGroupFunc( *it );
		if ( condition->calcValueBase( runtime ) ) res = true;
		runtime->popGroupFunc();
		it++;
	}
	return res;
}

RDOValue RDOFunCalcSelectNotExist::calcValue( RDORuntime* runtime ) const
{
	select->prepare( runtime );
	bool res = true;
	std::list< RDOResource* >::iterator it  = select->res_list.begin();
	std::list< RDOResource* >::iterator end = select->res_list.end();
	while ( it != end && res ) {
		runtime->pushGroupFunc( *it );
		if ( condition->calcValueBase( runtime ) ) res = false;
		runtime->popGroupFunc();
		it++;
	}
	return res;
}

RDOValue RDOFunCalcSelectForAll::calcValue( RDORuntime* runtime ) const
{
	select->prepare( runtime );
	if ( select->res_list.empty() ) return false;
	bool res = true;
	std::list< RDOResource* >::iterator it  = select->res_list.begin();
	std::list< RDOResource* >::iterator end = select->res_list.end();
	while ( it != end && res ) {
		runtime->pushGroupFunc( *it );
		if ( !condition->calcValueBase( runtime ) ) res = false;
		runtime->popGroupFunc();
		it++;
	}
	return res;
}

RDOValue RDOFunCalcSelectNotForAll::calcValue( RDORuntime* runtime ) const
{
	select->prepare( runtime );
	bool res = false;
	std::list< RDOResource* >::iterator it  = select->res_list.begin();
	std::list< RDOResource* >::iterator end = select->res_list.end();
	while ( it != end && !res ) {
		runtime->pushGroupFunc( *it );
		if ( !condition->calcValueBase( runtime ) ) res = true;
		runtime->popGroupFunc();
		it++;
	}
	return res;
}

// ----------------------------------------------------------------------------
// ---------- Sequence
// ----------------------------------------------------------------------------
RDOValue RDOCalcSeqInit::calcValue( RDORuntime* runtime ) const	
{
	gen->setSeed(base);
	return RDOValue(0);
}

RDOCalcSeqInit::~RDOCalcSeqInit() 
{ 
	delete gen;
}

RDOValue RDOCalcSeqNextUniform::calcValue( RDORuntime* runtime ) const	
{
	RDOValue res = gen->next( runtime->getFuncArgument(0), runtime->getFuncArgument(1) );
	if ( diap ) {
		if ( res < diap_min ) return res_real ? diap_min : static_cast<int>(diap_min > 0 ? diap_min + 0.5 : diap_min - 0.5);
		if ( res > diap_max ) return res_real ? diap_max : static_cast<int>(diap_max > 0 ? diap_max + 0.5 : diap_max - 0.5);
		return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
	} else {
		return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
	}
}

RDOValue RDOCalcSeqNextExponential::calcValue( RDORuntime* runtime ) const	
{
	RDOValue res = gen->next( runtime->getFuncArgument(0) );
	if ( diap ) {
		if ( res < diap_min ) return res_real ? diap_min : static_cast<int>(diap_min > 0 ? diap_min + 0.5 : diap_min - 0.5);
		if ( res > diap_max ) return res_real ? diap_max : static_cast<int>(diap_max > 0 ? diap_max + 0.5 : diap_max - 0.5);
		return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
	} else {
		return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
	}
}

RDOValue RDOCalcSeqNextNormal::calcValue( RDORuntime* runtime ) const
{
	RDOValue res = gen->next( runtime->getFuncArgument(0), runtime->getFuncArgument(1) );
	if ( diap ) {
		// В старом РДО при выходе за допустимый диапазон просто присваиваются значения диапазона, без выбора нового случайного числа. Вот этот алгоритм.
		if ( res < diap_min ) return res_real ? diap_min : static_cast<int>(diap_min > 0 ? diap_min + 0.5 : diap_min - 0.5);
		if ( res > diap_max ) return res_real ? diap_max : static_cast<int>(diap_max > 0 ? diap_max + 0.5 : diap_max - 0.5);
		return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
		// В новом РДО была сделана попытка выбирать новое случайное число, если ткущее вышло за диапазон. Но при этом смешается среднее (оно и в другом случае может смещаться imho). Для совместимости оставим первый вариант.
//		for ( int i = 0; i < 1000; i++ ) {
//			if ( res >= diap_min && res <= diap_max ) return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
//			res = gen->next( runtime->getFuncArgument(0), runtime->getFuncArgument(1) );
//		}
//		runtime->error( "Не удается получить значение, попадающее в назначенный диапазон", this );
//		return res_real ? diap_min : static_cast<int>(diap_min);
	} else {
		return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
	}
}

RDOValue RDOCalcSeqNextByHist::calcValue( RDORuntime* runtime ) const
{
	RDOValue res = gen->next();
	return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateNumberedResource
// ----------------------------------------------------------------------------
RDOCalcCreateNumberedResource::RDOCalcCreateNumberedResource( RDORuntimeParent* _parent, int _type, bool _traceFlag, const std::vector< RDOValue >& _paramsCalcs, int _number, bool _isPermanent ):
	RDOCalc( _parent ),
	type( _type ),
	traceFlag( _traceFlag ),
	number( _number ),
	isPermanent( _isPermanent )
{
	paramsCalcs.insert( paramsCalcs.begin(), _paramsCalcs.begin(), _paramsCalcs.end() );
}

RDOValue RDOCalcCreateNumberedResource::calcValue( RDORuntime* runtime ) const
{
	RDOResource* res = runtime->createNewResource( number, isPermanent );
	res->type  = type;
	res->trace = traceFlag;
	res->params.insert( res->params.begin(), paramsCalcs.begin(), paramsCalcs.end() );
	return RDOValue(1);		// just to return something
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateEmptyResource
// ----------------------------------------------------------------------------
RDOCalcCreateEmptyResource::RDOCalcCreateEmptyResource( RDORuntimeParent* _parent, int _type, bool _traceFlag, int _rel_res_id, int _numParameters ):
	RDOCalc( _parent ),
	type( _type ),
	traceFlag( _traceFlag ),
	rel_res_id( _rel_res_id ),
	numParameters( _numParameters )
{
}

RDOValue RDOCalcCreateEmptyResource::calcValue( RDORuntime* runtime ) const
{
	RDOResource* res = runtime->createNewResource();
	runtime->setRelRes( rel_res_id, res->number );
	res->type  = type;
	res->trace = traceFlag;
	res->params.insert( res->params.begin(), numParameters, 0 );
	return RDOValue(1);		// just to return something
}

// ----------------------------------------------------------------------------
// ---------- Выбор ресурсов
// ----------------------------------------------------------------------------
RDOSelectResourceCalc::RDOSelectResourceCalc( RDORuntimeParent* _parent, int _rel_res_id, RDOCalc* _choice, RDOCalc* _selection_calc, Type _selection_type ):
	RDOCalc( _parent ),
	rel_res_id( _rel_res_id ),
	choice( _choice ),
	selection_calc( _selection_calc ),
	selection_type( _selection_type )
{
}

RDOValue RDOSelectResourceDirectCalc::calcValue( RDORuntime* runtime ) const
{
	runtime->setRelRes( rel_res_id, res_id );
	if ( choice && !choice->calcValueBase( runtime ) ) {
		runtime->setRelRes( rel_res_id, -1 );
		return 0;
	}
	return 1;
}

RDOValue RDOSelectResourceByTypeCalc::calcValue( RDORuntime* runtime ) const
{
	RDOValue maxVal   = -DBL_MAX;
	RDOValue minVal   = DBL_MAX;
	int res_minmax_id = -1;
	std::list< RDOResource* >::iterator end = runtime->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = runtime->allResourcesByTime.begin(); it != end; it++ ) {

		if ( *it && (*it)->type == resType ) {

			int res_id = (*it)->number;

			switch ( selection_type ) {
				case st_empty: {
					return 1;
				}
				case st_first: {
					runtime->setRelRes( rel_res_id, res_id );
					if ( choice && !choice->calcValueBase( runtime ) ) {
						runtime->setRelRes( rel_res_id, -1 );
						continue;
					}
					return 1;
				}
				case st_with_min: {
					runtime->setRelRes( rel_res_id, res_id );
					if ( choice && !choice->calcValueBase( runtime ) ) {
						runtime->setRelRes( rel_res_id, -1 );
						continue;
					}
					RDOValue tmp = selection_calc->calcValueBase( runtime );
					if ( tmp < minVal ) {
						minVal        = tmp;
						res_minmax_id = res_id;
					}
					break;
				}
				case st_with_max: {
					runtime->setRelRes( rel_res_id, res_id );
					if ( choice && !choice->calcValueBase( runtime ) ) {
						runtime->setRelRes( rel_res_id, -1 );
						continue;
					}
					RDOValue tmp = selection_calc->calcValueBase( runtime );
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
		runtime->setRelRes( rel_res_id, res_minmax_id );
		return 1;
	}

	return 0;
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

		RDOValue newVal = choice->calcValueBase( sim );
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

RDOValue RDOSelectResourceCommonCalc::calcValue( RDORuntime* runtime ) const
{
	std::vector<std::vector<int> > allNumbs;
	std::vector<int> res;
	for(int i = 0; i < resSelectors.size(); i++)
	{
		allNumbs.push_back(resSelectors.at(i)->getPossibleNumbers(runtime));
		res.push_back(runtime->getResByRelRes(i));
	}

	RDOValue bestVal = 0;
	bool hasBest = false;
	getBest(allNumbs, 0, res, bestVal, runtime, hasBest);

	if(!hasBest)
		return false;

	for(i = 0; i < res.size(); i++)
		runtime->setRelRes(i, res.at(i));

	return true;
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

bool RDOSelectResourceDirectCommonCalc::callChoice(RDORuntime *sim) const
{
	if ( choice && !choice->calcValueBase( sim ) ) return 0;
	return 1;
}

bool RDOSelectResourceByTypeCommonCalc::callChoice(RDORuntime *sim) const
{
	if ( choice && !choice->calcValueBase( sim ) ) return 0;
	return 1;
}

} // namespace rdoParse 
