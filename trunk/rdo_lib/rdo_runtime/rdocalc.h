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

public:
	RDOCalcConst( RDORuntimeParent* _parent, const RDOValue& val ):
		RDOCalc( _parent ),
		constanta( val )
	{
	};
	virtual RDOValue calcValue( RDORuntime* runtime ) const { return constanta; }
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetResParam (Параметры ресурса)
// ----------------------------------------------------------------------------
class RDOCalcGetResParam: public RDOCalc
{
private:
	int resNumb;
	int parNumb;

public:
	RDOCalcGetResParam( RDORuntimeParent* _parent, int _resNumb, int _parNumb ):
		RDOCalc( _parent ),
		resNumb( _resNumb ),
		parNumb( _parNumb )
	{
	}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return runtime->getResParamVal( resNumb, parNumb );
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetGroupResParam
// ----------------------------------------------------------------------------
class RDOCalcGetGroupResParam: public RDOCalc
{
	int parNumb;
public:
	RDOCalcGetGroupResParam( RDORuntimeParent* _parent, int _parNumb ):
		RDOCalc( _parent ),
		parNumb( _parNumb )
	{
	}
	virtual RDOValue calcValue( RDORuntime* runtime ) const
	{
		RDOResource *currRes = (RDOResource *)runtime->getGroupFuncRes();
	   return currRes->params[parNumb];
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

public:
	RDOCalcGetRelevantResParam( RDORuntimeParent* _parent, int _relNumb, int _parNumb ):
		RDOCalc( _parent ),
		relNumb( _relNumb ),
		parNumb( _parNumb )
	{
	}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return runtime->getResParamVal( runtime->getResByRelRes(relNumb), parNumb );
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOSetRelParamCalc
// ----------------------------------------------------------------------------
class RDOSetRelParamCalc: public RDOCalc
{
private:
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
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		runtime->setResParamVal( runtime->getResByRelRes(relNumb), parNumb, calc->calcValueBase( runtime ) );
		return 1;
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

public:
	RDOSetResourceParamCalc( RDORuntimeParent* _parent, int _resNumb, int _parNumb, RDOCalc* _calc ):
		RDOCalc( _parent ),
		resNumb( _resNumb ),
		parNumb( _parNumb ),
		calc( _calc )
	{
		if ( calc ) setSrcInfo( calc->src_info() );
	}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		runtime->setResParamVal( resNumb, parNumb, calc->calcValueBase( runtime ) );
		return 1;
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

public:
	RDOCalcEraseRes( RDORuntimeParent* _parent, int _rel_res_id, const std::string& _rel_res_name ):
		RDOCalc( _parent ),
		rel_res_id( _rel_res_id ),
		rel_res_name( _rel_res_name )
	{
	}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		runtime->onEraseRes( runtime->getResByRelRes(rel_res_id), this );
		return 1;
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

public:
	RDOSetPatternParamCalc( RDORuntimeParent* _parent, int _parNumb, RDOValue _val ):
		RDOCalc( _parent ),
		parNumb( _parNumb ),
		val( _val )
	{
	}
	virtual RDOValue calcValue( RDORuntime* runtime ) const { runtime->setPatternParameter(parNumb, val); return 0; }
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcPatParam
// ----------------------------------------------------------------------------
class RDOCalcPatParam: public RDOCalc
{
private:
	int numberOfParam;

public:
	RDOCalcPatParam( RDORuntimeParent* _parent, int _numberOfParam ):
		RDOCalc( _parent ),
		numberOfParam( _numberOfParam )
	{
	}
	virtual RDOValue calcValue( RDORuntime* runtime ) const { 
		return runtime->getPatternParameter(numberOfParam); 
	}

};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTimeNow
// ----------------------------------------------------------------------------
class RDOCalcGetTimeNow: public RDOCalc
{
public:
	RDOCalcGetTimeNow( RDORuntimeParent* _parent ): RDOCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const { return runtime->getTimeNow(); } 
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetSeconds
// ----------------------------------------------------------------------------
class RDOCalcGetSeconds: public RDOCalc
{
public:
	RDOCalcGetSeconds( RDORuntimeParent* _parent ): RDOCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const { return runtime->getSeconds(); } 
};

// ----------------------------------------------------------------------------
// ---------- RDOFunCalc
// ----------------------------------------------------------------------------
class RDOFunCalc: public RDOCalc
{
protected:
	RDOFunCalc( RDORuntimeParent* _parent ): RDOCalc( _parent ) {}
};

class RDOFuncTableCalc: public RDOFunCalc
{
private:
	std::vector< RDOCalcConst* > results;
	RDOCalc* argCalc;

public:
	RDOFuncTableCalc( RDORuntimeParent* _parent, RDOCalc* _argCalc ):
		RDOFunCalc( _parent ),
		argCalc( _argCalc )
	{
	}
	void addResultCalc( RDOCalcConst* res ) { results.push_back(res); }
	virtual RDOValue calcValue( RDORuntime* runtime ) const
	{
		int index = argCalc->calcValueBase( runtime );
		return results.at(index)->calcValueBase( runtime );
	}
};

class RDOFunListCalc: public RDOFunCalc
{
private:
	std::vector< RDOCalc* > cases;
	std::vector< RDOCalcConst* > results;
	RDOCalcConst* defaultValue;

public:
	RDOFunListCalc( RDORuntimeParent* _parent, RDOCalcConst* _defaultValue ):
		RDOFunCalc( _parent ),
		defaultValue( _defaultValue )
	{
	}

	void addCase( RDOCalc* _caseCalc, RDOCalcConst* _resultCalc )
	{
		cases.push_back(_caseCalc); 
		results.push_back(_resultCalc);
	}

	virtual RDOValue calcValue( RDORuntime* runtime ) const
	{
		int size = cases.size();
		for(int i = 0; i < size; i++) {
			RDOCalc* cas = cases[i];
			if ( cas->calcValueBase( runtime ) ) return results[i]->calcValueBase( runtime );
		}
		return defaultValue->calcValueBase( runtime );
	}
};

class RDOFunAlgorithmicCalc: public RDOFunCalc
{
private:
	std::vector<RDOCalc *> conditions;
	std::vector<RDOCalc *> actions;

public:
	RDOFunAlgorithmicCalc( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}

	void addCalcIf(RDOCalc *cond, RDOCalc *act)
	{
		conditions.push_back(cond);
		actions.push_back(act);
	}

	virtual RDOValue calcValue( RDORuntime* runtime ) const
	{
		int size = conditions.size();
		for(int i = 0; i < size; i++)
		{
			if(conditions[i]->calcValueBase( runtime ))
				return actions[i]->calcValueBase( runtime );
		}
		return 0;
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
public:
	RDOFunCalcExist( RDORuntimeParent* _parent, int _nResType, RDOCalc* _condition): RDOFunCalcGroup( _parent, _nResType, _condition ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
};

class RDOFunCalcNotExist: public RDOFunCalcGroup
{
public:
	RDOFunCalcNotExist( RDORuntimeParent* _parent, int _nResType, RDOCalc* _condition ): RDOFunCalcGroup( _parent, _nResType, _condition ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
};

class RDOFunCalcForAll: public RDOFunCalcGroup
{
public:
	RDOFunCalcForAll( RDORuntimeParent* _parent, int _nResType, RDOCalc* _condition ): RDOFunCalcGroup( _parent, _nResType, _condition ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
};

class RDOFunCalcNotForAll: public RDOFunCalcGroup
{
public:
	RDOFunCalcNotForAll( RDORuntimeParent* _parent, int _nResType, RDOCalc* _condition ): RDOFunCalcGroup( _parent, _nResType, _condition ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcSelect
// ----------------------------------------------------------------------------
class RDOResource;

class RDOFunCalcSelect: public RDOFunCalcGroup
{
public:
	mutable std::list< RDOResource* > res_list;
	void prepare( RDORuntime* sim ) const;

	RDOFunCalcSelect( RDORuntimeParent* _parent, int _nResType, RDOCalc* _condition ): RDOFunCalcGroup( _parent, _nResType, _condition ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
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

class RDOFunCalcSelectSize: public RDOFunCalcSelectBase
{
public:
	RDOFunCalcSelectSize( RDORuntimeParent* _parent, RDOFunCalcSelect* _select ): RDOFunCalcSelectBase( _parent, _select, NULL ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
};

class RDOFunCalcSelectExist: public RDOFunCalcSelectBase
{
public:
	RDOFunCalcSelectExist( RDORuntimeParent* _parent, RDOFunCalcSelect* _select, RDOCalc* _condition ): RDOFunCalcSelectBase( _parent, _select, _condition ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
};

class RDOFunCalcSelectNotExist: public RDOFunCalcSelectBase
{
public:
	RDOFunCalcSelectNotExist( RDORuntimeParent* _parent, RDOFunCalcSelect* _select, RDOCalc* _condition ): RDOFunCalcSelectBase( _parent, _select, _condition ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
};

class RDOFunCalcSelectForAll: public RDOFunCalcSelectBase
{
public:
	RDOFunCalcSelectForAll( RDORuntimeParent* _parent, RDOFunCalcSelect* _select, RDOCalc* _condition ): RDOFunCalcSelectBase( _parent, _select, _condition ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
};

class RDOFunCalcSelectNotForAll: public RDOFunCalcSelectBase
{
public:
	RDOFunCalcSelectNotForAll( RDORuntimeParent* _parent, RDOFunCalcSelect* _select, RDOCalc* _condition ): RDOFunCalcSelectBase( _parent, _select, _condition ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
};

// ----------------------------------------------------------------------------
// ---------- Native functions
// ----------------------------------------------------------------------------
class RDOFunCalcAbs: public RDOFunCalc
{
public:
	RDOFunCalcAbs( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(fabs(runtime->getFuncArgument(0)));
	}
};

class RDOFunCalcArcCos: public RDOFunCalc
{
public:
	RDOFunCalcArcCos( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(acos(runtime->getFuncArgument(0)));
	}
};

class RDOFunCalcArcSin: public RDOFunCalc
{
public:
	RDOFunCalcArcSin( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(asin(runtime->getFuncArgument(0)));
	}
};

class RDOFunCalcArcTan: public RDOFunCalc
{
public:
	RDOFunCalcArcTan( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(atan(runtime->getFuncArgument(0)));
	}
};

class RDOFunCalcCos: public RDOFunCalc
{
public:
	RDOFunCalcCos( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(cos(runtime->getFuncArgument(0)));
	}
};

class RDOFunCalcCotan: public RDOFunCalc
{
public:
	RDOFunCalcCotan( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(1./tan(runtime->getFuncArgument(0)));
	}
};

class RDOFunCalcExp: public RDOFunCalc
{
public:
	RDOFunCalcExp( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(exp(runtime->getFuncArgument(0)));
	}
};

class RDOFunCalcFloor: public RDOFunCalc
{
public:
	RDOFunCalcFloor( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(floor(runtime->getFuncArgument(0)));
	}
};

class RDOFunCalcFrac: public RDOFunCalc
{
public:
	RDOFunCalcFrac( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		double tmp;
		return RDOValue(modf(runtime->getFuncArgument(0), &tmp));
	}
};

class RDOFunCalcIAbs: public RDOFunCalc
{
public:
	RDOFunCalcIAbs( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(abs(runtime->getFuncArgument(0)));
	}
};

class RDOFunCalcIMax: public RDOFunCalc
{
public:
	RDOFunCalcIMax( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue((int)std::_MAX(runtime->getFuncArgument(0), runtime->getFuncArgument(1)));
	}
};

class RDOFunCalcIMin: public RDOFunCalc
{
public:
	RDOFunCalcIMin( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue((int)std::_MIN(runtime->getFuncArgument(0), runtime->getFuncArgument(1)));
	}
};

class RDOFunCalcInt: public RDOFunCalc
{
public:
	RDOFunCalcInt( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(int(runtime->getFuncArgument(0)));
	}
};

class RDOFunCalcIntPower: public RDOFunCalc
{
public:
	RDOFunCalcIntPower( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(pow(runtime->getFuncArgument(0), runtime->getFuncArgument(1)));
	}
};

class RDOFunCalcLn: public RDOFunCalc
{
public:
	RDOFunCalcLn( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(log(runtime->getFuncArgument(0)));
	}
};

class RDOFunCalcLog10: public RDOFunCalc
{
public:
	RDOFunCalcLog10( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(log10(runtime->getFuncArgument(0)));
	}
};

class RDOFunCalcLog2: public RDOFunCalc
{
public:
	RDOFunCalcLog2( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(log(runtime->getFuncArgument(0)) / log(2) );
	}
};

class RDOFunCalcLogN: public RDOFunCalc
{
public:
	RDOFunCalcLogN( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(log(runtime->getFuncArgument(0)) / log(runtime->getFuncArgument(1)));
	}
};

class RDOFunCalcMax: public RDOFunCalc
{
public:
	RDOFunCalcMax( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(std::_MAX(runtime->getFuncArgument(0), runtime->getFuncArgument(1)));
	}
};

class RDOFunCalcMin: public RDOFunCalc
{
public:
	RDOFunCalcMin( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(std::_MIN(runtime->getFuncArgument(0), runtime->getFuncArgument(1)));
	}
};

class RDOFunCalcPower: public RDOFunCalc
{
public:
	RDOFunCalcPower( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(pow(runtime->getFuncArgument(0), runtime->getFuncArgument(1)));
	}
};

class RDOFunCalcRound: public RDOFunCalc
{
public:
	RDOFunCalcRound( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(floor(runtime->getFuncArgument(0) + 0.5));
	}
};

class RDOFunCalcSin: public RDOFunCalc
{
public:
	RDOFunCalcSin( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(sin(runtime->getFuncArgument(0)));
	}
};

class RDOFunCalcSqrt: public RDOFunCalc
{
public:
	RDOFunCalcSqrt( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(sqrt(runtime->getFuncArgument(0)));
	}
};

class RDOFunCalcTan: public RDOFunCalc
{
public:
	RDOFunCalcTan( RDORuntimeParent* _parent ): RDOFunCalc( _parent ) {}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return RDOValue(tan(runtime->getFuncArgument(0)));
	}
};

// ----------------------------------------------------------------------------
// ---------- Sequence
// ----------------------------------------------------------------------------
class RDOCalcSeqInit: public RDOCalc
{
private:
	int            base;
	RandGenerator* gen;

public:
	RDOCalcSeqInit( RDORuntimeParent* _parent, int _base, RandGenerator* _gen ):
		RDOCalc( _parent ),
		base( _base ),
		gen( _gen )
	{
	}
	~RDOCalcSeqInit();
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
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

public:
	RDOCalcSeqNextUniform( RDORuntimeParent* _parent, RandGeneratorUniform* _gen ):
		RDOCalcSeqNext( _parent ),
		gen( _gen )
	{
	}
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
};

class RDOCalcSeqNextExponential: public RDOCalcSeqNext
{
private:
	RandGeneratorExponential* gen;

public:
	RDOCalcSeqNextExponential( RDORuntimeParent* _parent, RandGeneratorExponential* _gen ):
		RDOCalcSeqNext( _parent ),
		gen( _gen )
	{
	}
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
};

class RDOCalcSeqNextNormal: public RDOCalcSeqNext
{
private:
	RandGeneratorNormal* gen;

public:
	RDOCalcSeqNextNormal( RDORuntimeParent* _parent, RandGeneratorNormal* _gen ):
		RDOCalcSeqNext( _parent ),
		gen( _gen )
	{
	}
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
};

class RDOCalcSeqNextByHist: public RDOCalcSeqNext
{
private:
	RandGeneratorByHistCommon* gen;

public:
	RDOCalcSeqNextByHist( RDORuntimeParent* _parent, RandGeneratorByHistCommon* _gen ):
		RDOCalcSeqNext( _parent ),
		gen( _gen )
	{
	}
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcFuncParam
// ----------------------------------------------------------------------------
class RDOCalcFuncParam: public RDOCalc
{
private:
	int numberOfParam;

public:
	RDOCalcFuncParam( RDORuntimeParent* _parent, int _numberOfParam):
		RDOCalc( _parent ),
		numberOfParam( _numberOfParam )
	{
	}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		return runtime->getFuncArgument(numberOfParam); 
	}

};

// ----------------------------------------------------------------------------
// ---------- RDOCalcBinary (Binary Ariphmetic calcs)
// ----------------------------------------------------------------------------
class RDOCalcBinary: public RDOCalc
{
protected:
	const RDOCalc* const left;
	const RDOCalc* const right;

public:
	RDOCalcBinary( RDORuntimeParent* _parent, const RDOCalc* const _left, const RDOCalc* const _right ):
		RDOCalc( _parent ),
		left( _left ),
		right( _right )
	{
	}
};

class RDOCalcPlus: public RDOCalcBinary
{
public:
	RDOCalcPlus( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc *_right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		if ( left && right ) { 
			setSrcInfo( left->src_info(), " + ", right->src_info() );
		}
	}
	RDOValue calcValue( RDORuntime* runtime ) const { return RDOValue(left->calcValueBase( runtime ) + right->calcValueBase( runtime )); }
};

class RDOCalcMinus: public RDOCalcBinary
{
public:
	RDOCalcMinus( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		if ( left && right ) {
			setSrcInfo( left->src_info(), " - ", right->src_info() );
		}
	}
	RDOValue calcValue( RDORuntime* runtime ) const { return RDOValue(left->calcValueBase( runtime ) - right->calcValueBase( runtime )); }
};

class RDOCalcMult: public RDOCalcBinary
{
public:
	RDOCalcMult( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		if ( left && right ) {
			setSrcInfo( left->src_info(), " * ", right->src_info() );
		}
	}
	RDOValue calcValue( RDORuntime* runtime ) const { return RDOValue(left->calcValueBase( runtime ) * right->calcValueBase( runtime )); }
};

class RDOCalcDiv: public RDOCalcBinary
{
public:
	RDOCalcDiv( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		if ( left && right ) {
			setSrcInfo( left->src_info(), " / ", right->src_info() );
		}
	}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		RDOValue rVal = right->calcValueBase( runtime );
		if ( rVal == 0 ) {
			runtime->error( "Деление на ноль", this );
//			runtime->error("Division by zero", this);
		}
		return RDOValue( left->calcValueBase( runtime ) / rVal );
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
		if ( left && right ) {
			setSrcInfo( left->src_info(), " and ", right->src_info() );
		}
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
		if ( left && right ) {
			setSrcInfo( left->src_info(), " or ", right->src_info() );
		}
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
		if ( left && right ) {
			setSrcInfo( left->src_info(), " == ", right->src_info() );
		}
	}
	RDOValue calcValue( RDORuntime* runtime ) const {
		return ( left->calcValueBase( runtime ) == right->calcValueBase( runtime ) );
	}
};

class RDOCalcIsNotEqual: public RDOCalcBinary
{
public:
	RDOCalcIsNotEqual( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		if ( left && right ) {
			setSrcInfo( left->src_info(), " <> ", right->src_info() );
		}
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
			setSrcInfo( left->src_info(), " < ", right->src_info() );
		}
	}
	RDOValue calcValue( RDORuntime* runtime ) const { return (left->calcValueBase( runtime ) < right->calcValueBase( runtime )); }
};

class RDOCalcIsGreater: public RDOCalcBinary
{
public:
	RDOCalcIsGreater( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		if ( left && right ) {
			setSrcInfo( left->src_info(), " > ", right->src_info() );
		}
	}
	RDOValue calcValue( RDORuntime* runtime ) const { return (left->calcValueBase( runtime ) > right->calcValueBase( runtime ));	}
};

class RDOCalcIsLEQ: public RDOCalcBinary
{
public:
	RDOCalcIsLEQ( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcBinary( _parent, _left, _right )
	{
		if ( left && right ) {
			setSrcInfo( left->src_info(), " <= ", right->src_info() );
		}
	}
	RDOValue calcValue( RDORuntime* runtime ) const { return (left->calcValueBase( runtime ) <= right->calcValueBase( runtime ));	}
};

class RDOCalcIsGEQ: public RDOCalcBinary
{
public:
	RDOCalcIsGEQ( RDORuntimeParent* _parent, RDOCalc* _left, RDOCalc* _right ):
	RDOCalcBinary( _parent, _left, _right )
	{
		if ( left && right ) {
			setSrcInfo( left->src_info(), " >= ", right->src_info() );
		}
	}
	RDOValue calcValue( RDORuntime* runtime ) const { return (left->calcValueBase( runtime ) >= right->calcValueBase( runtime ));	}
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
public:
	RDOCalcUMinus( RDORuntimeParent* _parent, RDOCalc* _oper ): RDOCalcUnary( _parent, _oper ) {}
	RDOValue calcValue( RDORuntime* runtime ) const { return RDOValue(-oper->calcValueBase( runtime )); }
};

class RDOCalcDoubleToInt: public RDOCalcUnary
{
public:
	RDOCalcDoubleToInt( RDORuntimeParent* _parent, RDOCalc* _oper ): RDOCalcUnary( _parent, _oper ) {}
	RDOValue calcValue( RDORuntime* runtime ) const { return RDOValue((int)oper->calcValueBase( runtime )); }
};

class RDOCalcCheckDiap: public RDOCalcUnary
{
private:
	RDOValue minVal, maxVal;

public:
	RDOCalcCheckDiap( RDORuntimeParent* _parent, RDOValue _minVal, RDOValue _maxVal, RDOCalc* _oper ):
		RDOCalcUnary( _parent, _oper ),
		minVal( _minVal ),
		maxVal( _maxVal )
	{
	}
	virtual RDOValue calcValue( RDORuntime* runtime ) const {
		RDOValue val = oper->calcValueBase( runtime );
		if ( val < minVal || val > maxVal ) {
			runtime->error( ("Parameter out of range: " + toString(val)).c_str(), this );
		}
		return val; 
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
// ---------- RDOCalcCreateNumberedResource (System calcs - создание нового временного ресурса или постоянного в начальный момент времени по индексу с параметрами)
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
// ---------- RDOCalcCreateEmptyResource (System calcs - создание нового временного ресурса с пустым списком параметров)
// ----------------------------------------------------------------------------
class RDOCalcCreateEmptyResource: public RDOCalc
{
private:
	int  type;
	bool traceFlag;
	int  rel_res_id;
	int  numParameters;

public:
	RDOCalcCreateEmptyResource( RDORuntimeParent* _parent, int _type, bool _traceFlag, int _rel_res_id, int _numParameters );
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
};

// ----------------------------------------------------------------------------
// ---------- Выбор ресурсов
// ----------------------------------------------------------------------------
class RDOSelectResourceCalc: public RDOCalc
{
public:
	enum Type {
		st_empty = 0,
		st_first,
		st_with_min, 
		st_with_max
	};

protected:
	int       rel_res_id;
	RDOCalc*  choice;
	RDOCalc*  selection_calc;
	Type      selection_type;
	RDOSelectResourceCalc( RDORuntimeParent* _parent, int _rel_res_id, RDOCalc* _choice = NULL, RDOCalc* _selection_calc = NULL, Type _selection_type = st_empty );
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
	RDOSelectResourceDirectCalc( RDORuntimeParent* _parent, int _rel_res_id, int _res_id, RDOCalc* _choice = NULL, RDOCalc* _selection_calc = NULL, Type _selection_type = st_empty ):
		RDOSelectResourceCalc( _parent, _rel_res_id, _choice, _selection_calc, _selection_type ),
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
	RDOSelectResourceByTypeCalc( RDORuntimeParent* _parent, int _rel_res_id, int _resType, RDOCalc* _choice = NULL, RDOCalc* _selection_calc = NULL, Type _selection_type = st_empty ):
		RDOSelectResourceCalc( _parent, _rel_res_id, _choice, _selection_calc, _selection_type ),
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
	RDOCalc* choice;
	std::vector<RDOSelectResourceCommon *> resSelectors;
	bool useCommonWithMax;
	void getBest(std::vector<std::vector<int> > &allNumbs, int level, std::vector<int> &res, RDOValue &bestVal, RDORuntime *sim, bool &hasBest) const;

public:
	RDOSelectResourceCommonCalc( RDORuntimeParent* _parent, const std::vector< RDOSelectResourceCommon* >& _resSelectors, bool _useCommonWithMax, RDOCalc* _choice ):
		RDOCalc( _parent ),
		resSelectors( _resSelectors ),
		useCommonWithMax( _useCommonWithMax ),
		choice( _choice )
	{
		if ( choice ) setSrcInfo( choice->src_info() );
	}
	virtual RDOValue calcValue( RDORuntime* runtime ) const;
};

class RDOSelectResourceDirectCommonCalc: public RDOSelectResourceDirectCalc, public RDOSelectResourceCommon
{
public:
	RDOSelectResourceDirectCommonCalc( RDORuntimeParent* _parent, int _relNumb, int _resNumb, RDOCalc* _choice = NULL, RDOCalc* _selection_calc = NULL, Type _selection_type = st_empty ):
		RDOSelectResourceDirectCalc( _parent, _relNumb, _resNumb, _choice, _selection_calc, _selection_type )
	{
	}
	std::vector<int> getPossibleNumbers( RDORuntime* sim ) const;
	virtual bool callChoice( RDORuntime* sim ) const;
};

class RDOSelectResourceByTypeCommonCalc: public RDOSelectResourceByTypeCalc, public RDOSelectResourceCommon
{
public:
	RDOSelectResourceByTypeCommonCalc( RDORuntimeParent* _parent, int _relNumb, int _resType, RDOCalc* _choice = NULL, RDOCalc* _selection_calc = NULL, Type _selection_type = st_empty ):
		RDOSelectResourceByTypeCalc( _parent, _relNumb, _resType, _choice, _selection_calc, _selection_type )
	{
	}
	std::vector<int> getPossibleNumbers( RDORuntime* sim ) const;
	virtual bool callChoice( RDORuntime* sim ) const;
};

} // namespace rdoRuntime

#endif // RDOCALC_H
