#ifndef RDOCALC_H
#define RDOCALC_H

#include "rdoruntime_object.h"
#include "rdo_runtime.h"
#include "rdofunc.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOCalc
// ----------------------------------------------------------------------------
class RDOCalc: public RDORuntimeObject, public RDOSrcInfo
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) = 0;

protected:
	RDOValue value;

public:
	RDOCalc( RDORuntimeParent* _parent );
	virtual ~RDOCalc();

	RDOValue& calcValueBase( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcConst
// ----------------------------------------------------------------------------
class RDOCalcConst: public RDOCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		return value;
	}

public:
	RDOCalcConst( RDORuntimeParent* _parent, const RDOValue& val ):
		RDOCalc( _parent )
	{
		value = val;
	};
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetResParam (Параметры ресурса)
// ----------------------------------------------------------------------------
class RDOCalcGetResParam: public RDOCalc
{
private:
	int resNumb;
	int parNumb;

	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = runtime->getResParamVal( resNumb, parNumb );
		return value;
	}

public:
	RDOCalcGetResParam( RDORuntimeParent* _parent, int _resNumb, int _parNumb ):
		RDOCalc( _parent ),
		resNumb( _resNumb ),
		parNumb( _parNumb )
	{
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetGroupResParam
// ----------------------------------------------------------------------------
class RDOCalcGetGroupResParam: public RDOCalc
{
private:
	int parNumb;

	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		RDOResource* currRes = (RDOResource*)runtime->getGroupFuncRes();
		return currRes->params[parNumb];
	}

public:
	RDOCalcGetGroupResParam( RDORuntimeParent* _parent, int _parNumb ):
		RDOCalc( _parent ),
		parNumb( _parNumb )
	{
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetRelevantResParam
// ----------------------------------------------------------------------------
class RDOCalcGetRelevantResParam: public RDOCalc
{
private:
	int relNumb;
	int parNumb;

	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = runtime->getResParamVal( runtime->getResByRelRes(relNumb), parNumb );
		return value;
	}

public:
	RDOCalcGetRelevantResParam( RDORuntimeParent* _parent, int _relNumb, int _parNumb ):
		RDOCalc( _parent ),
		relNumb( _relNumb ),
		parNumb( _parNumb )
	{
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOSetRelParamCalc
// ----------------------------------------------------------------------------
class RDOSetRelParamCalc: public RDOCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->setResParamVal( runtime->getResByRelRes(relNumb), parNumb, calc->calcValueBase( runtime ) );
		return value;
	}

protected:
	int relNumb;
	int parNumb;
	RDOCalc* calc;

public:
	RDOSetRelParamCalc( RDORuntimeParent* _parent, int _relNumb, int _parNumb, RDOCalc* _calc ):
		RDOCalc( _parent ),
		relNumb( _relNumb ),
		parNumb( _parNumb ),
		calc( _calc )
	{
		value = 1;
		if ( calc ) setSrcInfo( calc->src_info() );
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOSetRelParamDiapCalc
// ----------------------------------------------------------------------------
class RDOSetRelParamDiapCalc: public RDOSetRelParamCalc
{
private:
	RDOValue min_value;
	RDOValue max_value;

	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = calc->calcValueBase( runtime );
		if ( value < min_value || value > max_value ) {
			if ( value.getType() == RDOValue::pt_int && min_value.getType() == RDOValue::pt_int && max_value.getType() == RDOValue::pt_int ) {
				runtime->error( rdo::format("Значение выходит за допустимый диапазон [%d..%d]: %d", min_value.getInt(), max_value.getInt(), value.getInt()), this );
			} else {
				runtime->error( rdo::format("Значение выходит за допустимый диапазон [%f..%f]: %f", min_value.getDouble(), max_value.getDouble(), value.getDouble()), this );
			}
		}
		runtime->setResParamVal( runtime->getResByRelRes(relNumb), parNumb, value );
		return value;
	}

public:
	RDOSetRelParamDiapCalc( RDORuntimeParent* _parent, int _relNumb, int _parNumb, RDOCalc* _calc, RDOValue _min_value, RDOValue _max_value ):
		RDOSetRelParamCalc( _parent, _relNumb, _parNumb, _calc ),
		min_value( _min_value ),
		max_value( _max_value )
	{
		value = 1;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOSetResourceParamCalc
// ----------------------------------------------------------------------------
class RDOSetResourceParamCalc: public RDOCalc
{
private:
	int resNumb;
	int parNumb;
	RDOCalc* calc;

	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->setResParamVal( resNumb, parNumb, calc->calcValueBase( runtime ) );
		return value;
	}

public:
	RDOSetResourceParamCalc( RDORuntimeParent* _parent, int _resNumb, int _parNumb, RDOCalc* _calc ):
		RDOCalc( _parent ),
		resNumb( _resNumb ),
		parNumb( _parNumb ),
		calc( _calc )
	{
		value = 1;
		if ( calc ) setSrcInfo( calc->src_info() );
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcEraseRes
// ----------------------------------------------------------------------------
class RDOCalcEraseRes: public RDOCalc
{
private:
	int          rel_res_id;
	std::string  rel_res_name;

	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->onEraseRes( runtime->getResByRelRes(rel_res_id), this );
		return value;
	}

public:
	RDOCalcEraseRes( RDORuntimeParent* _parent, int _rel_res_id, const std::string& _rel_res_name ):
		RDOCalc( _parent ),
		rel_res_id( _rel_res_id ),
		rel_res_name( _rel_res_name )
	{
		value = 1;
	}
	std::string getName() const { return rel_res_name; }
};

// ----------------------------------------------------------------------------
// ---------- RDOSetPatternParamCalc
// ----------------------------------------------------------------------------
class RDOSetPatternParamCalc: public RDOCalc
{
private:
	int parNumb;
	RDOValue val;

	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->setPatternParameter( parNumb, val );
		return value;
	}

public:
	RDOSetPatternParamCalc( RDORuntimeParent* _parent, int _parNumb, RDOValue _val ):
		RDOCalc( _parent ),
		parNumb( _parNumb ),
		val( _val )
	{
		value = 0;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcPatParam
// ----------------------------------------------------------------------------
class RDOCalcPatParam: public RDOCalc
{
private:
	int numberOfParam;

	virtual RDOValue& calcValue( RDORuntime* runtime ) { 
		value = runtime->getPatternParameter(numberOfParam);
		return value;
	}

public:
	RDOCalcPatParam( RDORuntimeParent* _parent, int _numberOfParam ):
		RDOCalc( _parent ),
		numberOfParam( _numberOfParam )
	{
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTimeNow
// ----------------------------------------------------------------------------
class RDOCalcGetTimeNow: public RDOCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = runtime->getTimeNow();
		return value;
	}

public:
	RDOCalcGetTimeNow( RDORuntimeParent* _parent ): RDOCalc( _parent ) {}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetSeconds
// ----------------------------------------------------------------------------
class RDOCalcGetSeconds: public RDOCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = runtime->getSeconds();
		return value;
	}

public:
	RDOCalcGetSeconds( RDORuntimeParent* _parent ): RDOCalc( _parent ) {}
};

// ----------------------------------------------------------------------------
// ---------- RDOFunCalc
// ----------------------------------------------------------------------------
class RDOFunCalc: public RDOCalc
{
protected:
	RDOFunCalc( RDORuntimeParent* _parent ):
		RDOCalc( _parent )
	{
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOFunListCalc
// ----------------------------------------------------------------------------
// Функция типа список
// ----------------------------------------------------------------------------
class RDOFuncTableCalc: public RDOFunCalc
{
private:
	std::vector< RDOCalcConst* > results;
	RDOCalc* argCalc;

	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		int index = argCalc->calcValueBase( runtime ).getInt();
		return results.at(index)->calcValueBase( runtime );
	}

public:
	RDOFuncTableCalc( RDORuntimeParent* _parent, RDOCalc* _argCalc ):
		RDOFunCalc( _parent ),
		argCalc( _argCalc )
	{
	}
	void addResultCalc( RDOCalcConst* res ) {
		results.push_back( res );
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOFunListCalc
// ----------------------------------------------------------------------------
// Функция типа список
// ----------------------------------------------------------------------------
class RDOFunListCalc: public RDOFunCalc
{
private:
	std::vector< RDOCalc* >      cases;
	std::vector< RDOCalcConst* > results;
	RDOCalcConst*                default_value;

	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		int size = cases.size();
		for ( int i = 0; i < size; i++ ) {
			RDOCalc* cas = cases[i];
			if ( cas->calcValueBase( runtime ).getBool() ) {
				return results[i]->calcValueBase( runtime );
			}
		}
		return default_value->calcValueBase( runtime );
	}

public:
	RDOFunListCalc( RDORuntimeParent* _parent, RDOCalcConst* _default_value ):
		RDOFunCalc( _parent ),
		default_value( _default_value )
	{
	}

	void addCase( RDOCalc* _case_calc, RDOCalcConst* _result_calc ) {
		cases.push_back( _case_calc ); 
		results.push_back( _result_calc );
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOFunAlgorithmicCalc
// ----------------------------------------------------------------------------
// Алгоритмическая функция
// ----------------------------------------------------------------------------
class RDOFunAlgorithmicCalc: public RDOFunCalc
{
protected:
	std::vector< RDOCalc* > conditions;
	std::vector< RDOCalc* > actions;

	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		int size = conditions.size();
		for ( int i = 0; i < size; i++ ) {
			if ( conditions[i]->calcValueBase( runtime ).getBool() ) {
				return actions[i]->calcValueBase( runtime );
			}
		}
		// До сюда дело дойти не должно, т.к. последний conditions должен быть значением по-умолчанию
		runtime->error( "Внутренная ошибка, RDOFunAlgorithmicCalc", this );
		return value;
	}

public:
	RDOFunAlgorithmicCalc( RDORuntimeParent* _parent ):
		RDOFunCalc( _parent )
	{
		value = 0;
	}
	void addCalcIf( RDOCalc* cond, RDOCalc* act ) {
		conditions.push_back( cond );
		actions.push_back( act );
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOFunAlgorithmicDiapCalc
// ----------------------------------------------------------------------------
// Алгоритмическая функция с проверкой на диапазон возвращаемого значения
// ----------------------------------------------------------------------------
class RDOFunAlgorithmicDiapCalc: public RDOFunAlgorithmicCalc
{
private:
	RDOValue min_value;
	RDOValue max_value;

	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		int size = conditions.size();
		for ( int i = 0; i < size; i++ ) {
			if ( conditions[i]->calcValueBase( runtime ).getBool() ) {
				value = actions[i]->calcValueBase( runtime );
				if ( value < min_value || value > max_value ) {
					if ( value.getType() == RDOValue::pt_int && min_value.getType() == RDOValue::pt_int && max_value.getType() == RDOValue::pt_int ) {
						runtime->error( rdo::format("Значение выходит за допустимый диапазон [%d..%d]: %d", min_value.getInt(), max_value.getInt(), value.getInt()), actions[i] );
					} else {
						runtime->error( rdo::format("Значение выходит за допустимый диапазон [%f..%f]: %f", min_value.getDouble(), max_value.getDouble(), value.getDouble()), actions[i] );
					}
				}
				return value;
			}
		}
		// До сюда дело дойти не должно, т.к. последний conditions должен быть значением по-умолчанию
		runtime->error( "Внутренная ошибка, RDOFunAlgorithmicDiapCalc", this );
		return value;
	}

public:
	RDOFunAlgorithmicDiapCalc( RDORuntimeParent* _parent, RDOValue _min_value, RDOValue _max_value ):
		RDOFunAlgorithmicCalc( _parent ),
		min_value( _min_value ),
		max_value( _max_value )
	{
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcGroup
// ----------------------------------------------------------------------------
class RDOFunCalcGroup: public RDOFunCalc
{
protected:
	int      nResType;
	RDOCalc* condition;

	RDOFunCalcGroup( RDORuntimeParent* _parent, int _nResType, RDOCalc* _condition ):
		RDOFunCalc( _parent ),
		nResType( _nResType ),
		condition( _condition )
	{
	}
};

class RDOFunCalcExist: public RDOFunCalcGroup
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime );

public:
	RDOFunCalcExist( RDORuntimeParent* _parent, int _nResType, RDOCalc* _condition):
		RDOFunCalcGroup( _parent, _nResType, _condition )
	{
	}
};

class RDOFunCalcNotExist: public RDOFunCalcGroup
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime );

public:
	RDOFunCalcNotExist( RDORuntimeParent* _parent, int _nResType, RDOCalc* _condition ):
		RDOFunCalcGroup( _parent, _nResType, _condition )
	{
	}
};

class RDOFunCalcForAll: public RDOFunCalcGroup
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime );

public:
	RDOFunCalcForAll( RDORuntimeParent* _parent, int _nResType, RDOCalc* _condition ):
		RDOFunCalcGroup( _parent, _nResType, _condition )
	{
	}
};

class RDOFunCalcNotForAll: public RDOFunCalcGroup
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime );

public:
	RDOFunCalcNotForAll( RDORuntimeParent* _parent, int _nResType, RDOCalc* _condition ):
		RDOFunCalcGroup( _parent, _nResType, _condition )
	{
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcSelect
// ----------------------------------------------------------------------------
class RDOResource;

class RDOFunCalcSelect: public RDOFunCalcGroup
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime );

public:
	mutable std::list< RDOResource* > res_list;
	void prepare( RDORuntime* sim ) const;

	RDOFunCalcSelect( RDORuntimeParent* _parent, int _nResType, RDOCalc* _condition ):
		RDOFunCalcGroup( _parent, _nResType, _condition )
	{
		value = 1;
	}
};

class RDOFunCalcSelectBase: public RDOFunCalc
{
protected:
	RDOFunCalcSelect* select;
	RDOCalc*          condition;

public:
	RDOFunCalcSelectBase( RDORuntimeParent* _parent, RDOFunCalcSelect* _select, RDOCalc* _condition ):
		RDOFunCalc( _parent ),
		select( _select ),
		condition( _condition )
	{
	}
};

class RDOFunCalcSelectEmpty: public RDOFunCalcSelectBase
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime );

public:
	RDOFunCalcSelectEmpty( RDORuntimeParent* _parent, RDOFunCalcSelect* _select ):
		RDOFunCalcSelectBase( _parent, _select, NULL )
	{
	}
};

