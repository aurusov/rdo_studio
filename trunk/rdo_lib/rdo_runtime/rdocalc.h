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
	virtual RDOValue calcValue( RDORuntime* runtime ) const = 0;

public:
	RDOCalc( RDORuntimeParent* _parent );
	virtual ~RDOCalc();

	RDOValue calcValueBase( RDORuntime* runtime ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcConst
// ----------------------------------------------------------------------------
class RDOCalcConst: public RDOCalc
{
private:
	RDOValue constanta;

	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return constanta;
	}

public:
	RDOCalcConst( RDORuntimeParent* _parent, const RDOValue& val ):
		RDOCalc( _parent ),
		constanta( val )
	{
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

	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return runtime->getResParamVal( resNumb, parNumb );
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

	virtual RDOValue calcValue( RDORuntime* runtime ) const {
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

	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return runtime->getResParamVal( runtime->getResByRelRes(relNumb), parNumb );
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
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		runtime->setResParamVal( runtime->getResByRelRes(relNumb), parNumb, calc->calcValueBase( runtime ) );
		return 1;
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

	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		RDOValue value = calc->calcValueBase( runtime );
		if ( value < min_value || value > max_value ) {
			if ( value == (int)value && min_value == (int)min_value && max_value == (int)max_value ) {
				runtime->error( rdo::format("Значение выходит за допустимый диапазон [%d..%d]: %d", (int)min_value, (int)max_value, (int)value), this );
			} else {
				runtime->error( rdo::format("Значение выходит за допустимый диапазон [%f..%f]: %f", min_value, max_value, value), this );
			}
		}
		runtime->setResParamVal( runtime->getResByRelRes(relNumb), parNumb, value );
		return 1;
	}

public:
	RDOSetRelParamDiapCalc( RDORuntimeParent* _parent, int _relNumb, int _parNumb, RDOCalc* _calc, RDOValue _min_value, RDOValue _max_value ):
		RDOSetRelParamCalc( _parent, _relNumb, _parNumb, _calc ),
		min_value( _min_value ),
		max_value( _max_value )
	{
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

	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		runtime->setResParamVal( resNumb, parNumb, calc->calcValueBase( runtime ) );
		return 1;
	}

public:
	RDOSetResourceParamCalc( RDORuntimeParent* _parent, int _resNumb, int _parNumb, RDOCalc* _calc ):
		RDOCalc( _parent ),
		resNumb( _resNumb ),
		parNumb( _parNumb ),
		calc( _calc )
	{
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

	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		runtime->onEraseRes( runtime->getResByRelRes(rel_res_id), this );
		return 1;
	}

public:
	RDOCalcEraseRes( RDORuntimeParent* _parent, int _rel_res_id, const std::string& _rel_res_name ):
		RDOCalc( _parent ),
		rel_res_id( _rel_res_id ),
		rel_res_name( _rel_res_name )
	{
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

	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		runtime->setPatternParameter( parNumb, val );
		return 0;
	}

public:
	RDOSetPatternParamCalc( RDORuntimeParent* _parent, int _parNumb, RDOValue _val ):
		RDOCalc( _parent ),
		parNumb( _parNumb ),
		val( _val )
	{
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcPatParam
// ----------------------------------------------------------------------------
class RDOCalcPatParam: public RDOCalc
{
private:
	int numberOfParam;

	virtual RDOValue calcValue( RDORuntime* runtime ) const { 
		return runtime->getPatternParameter(numberOfParam); 
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
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return runtime->getTimeNow();
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
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return runtime->getSeconds();
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

	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		int index = argCalc->calcValueBase( runtime );
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

	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		int size = cases.size();
		for ( int i = 0; i < size; i++ ) {
			RDOCalc* cas = cases[i];
			if ( cas->calcValueBase( runtime ) ) {
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

	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		int size = conditions.size();
		for ( int i = 0; i < size; i++ ) {
			if ( conditions[i]->calcValueBase( runtime ) ) {
				return actions[i]->calcValueBase( runtime );
			}
		}
		// До сюда дело дойти не должно, т.к. последний conditions должен быть значением по-умолчанию
		runtime->error( "Внутренная ошибка, RDOFunAlgorithmicCalc", this );
		return 0;
	}

public:
	RDOFunAlgorithmicCalc( RDORuntimeParent* _parent ):
		RDOFunCalc( _parent )
	{
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

	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		int size = conditions.size();
		for ( int i = 0; i < size; i++ ) {
			if ( conditions[i]->calcValueBase( runtime ) ) {
				RDOValue value = actions[i]->calcValueBase( runtime );
				if ( value < min_value || value > max_value ) {
					if ( value == (int)value && min_value == (int)min_value && max_value == (int)max_value ) {
						runtime->error( rdo::format("Значение выходит за допустимый диапазон [%d..%d]: %d", (int)min_value, (int)max_value, (int)value), actions[i] );
					} else {
						runtime->error( rdo::format("Значение выходит за допустимый диапазон [%f..%f]: %f", min_value, max_value, value), actions[i] );
					}
				}
				return value;
			}
		}
		// До сюда дело дойти не должно, т.к. последний conditions должен быть значением по-умолчанию
		runtime->error( "Внутренная ошибка, RDOFunAlgorithmicDiapCalc", this );
		return 0;
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
	virtual RDOValue calcValue( RDORuntime* runtime ) const;

public:
	RDOFunCalcExist( RDORuntimeParent* _parent, int _nResType, RDOCalc* _condition):
		RDOFunCalcGroup( _parent, _nResType, _condition )
	{
	}
};

class RDOFunCalcNotExist: public RDOFunCalcGroup
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const;

public:
	RDOFunCalcNotExist( RDORuntimeParent* _parent, int _nResType, RDOCalc* _condition ):
		RDOFunCalcGroup( _parent, _nResType, _condition )
	{
	}
};

class RDOFunCalcForAll: public RDOFunCalcGroup
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const;

public:
	RDOFunCalcForAll( RDORuntimeParent* _parent, int _nResType, RDOCalc* _condition ):
		RDOFunCalcGroup( _parent, _nResType, _condition )
	{
	}
};

class RDOFunCalcNotForAll: public RDOFunCalcGroup
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const;

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
	virtual RDOValue calcValue( RDORuntime* runtime ) const;

public:
	mutable std::list< RDOResource* > res_list;
	void prepare( RDORuntime* sim ) const;

	RDOFunCalcSelect( RDORuntimeParent* _parent, int _nResType, RDOCalc* _condition ):
		RDOFunCalcGroup( _parent, _nResType, _condition )
	{
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
	virtual RDOValue calcValue( RDORuntime* runtime ) const;

public:
	RDOFunCalcSelectEmpty( RDORuntimeParent* _parent, RDOFunCalcSelect* _select ):
		RDOFunCalcSelectBase( _parent, _select, NULL )
	{
	}
};

class RDOFunCalcSelectSize: public RDOFunCalcSelectBase
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const;

public:
	RDOFunCalcSelectSize( RDORuntimeParent* _parent, RDOFunCalcSelect* _select ):
		RDOFunCalcSelectBase( _parent, _select, NULL )
	{
	}
};

class RDOFunCalcSelectExist: public RDOFunCalcSelectBase
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const;

public:
	RDOFunCalcSelectExist( RDORuntimeParent* _parent, RDOFunCalcSelect* _select, RDOCalc* _condition ):
		RDOFunCalcSelectBase( _parent, _select, _condition )
	{
	}
};

class RDOFunCalcSelectNotExist: public RDOFunCalcSelectBase
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const;

public:
	RDOFunCalcSelectNotExist( RDORuntimeParent* _parent, RDOFunCalcSelect* _select, RDOCalc* _condition ):
		RDOFunCalcSelectBase( _parent, _select, _condition )
	{
	}
};

class RDOFunCalcSelectForAll: public RDOFunCalcSelectBase
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const;

public:
	RDOFunCalcSelectForAll( RDORuntimeParent* _parent, RDOFunCalcSelect* _select, RDOCalc* _condition ):
		RDOFunCalcSelectBase( _parent, _select, _condition )
	{
	}
};

