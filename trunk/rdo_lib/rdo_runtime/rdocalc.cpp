#include "pch.h"
#include "rdocalc.h"
#include "rdoprocess.h"
#include "rdo_runtime.h"
#include "rdo_activity.h"
#include <limits>
#include <math.h>

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
RDOCalc::RDOCalc( RDORuntimeParent* parent ):
	RDORuntimeObject( parent )
{
}

RDOCalc::~RDOCalc()
{
}

RDOValue& RDOCalc::calcValue( RDORuntime* runtime )
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
		return doCalc( runtime );
	} catch ( RDORuntimeException& ) {
		if ( runtime->errors.empty() ) {
			runtime->error( "ошибка в", this );
//			runtime->error( "in", this );
		} else {
			runtime->error( "", this );
		}
	}
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetResParam (Параметры постоянного ресурса)
// ----------------------------------------------------------------------------
RDOValue& RDOCalcGetResParam::doCalc( RDORuntime* runtime )
{
	m_value = runtime->getResParamVal( m_resID, m_paramID );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTempResParamFRM (Параметры временного ресурса для FRM)
// ----------------------------------------------------------------------------
RDOCalcGetTempResParamFRM::RDOCalcGetTempResParamFRM( RDORuntime* parent, int _resNumb, int _parNumb ):
	RDOCalcGetResParam( parent, _resNumb, _parNumb )
{
	parent->connect( this, RDORuntime::RO_BEFOREDELETE );
}

RDOValue& RDOCalcGetTempResParamFRM::doCalc( RDORuntime* runtime )
{
	if ( m_resID >= 0 ) {
		m_value = runtime->getResParamVal( m_resID, m_paramID );
	} else if ( m_resID == -1 ) {
		RDOEnumType* _enum = new RDOEnumType( runtime );
		_enum->add( "Удален" );
		m_value = RDOValue( *_enum );
		m_resID = -2;
	}
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetUnknowResParam (Параметры несуществующего ресурса)
// ----------------------------------------------------------------------------
RDOValue& RDOCalcGetUnknowResParam::doCalc( RDORuntime* runtime )
{
	runtime->error( rdo::format("Попытка использовать несуществующий ресурс: %s.%s", m_resName.c_str(), m_parName.c_str()), this );
	return m_value;
}


// ----------------------------------------------------------------------------
// ---------- RDOCalcGetGroupResParam
// ----------------------------------------------------------------------------
RDOValue& RDOCalcGetGroupResParam::doCalc( RDORuntime* runtime )
{
	RDOResource* currRes = static_cast<RDOResource*>(runtime->getGroupFuncRes());
	m_value = currRes->getParam( m_parNumb );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetRelevantResParam
// ----------------------------------------------------------------------------
RDOValue& RDOCalcGetRelevantResParam::doCalc( RDORuntime* runtime )
{
	m_value = runtime->getResParamVal( runtime->getCurrentActivity()->getResByRelRes(m_relNumb), m_parNumb );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOSetRelParamCalc
// ----------------------------------------------------------------------------
RDOValue& RDOSetRelParamCalc::doCalc( RDORuntime* runtime )
{
	runtime->setResParamVal( runtime->getCurrentActivity()->getResByRelRes(m_relNumb), m_parNumb, m_calc->calcValue( runtime ) );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOSetRelParamDiapCalc
// ----------------------------------------------------------------------------
RDOValue& RDOSetRelParamDiapCalc::doCalc( RDORuntime* runtime )
{
	m_value = m_calc->calcValue( runtime );
	if ( m_value < m_min_value || m_value > m_max_value ) {
		if ( m_value.typeID() == RDOType::t_int && m_min_value.typeID() == RDOType::t_int && m_max_value.typeID() == RDOType::t_int ) {
			runtime->error( rdo::format("Значение выходит за допустимый диапазон [%d..%d]: %d", m_min_value.getInt(), m_max_value.getInt(), m_value.getInt()), this );
		} else {
			runtime->error( rdo::format("Значение выходит за допустимый диапазон [%f..%f]: %f", m_min_value.getDouble(), m_max_value.getDouble(), m_value.getDouble()), this );
		}
	}
	runtime->setResParamVal( runtime->getCurrentActivity()->getResByRelRes(m_relNumb), m_parNumb, m_value );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOSetResourceParamCalc
// ----------------------------------------------------------------------------
RDOValue& RDOSetResourceParamCalc::doCalc( RDORuntime* runtime )
{
	runtime->setResParamVal( m_resNumb, m_parNumb, m_calc->calcValue( runtime ) );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcEraseRes
// ----------------------------------------------------------------------------
RDOValue& RDOCalcEraseRes::doCalc( RDORuntime* runtime )
{
	runtime->onEraseRes( runtime->getCurrentActivity()->getResByRelRes(m_rel_res_id), this );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOSetPatternParamCalc
// ----------------------------------------------------------------------------
RDOValue& RDOSetPatternParamCalc::doCalc( RDORuntime* runtime )
{
	runtime->setPatternParameter( m_parNumb, m_val );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcPatParam
// ----------------------------------------------------------------------------
RDOValue& RDOCalcPatParam::doCalc( RDORuntime* runtime )
{
	m_value = runtime->getPatternParameter(m_numberOfParam);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTimeNow
// ----------------------------------------------------------------------------
RDOValue& RDOCalcGetTimeNow::doCalc( RDORuntime* runtime )
{
	m_value = runtime->getTimeNow();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetSeconds
// ----------------------------------------------------------------------------
RDOValue& RDOCalcGetSeconds::doCalc( RDORuntime* runtime )
{
	m_value = runtime->getSeconds();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOFunAlgorithmicCalc
// ----------------------------------------------------------------------------
RDOValue& RDOFunAlgorithmicCalc::doCalc( RDORuntime* runtime )
{
	int size = m_conditions.size();
	for ( int i = 0; i < size; i++ ) {
		if ( m_conditions[i]->calcValue( runtime ).getAsBool() ) {
			return m_actions[i]->calcValue( runtime );
		}
	}
	// До сюда дело дойти не должно, т.к. последний conditions должен быть значением по-умолчанию
	runtime->error( "Внутренная ошибка, RDOFunAlgorithmicCalc", this );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOFunAlgorithmicDiapCalc
// ----------------------------------------------------------------------------
RDOValue& RDOFunAlgorithmicDiapCalc::doCalc( RDORuntime* runtime )
{
	int size = m_conditions.size();
	for ( int i = 0; i < size; i++ ) {
		if ( m_conditions[i]->calcValue( runtime ).getAsBool() ) {
			m_value = m_actions[i]->calcValue( runtime );
			if ( m_value < m_min_value || m_value > m_max_value ) {
				if ( m_value.typeID() == RDOType::t_int && m_min_value.typeID() == RDOType::t_int && m_max_value.typeID() == RDOType::t_int ) {
					runtime->error( rdo::format("Значение выходит за допустимый диапазон [%d..%d]: %d", m_min_value.getInt(), m_max_value.getInt(), m_value.getInt()), m_actions[i] );
				} else {
					runtime->error( rdo::format("Значение выходит за допустимый диапазон [%f..%f]: %f", m_min_value.getDouble(), m_max_value.getDouble(), m_value.getDouble()), m_actions[i] );
				}
			}
			return m_value;
		}
	}
	// До сюда дело дойти не должно, т.к. последний conditions должен быть значением по-умолчанию
	runtime->error( "Внутренная ошибка, RDOFunAlgorithmicDiapCalc", this );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcGroup
// ----------------------------------------------------------------------------
RDOValue& RDOFunCalcExist::doCalc( RDORuntime* runtime )
{
	bool res = false;
	RDORuntime::ResCIterator end = runtime->res_end();
	for ( RDORuntime::ResCIterator it = runtime->res_begin(); it != end && !res; it++ ) {
		if ( *it == NULL ) continue;
		if ( !(*it)->checkType(m_nResType) ) continue;
		runtime->pushGroupFunc( *it );
		if ( m_condition->calcValue( runtime ).getAsBool() ) res = true;
		runtime->popGroupFunc();
	}
	m_value = res;
	return m_value;
}

RDOValue& RDOFunCalcNotExist::doCalc( RDORuntime* runtime )
{
	bool res = true;
	RDORuntime::ResCIterator end = runtime->res_end();
	for ( RDORuntime::ResCIterator it = runtime->res_begin(); it != end && res; it++ ) {
		if ( *it == NULL ) continue;
		if ( !(*it)->checkType(m_nResType) ) continue;
		runtime->pushGroupFunc( *it );
		if ( m_condition->calcValue( runtime ).getAsBool() ) res = false;
		runtime->popGroupFunc();
	}
	m_value = res;
	return m_value;
}

RDOValue& RDOFunCalcForAll::doCalc( RDORuntime* runtime )
{
	bool first_found = false;
	bool res = true;
	RDORuntime::ResCIterator end = runtime->res_end();
	for ( RDORuntime::ResCIterator it = runtime->res_begin(); it != end && res; it++ ) {
		if ( *it == NULL ) continue;
		if ( !(*it)->checkType(m_nResType) ) continue;
		runtime->pushGroupFunc( *it );
		if ( !m_condition->calcValue( runtime ).getAsBool() ) {
			res = false;
		} else if ( !first_found ) {
			first_found = true;
		}
		runtime->popGroupFunc();
	}
	m_value = first_found ? res : false;
	return m_value;
}

RDOValue& RDOFunCalcNotForAll::doCalc( RDORuntime* runtime )
{
	bool res = false;
	RDORuntime::ResCIterator end = runtime->res_end();
	for ( RDORuntime::ResCIterator it = runtime->res_begin(); it != end && !res; it++ ) {
		if ( *it == NULL ) continue;
		if ( !(*it)->checkType(m_nResType) ) continue;
		runtime->pushGroupFunc( *it );
		if( !m_condition->calcValue( runtime ).getAsBool() ) res = true;
		runtime->popGroupFunc();
	}
	m_value = res;
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcSelect
// ----------------------------------------------------------------------------
void RDOFunCalcSelect::prepare( RDORuntime* sim ) const
{
	res_list.clear();
	RDORuntime::ResCIterator end = sim->res_end();
	for ( RDORuntime::ResCIterator it = sim->res_begin(); it != end; it++ ) {
		if ( *it == NULL ) continue;
		if ( !(*it)->checkType(m_nResType) ) continue;
		sim->pushGroupFunc( *it );
		if ( m_condition->calcValue( sim ).getAsBool() ) {
			res_list.push_back( *it );
		}
		sim->popGroupFunc();
	}
}

RDOValue& RDOFunCalcSelect::doCalc( RDORuntime* runtime )
{
	prepare( runtime );
	return m_value;
}

RDOValue& RDOFunCalcSelectExist::doCalc( RDORuntime* runtime )
{
	m_select->prepare( runtime );
	bool res = false;
	std::list< RDOResource* >::iterator it  = m_select->res_list.begin();
	std::list< RDOResource* >::iterator end = m_select->res_list.end();
	while ( it != end && !res ) {
		runtime->pushGroupFunc( *it );
		if ( m_condition->calcValue( runtime ).getAsBool() ) res = true;
		runtime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

RDOValue& RDOFunCalcSelectNotExist::doCalc( RDORuntime* runtime )
{
	m_select->prepare( runtime );
	bool res = true;
	std::list< RDOResource* >::iterator it  = m_select->res_list.begin();
	std::list< RDOResource* >::iterator end = m_select->res_list.end();
	while ( it != end && res ) {
		runtime->pushGroupFunc( *it );
		if ( m_condition->calcValue( runtime ).getAsBool() ) res = false;
		runtime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

RDOValue& RDOFunCalcSelectForAll::doCalc( RDORuntime* runtime )
{
	m_select->prepare( runtime );
	if ( m_select->res_list.empty() ) {
		m_value = false;
		return m_value;
	}
	bool res = true;
	std::list< RDOResource* >::iterator it  = m_select->res_list.begin();
	std::list< RDOResource* >::iterator end = m_select->res_list.end();
	while ( it != end && res ) {
		runtime->pushGroupFunc( *it );
		if ( !m_condition->calcValue( runtime ).getAsBool() ) res = false;
		runtime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

RDOValue& RDOFunCalcSelectNotForAll::doCalc( RDORuntime* runtime )
{
	m_select->prepare( runtime );
	bool res = false;
	std::list< RDOResource* >::iterator it  = m_select->res_list.begin();
	std::list< RDOResource* >::iterator end = m_select->res_list.end();
	while ( it != end && !res ) {
		runtime->pushGroupFunc( *it );
		if ( !m_condition->calcValue( runtime ).getAsBool() ) res = true;
		runtime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

RDOValue& RDOFunCalcSelectSize::doCalc( RDORuntime* runtime )
{
	m_select->prepare( runtime );
	m_value = m_select->res_list.size();
	return m_value;
}

RDOValue& RDOFunCalcSelectEmpty::doCalc( RDORuntime* runtime )
{
	m_select->prepare( runtime );
	m_value = m_select->res_list.empty();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- Стандартные функции языка
// ----------------------------------------------------------------------------
#define DECLARE_RDO_STD_FUN_DOUBLE( CalcName, FunName ) \
RDOValue& RDOFunCalc##CalcName::doCalc( RDORuntime* runtime ) \
{ \
	m_value = FunName( runtime->getFuncArgument(0).getDouble() ); \
	return m_value; \
}

#define DECLARE_RDO_STD_FUN_DOUBLE_DOUBLE( CalcName, FunName ) \
RDOValue& RDOFunCalc##CalcName::doCalc( RDORuntime* runtime ) \
{ \
	m_value = FunName( runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getDouble() ); \
	return m_value; \
}

#define DECLARE_RDO_STD_FUN_DOUBLE_INT( CalcName, FunName ) \
RDOValue& RDOFunCalc##CalcName::doCalc( RDORuntime* runtime ) \
{ \
	m_value = FunName( runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getInt() ); \
	return m_value; \
}

#define DECLARE_RDO_STD_FUN_INT( CalcName, FunName ) \
RDOValue& RDOFunCalc##CalcName::doCalc( RDORuntime* runtime ) \
{ \
	m_value = FunName( runtime->getFuncArgument(0).getInt() ); \
	return m_value; \
}

#define DECLARE_RDO_STD_FUN_INT_INT( CalcName, FunName ) \
RDOValue& RDOFunCalc##CalcName::doCalc( RDORuntime* runtime ) \
{ \
	m_value = FunName( runtime->getFuncArgument(0).getInt(), runtime->getFuncArgument(1).getInt() ); \
	return m_value; \
}

double modf( double value )
{
	double tmp;
	return ::modf( value, &tmp );
}

double log2( double value )
{
	return log( value ) / log(2.0);
}

double logN( double value1, double value2 )
{
	return log( value1 ) / log( value2 );
}

int round( double value )
{
	return (int)floor( value + 0.5 );
}

DECLARE_RDO_STD_FUN_DOUBLE       ( Sin     , sin       );
DECLARE_RDO_STD_FUN_DOUBLE       ( Cos     , cos       );
DECLARE_RDO_STD_FUN_DOUBLE       ( Tan     , tan       );
DECLARE_RDO_STD_FUN_DOUBLE       ( Cotan   , 1.0 / tan );
DECLARE_RDO_STD_FUN_DOUBLE       ( ArcCos  , acos      );
DECLARE_RDO_STD_FUN_DOUBLE       ( ArcSin  , asin      );
DECLARE_RDO_STD_FUN_DOUBLE       ( ArcTan  , atan      );
DECLARE_RDO_STD_FUN_DOUBLE       ( Abs     , fabs      );
DECLARE_RDO_STD_FUN_DOUBLE       ( Sqrt    , sqrt      );
DECLARE_RDO_STD_FUN_DOUBLE       ( Round   , round     );
DECLARE_RDO_STD_FUN_DOUBLE       ( Exp     , exp       );
DECLARE_RDO_STD_FUN_DOUBLE       ( Floor   , floor     );
DECLARE_RDO_STD_FUN_DOUBLE       ( Frac    , modf      );
DECLARE_RDO_STD_FUN_DOUBLE       ( Ln      , log       );
DECLARE_RDO_STD_FUN_DOUBLE       ( Log10   , log10     );
DECLARE_RDO_STD_FUN_DOUBLE       ( Log2    , log2      );
DECLARE_RDO_STD_FUN_DOUBLE_DOUBLE( LogN    , logN      );
DECLARE_RDO_STD_FUN_DOUBLE_DOUBLE( Max     , max       );
DECLARE_RDO_STD_FUN_DOUBLE_DOUBLE( Min     , min       );
DECLARE_RDO_STD_FUN_DOUBLE_DOUBLE( Power   , pow       );
DECLARE_RDO_STD_FUN_INT          ( IAbs    , abs       );
DECLARE_RDO_STD_FUN_INT_INT      ( IMax    , max       );
DECLARE_RDO_STD_FUN_INT_INT      ( IMin    , min       );
DECLARE_RDO_STD_FUN_INT          ( Int     , int       );
DECLARE_RDO_STD_FUN_DOUBLE_INT   ( IntPower, pow       );

// ----------------------------------------------------------------------------
// ---------- RDOCalcFuncParam
// ----------------------------------------------------------------------------
RDOValue& RDOCalcFuncParam::doCalc( RDORuntime* runtime )
{
	m_value = runtime->getFuncArgument( m_param_number );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcBinary
// ----------------------------------------------------------------------------
#define DECLARE_BINARY_CALC( CalcName, CalcOpr ) \
RDOValue& RDOCalc##CalcName::doCalc( RDORuntime* runtime ) \
{ \
	runtime->inc_cnt_calc_logic(); \
	m_value = m_left->calcValue( runtime ) CalcOpr m_right->calcValue( runtime ); \
	return m_value; \
}

// ----------------------------------------------------------------------------
// ---------- Арифметические функции
// ----------------------------------------------------------------------------
DECLARE_BINARY_CALC( Plus , + );
DECLARE_BINARY_CALC( Minus, - );
DECLARE_BINARY_CALC( Mult , * );

RDOValue& RDOCalcDiv::doCalc( RDORuntime* runtime ) {
	runtime->inc_cnt_calc_arithm();
	RDOValue& rVal = m_right->calcValue( runtime );
	if ( rVal == 0 ) {
		runtime->error( "Деление на ноль", this );
//		runtime->error("Division by zero", this);
	}
	m_value = m_left->calcValue( runtime ) / rVal;
	return m_value;
}

RDOValue& RDOCalcPlusEnumSafe::doCalc( RDORuntime* runtime )
{
	runtime->inc_cnt_calc_arithm();
	m_value = m_left->calcValue( runtime ).getEnumAsInt() + m_right->calcValue( runtime ).getEnumAsInt();
	return m_value;
}

RDOValue& RDOCalcMultEnumSafe::doCalc( RDORuntime* runtime )
{
	runtime->inc_cnt_calc_arithm();
	m_value = m_left->calcValue( runtime ).getEnumAsInt() * m_right->calcValue( runtime ).getEnumAsInt();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- Логические функции
// ----------------------------------------------------------------------------
RDOValue& RDOCalcAnd::doCalc( RDORuntime* runtime )
{
	runtime->inc_cnt_calc_logic();
	if ( !m_left->calcValue( runtime ).getAsBool()  ) {
		return m_value_false;
	}
	if ( !m_right->calcValue( runtime ).getAsBool() ) {
		return m_value_false;
	}
	return m_value_true;
}

RDOValue& RDOCalcOr::doCalc( RDORuntime* runtime )
{
	runtime->inc_cnt_calc_logic();
	if ( m_left->calcValue( runtime ).getAsBool()  ) {
		return m_value_true;
	}
	if ( m_right->calcValue( runtime ).getAsBool() ) {
		return m_value_true;
	}
	return m_value_false;
}

RDOValue& RDOCalcNot::doCalc( RDORuntime* runtime )
{
	runtime->inc_cnt_calc_logic();
	m_value = !m_calc->calcValue( runtime ).getAsBool();
	return m_value;
}

DECLARE_BINARY_CALC( IsEqual   , == );
DECLARE_BINARY_CALC( IsNotEqual, != );
DECLARE_BINARY_CALC( IsLess    , <  );
DECLARE_BINARY_CALC( IsGreater , >  );
DECLARE_BINARY_CALC( IsLEQ     , <= );
DECLARE_BINARY_CALC( IsGEQ     , >= );

// ----------------------------------------------------------------------------
// ---------- Унарные операции
// ----------------------------------------------------------------------------
RDOValue& RDOCalcCheckDiap::doCalc( RDORuntime* runtime )
{
	m_value = m_oper->calcValue( runtime );
	if ( m_value < m_min_value || m_value > m_max_value ) {
		if ( m_value.typeID() == RDOType::t_int && m_min_value.typeID() == RDOType::t_int && m_max_value.typeID() == RDOType::t_int ) {
			runtime->error( rdo::format("Значение выходит за допустимый диапазон [%d..%d]: %d", m_min_value.getInt(), m_max_value.getInt(), m_value.getInt()), this );
		} else {
			runtime->error( rdo::format("Значение выходит за допустимый диапазон [%f..%f]: %f", m_min_value.getDouble(), m_max_value.getDouble(), m_value.getDouble()), this );
		}
	}
	return m_value; 
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetConst / RDOCalcSetConst
// ----------------------------------------------------------------------------
RDOValue& RDOCalcGetConst::doCalc( RDORuntime* runtime )
{
	m_value = runtime->getConstValue( m_number );
	return m_value;
}

RDOValue& RDOCalcSetConst::doCalc( RDORuntime* runtime )
{
	runtime->setConstValue( m_number, m_calc->calcValue( runtime ) );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcFunctionCall
// ----------------------------------------------------------------------------
RDOValue& RDOCalcFunctionCall::doCalc( RDORuntime* runtime )
{
	runtime->pushFuncTop();
	int size = m_parameters.size();
	for ( int i = 0; i < size; i++ ) {
		RDOValue arg = m_parameters[i]->calcValue( runtime );
		runtime->pushFuncArgument( arg );
	}
	runtime->resetFuncTop( m_parameters.size() );
	m_value = m_function->calcValue( runtime );
	size = m_parameters.size();
	for ( int i = 0; i < size; i++ ) {
		runtime->popFuncArgument();
	}
	runtime->popFuncTop();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- Выбор ресурсов
// ----------------------------------------------------------------------------
RDOValue& RDOSelectResourceNonExistCalc::doCalc( RDORuntime* runtime )
{
	runtime->getCurrentActivity()->setRelRes( rel_res_id, -1 );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- Последовательности
// ----------------------------------------------------------------------------
RDOValue& RDOCalcSeqInit::doCalc( RDORuntime* runtime )	
{
	m_gen->setSeed( m_base );
	return m_value;
}

RDOCalcSeqInit::~RDOCalcSeqInit() 
{ 
//	delete m_gen;
}

RDOValue RDOCalcSeqNextUniform::getNextValue( RDORuntime* runtime )
{
	return m_gen->next( runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getDouble() );
}

RDOValue RDOCalcSeqNextNormal::getNextValue( RDORuntime* runtime )
{
	return m_gen->next( runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getDouble() );
}

RDOValue RDOCalcSeqNextExponential::getNextValue( RDORuntime* runtime )
{
	return m_gen->next( runtime->getFuncArgument(0).getDouble() );
}

RDOValue RDOCalcSeqNextByHist::getNextValue( RDORuntime* runtime )
{
	return m_gen->next();
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateNumberedResource
// ----------------------------------------------------------------------------
RDOCalcCreateNumberedResource::RDOCalcCreateNumberedResource( RDORuntimeParent* parent, int _type, bool _traceFlag, const std::vector< RDOValue >& _paramsCalcs, int _number, bool _isPermanent ):
	RDOCalc( parent ),
	type( _type ),
	traceFlag( _traceFlag ),
	number( _number ),
	isPermanent( _isPermanent )
{
	paramsCalcs.insert( paramsCalcs.begin(), _paramsCalcs.begin(), _paramsCalcs.end() );
}

RDOValue& RDOCalcCreateNumberedResource::doCalc( RDORuntime* runtime )
{
	RDOResource* res = runtime->createNewResource( type, this );
	if ( !isPermanent ) {
		res->makeTemporary( true );
	}
	res->appendParams( paramsCalcs.begin(), paramsCalcs.end() );
	return m_value; // just to return something
}

RDOResource* RDOCalcCreateNumberedResource::createResource( RDORuntime* runtime ) const
{
	return new RDOResource( runtime, number, type, traceFlag );
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateProcessResource
// ----------------------------------------------------------------------------
RDOCalcCreateProcessResource::RDOCalcCreateProcessResource( RDORuntimeParent* parent, int _type, bool _traceFlag, const std::vector< RDOValue >& _paramsCalcs, int _number, bool _isPermanent ):
	RDOCalcCreateNumberedResource( parent, _type, _traceFlag, _paramsCalcs, _number, _isPermanent )
{
}

RDOResource* RDOCalcCreateProcessResource::createResource( RDORuntime* runtime ) const
{
	return new RDOPROCResource( runtime, number, type, traceFlag );
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateEmptyResource
// ----------------------------------------------------------------------------
RDOCalcCreateEmptyResource::RDOCalcCreateEmptyResource( RDORuntimeParent* parent, int _type, bool _traceFlag, const std::vector< RDOValue >& _params_default, int _rel_res_id ):
	RDOCalc( parent ),
	type( _type ),
	traceFlag( _traceFlag ),
	rel_res_id( _rel_res_id )
{
	params_default.insert( params_default.begin(), _params_default.begin(), _params_default.end() );
}

RDOValue& RDOCalcCreateEmptyResource::doCalc( RDORuntime* runtime )
{
	RDOResource* res = runtime->createNewResource( type, traceFlag );
	runtime->getCurrentActivity()->setRelRes( rel_res_id, res->getTraceID() );
	res->appendParams( params_default.begin(), params_default.end() );
	return m_value; // just to return something
}

// ----------------------------------------------------------------------------
// ---------- Выбор ресурсов
// ----------------------------------------------------------------------------
RDOSelectResourceCalc::RDOSelectResourceCalc( RDORuntimeParent* parent, int _rel_res_id, RDOCalc* _choice_calc, RDOCalc* _order_calc, Type _order_type ):
	RDOCalc( parent ),
	rel_res_id( _rel_res_id ),
	choice_calc( _choice_calc ),
	order_calc( _order_calc ),
	order_type( _order_type )
{
}

RDOValue& RDOSelectResourceDirectCalc::doCalc( RDORuntime* runtime )
{
	runtime->getCurrentActivity()->setRelRes( rel_res_id, res_id );
	if ( choice_calc && !choice_calc->calcValue( runtime ).getAsBool() ) {
		runtime->getCurrentActivity()->setRelRes( rel_res_id, -1 );
		m_value = 0;
		return m_value;
	}
	m_value = 1;
	return m_value;
}

RDOValue& RDOSelectResourceByTypeCalc::doCalc( RDORuntime* runtime )
{
	RDOValue maxVal   = -DBL_MAX;
	RDOValue minVal   = DBL_MAX;
	int res_minmax_id = -1;
	RDORuntime::ResCIterator end = runtime->res_end();
	for ( RDORuntime::ResCIterator it = runtime->res_begin(); it != end; it++ ) {

		if ( *it && (*it)->checkType(resType) ) {

			int res_id = (*it)->getTraceID();

			switch ( order_type ) {
				case order_empty:
				case order_first: {
					runtime->getCurrentActivity()->setRelRes( rel_res_id, res_id );
					if ( choice_calc && !choice_calc->calcValue( runtime ).getAsBool() ) {
						runtime->getCurrentActivity()->setRelRes( rel_res_id, -1 );
						continue;
					}
					m_value = 1;
					return m_value;
				}
				case order_with_min: {
					runtime->getCurrentActivity()->setRelRes( rel_res_id, res_id );
					if ( choice_calc && !choice_calc->calcValue( runtime ).getAsBool() ) {
						runtime->getCurrentActivity()->setRelRes( rel_res_id, -1 );
						continue;
					}
					RDOValue tmp = order_calc->calcValue( runtime );
					if ( tmp < minVal ) {
						minVal        = tmp;
						res_minmax_id = res_id;
					}
					break;
				}
				case order_with_max: {
					runtime->getCurrentActivity()->setRelRes( rel_res_id, res_id );
					if ( choice_calc && !choice_calc->calcValue( runtime ).getAsBool() ) {
						runtime->getCurrentActivity()->setRelRes( rel_res_id, -1 );
						continue;
					}
					RDOValue tmp = order_calc->calcValue( runtime );
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
		runtime->getCurrentActivity()->setRelRes( rel_res_id, res_minmax_id );
		m_value = 1;
		return m_value;
	}
	m_value = 0;
	return m_value;
}

void RDOSelectResourceCommonCalc::getBest( std::vector< std::vector< int > >& allNumbs, unsigned int level, std::vector< int >& res, RDOValue& bestVal, RDORuntime* sim, bool& hasBest ) const
{
	if ( level >= allNumbs.size() ) {
		for ( unsigned int i = 0; i < resSelectors.size(); i++ ) {
			if( !resSelectors.at(i)->callChoice(sim) ) {
				return; // state not valid
			}
		}
		RDOValue newVal = choice_calc->calcValue( sim );
		if ( !hasBest || (useCommonWithMax && (newVal > bestVal)) ||
			(!useCommonWithMax && (newVal < bestVal))) // found better value
		{
			for ( unsigned int i = 0; i < resSelectors.size(); i++ ) {
				res.at(i) = sim->getCurrentActivity()->getResByRelRes(i);
			}
			bestVal = newVal;
			hasBest = true;
		}
		return;
	}
	std::vector< int >& ourLevel = allNumbs.at(level);
	for ( unsigned int i = 0; i < ourLevel.size(); i++ ) {
		sim->getCurrentActivity()->setRelRes( level, ourLevel.at(i) );
		getBest( allNumbs, level+1, res, bestVal, sim, hasBest );
	}
}

bool RDOSelectResourceCommonCalc::getFirst( std::vector< std::vector< int > >& allNumbs, unsigned int level, RDORuntime* sim ) const
{
	if ( level >= allNumbs.size() ) {
		for ( unsigned int i = 0; i < resSelectors.size(); i++ ) {
			if( !resSelectors.at(i)->callChoice(sim) ) {
				return false;
			}
		}
		return true;
	}
	std::vector< int >& ourLevel = allNumbs.at(level);
	for ( unsigned int i = 0; i < ourLevel.size(); i++ ) {
		sim->getCurrentActivity()->setRelRes( level, ourLevel.at(i) );
		if ( getFirst( allNumbs, level+1, sim ) ) return true;
	}
	return false;
}

//bool RDOSelectResourceCommonCalc::getFirst( std::vector< std::vector< int > >& allNumbs, int level, RDORuntime* sim ) const
//{
//	if ( level <= 0 ) {
//		for ( int i = 0; i < resSelectors.size(); i++ ) {
//			if( !resSelectors.at(i)->callChoice(sim) ) {
//				return false;
//			}
//		}
//		return true;
//	} else {
//		level--;
//		std::vector< int >& ourLevel = allNumbs.at(level);
//		for ( int i = 0; i < ourLevel.size(); i++ ) {
//			sim->setRelRes( level, ourLevel.at(i) );
//			if ( getFirst( allNumbs, level, sim ) ) return true;
//		}
//	}
//	return false;
//}

RDOValue& RDOSelectResourceCommonCalc::doCalc( RDORuntime* runtime )
{
	std::vector< std::vector<int> > allNumbs;
	std::vector< int > res;
	for ( unsigned int i = 0; i < resSelectors.size(); i++ ) {
		allNumbs.push_back( resSelectors.at(i)->getPossibleNumbers(runtime) );
		res.push_back( runtime->getCurrentActivity()->getResByRelRes(i) );
	}
	if ( !choice_calc ) {
		// first
//		if ( getFirst( allNumbs, allNumbs.size(), runtime ) ) {
//			return true;
//		}
		if ( getFirst( allNumbs, 0, runtime ) ) {
			m_value = 1;
			return m_value;
		}
	} else {
		// with_min / with_max
		RDOValue bestVal = 0;
		bool found = false;
		getBest( allNumbs, 0, res, bestVal, runtime, found );
		if ( found ) {
			for ( unsigned int i = 0; i < res.size(); i++ ) {
				runtime->getCurrentActivity()->setRelRes( i, res.at(i) );
			}
			m_value = 1;
			return m_value;
		}
	}
	m_value = 0;
	return m_value;
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
	RDORuntime::ResCIterator end = sim->res_end();
	for ( RDORuntime::ResCIterator it = sim->res_begin(); it != end; it++ )
	{
		if ( *it == NULL )
		{
			continue;
		}
		if ( !(*it)->checkType(resType) )
		{
			continue;
		}
		res.push_back((*it)->getTraceID());
	}

	return res;
}

bool RDOSelectResourceDirectCommonCalc::callChoice(RDORuntime *sim) const
{
	if ( choice_calc && !choice_calc->calcValue( sim ).getAsBool() ) return 0;
	return 1;
}

bool RDOSelectResourceByTypeCommonCalc::callChoice(RDORuntime *sim) const
{
	if ( choice_calc && !choice_calc->calcValue( sim ).getAsBool() ) return 0;
	return 1;
}

} // namespace rdoParse 