class RDOFunCalcSelectSize: public RDOFunCalcSelectBase
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime );

public:
	RDOFunCalcSelectSize( RDORuntimeParent* _parent, RDOFunCalcSelect* _select ):
		RDOFunCalcSelectBase( _parent, _select, NULL )
	{
	}
};

class RDOFunCalcSelectExist: public RDOFunCalcSelectBase
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime );

public:
	RDOFunCalcSelectExist( RDORuntimeParent* _parent, RDOFunCalcSelect* _select, RDOCalc* _condition ):
		RDOFunCalcSelectBase( _parent, _select, _condition )
	{
	}
};

class RDOFunCalcSelectNotExist: public RDOFunCalcSelectBase
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime );

public:
	RDOFunCalcSelectNotExist( RDORuntimeParent* _parent, RDOFunCalcSelect* _select, RDOCalc* _condition ):
		RDOFunCalcSelectBase( _parent, _select, _condition )
	{
	}
};

class RDOFunCalcSelectForAll: public RDOFunCalcSelectBase
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime );

public:
	RDOFunCalcSelectForAll( RDORuntimeParent* _parent, RDOFunCalcSelect* _select, RDOCalc* _condition ):
		RDOFunCalcSelectBase( _parent, _select, _condition )
	{
	}
};