class RDOFunCalcSelectNotForAll: public RDOFunCalcSelectBase
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const;

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
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(fabs(runtime->getFuncArgument(0)));
	}
public:
	RDOFunCalcAbs( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcArcCos: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(acos(runtime->getFuncArgument(0)));
	}
public:
	RDOFunCalcArcCos( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcArcSin: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(asin(runtime->getFuncArgument(0)));
	}
public:
	RDOFunCalcArcSin( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcArcTan: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(atan(runtime->getFuncArgument(0)));
	}
public:
	RDOFunCalcArcTan( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcCos: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(cos(runtime->getFuncArgument(0)));
	}
public:
	RDOFunCalcCos( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcCotan: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(1.0/tan(runtime->getFuncArgument(0)));
	}
public:
	RDOFunCalcCotan( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcExp: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(exp(runtime->getFuncArgument(0)));
	}
public:
	RDOFunCalcExp( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcFloor: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(floor(runtime->getFuncArgument(0)));
	}
public:
	RDOFunCalcFloor( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcFrac: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		double tmp;
		return RDOValue(modf(runtime->getFuncArgument(0), &tmp));
	}
public:
	RDOFunCalcFrac( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcIAbs: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(abs(runtime->getFuncArgument(0)));
	}
public:
	RDOFunCalcIAbs( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcIMax: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue((int)std::_MAX(runtime->getFuncArgument(0), runtime->getFuncArgument(1)));
	}
