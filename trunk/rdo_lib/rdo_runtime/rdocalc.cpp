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

RDOValue& RDOCalc::calcValueBase( RDORuntime* runtime )
{
	try {
#ifdef _DEBUG
/*
		if ( src_text().empty() ) {
			TRACE( "%d\n", sizeof(std::string) );
		} else if ( src_text().length() < 500 ) {
			TRACE( "calc: %s\n", src_text().c_str() );
			if ( src_text() == "класс" ) {
				TRACE( "calc: %s\n", src_text().c_str() );
			}
		} else {
			std::string str = src_text();
			str.resize( 500 );
			TRACE( "calc: %s\n", str.c_str() );
		}
*/
#endif
		return calcValue( runtime );
	} catch ( RDORuntimeException& ) {
		if ( runtime->errors.empty() ) {
			runtime->error( "ошибка в", this );
//			runtime->error( "in", this );
		} else {
			runtime->error( "", this );
		}
	}
	return value;
}

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcGroup
// ----------------------------------------------------------------------------
RDOValue& RDOFunCalcExist::calcValue( RDORuntime* runtime )
{
	bool res = false;
	std::list< RDOResource* >::iterator end = runtime->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = runtime->allResourcesByTime.begin(); it != end && !res; it++ ) {
		if ( *it == NULL ) continue;
		if ( (*it)->type != nResType ) continue;
		runtime->pushGroupFunc( *it );
		if ( condition->calcValueBase( runtime ).getBool() ) res = true;
		runtime->popGroupFunc();
	}
	value = res;
	return value;
}

RDOValue& RDOFunCalcNotExist::calcValue( RDORuntime* runtime )
{
	bool res = true;
	std::list< RDOResource* >::iterator end = runtime->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = runtime->allResourcesByTime.begin(); it != end && res; it++ ) {
		if ( *it == NULL ) continue;
		if ( (*it)->type != nResType ) continue;
		runtime->pushGroupFunc( *it );
		if ( condition->calcValueBase( runtime ).getBool() ) res = false;
		runtime->popGroupFunc();
	}
	value = res;
	return value;
}

RDOValue& RDOFunCalcForAll::calcValue( RDORuntime* runtime )
{
	bool first_found = false;
	bool res = true;
	std::list< RDOResource* >::iterator end = runtime->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = runtime->allResourcesByTime.begin(); it != end && res; it++ ) {
		if ( *it == NULL ) continue;
		if ( (*it)->type != nResType ) continue;
		runtime->pushGroupFunc( *it );
		if ( !condition->calcValueBase( runtime ).getBool() ) {
			res = false;
		} else if ( !first_found ) {
			first_found = true;
		}
		runtime->popGroupFunc();
	}
	value = first_found ? res : false;
	return value;
}

RDOValue& RDOFunCalcNotForAll::calcValue( RDORuntime* runtime )
{
	bool res = false;
	std::list< RDOResource* >::iterator end = runtime->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = runtime->allResourcesByTime.begin(); it != end && !res; it++ ) {
		if ( *it == NULL ) continue;
		if ( (*it)->type != nResType ) continue;
		runtime->pushGroupFunc( *it );
		if( !condition->calcValueBase( runtime ).getBool() ) res = true;
		runtime->popGroupFunc();
	}
	value = res;
	return value;
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
		if ( condition->calcValueBase( sim ).getBool() ) {
			res_list.push_back( *it );
		}
		sim->popGroupFunc();
	}
}

RDOValue& RDOFunCalcSelect::calcValue( RDORuntime* runtime )
{
	prepare( runtime );
	return value;
}

RDOValue& RDOFunCalcSelectSize::calcValue( RDORuntime* runtime )
{
	select->prepare( runtime );
	value = select->res_list.size();
	return value;
}

RDOValue& RDOFunCalcSelectEmpty::calcValue( RDORuntime* runtime )
{
	select->prepare( runtime );
	value = select->res_list.empty();
	return value;
}

RDOValue& RDOFunCalcSelectExist::calcValue( RDORuntime* runtime )
{
	select->prepare( runtime );
	bool res = false;
	std::list< RDOResource* >::iterator it  = select->res_list.begin();
	std::list< RDOResource* >::iterator end = select->res_list.end();
	while ( it != end && !res ) {
		runtime->pushGroupFunc( *it );
		if ( condition->calcValueBase( runtime ).getBool() ) res = true;
		runtime->popGroupFunc();
		it++;
	}
	value = res;
	return value;
}