class RDOFunCalcSelectNotForAll: public RDOFunCalcSelectBase
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime );

public:
	RDOFunCalcSelectNotForAll( RDORuntimeParent* _parent, RDOFunCalcSelect* _select, RDOCalc* _condition ):
		RDOFunCalcSelectBase( _parent, _select, _condition )
	{
	}
};

// ----------------------------------------------------------------------------
// ---------- Стандартные функции языка
// ----------------------------------------------------------------------------
class RDOFunCalcAbs: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = fabs( runtime->getFuncArgument(0).getDouble() );
		return value;
	}
public:
	RDOFunCalcAbs( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcArcCos: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = acos( runtime->getFuncArgument(0).getDouble() );
		return value;
	}
public:
	RDOFunCalcArcCos( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcArcSin: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = asin( runtime->getFuncArgument(0).getDouble() );
		return value;
	}
public:
	RDOFunCalcArcSin( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcArcTan: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = atan( runtime->getFuncArgument(0).getDouble() );
		return value;
	}
public:
	RDOFunCalcArcTan( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcCos: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = cos( runtime->getFuncArgument(0).getDouble() );
		return value;
	}
public:
	RDOFunCalcCos( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcCotan: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = 1.0 / tan( runtime->getFuncArgument(0).getDouble() );
		return value;
	}