public:
	RDOFunCalcIMax( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcIMin: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue((int)std::_MIN(runtime->getFuncArgument(0), runtime->getFuncArgument(1)));
	}
public:
	RDOFunCalcIMin( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcInt: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(int(runtime->getFuncArgument(0)));
	}
public:
	RDOFunCalcInt( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcIntPower: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(pow(runtime->getFuncArgument(0), runtime->getFuncArgument(1)));
	}
public:
	RDOFunCalcIntPower( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcLn: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(log(runtime->getFuncArgument(0)));
	}
public:
	RDOFunCalcLn( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcLog10: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(log10(runtime->getFuncArgument(0)));
	}
public:
	RDOFunCalcLog10( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcLog2: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(log(runtime->getFuncArgument(0)) / log(2) );
	}
public:
	RDOFunCalcLog2( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcLogN: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(log(runtime->getFuncArgument(0)) / log(runtime->getFuncArgument(1)));
	}
public:
	RDOFunCalcLogN( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcMax: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(std::_MAX(runtime->getFuncArgument(0), runtime->getFuncArgument(1)));
	}
public:
	RDOFunCalcMax( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcMin: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(std::_MIN(runtime->getFuncArgument(0), runtime->getFuncArgument(1)));
	}
public:
	RDOFunCalcMin( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcPower: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(pow(runtime->getFuncArgument(0), runtime->getFuncArgument(1)));
	}
public:
	RDOFunCalcPower( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcRound: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(floor(runtime->getFuncArgument(0) + 0.5));
	}
public:
	RDOFunCalcRound( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcSin: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(sin(runtime->getFuncArgument(0)));
	}
public:
	RDOFunCalcSin( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcSqrt: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(sqrt(runtime->getFuncArgument(0)));
	}
public:
	RDOFunCalcSqrt( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
};

class RDOFunCalcTan: public RDOFunCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(tan(runtime->getFuncArgument(0)));
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
	virtual RDOValue calcValue( RDORuntime* runtime ) const;

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
	bool     res_real;
	bool     diap;
	RDOValue diap_min;
	RDOValue diap_max;

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
	virtual RDOValue calcValue( RDORuntime* runtime ) const;

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
	virtual RDOValue calcValue( RDORuntime* runtime ) const;

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
	virtual RDOValue calcValue( RDORuntime* runtime ) const;

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
	virtual RDOValue calcValue( RDORuntime* runtime ) const;

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

	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return runtime->getFuncArgument( param_number ); 
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
	const RDOCalc* left;
	const RDOCalc* right;

