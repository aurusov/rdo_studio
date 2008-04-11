#include "pch.h"
#include "rdocalc.h"
#include "rdoprocess.h"
#include "rdo_runtime.h"
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
RDOCalc::RDOCalc( RDORuntimeParent* parent ):
	RDORuntimeObject( parent )
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
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetResParam (Параметры постоянного ресурса)
// ----------------------------------------------------------------------------
RDOValue& RDOCalcGetResParam::calcValue( RDORuntime* runtime )
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

RDOValue& RDOCalcGetTempResParamFRM::calcValue( RDORuntime* runtime )
{
	if ( m_resID >= 0 ) {
		m_value = runtime->getResParamVal( m_resID, m_paramID );
	} else if ( m_resID == -1 ) {
		RDOEnum* _enum = new RDOEnum( runtime );
		_enum->add( "Удален" );
		m_value = RDOValue( *_enum );
		m_resID = -2;
	}
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetUnknowResParam (Параметры несуществующего ресурса)
// ----------------------------------------------------------------------------
RDOValue& RDOCalcGetUnknowResParam::calcValue( RDORuntime* runtime )
{
	runtime->error( rdo::format("Попытка использовать несуществующий ресурс: %s.%s", m_resName.c_str(), m_parName.c_str()), this );
	return m_value;
}


// ----------------------------------------------------------------------------
// ---------- RDOCalcGetGroupResParam
// ----------------------------------------------------------------------------
RDOValue& RDOCalcGetGroupResParam::calcValue( RDORuntime* runtime )
{
	RDOResource* currRes = static_cast<RDOResource*>(runtime->getGroupFuncRes());
	m_value = currRes->getParam( m_parNumb );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetRelevantResParam
// ----------------------------------------------------------------------------
RDOValue& RDOCalcGetRelevantResParam::calcValue( RDORuntime* runtime )
{
	m_value = runtime->getResParamVal( runtime->getResByRelRes(m_relNumb), m_parNumb );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOSetRelParamCalc
// ----------------------------------------------------------------------------
RDOValue& RDOSetRelParamCalc::calcValue( RDORuntime* runtime )
{
	runtime->setResParamVal( runtime->getResByRelRes(m_relNumb), m_parNumb, m_calc->calcValueBase( runtime ) );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOSetRelParamDiapCalc
// ----------------------------------------------------------------------------
RDOValue& RDOSetRelParamDiapCalc::calcValue( RDORuntime* runtime )
{
	m_value = m_calc->calcValueBase( runtime );
	if ( m_value < m_min_value || m_value > m_max_value ) {
		if ( m_value.getType() == RDOValue::rvt_int && m_min_value.getType() == RDOValue::rvt_int && m_max_value.getType() == RDOValue::rvt_int ) {
			runtime->error( rdo::format("Значение выходит за допустимый диапазон [%d..%d]: %d", m_min_value.getInt(), m_max_value.getInt(), m_value.getInt()), this );
		} else {
			runtime->error( rdo::format("Значение выходит за допустимый диапазон [%f..%f]: %f", m_min_value.getDouble(), m_max_value.getDouble(), m_value.getDouble()), this );
		}
	}
	runtime->setResParamVal( runtime->getResByRelRes(m_relNumb), m_parNumb, m_value );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOSetResourceParamCalc
// ----------------------------------------------------------------------------
RDOValue& RDOSetResourceParamCalc::calcValue( RDORuntime* runtime )
{
	runtime->setResParamVal( m_resNumb, m_parNumb, m_calc->calcValueBase( runtime ) );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcEraseRes
// ----------------------------------------------------------------------------
RDOValue& RDOCalcEraseRes::calcValue( RDORuntime* runtime )
{
	runtime->onEraseRes( runtime->getResByRelRes(m_rel_res_id), this );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOSetPatternParamCalc
// ----------------------------------------------------------------------------
RDOValue& RDOSetPatternParamCalc::calcValue( RDORuntime* runtime )
{
	runtime->setPatternParameter( m_parNumb, m_val );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcPatParam
// ----------------------------------------------------------------------------
RDOValue& RDOCalcPatParam::calcValue( RDORuntime* runtime )
{
	m_value = runtime->getPatternParameter(m_numberOfParam);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTimeNow
// ----------------------------------------------------------------------------
RDOValue& RDOCalcGetTimeNow::calcValue( RDORuntime* runtime )
{
	m_value = runtime->getTimeNow();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetSeconds
// ----------------------------------------------------------------------------
RDOValue& RDOCalcGetSeconds::calcValue( RDORuntime* runtime )
{
	m_value = runtime->getSeconds();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOFunAlgorithmicCalc
// ----------------------------------------------------------------------------
RDOValue& RDOFunAlgorithmicCalc::calcValue( RDORuntime* runtime )
{
	int size = m_conditions.size();
	for ( int i = 0; i < size; i++ ) {
		if ( m_conditions[i]->calcValueBase( runtime ).getBool() ) {
			return m_actions[i]->calcValueBase( runtime );
		}
	}
	// До сюда дело дойти не должно, т.к. последний conditions должен быть значением по-умолчанию
	runtime->error( "Внутренная ошибка, RDOFunAlgorithmicCalc", this );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOFunAlgorithmicDiapCalc
// ----------------------------------------------------------------------------
RDOValue& RDOFunAlgorithmicDiapCalc::calcValue( RDORuntime* runtime )
{
	int size = m_conditions.size();
	for ( int i = 0; i < size; i++ ) {
		if ( m_conditions[i]->calcValueBase( runtime ).getBool() ) {
			m_value = m_actions[i]->calcValueBase( runtime );
			if ( m_value < m_min_value || m_value > m_max_value ) {
				if ( m_value.getType() == RDOValue::rvt_int && m_min_value.getType() == RDOValue::rvt_int && m_max_value.getType() == RDOValue::rvt_int ) {
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
RDOValue& RDOFunCalcExist::calcValue( RDORuntime* runtime )
{
	bool res = false;
	std::list< RDOResource* >::iterator end = runtime->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = runtime->allResourcesByTime.begin(); it != end && !res; it++ ) {
		if ( *it == NULL ) continue;
		if ( !(*it)->checkType(m_nResType) ) continue;
		runtime->pushGroupFunc( *it );
		if ( m_condition->calcValueBase( runtime ).getBool() ) res = true;
		runtime->popGroupFunc();
	}
	m_value = res;
	return m_value;
}

RDOValue& RDOFunCalcNotExist::calcValue( RDORuntime* runtime )
{
	bool res = true;
	std::list< RDOResource* >::iterator end = runtime->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = runtime->allResourcesByTime.begin(); it != end && res; it++ ) {
		if ( *it == NULL ) continue;
		if ( !(*it)->checkType(m_nResType) ) continue;
		runtime->pushGroupFunc( *it );
		if ( m_condition->calcValueBase( runtime ).getBool() ) res = false;
		runtime->popGroupFunc();
	}
	m_value = res;
	return m_value;
}

RDOValue& RDOFunCalcForAll::calcValue( RDORuntime* runtime )
{
	bool first_found = false;
	bool res = true;
	std::list< RDOResource* >::iterator end = runtime->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = runtime->allResourcesByTime.begin(); it != end && res; it++ ) {
		if ( *it == NULL ) continue;
		if ( !(*it)->checkType(m_nResType) ) continue;
		runtime->pushGroupFunc( *it );
		if ( !m_condition->calcValueBase( runtime ).getBool() ) {
			res = false;
		} else if ( !first_found ) {
			first_found = true;
		}
		runtime->popGroupFunc();
	}
	m_value = first_found ? res : false;
	return m_value;
}

RDOValue& RDOFunCalcNotForAll::calcValue( RDORuntime* runtime )
{
	bool res = false;
	std::list< RDOResource* >::iterator end = runtime->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = runtime->allResourcesByTime.begin(); it != end && !res; it++ ) {
		if ( *it == NULL ) continue;
		if ( !(*it)->checkType(m_nResType) ) continue;
		runtime->pushGroupFunc( *it );
		if( !m_condition->calcValueBase( runtime ).getBool() ) res = true;
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
	std::list< RDOResource* >::iterator end = sim->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = sim->allResourcesByTime.begin(); it != end; it++ ) {
		if ( *it == NULL ) continue;
		if ( !(*it)->checkType(m_nResType) ) continue;
		sim->pushGroupFunc( *it );
		if ( m_condition->calcValueBase( sim ).getBool() ) {
			res_list.push_back( *it );
		}
		sim->popGroupFunc();
	}
}

RDOValue& RDOFunCalcSelect::calcValue( RDORuntime* runtime )
{
	prepare( runtime );
	return m_value;
}

RDOValue& RDOFunCalcSelectExist::calcValue( RDORuntime* runtime )
{
	m_select->prepare( runtime );
	bool res = false;
	std::list< RDOResource* >::iterator it  = m_select->res_list.begin();
	std::list< RDOResource* >::iterator end = m_select->res_list.end();
	while ( it != end && !res ) {
		runtime->pushGroupFunc( *it );
		if ( m_condition->calcValueBase( runtime ).getBool() ) res = true;
		runtime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

RDOValue& RDOFunCalcSelectNotExist::calcValue( RDORuntime* runtime )
{
	m_select->prepare( runtime );
	bool res = true;
	std::list< RDOResource* >::iterator it  = m_select->res_list.begin();
	std::list< RDOResource* >::iterator end = m_select->res_list.end();
	while ( it != end && res ) {
		runtime->pushGroupFunc( *it );
		if ( m_condition->calcValueBase( runtime ).getBool() ) res = false;
		runtime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

RDOValue& RDOFunCalcSelectForAll::calcValue( RDORuntime* runtime )
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
		if ( !m_condition->calcValueBase( runtime ).getBool() ) res = false;
		runtime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

RDOValue& RDOFunCalcSelectNotForAll::calcValue( RDORuntime* runtime )
{
	m_select->prepare( runtime );
	bool res = false;
	std::list< RDOResource* >::iterator it  = m_select->res_list.begin();
	std::list< RDOResource* >::iterator end = m_select->res_list.end();
	while ( it != end && !res ) {
		runtime->pushGroupFunc( *it );
		if ( !m_condition->calcValueBase( runtime ).getBool() ) res = true;
		runtime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

RDOValue& RDOFunCalcSelectSize::calcValue( RDORuntime* runtime )
{
	m_select->prepare( runtime );
	m_value = m_select->res_list.size();
	return m_value;
}

RDOValue& RDOFunCalcSelectEmpty::calcValue( RDORuntime* runtime )
{
	m_select->prepare( runtime );
	m_value = m_select->res_list.empty();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- Стандартные функции языка
// ----------------------------------------------------------------------------
#define DECLARE_RDO_STD_FUN_DOUBLE( CalcName, FunName ) \
RDOValue& RDOFunCalc##CalcName::calcValue( RDORuntime* runtime ) \
{ \
	m_value = FunName( runtime->getFuncArgument(0).getDouble() ); \
	return m_value; \
}

#define DECLARE_RDO_STD_FUN_DOUBLE_DOUBLE( CalcName, FunName ) \
RDOValue& RDOFunCalc##CalcName::calcValue( RDORuntime* runtime ) \
{ \
	m_value = FunName( runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getDouble() ); \
	return m_value; \
}

#define DECLARE_RDO_STD_FUN_DOUBLE_INT( CalcName, FunName ) \
RDOValue& RDOFunCalc##CalcName::calcValue( RDORuntime* runtime ) \
{ \
	m_value = FunName( runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getInt() ); \
	return m_value; \
}

#define DECLARE_RDO_STD_FUN_INT( CalcName, FunName ) \
RDOValue& RDOFunCalc##CalcName::calcValue( RDORuntime* runtime ) \
{ \
	m_value = FunName( runtime->getFuncArgument(0).getInt() ); \
	return m_value; \
}

#define DECLARE_RDO_STD_FUN_INT_INT( CalcName, FunName ) \
RDOValue& RDOFunCalc##CalcName::calcValue( RDORuntime* runtime ) \
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
	return log( value ) / log(2);
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
DECLARE_RDO_STD_FUN_DOUBLE_DOUBLE( Max     , std::_MAX );
DECLARE_RDO_STD_FUN_DOUBLE_DOUBLE( Min     , std::_MIN );
DECLARE_RDO_STD_FUN_DOUBLE_DOUBLE( Power   , pow       );
DECLARE_RDO_STD_FUN_INT          ( IAbs    , abs       );
DECLARE_RDO_STD_FUN_INT_INT      ( IMax    , std::_MAX );
DECLARE_RDO_STD_FUN_INT_INT      ( IMin    , std::_MIN );
DECLARE_RDO_STD_FUN_INT          ( Int     , int       );
DECLARE_RDO_STD_FUN_DOUBLE_INT   ( IntPower, pow       );

// ----------------------------------------------------------------------------
// ---------- RDOCalcFuncParam
// ----------------------------------------------------------------------------
RDOValue& RDOCalcFuncParam::calcValue( RDORuntime* runtime )
{
	m_value = runtime->getFuncArgument( m_param_number );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcBinary
// ----------------------------------------------------------------------------
#define DECLARE_BINARY_CALC( CalcName, CalcOpr ) \
RDOValue& RDOCalc##CalcName::calcValue( RDORuntime* runtime ) \
{ \
	runtime->inc_cnt_calc_logic(); \
	m_value = m_left->calcValueBase( runtime ) CalcOpr m_right->calcValueBase( runtime ); \
	return m_value; \
}

// ----------------------------------------------------------------------------
// ---------- Арифметические функции
// ----------------------------------------------------------------------------
DECLARE_BINARY_CALC( Plus , + );
DECLARE_BINARY_CALC( Minus, - );
DECLARE_BINARY_CALC( Mult , * );

RDOValue& RDOCalcDiv::calcValue( RDORuntime* runtime ) {
	runtime->inc_cnt_calc_arithm();
	RDOValue& rVal = m_right->calcValueBase( runtime );
	if ( rVal == 0 ) {
		runtime->error( "Деление на ноль", this );
//		runtime->error("Division by zero", this);
	}
	m_value = m_left->calcValueBase( runtime ) / rVal;
	return m_value;
}

RDOValue& RDOCalcPlusEnumSafe::calcValue( RDORuntime* runtime )
{
	runtime->inc_cnt_calc_arithm();
	m_value = m_left->calcValueBase( runtime ).getEnumAsInt() + m_right->calcValueBase( runtime ).getEnumAsInt();
	return m_value;
}

RDOValue& RDOCalcMultEnumSafe::calcValue( RDORuntime* runtime )
{
	runtime->inc_cnt_calc_arithm();
	m_value = m_left->calcValueBase( runtime ).getEnumAsInt() * m_right->calcValueBase( runtime ).getEnumAsInt();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- Логические функции
// ----------------------------------------------------------------------------
RDOValue& RDOCalcAnd::calcValue( RDORuntime* runtime )
{
	runtime->inc_cnt_calc_logic();
	if ( !m_left->calcValueBase( runtime ).getBool()  ) {
		return m_value_false;
	}
	if ( !m_right->calcValueBase( runtime ).getBool() ) {
		return m_value_false;
	}
	return m_value_true;
}

RDOValue& RDOCalcOr::calcValue( RDORuntime* runtime )
{
	runtime->inc_cnt_calc_logic();
	if ( m_left->calcValueBase( runtime ).getBool()  ) {
		return m_value_true;
	}
	if ( m_right->calcValueBase( runtime ).getBool() ) {
		return m_value_true;
	}
	return m_value_false;
}

RDOValue& RDOCalcNot::calcValue( RDORuntime* runtime )
{
	runtime->inc_cnt_calc_logic();
	m_value = !m_calc->calcValueBase( runtime ).getBool();
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
RDOValue& RDOCalcCheckDiap::calcValue( RDORuntime* runtime )
{
	m_value = m_oper->calcValueBase( runtime );
	if ( m_value < m_min_value || m_value > m_max_value ) {
		if ( m_value.getType() == RDOValue::rvt_int && m_min_value.getType() == RDOValue::rvt_int && m_max_value.getType() == RDOValue::rvt_int ) {
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
RDOValue& RDOCalcGetConst::calcValue( RDORuntime* runtime )
{
	m_value = runtime->getConstValue( m_number );
	return m_value;
}

RDOValue& RDOCalcSetConst::calcValue( RDORuntime* runtime )
{
	runtime->setConstValue( m_number, m_calc->calcValueBase( runtime ) );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcFunctionCall
// ----------------------------------------------------------------------------
RDOValue& RDOCalcFunctionCall::calcValue( RDORuntime* runtime )
{
	runtime->pushFuncTop();
	int size = m_parameters.size();
	for ( int i = 0; i < size; i++ ) {
		RDOValue arg = m_parameters[i]->calcValueBase( runtime );
		runtime->pushFuncArgument( arg );
	}
	runtime->resetFuncTop( m_parameters.size() );
	m_value = m_function->calcValueBase( runtime );
	size = m_parameters.size();
	for ( i = 0; i < size; i++ ) {
		runtime->popFuncArgument();
	}
	runtime->popFuncTop();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- Выбор ресурсов
// ----------------------------------------------------------------------------
RDOValue& RDOSelectResourceNonExistCalc::calcValue( RDORuntime* runtime )
{
	runtime->setRelRes( rel_res_id, -1 );
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- Последовательности
// ----------------------------------------------------------------------------
RDOValue& RDOCalcSeqInit::calcValue( RDORuntime* runtime )	
{
	m_gen->setSeed( m_base );
	return m_value;
}

RDOCalcSeqInit::~RDOCalcSeqInit() 
{ 
	delete m_gen;
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

RDOValue& RDOCalcCreateNumberedResource::calcValue( RDORuntime* runtime )
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

RDOValue& RDOCalcCreateEmptyResource::calcValue( RDORuntime* runtime )
{
	RDOResource* res = runtime->createNewResource( type, traceFlag );
	runtime->setRelRes( rel_res_id, res->getTraceID() );
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

RDOValue& RDOSelectResourceDirectCalc::calcValue( RDORuntime* runtime )
{
	runtime->setRelRes( rel_res_id, res_id );
	if ( choice_calc && !choice_calc->calcValueBase( runtime ).getBool() ) {
		runtime->setRelRes( rel_res_id, -1 );
		m_value = 0;
		return m_value;
	}
	m_value = 1;
	return m_value;
}

RDOValue& RDOSelectResourceByTypeCalc::calcValue( RDORuntime* runtime )
{
	RDOValue maxVal   = -DBL_MAX;
	RDOValue minVal   = DBL_MAX;
	int res_minmax_id = -1;
	std::list< RDOResource* >::iterator end = runtime->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = runtime->allResourcesByTime.begin(); it != end; it++ ) {

		if ( *it && (*it)->checkType(resType) ) {

			int res_id = (*it)->getTraceID();

			switch ( order_type ) {
				case order_empty:
				case order_first: {
					runtime->setRelRes( rel_res_id, res_id );
					if ( choice_calc && !choice_calc->calcValueBase( runtime ).getBool() ) {
						runtime->setRelRes( rel_res_id, -1 );
						continue;
					}
					m_value = 1;
					return m_value;
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
		m_value = 1;
		return m_value;
	}
	m_value = 0;
	return m_value;
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
			for ( i = 0; i < res.size(); i++ ) {
				runtime->setRelRes( i, res.at(i) );
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
	std::list< RDOResource* >::iterator end = sim->allResourcesByTime.end();
	for ( std::list< RDOResource* >::iterator it = sim->allResourcesByTime.begin(); it != end; it++ ) {
		if(*it == NULL)
			continue;

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
	if ( choice_calc && !choice_calc->calcValueBase( sim ).getBool() ) return 0;
	return 1;
}

bool RDOSelectResourceByTypeCommonCalc::callChoice(RDORuntime *sim) const
{
	if ( choice_calc && !choice_calc->calcValueBase( sim ).getBool() ) return 0;
	return 1;
}

} // namespace rdoParse 