public:
	RDOFunCalcCotan( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcExp: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = exp( runtime->getFuncArgument(0).getDouble() );
		return value;
	}
public:
	RDOFunCalcExp( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcFloor: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = floor( runtime->getFuncArgument(0).getDouble() );
		return value;
	}
public:
	RDOFunCalcFloor( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcFrac: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		double tmp;
		value = modf( runtime->getFuncArgument(0).getDouble(), &tmp );
		return value;
	}
public:
	RDOFunCalcFrac( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcIAbs: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = abs( runtime->getFuncArgument(0).getInt() );
		return value;
	}
public:
	RDOFunCalcIAbs( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcIMax: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = std::_MAX(runtime->getFuncArgument(0).getInt(), runtime->getFuncArgument(1).getInt());
		return value;
	}
public:
	RDOFunCalcIMax( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcIMin: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = std::_MIN(runtime->getFuncArgument(0).getInt(), runtime->getFuncArgument(1).getInt());
		return value;
	}
public:
	RDOFunCalcIMin( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcInt: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = runtime->getFuncArgument(0).getInt();
		return value;
	}
public:
	RDOFunCalcInt( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcIntPower: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = pow( runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getInt() );
		return value;
	}
public:
	RDOFunCalcIntPower( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcLn: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = log( runtime->getFuncArgument(0).getDouble() );
		return value;
	}
public:
	RDOFunCalcLn( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcLog10: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = log10( runtime->getFuncArgument(0).getDouble() );
		return value;
	}
public:
	RDOFunCalcLog10( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcLog2: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = log( runtime->getFuncArgument(0).getDouble()) / log(2);
		return value;
	}
public:
	RDOFunCalcLog2( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcLogN: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = log( runtime->getFuncArgument(0).getDouble()) / log(runtime->getFuncArgument(1).getDouble() );
		return value;
	}
public:
	RDOFunCalcLogN( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcMax: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = std::_MAX(runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getDouble());
		return value;
	}
public:
	RDOFunCalcMax( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcMin: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = std::_MIN(runtime->getFuncArgument(0), runtime->getFuncArgument(1));
		return value;
	}
public:
	RDOFunCalcMin( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcPower: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = pow(runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getDouble());
		return value;
	}
public:
	RDOFunCalcPower( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcRound: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = (int)floor( runtime->getFuncArgument(0).getDouble() + 0.5 );
		return value;
	}
public:
	RDOFunCalcRound( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcSin: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = sin(runtime->getFuncArgument(0).getDouble());
		return value;
	}
public:
	RDOFunCalcSin( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcSqrt: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = sqrt(runtime->getFuncArgument(0).getDouble());
		return value;
	}
public:
	RDOFunCalcSqrt( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcTan: public RDOFunCalc
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = tan( runtime->getFuncArgument(0).getDouble() );
		return value;
	}