public:
	RDOCalcBinary( RDORuntimeParent* _parent, const RDOCalc* _left, const RDOCalc* _right ):
		RDOCalc( _parent ),
		left( _left ),
		right( _right )
	{
	}
	const RDOCalc*      getLeft() const         { return left; }
	const RDOCalcConst* getRightAsConst() const { return dynamic_cast<const RDOCalcConst*>(right); }
	void setRight( const RDOCalc* _right )      { right = _right; }
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
	RDOValue calcValue( RDORuntime* runtime ) const {
		return left->calcValueBase( runtime ) + right->calcValueBase( runtime );
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
	RDOValue calcValue( RDORuntime* runtime ) const {
		return left->calcValueBase( runtime ) - right->calcValueBase( runtime );
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
	RDOValue calcValue( RDORuntime* runtime ) const {
		return left->calcValueBase( runtime ) * right->calcValueBase( runtime );
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
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		RDOValue rVal = right->calcValueBase( runtime );
		if ( rVal == 0 ) {
			runtime->error( "Деление на ноль", this );
//			runtime->error("Division by zero", this);
		}
		return left->calcValueBase( runtime ) / rVal;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcBinary (Binary Logic calcs)
// ----------------------------------------------------------------------------
class RDOCalcAnd: public RDOCalcBinary
{
public:
	RDOCalcAnd( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		setSrcInfo( getStaticSrcInfo( left, right ) );
	}
	static RDOSrcInfo getStaticSrcInfo( const RDOCalc* left, const RDOCalc* right ) {
		RDOSrcInfo src_info;
		src_info.setSrcInfo( left->src_info(), " and ", right->src_info() );
		return src_info;
	}
	RDOValue calcValue( RDORuntime* runtime ) const {
		if ( !left->calcValueBase( runtime )  ) return false;
		if ( !right->calcValueBase( runtime ) ) return false;
		return true;
	}
};

class RDOCalcOr: public RDOCalcBinary
{
public:
	RDOCalcOr( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		setSrcInfo( getStaticSrcInfo( left, right ) );
	}
	static RDOSrcInfo getStaticSrcInfo( const RDOCalc* left, const RDOCalc* right ) {
		RDOSrcInfo src_info;
		src_info.setSrcInfo( left->src_info(), " or ", right->src_info() );
		return src_info;
	}
	RDOValue calcValue( RDORuntime* runtime ) const {
		if ( left->calcValueBase( runtime )  ) return true;
		if ( right->calcValueBase( runtime ) ) return true;
		return false;
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
	RDOValue calcValue( RDORuntime* runtime ) const {
		return !calc->calcValueBase( runtime );
	}
};

class RDOCalcIsEqual: public RDOCalcBinary
{
public:
	RDOCalcIsEqual( RDORuntimeParent* _parent, const RDOCalc* const _left, const RDOCalc* const _right):
		RDOCalcBinary( _parent, _left, _right )
	{
		setSrcInfo( getStaticSrcInfo( left, right ) );
	}
	RDOValue calcValue( RDORuntime* runtime ) const {
		return ( left->calcValueBase( runtime ) == right->calcValueBase( runtime ) );
	}
	static RDOSrcInfo getStaticSrcInfo( const RDOCalc* left, const RDOCalc* right ) {
		RDOSrcInfo src_info;
		src_info.setSrcInfo( left->src_info(), " = ", right->src_info() );
		return src_info;
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
	RDOValue calcValue( RDORuntime* runtime ) const {
		return ( left->calcValueBase( runtime ) != right->calcValueBase( runtime ) );
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
	RDOValue calcValue( RDORuntime* runtime ) const {
		return left->calcValueBase( runtime ) < right->calcValueBase( runtime );
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
	RDOValue calcValue( RDORuntime* runtime ) const {
		return left->calcValueBase( runtime ) > right->calcValueBase( runtime );
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
	RDOValue calcValue( RDORuntime* runtime ) const {
		return left->calcValueBase( runtime ) <= right->calcValueBase( runtime );
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
	RDOValue calcValue( RDORuntime* runtime ) const {
		return left->calcValueBase( runtime ) >= right->calcValueBase( runtime );
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
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(-oper->calcValueBase( runtime ));
	}

public:
	RDOCalcUMinus( RDORuntimeParent* _parent, RDOCalc* _oper ): RDOCalcUnary( _parent, _oper ) {}
};

class RDOCalcDoubleToInt: public RDOCalcUnary
{
private:
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue( static_cast<int>(oper->calcValueBase( runtime )) );
	}

public:
	RDOCalcDoubleToInt( RDORuntimeParent* _parent, RDOCalc* _oper ):
		RDOCalcUnary( _parent, _oper )
	{
	}
};

class RDOCalcCheckDiap: public RDOCalcUnary
{
private:
	RDOValue min_value;
	RDOValue max_value;

	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		RDOValue value = oper->calcValueBase( runtime );
		if ( value < min_value || value > max_value ) {
			if ( value == (int)value && min_value == (int)min_value && max_value == (int)max_value ) {
				runtime->error( rdo::format("Значение выходит за допустимый диапазон [%d..%d]: %d", (int)min_value, (int)max_value, (int)value), this );
			} else {
				runtime->error( rdo::format("Значение выходит за допустимый диапазон [%f..%f]: %f", min_value, max_value, value), this );
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
	virtual RDOValue calcValue( RDORuntime* runtime ) const { return runtime->getConstValue(number); } 
};

class RDOCalcSetConst: public RDOCalc
{
private:
	int number;
	RDOCalc* value;

public:
	RDOCalcSetConst( RDORuntimeParent* _parent, int _number, RDOCalc *_value ):
		RDOCalc( _parent ),
		number( _number ),
		value( _value )
	{
		if ( value ) setSrcInfo( value->src_info() );
	}
	virtual RDOValue calcValue( RDORuntime* runtime ) const { runtime->setConstValue(number, value->calcValueBase( runtime )); return 0; } 
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcInt (приведение к целому)
// ----------------------------------------------------------------------------
class RDOCalcInt: public RDOCalcUnary
{
public:
	RDOCalcInt( RDORuntimeParent* _parent, RDOCalc* oper ): RDOCalcUnary( _parent, oper ) {}
	RDOValue calcValue( RDORuntime* runtime ) const {
		RDOValue res = oper->calcValueBase( runtime );
		return static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcFunctionCall
// ----------------------------------------------------------------------------
class RDOCalcFunctionCall: public RDOCalc
{
private:
	std::vector< const RDOCalc* > parameters;
	const RDOFunCalc* const function;

public:
	RDOCalcFunctionCall( RDORuntimeParent* _parent, const RDOFunCalc* const _function ):
		RDOCalc( _parent ),
		function( _function )
	{
	}

	void addParameter(const RDOCalc *calc) { parameters.push_back(calc); }
	virtual RDOValue calcValue( RDORuntime* runtime ) const
	{
		runtime->pushFuncTop();
		int size = parameters.size();
		for(int i = 0; i < size; i++)
		{
			RDOValue arg = parameters[i]->calcValueBase( runtime );
			runtime->pushFuncArgument(arg);
		}
		runtime->resetFuncTop(parameters.size());
		RDOValue val = function->calcValueBase( runtime );
		size = parameters.size();
		for(i = 0; i < size; i++)
			runtime->popFuncArgument();

		runtime->popFuncTop();
		return val;
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
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
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
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
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
	}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		runtime->setRelRes( rel_res_id, -1 );
		return 1;
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
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
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
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
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
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
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