RDOValue& RDOFunCalcSelectNotExist::calcValue( RDORuntime* runtime )
{
	select->prepare( runtime );
	bool res = true;
	std::list< RDOResource* >::iterator it  = select->res_list.begin();
	std::list< RDOResource* >::iterator end = select->res_list.end();
	while ( it != end && res ) {
		runtime->pushGroupFunc( *it );
		if ( condition->calcValueBase( runtime ).getBool() ) res = false;
		runtime->popGroupFunc();
		it++;
	}
	value = res;
	return value;
}

RDOValue& RDOFunCalcSelectForAll::calcValue( RDORuntime* runtime )
{
	select->prepare( runtime );
	if ( select->res_list.empty() ) {
		value = false;
		return value;
	}
	bool res = true;
	std::list< RDOResource* >::iterator it  = select->res_list.begin();
	std::list< RDOResource* >::iterator end = select->res_list.end();
	while ( it != end && res ) {
		runtime->pushGroupFunc( *it );
		if ( !condition->calcValueBase( runtime ).getBool() ) res = false;
		runtime->popGroupFunc();
		it++;
	}
	value = res;
	return value;
}

RDOValue& RDOFunCalcSelectNotForAll::calcValue( RDORuntime* runtime )
{
	select->prepare( runtime );
	bool res = false;
	std::list< RDOResource* >::iterator it  = select->res_list.begin();
	std::list< RDOResource* >::iterator end = select->res_list.end();
	while ( it != end && !res ) {
		runtime->pushGroupFunc( *it );
		if ( !condition->calcValueBase( runtime ).getBool() ) res = true;
		runtime->popGroupFunc();
		it++;
	}
	value = res;
	return value;
}

// ----------------------------------------------------------------------------
// ---------- Последовательности
// ----------------------------------------------------------------------------
RDOValue& RDOCalcSeqInit::calcValue( RDORuntime* runtime )	
{
	gen->setSeed( base );
	return value;
}

RDOCalcSeqInit::~RDOCalcSeqInit() 
{ 
	delete gen;
}

RDOValue& RDOCalcSeqNextUniform::calcValue( RDORuntime* runtime )	
{
	RDOValue res = gen->next( runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getDouble() );
	if ( diap ) {
		if ( res < diap_min ) {
			value = res_real ? diap_min : RDOValue(diap_min > 0 ? diap_min + 0.5 : diap_min - 0.5).getInt();
			return value;
		}
		if ( res > diap_max ) {
			value = res_real ? diap_max : RDOValue(diap_max > 0 ? diap_max + 0.5 : diap_max - 0.5).getInt();
			return value;
		}
		value = res_real ? res : RDOValue(res > 0 ? res + 0.5 : res - 0.5).getInt();
		return value;
	} else {
		value = res_real ? res : RDOValue(res > 0 ? res + 0.5 : res - 0.5).getInt();
		return value;
	}
}

RDOValue& RDOCalcSeqNextExponential::calcValue( RDORuntime* runtime )	
{
	RDOValue res = gen->next( runtime->getFuncArgument(0).getDouble() );
	if ( diap ) {
		if ( res < diap_min ) {
			value = res_real ? diap_min : RDOValue(diap_min > 0 ? diap_min + 0.5 : diap_min - 0.5).getInt();
			return value;
		}
		if ( res > diap_max ) {
			value = res_real ? diap_max : RDOValue(diap_max > 0 ? diap_max + 0.5 : diap_max - 0.5).getInt();
			return value;
		}
		value = res_real ? res : RDOValue(res > 0 ? res + 0.5 : res - 0.5).getInt();
		return value;
	} else {
		value = res_real ? res : RDOValue(res > 0 ? res + 0.5 : res - 0.5).getInt();
		return value;
	}
}

RDOValue& RDOCalcSeqNextNormal::calcValue( RDORuntime* runtime )
{
	RDOValue res = gen->next( runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getDouble() );
	if ( diap ) {
		// В старом РДО при выходе за допустимый диапазон просто присваиваются значения диапазона, без выбора нового случайного числа. Вот этот алгоритм.
		if ( res < diap_min ) {
			value = res_real ? diap_min : RDOValue(diap_min > 0 ? diap_min + 0.5 : diap_min - 0.5).getInt();
			return value;
		}
		if ( res > diap_max ) {
			value = res_real ? diap_max : RDOValue(diap_max > 0 ? diap_max + 0.5 : diap_max - 0.5).getInt();
			return value;
		}
		value = res_real ? res : RDOValue(res > 0 ? res + 0.5 : res - 0.5).getInt();
		return value;
		// В новом РДО была сделана попытка выбирать новое случайное число, если текущее вышло за диапазон. Но при этом смешается среднее (оно и в другом случае может смещаться imho). Для совместимости оставим первый вариант.
//		for ( int i = 0; i < 1000; i++ ) {
//			if ( res >= diap_min && res <= diap_max ) return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
//			res = gen->next( runtime->getFuncArgument(0), runtime->getFuncArgument(1) );
//		}
//		runtime->error( "Не удается получить значение, попадающее в назначенный диапазон", this );
//		return res_real ? diap_min : static_cast<int>(diap_min);
	} else {
		value = res_real ? res : RDOValue(res > 0 ? res + 0.5 : res - 0.5).getInt();
		return value;
	}
}