public:
	RDOFunCalcTan( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

// ----------------------------------------------------------------------------
// ---------- Последовательности
// ----------------------------------------------------------------------------
class RDOCalcSeqInit: public RDOCalc
{
private:
	int            base;
	RandGenerator* gen;
	virtual RDOValue& calcValue( RDORuntime* runtime );

public:
	RDOCalcSeqInit( RDORuntimeParent* _parent, int _base, RandGenerator* _gen ):
		RDOCalc( _parent ),
		base( _base ),
		gen( _gen )
	{
	}
	virtual ~RDOCalcSeqInit();
	void setBase( int _base ) { base = _base; }
};

class RDOCalcSeqNext: public RDOFunCalc
{
public:
	bool   res_real;
	bool   diap;
	double diap_min;
	double diap_max;

	RDOCalcSeqNext( RDORuntimeParent* _parent ):
		RDOFunCalc( _parent ),
		res_real( true ),
		diap( false ),
		diap_min( 0 ),
		diap_max( 0 )
	{
	}
};

class RDOCalcSeqNextUniform: public RDOCalcSeqNext
{
private:
	RandGeneratorUniform* gen;
	virtual RDOValue& calcValue( RDORuntime* runtime );

public:
	RDOCalcSeqNextUniform( RDORuntimeParent* _parent, RandGeneratorUniform* _gen ):
		RDOCalcSeqNext( _parent ),
		gen( _gen )
	{
	}
};

class RDOCalcSeqNextExponential: public RDOCalcSeqNext
{
private:
	RandGeneratorExponential* gen;
	virtual RDOValue& calcValue( RDORuntime* runtime );

public:
	RDOCalcSeqNextExponential( RDORuntimeParent* _parent, RandGeneratorExponential* _gen ):
		RDOCalcSeqNext( _parent ),
		gen( _gen )
	{
	}
};

class RDOCalcSeqNextNormal: public RDOCalcSeqNext
{
private:
	RandGeneratorNormal* gen;
	virtual RDOValue& calcValue( RDORuntime* runtime );

public:
	RDOCalcSeqNextNormal( RDORuntimeParent* _parent, RandGeneratorNormal* _gen ):
		RDOCalcSeqNext( _parent ),
		gen( _gen )
	{
	}
};

class RDOCalcSeqNextByHist: public RDOCalcSeqNext
{
private:
	RandGeneratorCommonNext* gen;
	virtual RDOValue& calcValue( RDORuntime* runtime );

public:
	RDOCalcSeqNextByHist( RDORuntimeParent* _parent, RandGeneratorCommonNext* _gen ):
		RDOCalcSeqNext( _parent ),
		gen( _gen )
	{
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcFuncParam
// ----------------------------------------------------------------------------
class RDOCalcFuncParam: public RDOCalc
{
private:
	int param_number;

	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = runtime->getFuncArgument( param_number );
		return value;
	}

public:
	RDOCalcFuncParam( RDORuntimeParent* _parent, int _param_number, const RDOSrcInfo& _src_info ):
		RDOCalc( _parent ),
		param_number( _param_number )
	{
		setSrcInfo( _src_info );
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcBinary (Binary Ariphmetic calcs)
// ----------------------------------------------------------------------------
class RDOCalcBinary: public RDOCalc
{
protected:
	RDOCalc* left;
	RDOCalc* right;

public:
	RDOCalcBinary( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalc( _parent ),
		left( _left ),
		right( _right )
	{
	}
	RDOCalc*      getLeft() const          { return left; }
	RDOCalcConst* getRightAsConst() const  { return dynamic_cast<RDOCalcConst*>(right); }
	void setRight( RDOCalc* _right )       { right = _right; }
};

class RDOCalcPlus: public RDOCalcBinary
{
public:
	RDOCalcPlus( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc *_right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		setSrcInfo( getStaticSrcInfo( left, right ) );
	}
	static RDOSrcInfo getStaticSrcInfo( const RDOCalc* left, const RDOCalc* right ) {
		RDOSrcInfo src_info;
		src_info.setSrcInfo( left->src_info(), " + ", right->src_info() );
		return src_info;
	}
	RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->inc_cnt_calc_arithm();
		value = left->calcValueBase( runtime ) + right->calcValueBase( runtime );
		return value;
	}
};

class RDOCalcMinus: public RDOCalcBinary
{
public:
	RDOCalcMinus( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		setSrcInfo( getStaticSrcInfo( left, right ) );
	}
	static RDOSrcInfo getStaticSrcInfo( const RDOCalc* left, const RDOCalc* right ) {
		RDOSrcInfo src_info;
		src_info.setSrcInfo( left->src_info(), " - ", right->src_info() );
		return src_info;
	}
	RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->inc_cnt_calc_arithm();
		value = left->calcValueBase( runtime ) - right->calcValueBase( runtime );
		return value;
	}
};

class RDOCalcMult: public RDOCalcBinary
{
public:
	RDOCalcMult( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		setSrcInfo( getStaticSrcInfo( left, right ) );
	}
	static RDOSrcInfo getStaticSrcInfo( const RDOCalc* left, const RDOCalc* right ) {
		RDOSrcInfo src_info;
		src_info.setSrcInfo( left->src_info(), " * ", right->src_info() );
		return src_info;
	}
	RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->inc_cnt_calc_arithm();
		value = left->calcValueBase( runtime ) * right->calcValueBase( runtime );
		return value;
	}
};

class RDOCalcDiv: public RDOCalcBinary
{
public:
	RDOCalcDiv( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		setSrcInfo( getStaticSrcInfo( left, right ) );
	}
	static RDOSrcInfo getStaticSrcInfo( const RDOCalc* left, const RDOCalc* right ) {
		RDOSrcInfo src_info;
		src_info.setSrcInfo( left->src_info(), " / ", right->src_info() );
		return src_info;
	}
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->inc_cnt_calc_arithm();
		RDOValue& rVal = right->calcValueBase( runtime );
		if ( rVal == 0 ) {
			runtime->error( "Деление на ноль", this );
//			runtime->error("Division by zero", this);
		}
		value = left->calcValueBase( runtime ) / rVal;
		return value;
	}
};

class RDOCalcPlusEnumSafe: public RDOCalcPlus
{
public:
	RDOCalcPlusEnumSafe( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc *_right ):
		RDOCalcPlus( _parent, _left, _right )
	{
	}
	RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->inc_cnt_calc_arithm();
		value = left->calcValueBase( runtime ).getEnumAsInt() + right->calcValueBase( runtime ).getEnumAsInt();
		return value;
	}
};

class RDOCalcMultEnumSafe: public RDOCalcMult
{
public:
	RDOCalcMultEnumSafe( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcMult( _parent, _left, _right )
	{
	}
	RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->inc_cnt_calc_arithm();
		value = left->calcValueBase( runtime ).getEnumAsInt() * right->calcValueBase( runtime ).getEnumAsInt();
		return value;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcBinary (Binary Logic calcs)
// ----------------------------------------------------------------------------
class RDOCalcAnd: public RDOCalcBinary
{
private:
	RDOValue value_true;
	RDOValue value_false;

public:
	RDOCalcAnd( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		value_true  = 1;
		value_false = 0;
		setSrcInfo( getStaticSrcInfo( left, right ) );
	}
	static RDOSrcInfo getStaticSrcInfo( const RDOCalc* left, const RDOCalc* right ) {
		RDOSrcInfo src_info;
		src_info.setSrcInfo( left->src_info(), " and ", right->src_info() );
		return src_info;
	}
	RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->inc_cnt_calc_logic();
		if ( !left->calcValueBase( runtime ).getBool()  ) {
			return value_false;
		}
		if ( !right->calcValueBase( runtime ).getBool() ) {
			return value_false;
		}
		return value_true;
	}
};

class RDOCalcOr: public RDOCalcBinary
{
private:
	RDOValue value_true;
	RDOValue value_false;

public:
	RDOCalcOr( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		value_true  = 1;
		value_false = 0;
		setSrcInfo( getStaticSrcInfo( left, right ) );
	}
	static RDOSrcInfo getStaticSrcInfo( const RDOCalc* left, const RDOCalc* right ) {
		RDOSrcInfo src_info;
		src_info.setSrcInfo( left->src_info(), " or ", right->src_info() );
		return src_info;
	}
	RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->inc_cnt_calc_logic();
		if ( left->calcValueBase( runtime ).getBool()  ) {
			return value_true;
		}
		if ( right->calcValueBase( runtime ).getBool() ) {
			return value_true;
		}
		return value_false;
	}
};

class RDOCalcNot: public RDOCalc
{
private:
	RDOCalc* calc;

public:
	RDOCalcNot( RDORuntimeParent* _parent, RDOCalc* _calc ):
		RDOCalc( _parent ),
		calc( _calc )
	{
	}
	RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->inc_cnt_calc_logic();
		value = !calc->calcValueBase( runtime ).getBool();
		return value;
	}
};

class RDOCalcIsEqual: public RDOCalcBinary
{
public:
	RDOCalcIsEqual( RDORuntimeParent* _parent, RDOCalc* const _left, RDOCalc* const _right):
		RDOCalcBinary( _parent, _left, _right )
	{
		setSrcInfo( getStaticSrcInfo( left, right ) );
	}
	static RDOSrcInfo getStaticSrcInfo( const RDOCalc* left, const RDOCalc* right ) {
		RDOSrcInfo src_info;
		src_info.setSrcInfo( left->src_info(), " = ", right->src_info() );
		return src_info;
	}
	RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->inc_cnt_calc_logic();
		value = left->calcValueBase( runtime ) == right->calcValueBase( runtime );
		return value;
	}
};

class RDOCalcIsNotEqual: public RDOCalcBinary
{
public:
	RDOCalcIsNotEqual( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		setSrcInfo( getStaticSrcInfo( left, right ) );
	}
	static RDOSrcInfo getStaticSrcInfo( const RDOCalc* left, const RDOCalc* right ) {
		RDOSrcInfo src_info;
		src_info.setSrcInfo( left->src_info(), " <> ", right->src_info() );
		return src_info;
	}
	RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->inc_cnt_calc_logic();
		value = left->calcValueBase( runtime ) != right->calcValueBase( runtime );
		return value;
	}
};

class RDOCalcIsLess: public RDOCalcBinary
{
public:
	RDOCalcIsLess( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		if ( left && right ) {
			setSrcInfo( getStaticSrcInfo( left, right ) );
		}
	}
	static RDOSrcInfo getStaticSrcInfo( const RDOCalc* left, const RDOCalc* right ) {
		RDOSrcInfo src_info;
		src_info.setSrcInfo( left->src_info(), " < ", right->src_info() );
		return src_info;
	}
	RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->inc_cnt_calc_logic();
		value = left->calcValueBase( runtime ) < right->calcValueBase( runtime );
		return value;
	}
};

class RDOCalcIsGreater: public RDOCalcBinary
{
public:
	RDOCalcIsGreater( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		setSrcInfo( getStaticSrcInfo( left, right ) );
	}
	static RDOSrcInfo getStaticSrcInfo( const RDOCalc* left, const RDOCalc* right ) {
		RDOSrcInfo src_info;
		src_info.setSrcInfo( left->src_info(), " > ", right->src_info() );
		return src_info;
	}
	RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->inc_cnt_calc_logic();
		value = left->calcValueBase( runtime ) > right->calcValueBase( runtime );
		return value;
	}
};

class RDOCalcIsLEQ: public RDOCalcBinary
{
public:
	RDOCalcIsLEQ( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		setSrcInfo( getStaticSrcInfo( left, right ) );
	}
	static RDOSrcInfo getStaticSrcInfo( const RDOCalc* left, const RDOCalc* right ) {
		RDOSrcInfo src_info;
		src_info.setSrcInfo( left->src_info(), " <= ", right->src_info() );
		return src_info;
	}
	RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->inc_cnt_calc_logic();
		value = left->calcValueBase( runtime ) <= right->calcValueBase( runtime );
		return value;
	}
};

class RDOCalcIsGEQ: public RDOCalcBinary
{
public:
	RDOCalcIsGEQ( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		setSrcInfo( getStaticSrcInfo( left, right ) );
	}
	static RDOSrcInfo getStaticSrcInfo( const RDOCalc* left, const RDOCalc* right ) {
		RDOSrcInfo src_info;
		src_info.setSrcInfo( left->src_info(), " >= ", right->src_info() );
		return src_info;
	}
	RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->inc_cnt_calc_logic();
		value = left->calcValueBase( runtime ) >= right->calcValueBase( runtime );
		return value;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcUnary (Unary Ariphmetic calcs)
// ----------------------------------------------------------------------------
class RDOCalcUnary: public RDOCalc
{
protected:
	RDOCalc* oper;