RDOValue& RDOCalcSeqNextByHist::calcValue( RDORuntime* runtime )
{
	RDOValue res = gen->next();
	value = res_real ? res : RDOValue(res > 0 ? res + 0.5 : res - 0.5).getInt();
	return value;
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

RDOValue& RDOCalcCreateNumberedResource::calcValue( RDORuntime* runtime )
{
	RDOResource* res = runtime->createNewResource( number, isPermanent, traceFlag );
	res->type  = type;
	res->params.insert( res->params.begin(), paramsCalcs.begin(), paramsCalcs.end() );
	return value; // just to return something
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateEmptyResource
// ----------------------------------------------------------------------------
RDOCalcCreateEmptyResource::RDOCalcCreateEmptyResource( RDORuntimeParent* _parent, int _type, bool _traceFlag, const std::vector< RDOValue >& _params_default, int _rel_res_id ):
	RDOCalc( _parent ),
	type( _type ),
	traceFlag( _traceFlag ),
	rel_res_id( _rel_res_id )
{
	params_default.insert( params_default.begin(), _params_default.begin(), _params_default.end() );
}

RDOValue& RDOCalcCreateEmptyResource::calcValue( RDORuntime* runtime )
{
	RDOResource* res = runtime->createNewResource( traceFlag );
	runtime->setRelRes( rel_res_id, res->getTraceID() );
	res->type  = type;
	res->params.insert( res->params.begin(), params_default.begin(), params_default.end() );
	return value; // just to return something
}

// ----------------------------------------------------------------------------
// ---------- Выбор ресурсов
// ----------------------------------------------------------------------------
RDOSelectResourceCalc::RDOSelectResourceCalc( RDORuntimeParent* _parent, int _rel_res_id, RDOCalc* _choice_calc, RDOCalc* _order_calc, Type _order_type ):
	RDOCalc( _parent ),
	rel_res_id( _rel_res_id ),
	choice_calc( _choice_calc ),
	order_calc( _order_calc ),
	order_type( _order_type )
{
}

RDOValue& RDOSelectResourceDirectCalc::calcValue( RDORuntime* runtime )
{
	runtime->setRelRes( rel_res_id, res_id );
	if ( choice_calc && !choice_calc->calcValueBase( runtime ).getBool() ) {
		runtime->setRelRes( rel_res_id, -1 );
		value = 0;
		return value;
	}
	value = 1;
	return value;
}

RDOValue& RDOSelectResourceByTypeCalc::calcValue( RDORuntime* runtime )
{
	RDOValue maxVal   = -DBL_MAX;
	RDOValue minVal   = DBL_MAX;
	int res_minmax_id = -1;
	std::list< RDOResource* >::iterator end = runtime->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = runtime->allResourcesByTime.begin(); it != end; it++ ) {

		if ( *it && (*it)->type == resType ) {

			int res_id = (*it)->getTraceID();

			switch ( order_type ) {
				case order_empty:
				case order_first: {
					runtime->setRelRes( rel_res_id, res_id );
					if ( choice_calc && !choice_calc->calcValueBase( runtime ).getBool() ) {
						runtime->setRelRes( rel_res_id, -1 );
						continue;
					}
					value = 1;
					return value;
				}
				case order_with_min: {
					runtime->setRelRes( rel_res_id, res_id );
					if ( choice_calc && !choice_calc->calcValueBase( runtime ).getBool() ) {
						runtime->setRelRes( rel_res_id, -1 );
						continue;
					}
					RDOValue tmp = order_calc->calcValueBase( runtime );
					if ( tmp < minVal ) {
						minVal        = tmp;
						res_minmax_id = res_id;
					}
					break;
				}
				case order_with_max: {
					runtime->setRelRes( rel_res_id, res_id );
					if ( choice_calc && !choice_calc->calcValueBase( runtime ).getBool() ) {
						runtime->setRelRes( rel_res_id, -1 );
						continue;
					}
					RDOValue tmp = order_calc->calcValueBase( runtime );
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
		value = 1;
		return value;
	}
	value = 0;
	return value;
}

void RDOSelectResourceCommonCalc::getBest( std::vector< std::vector< int > >& allNumbs, int level, std::vector< int >& res, RDOValue& bestVal, RDORuntime* sim, bool& hasBest ) const
{
	if ( level >= allNumbs.size() ) {
		for ( int i = 0; i < resSelectors.size(); i++ ) {
			if( !resSelectors.at(i)->callChoice(sim) ) {
				return; // state not valid
			}
		}
		RDOValue newVal = choice_calc->calcValueBase( sim );
		if ( !hasBest || (useCommonWithMax && (newVal > bestVal)) ||
			(!useCommonWithMax && (newVal < bestVal))) // found better value
		{
			for ( int i = 0; i < resSelectors.size(); i++ ) {
				res.at(i) = sim->getResByRelRes(i);
			}
			bestVal = newVal;
			hasBest = true;
		}
		return;
	}
	std::vector< int >& ourLevel = allNumbs.at(level);
	for ( int i = 0; i < ourLevel.size(); i++ ) {
		sim->setRelRes( level, ourLevel.at(i) );
		getBest( allNumbs, level+1, res, bestVal, sim, hasBest );
	}
}

bool RDOSelectResourceCommonCalc::getFirst( std::vector< std::vector< int > >& allNumbs, int level, RDORuntime* sim ) const
{
	if ( level >= allNumbs.size() ) {
		for ( int i = 0; i < resSelectors.size(); i++ ) {
			if( !resSelectors.at(i)->callChoice(sim) ) {
				return false;
			}
		}
		return true;
	}
	std::vector< int >& ourLevel = allNumbs.at(level);
	for ( int i = 0; i < ourLevel.size(); i++ ) {
		sim->setRelRes( level, ourLevel.at(i) );
		if ( getFirst( allNumbs, level+1, sim ) ) return true;
	}
	return false;
}

/*
bool RDOSelectResourceCommonCalc::getFirst( std::vector< std::vector< int > >& allNumbs, int level, RDORuntime* sim ) const
{
	if ( level <= 0 ) {
		for ( int i = 0; i < resSelectors.size(); i++ ) {
			if( !resSelectors.at(i)->callChoice(sim) ) {
				return false;
			}
		}
		return true;
	} else {
		level--;
		std::vector< int >& ourLevel = allNumbs.at(level);
		for ( int i = 0; i < ourLevel.size(); i++ ) {
			sim->setRelRes( level, ourLevel.at(i) );
			if ( getFirst( allNumbs, level, sim ) ) return true;
		}
	}
	return false;
}
*/

RDOValue& RDOSelectResourceCommonCalc::calcValue( RDORuntime* runtime )
{
	std::vector< std::vector<int> > allNumbs;
	std::vector< int > res;
	for ( int i = 0; i < resSelectors.size(); i++ ) {
		allNumbs.push_back( resSelectors.at(i)->getPossibleNumbers(runtime) );
		res.push_back( runtime->getResByRelRes(i) );
	}
	if ( !choice_calc ) {
		// first
/*
		if ( getFirst( allNumbs, allNumbs.size(), runtime ) ) {
			return true;
		}
*/
		if ( getFirst( allNumbs, 0, runtime ) ) {
			value = 1;
			return value;
		}
	} else {
		// with_min / with_max
		RDOValue bestVal = 0;
		bool found = false;
		getBest( allNumbs, 0, res, bestVal, runtime, found );
		if ( found ) {
			for ( i = 0; i < res.size(); i++ ) {
				runtime->setRelRes( i, res.at(i) );
			}
			value = 1;
			return value;
		}
	}
	value = 0;
	return value;
}

std::vector< int > RDOSelectResourceDirectCommonCalc::getPossibleNumbers( RDORuntime* sim ) const
{
	std::vector< int > res;	
	res.push_back( res_id );
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

		res.push_back((*it)->getTraceID());
	}

	return res;
}

bool RDOSelectResourceDirectCommonCalc::callChoice(RDORuntime *sim) const
{
	if ( choice_calc && !choice_calc->calcValueBase( sim ).getBool() ) return 0;
	return 1;
}

bool RDOSelectResourceByTypeCommonCalc::callChoice(RDORuntime *sim) const
{
	if ( choice_calc && !choice_calc->calcValueBase( sim ).getBool() ) return 0;
	return 1;
}

} // namespace rdoParse 