	RDOCalcUnary( RDORuntimeParent* _parent, RDOCalc* _oper ):
		RDOCalc( _parent ),
		oper( _oper )
	{
		if ( oper ) setSrcInfo( oper->src_info() );
	}
};

class RDOCalcUMinus: public RDOCalcUnary
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = -oper->calcValueBase( runtime );
		return value;
	}

public:
	RDOCalcUMinus( RDORuntimeParent* _parent, RDOCalc* _oper ): RDOCalcUnary( _parent, _oper ) {}
};

class RDOCalcDoubleToInt: public RDOCalcUnary
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = oper->calcValueBase( runtime ).getInt();
		return value;
	}

public:
	RDOCalcDoubleToInt( RDORuntimeParent* _parent, RDOCalc* _oper ):
		RDOCalcUnary( _parent, _oper )
	{
	}
};

class RDOCalcDoubleToIntByResult: public RDOCalcUnary
{
private:
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = round ? RDOValue( oper->calcValueBase( runtime ).getInt() ) : oper->calcValueBase( runtime );
		return value;
	}

public:
	bool round;
	RDOCalcDoubleToIntByResult( RDORuntimeParent* _parent, RDOCalc* _oper ):
		RDOCalcUnary( _parent, _oper ),
		round( false )
	{
	}
};

class RDOCalcCheckDiap: public RDOCalcUnary
{
private:
	RDOValue min_value;
	RDOValue max_value;

	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = oper->calcValueBase( runtime );
		if ( value < min_value || value > max_value ) {
			if ( value.getType() == RDOValue::pt_int && min_value.getType() == RDOValue::pt_int && max_value.getType() == RDOValue::pt_int ) {
				runtime->error( rdo::format("Значение выходит за допустимый диапазон [%d..%d]: %d", min_value.getInt(), max_value.getInt(), value.getInt()), this );
			} else {
				runtime->error( rdo::format("Значение выходит за допустимый диапазон [%f..%f]: %f", min_value.getDouble(), max_value.getDouble(), value.getDouble()), this );
			}
		}
		return value; 
	}

public:
	RDOCalcCheckDiap( RDORuntimeParent* _parent, RDOValue _min_value, RDOValue _max_value, RDOCalc* _oper ):
		RDOCalcUnary( _parent, _oper ),
		min_value( _min_value ),
		max_value( _max_value )
	{
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetConst / RDOCalcSetConst
// ----------------------------------------------------------------------------
class RDOCalcGetConst: public RDOCalc
{
private:
	int number;

public:
	RDOCalcGetConst( RDORuntimeParent* _parent, int _number ):
		RDOCalc( _parent ),
		number(_number)
	{
	}
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		value = runtime->getConstValue( number );
		return value;
	}
};

class RDOCalcSetConst: public RDOCalc
{
private:
	int number;
	RDOCalc* calc;

public:
	RDOCalcSetConst( RDORuntimeParent* _parent, int _number, RDOCalc *_calc ):
		RDOCalc( _parent ),
		number( _number ),
		calc( _calc )
	{
		value = 0;
		if ( calc ) setSrcInfo( calc->src_info() );
	}
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->setConstValue( number, calc->calcValueBase( runtime ) );
		return value;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcInt (приведение к целому)
// ----------------------------------------------------------------------------
class RDOCalcInt: public RDOCalcUnary
{
public:
	RDOCalcInt( RDORuntimeParent* _parent, RDOCalc* oper ): RDOCalcUnary( _parent, oper ) {}
	RDOValue& calcValue( RDORuntime* runtime ) {
		RDOValue res = oper->calcValueBase( runtime );
		value = res > 0 ? RDOValue( (int)(res.getDouble() + 0.5) ) : RDOValue( (int)(res.getDouble() - 0.5) );
		return value;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcFunctionCall
// ----------------------------------------------------------------------------
class RDOCalcFunctionCall: public RDOCalc
{
private:
	std::vector< RDOCalc* > parameters;
	RDOFunCalc* const function;

public:
	RDOCalcFunctionCall( RDORuntimeParent* _parent, RDOFunCalc* const _function ):
		RDOCalc( _parent ),
		function( _function )
	{
	}
	void addParameter( RDOCalc* calc )
	{
		parameters.push_back( calc );
	}
	virtual RDOValue& calcValue( RDORuntime* runtime )
	{
		runtime->pushFuncTop();
		int size = parameters.size();
		for ( int i = 0; i < size; i++ ) {
			RDOValue arg = parameters[i]->calcValueBase( runtime );
			runtime->pushFuncArgument( arg );
		}
		runtime->resetFuncTop( parameters.size() );
		value = function->calcValueBase( runtime );
		size = parameters.size();
		for ( i = 0; i < size; i++ ) {
			runtime->popFuncArgument();
		}
		runtime->popFuncTop();
		return value;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateNumberedResource (создание нового временного ресурса или постоянного в начальный момент времени по индексу с параметрами)
// ----------------------------------------------------------------------------
class RDOCalcCreateNumberedResource: public RDOCalc
{
private:
	int  type;
	bool traceFlag;
	std::vector< RDOValue > paramsCalcs;
	int  number;
	bool isPermanent;

public:
	RDOCalcCreateNumberedResource( RDORuntimeParent* _parent, int _type, bool _traceFlag, const std::vector< RDOValue >& _paramsCalcs, int _number, bool _isPermanent );
	virtual RDOValue& calcValue( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateEmptyResource (создание нового временного ресурса с пустым списком параметров)
// ----------------------------------------------------------------------------
class RDOCalcCreateEmptyResource: public RDOCalc
{
private:
	int  type;
	bool traceFlag;
	std::vector< RDOValue > params_default;
	int  rel_res_id;

public:
	RDOCalcCreateEmptyResource( RDORuntimeParent* _parent, int _type, bool _traceFlag, const std::vector< RDOValue >& _params_default, int _rel_res_id );
	virtual RDOValue& calcValue( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- Выбор ресурсов
// ----------------------------------------------------------------------------
class RDOSelectResourceCalc: public RDOCalc
{
public:
	enum Type {
		order_empty = 0,
		order_first,
		order_with_min, 
		order_with_max
	};

protected:
	int       rel_res_id;
	RDOCalc*  choice_calc;
	RDOCalc*  order_calc;
	Type      order_type;
	RDOSelectResourceCalc( RDORuntimeParent* _parent, int _rel_res_id, RDOCalc* _choice_calc = NULL, RDOCalc* _order_calc = NULL, Type _order_type = order_empty );
};

class RDOSelectResourceNonExistCalc: public RDOSelectResourceCalc
{
public:
	RDOSelectResourceNonExistCalc( RDORuntimeParent* _parent, int _rel_res_id ):
		RDOSelectResourceCalc( _parent, _rel_res_id, NULL, NULL )
	{
		value = 1;
	}
	virtual RDOValue& calcValue( RDORuntime* runtime ) {
		runtime->setRelRes( rel_res_id, -1 );
		return value;
	}
};

class RDOSelectResourceDirectCalc: public RDOSelectResourceCalc
{
protected:
	int res_id;

public:
	RDOSelectResourceDirectCalc( RDORuntimeParent* _parent, int _rel_res_id, int _res_id, RDOCalc* _choice_calc = NULL, RDOCalc* _order_calc = NULL, Type _order_type = order_empty ):
		RDOSelectResourceCalc( _parent, _rel_res_id, _choice_calc, _order_calc, _order_type ),
		res_id( _res_id )
	{
	}
	virtual RDOValue& calcValue( RDORuntime* runtime );
};

class RDOSelectResourceByTypeCalc: public RDOSelectResourceCalc
{
protected:
	int resType;

public:
	RDOSelectResourceByTypeCalc( RDORuntimeParent* _parent, int _rel_res_id, int _resType, RDOCalc* _choice_calc = NULL, RDOCalc* _order_calc = NULL, Type _order_type = order_empty ):
		RDOSelectResourceCalc( _parent, _rel_res_id, _choice_calc, _order_calc, _order_type ),
		resType( _resType )
	{
	}
	virtual RDOValue& calcValue( RDORuntime* runtime );
};

class RDOSelectResourceCommon
{
public:
	virtual std::vector<int> getPossibleNumbers( RDORuntime* sim ) const = 0;
	virtual bool callChoice( RDORuntime* sim ) const = 0;
};

class RDOSelectResourceCommonCalc: public RDOCalc
{
private:
	RDOCalc* choice_calc;
	std::vector< RDOSelectResourceCommon* > resSelectors;
	bool useCommonWithMax;
	void getBest( std::vector< std::vector< int > >& allNumbs, int level, std::vector< int >& res, RDOValue& bestVal, RDORuntime* sim, bool& hasBest ) const;
	bool getFirst( std::vector< std::vector< int > >& allNumbs, int level, RDORuntime* sim ) const;

public:
	RDOSelectResourceCommonCalc( RDORuntimeParent* _parent, const std::vector< RDOSelectResourceCommon* >& _resSelectors, bool _useCommonWithMax, RDOCalc* _choice_calc ):
		RDOCalc( _parent ),
		resSelectors( _resSelectors ),
		useCommonWithMax( _useCommonWithMax ),
		choice_calc( _choice_calc )
	{
		if ( choice_calc ) setSrcInfo( choice_calc->src_info() );
	}
	virtual RDOValue& calcValue( RDORuntime* runtime );
};

class RDOSelectResourceDirectCommonCalc: public RDOSelectResourceDirectCalc, public RDOSelectResourceCommon
{
public:
	RDOSelectResourceDirectCommonCalc( RDORuntimeParent* _parent, int _relNumb, int _resNumb, RDOCalc* _choice_calc = NULL, RDOCalc* _order_calc = NULL, Type _order_type = order_empty ):
		RDOSelectResourceDirectCalc( _parent, _relNumb, _resNumb, _choice_calc, _order_calc, _order_type )
	{
	}
	std::vector<int> getPossibleNumbers( RDORuntime* sim ) const;
	virtual bool callChoice( RDORuntime* sim ) const;
};

class RDOSelectResourceByTypeCommonCalc: public RDOSelectResourceByTypeCalc, public RDOSelectResourceCommon
{
public:
	RDOSelectResourceByTypeCommonCalc( RDORuntimeParent* _parent, int _relNumb, int _resType, RDOCalc* _choice_calc = NULL, RDOCalc* _order_calc = NULL, Type _order_type = order_empty ):
		RDOSelectResourceByTypeCalc( _parent, _relNumb, _resType, _choice_calc, _order_calc, _order_type )
	{
	}
	std::vector<int> getPossibleNumbers( RDORuntime* sim ) const;
	virtual bool callChoice( RDORuntime* sim ) const;
};

} // namespace rdoRuntime

#endif // RDOCALC_H
