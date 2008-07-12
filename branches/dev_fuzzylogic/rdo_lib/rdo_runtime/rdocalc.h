#ifndef RDOCALC_H
#define RDOCALC_H

#include "rdo_value.h"
#include "rdoruntime_object.h"
#include "rdo_random_distribution.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOCalc
// ----------------------------------------------------------------------------
class RDOCalc: public RDORuntimeObject, public RDOSrcInfo
{
public:
	RDOCalc( RDORuntimeParent* parent );
	virtual ~RDOCalc();

	RDOValue& calcValue( RDORuntime* runtime );

	bool operator== ( const RDOCalc& calc ) const
	{
		return compare( calc );
	}

protected:
	RDOValue m_value;

	virtual RDOValue& doCalc ( RDORuntime* runtime ) = 0;
	virtual bool      compare( const RDOCalc& calc ) const
	{
		return false;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcConst
// ----------------------------------------------------------------------------
class RDOCalcConst: public RDOCalc
{
public:
	RDOCalcConst( RDORuntimeParent* parent, const RDOValue& val ):
		RDOCalc( parent )
	{
		m_value = val;
	};

private:
	virtual RDOValue& doCalc( RDORuntime* runtime ) {
		return m_value;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetResParam (Параметры постоянного ресурса)
// ----------------------------------------------------------------------------
class RDOCalcGetResParam: public RDOCalc
{
public:
	RDOCalcGetResParam( RDORuntimeParent* parent, int _resNumb, int _parNumb ):
		RDOCalc( parent ),
		m_resID( _resNumb ),
		m_paramID( _parNumb )
	{
	}

protected:
	int m_resID;
	int m_paramID;

	virtual RDOValue& doCalc( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTempResParamFRM (Параметры временного ресурса для FRM)
// ----------------------------------------------------------------------------
class RDOCalcGetTempResParamFRM: public RDOCalcGetResParam
{
public:
	RDOCalcGetTempResParamFRM( RDORuntime* parent, int _resNumb, int _parNumb );

private:
	virtual RDOValue& doCalc( RDORuntime* runtime );

	virtual void notify( RDORuntimeObject* from, unsigned int message, void* param = NULL )
	{
		if ( (int)param == m_resID )
		{
			m_resID = -1;
		}
	};
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetUnknowResParam (Параметры несуществующего ресурса)
// ----------------------------------------------------------------------------
class RDOCalcGetUnknowResParam: public RDOCalc
{
public:
	RDOCalcGetUnknowResParam( RDORuntimeParent* parent, const std::string& resName, const std::string& parName ):
		RDOCalc( parent ),
		m_resName( resName ),
		m_parName( parName )
	{
	}

private:
	std::string m_resName;
	std::string m_parName;

	virtual RDOValue& doCalc( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetGroupResParam
// ----------------------------------------------------------------------------
class RDOCalcGetGroupResParam: public RDOCalc
{
public:
	RDOCalcGetGroupResParam( RDORuntimeParent* parent, int parNumb ):
		RDOCalc( parent ),
		m_parNumb( parNumb )
	{
	}

private:
	int m_parNumb;

	virtual RDOValue& doCalc( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetRelevantResParam
// ----------------------------------------------------------------------------
class RDOCalcGetRelevantResParam: public RDOCalc
{
public:
	RDOCalcGetRelevantResParam( RDORuntimeParent* parent, int relNumb, int parNumb ):
		RDOCalc( parent ),
		m_relNumb( relNumb ),
		m_parNumb( parNumb )
	{
	}

private:
	int m_relNumb;
	int m_parNumb;

	virtual RDOValue& doCalc( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- RDOSetRelParamCalc
// ----------------------------------------------------------------------------
class RDOSetRelParamCalc: public RDOCalc
{
public:
	RDOSetRelParamCalc( RDORuntimeParent* parent, int relNumb, int parNumb, RDOCalc* calc ):
		RDOCalc( parent ),
		m_relNumb( relNumb ),
		m_parNumb( parNumb ),
		m_calc( calc )
	{
		m_value = 1;
		if ( m_calc ) setSrcInfo( m_calc->src_info() );
	}

protected:
	int      m_relNumb;
	int      m_parNumb;
	RDOCalc* m_calc;

private:
	virtual RDOValue& doCalc( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- RDOSetRelParamDiapCalc
// ----------------------------------------------------------------------------
class RDOSetRelParamDiapCalc: public RDOSetRelParamCalc
{
public:
	RDOSetRelParamDiapCalc( RDORuntimeParent* parent, int relNumb, int parNumb, RDOCalc* calc, RDOValue min_value, RDOValue max_value ):
		RDOSetRelParamCalc( parent, relNumb, parNumb, calc ),
		m_min_value( min_value ),
		m_max_value( max_value )
	{
		m_value = 1;
	}

private:
	RDOValue m_min_value;
	RDOValue m_max_value;

	virtual RDOValue& doCalc( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- RDOSetResourceParamCalc
// ----------------------------------------------------------------------------
class RDOSetResourceParamCalc: public RDOCalc
{
public:
	RDOSetResourceParamCalc( RDORuntimeParent* parent, int resNumb, int parNumb, RDOCalc* calc ):
		RDOCalc( parent ),
		m_resNumb( resNumb ),
		m_parNumb( parNumb ),
		m_calc( calc )
	{
		m_value = 1;
		if ( m_calc ) setSrcInfo( m_calc->src_info() );
	}

private:
	int      m_resNumb;
	int      m_parNumb;
	RDOCalc* m_calc;

	virtual RDOValue& doCalc( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcEraseRes
// ----------------------------------------------------------------------------
class RDOCalcEraseRes: public RDOCalc
{
public:
	RDOCalcEraseRes( RDORuntimeParent* parent, int rel_res_id, const std::string& rel_res_name ):
		RDOCalc( parent ),
		m_rel_res_id( rel_res_id ),
		m_rel_res_name( rel_res_name )
	{
		m_value = 1;
	}
	std::string getName() const { return m_rel_res_name; }

private:
	int          m_rel_res_id;
	std::string  m_rel_res_name;

	virtual RDOValue& doCalc( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- RDOSetPatternParamCalc
// ----------------------------------------------------------------------------
class RDOSetPatternParamCalc: public RDOCalc
{
public:
	RDOSetPatternParamCalc( RDORuntimeParent* parent, int parNumb, RDOValue val ):
		RDOCalc( parent ),
		m_parNumb( parNumb ),
		m_val( val )
	{
		m_value = 0;
	}

private:
	int      m_parNumb;
	RDOValue m_val;

	virtual RDOValue& doCalc( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcPatParam
// ----------------------------------------------------------------------------
class RDOCalcPatParam: public RDOCalc
{
public:
	RDOCalcPatParam( RDORuntimeParent* parent, int numberOfParam ):
		RDOCalc( parent ),
		m_numberOfParam( numberOfParam )
	{
	}

private:
	int m_numberOfParam;

	virtual RDOValue& doCalc( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTimeNow
// ----------------------------------------------------------------------------
class RDOCalcGetTimeNow: public RDOCalc
{
public:
	RDOCalcGetTimeNow( RDORuntimeParent* parent ): RDOCalc( parent ) {}

private:
	virtual RDOValue& doCalc( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetSeconds
// ----------------------------------------------------------------------------
class RDOCalcGetSeconds: public RDOCalc
{
public:
	RDOCalcGetSeconds( RDORuntimeParent* parent ): RDOCalc( parent ) {}

private:
	virtual RDOValue& doCalc( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- RDOFunCalc
// ----------------------------------------------------------------------------
class RDOFunCalc: public RDOCalc
{
protected:
	RDOFunCalc( RDORuntimeParent* parent ):
		RDOCalc( parent )
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
public:
	RDOFuncTableCalc( RDORuntimeParent* parent, RDOCalc* argCalc ):
		RDOFunCalc( parent ),
		m_argCalc( argCalc )
	{
	}
	void addResultCalc( RDOCalcConst* res )
	{
		m_results.push_back( res );
	}

private:
	std::vector< RDOCalcConst* > m_results;
	RDOCalc*                     m_argCalc;

	virtual RDOValue& doCalc( RDORuntime* runtime )
	{
		int index = m_argCalc->calcValue( runtime ).getInt();
		return m_results.at(index)->calcValue( runtime );
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOFunListCalc
// ----------------------------------------------------------------------------
// Функция типа список
// ----------------------------------------------------------------------------
class RDOFunListCalc: public RDOFunCalc
{
public:
	RDOFunListCalc( RDORuntimeParent* parent, RDOCalcConst* default_value ):
		RDOFunCalc( parent ),
		m_default_value( default_value )
	{
	}

	void addCase( RDOCalc* case_calc, RDOCalcConst* result_calc )
	{
		m_cases.push_back( case_calc ); 
		m_results.push_back( result_calc );
	}

private:
	std::vector< RDOCalc* >      m_cases;
	std::vector< RDOCalcConst* > m_results;
	RDOCalcConst*                m_default_value;

	virtual RDOValue& doCalc( RDORuntime* runtime )
	{
		int size = m_cases.size();
		for ( int i = 0; i < size; i++ ) {
			RDOCalc* cas = m_cases[i];
			if ( cas->calcValue( runtime ).getAsBool() ) {
				return m_results[i]->calcValue( runtime );
			}
		}
		return m_default_value->calcValue( runtime );
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOFunAlgorithmicCalc
// ----------------------------------------------------------------------------
// Алгоритмическая функция
// ----------------------------------------------------------------------------
class RDOFunAlgorithmicCalc: public RDOFunCalc
{
public:
	RDOFunAlgorithmicCalc( RDORuntimeParent* parent ):
		RDOFunCalc( parent )
	{
		m_value = 0;
	}
	void addCalcIf( RDOCalc* cond, RDOCalc* act )
	{
		m_conditions.push_back( cond );
		m_actions.push_back( act );
	}

protected:
	std::vector< RDOCalc* > m_conditions;
	std::vector< RDOCalc* > m_actions;

	virtual RDOValue& doCalc( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- RDOFunAlgorithmicDiapCalc
// ----------------------------------------------------------------------------
// Алгоритмическая функция с проверкой на диапазон возвращаемого значения
// ----------------------------------------------------------------------------
class RDOFunAlgorithmicDiapCalc: public RDOFunAlgorithmicCalc
{
public:
	RDOFunAlgorithmicDiapCalc( RDORuntimeParent* parent, const RDOValue& min_value, const RDOValue& max_value ):
		RDOFunAlgorithmicCalc( parent ),
		m_min_value( min_value ),
		m_max_value( max_value )
	{
	}

private:
	RDOValue m_min_value;
	RDOValue m_max_value;

	virtual RDOValue& doCalc( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcGroup
// ----------------------------------------------------------------------------
class RDOFunCalcGroup: public RDOFunCalc
{
protected:
	int      m_nResType;
	RDOCalc* m_condition;

	RDOFunCalcGroup( RDORuntimeParent* parent, int nResType, RDOCalc* condition ):
		RDOFunCalc( parent ),
		m_nResType( nResType ),
		m_condition( condition )
	{
	}
};

#define DEFINE_CALC_GROUP( CalcName ) \
class RDOFunCalc##CalcName: public RDOFunCalcGroup \
{ \
public: \
	RDOFunCalc##CalcName( RDORuntimeParent* parent, int nResType, RDOCalc* condition): \
		RDOFunCalcGroup( parent, nResType, condition ) \
	{ \
	} \
 \
private: \
	virtual RDOValue& doCalc( RDORuntime* runtime ); \
};

DEFINE_CALC_GROUP( Exist     );
DEFINE_CALC_GROUP( NotExist  );
DEFINE_CALC_GROUP( ForAll    );
DEFINE_CALC_GROUP( NotForAll );

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcSelect
// ----------------------------------------------------------------------------
class RDOResource;

class RDOFunCalcSelect: public RDOFunCalcGroup
{
public:
	mutable std::list< RDOResource* > res_list;
	void prepare( RDORuntime* sim ) const;

	RDOFunCalcSelect( RDORuntimeParent* parent, int nResType, RDOCalc* condition ):
		RDOFunCalcGroup( parent, nResType, condition )
	{
		m_value = 1;
	}

private:
	virtual RDOValue& doCalc( RDORuntime* runtime );
};

class RDOFunCalcSelectBase: public RDOFunCalc
{
public:
	RDOFunCalcSelectBase( RDORuntimeParent* parent, RDOFunCalcSelect* select, RDOCalc* condition ):
		RDOFunCalc( parent ),
		m_select( select ),
		m_condition( condition )
	{
	}

protected:
	RDOFunCalcSelect* m_select;
	RDOCalc*          m_condition;
};

#define DEFINE_CALC_SELECT_GROUP( CalcName ) \
class RDOFunCalcSelect##CalcName: public RDOFunCalcSelectBase \
{ \
public: \
	RDOFunCalcSelect##CalcName( RDORuntimeParent* parent, RDOFunCalcSelect* select, RDOCalc* condition ): \
		RDOFunCalcSelectBase( parent, select, condition ) \
	{ \
	} \
 \
private: \
	virtual RDOValue& doCalc( RDORuntime* runtime ); \
};

#define DEFINE_CALC_SELECT_METHOD( CalcName ) \
class RDOFunCalcSelect##CalcName: public RDOFunCalcSelectBase \
{ \
public: \
	RDOFunCalcSelect##CalcName( RDORuntimeParent* parent, RDOFunCalcSelect* select ): \
		RDOFunCalcSelectBase( parent, select, NULL ) \
	{ \
	} \
 \
private: \
	virtual RDOValue& doCalc( RDORuntime* runtime ); \
};

DEFINE_CALC_SELECT_GROUP ( Exist     );
DEFINE_CALC_SELECT_GROUP ( NotExist  );
DEFINE_CALC_SELECT_GROUP ( ForAll    );
DEFINE_CALC_SELECT_GROUP ( NotForAll );
DEFINE_CALC_SELECT_METHOD( Empty     );
DEFINE_CALC_SELECT_METHOD( Size      );

// ----------------------------------------------------------------------------
// ---------- Стандартные функции языка
// ----------------------------------------------------------------------------
#define DEFINE_RDO_STD_FUN( CalcName ) \
class RDOFunCalc##CalcName: public RDOFunCalc \
{ \
public: \
	RDOFunCalc##CalcName( RDORuntimeParent* parent ): \
		RDOFunCalc( parent ) \
	{ \
	} \
\
private: \
	virtual RDOValue& doCalc( RDORuntime* runtime ); \
};

DEFINE_RDO_STD_FUN( Sin      );
DEFINE_RDO_STD_FUN( Cos      );
DEFINE_RDO_STD_FUN( Tan      );
DEFINE_RDO_STD_FUN( Cotan    );
DEFINE_RDO_STD_FUN( ArcCos   );
DEFINE_RDO_STD_FUN( ArcSin   );
DEFINE_RDO_STD_FUN( ArcTan   );
DEFINE_RDO_STD_FUN( Abs      );
DEFINE_RDO_STD_FUN( Sqrt     );
DEFINE_RDO_STD_FUN( Round    );
DEFINE_RDO_STD_FUN( Exp      );
DEFINE_RDO_STD_FUN( Floor    );
DEFINE_RDO_STD_FUN( Frac     );
DEFINE_RDO_STD_FUN( IAbs     );
DEFINE_RDO_STD_FUN( IMax     );
DEFINE_RDO_STD_FUN( IMin     );
DEFINE_RDO_STD_FUN( Int      );
DEFINE_RDO_STD_FUN( IntPower );
DEFINE_RDO_STD_FUN( Ln       );
DEFINE_RDO_STD_FUN( Log2     );
DEFINE_RDO_STD_FUN( LogN     );
DEFINE_RDO_STD_FUN( Log10    );
DEFINE_RDO_STD_FUN( Max      );
DEFINE_RDO_STD_FUN( Min      );
DEFINE_RDO_STD_FUN( Power    );

// ----------------------------------------------------------------------------
// ---------- RDOCalcBinary
// ----------------------------------------------------------------------------
class RDOCalcBinary: public RDOCalc
{
public:
	RDOCalcBinary( RDORuntimeParent* parent, RDOCalc* left, RDOCalc* right ):
		RDOCalc( parent ),
		m_left( left ),
		m_right( right )
	{
	}
	RDOCalc*      getLeft() const          { return m_left; }
	RDOCalcConst* getRightAsConst() const  { return dynamic_cast<RDOCalcConst*>(m_right); }
	void setRight( RDOCalc* right )        { m_right = right; }

protected:
	RDOCalc* m_left;
	RDOCalc* m_right;
};

#define DEFINE_BINARY_CALC( CalcName, CalcOpr ) \
class RDOCalc##CalcName: public RDOCalcBinary \
{ \
public: \
	RDOCalc##CalcName( RDORuntimeParent* parent, RDOCalc* const left, RDOCalc* const right): \
		RDOCalcBinary( parent, left, right ) \
	{ \
		setSrcInfo( getStaticSrcInfo( m_left, m_right ) ); \
	} \
	static RDOSrcInfo getStaticSrcInfo( const RDOCalc* left, const RDOCalc* right ) \
	{ \
		RDOSrcInfo src_info; \
		src_info.setSrcInfo( left->src_info(), CalcOpr, right->src_info() ); \
		return src_info; \
	} \
 \
private: \
	virtual RDOValue& doCalc( RDORuntime* runtime ); \
};

// ----------------------------------------------------------------------------
// ---------- Арифметические функции
// ----------------------------------------------------------------------------
DEFINE_BINARY_CALC( Plus , " + "  );
DEFINE_BINARY_CALC( Minus, " - "  );
DEFINE_BINARY_CALC( Mult , " * "  );
DEFINE_BINARY_CALC( Div  , " / "  );

class RDOCalcPlusEnumSafe: public RDOCalcPlus
{
public:
	RDOCalcPlusEnumSafe( RDORuntimeParent* parent, RDOCalc* _left, RDOCalc *_right ):
		RDOCalcPlus( parent, _left, _right )
	{
	}

private:
	virtual RDOValue& doCalc( RDORuntime* runtime );
};

class RDOCalcMultEnumSafe: public RDOCalcMult
{
public:
	RDOCalcMultEnumSafe( RDORuntimeParent* parent, RDOCalc* _left, RDOCalc* _right ):
		RDOCalcMult( parent, _left, _right )
	{
	}

private:
	virtual RDOValue& doCalc( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- Логические функции
// ----------------------------------------------------------------------------
class RDOCalcAnd: public RDOCalcBinary
{
public:
	RDOCalcAnd( RDORuntimeParent* parent, RDOCalc* left, RDOCalc* right ):
		RDOCalcBinary( parent, left, right )
	{
		m_value_true  = 1;
		m_value_false = 0;
		setSrcInfo( getStaticSrcInfo( m_left, m_right ) );
	}
	static RDOSrcInfo getStaticSrcInfo( const RDOCalc* left, const RDOCalc* right )
	{
		RDOSrcInfo src_info;
		src_info.setSrcInfo( left->src_info(), " and ", right->src_info() );
		return src_info;
	}

private:
	RDOValue m_value_true;
	RDOValue m_value_false;

	virtual RDOValue& doCalc( RDORuntime* runtime );
};

class RDOCalcOr: public RDOCalcBinary
{
public:
	RDOCalcOr( RDORuntimeParent* parent, RDOCalc* left, RDOCalc* right ):
		RDOCalcBinary( parent, left, right )
	{
		m_value_true  = 1;
		m_value_false = 0;
		setSrcInfo( getStaticSrcInfo( m_left, m_right ) );
	}
	static RDOSrcInfo getStaticSrcInfo( const RDOCalc* left, const RDOCalc* right )
	{
		RDOSrcInfo src_info;
		src_info.setSrcInfo( left->src_info(), " or ", right->src_info() );
		return src_info;
	}

private:
	RDOValue m_value_true;
	RDOValue m_value_false;

	virtual RDOValue& doCalc( RDORuntime* runtime );
};

class RDOCalcNot: public RDOCalc
{
public:
	RDOCalcNot( RDORuntimeParent* parent, RDOCalc* calc ):
		RDOCalc( parent ),
		m_calc( calc )
	{
	}

private:
	RDOCalc* m_calc;

	virtual RDOValue& doCalc( RDORuntime* runtime );
};

DEFINE_BINARY_CALC( IsEqual   , " = "  );
DEFINE_BINARY_CALC( IsNotEqual, " <> " );
DEFINE_BINARY_CALC( IsLess    , " < "  );
DEFINE_BINARY_CALC( IsGreater , " > "  );
DEFINE_BINARY_CALC( IsLEQ     , " <= " );
DEFINE_BINARY_CALC( IsGEQ     , " >= " );

// ----------------------------------------------------------------------------
// ---------- RDOCalcUnary
// ----------------------------------------------------------------------------
class RDOCalcUnary: public RDOCalc
{
protected:
	RDOCalc* m_oper;

	RDOCalcUnary( RDORuntimeParent* parent, RDOCalc* oper ):
		RDOCalc( parent ),
		m_oper( oper )
	{
		if ( m_oper ) setSrcInfo( m_oper->src_info() );
	}
};

// ----------------------------------------------------------------------------
// ---------- Унарные операции
// ----------------------------------------------------------------------------
class RDOCalcUMinus: public RDOCalcUnary
{
public:
	RDOCalcUMinus( RDORuntimeParent* parent, RDOCalc* _oper ): RDOCalcUnary( parent, _oper ) {}

private:
	virtual RDOValue& doCalc( RDORuntime* runtime ) {
		m_value = -m_oper->calcValue( runtime );
		return m_value;
	}
};

class RDOCalcDoubleToInt: public RDOCalcUnary
{
public:
	RDOCalcDoubleToInt( RDORuntimeParent* parent, RDOCalc* _oper ):
		RDOCalcUnary( parent, _oper )
	{
	}

private:
	virtual RDOValue& doCalc( RDORuntime* runtime ) {
		m_value = m_oper->calcValue( runtime ).getInt();
		return m_value;
	}
};

class RDOCalcDoubleToIntByResult: public RDOCalcUnary
{
public:
	RDOCalcDoubleToIntByResult( RDORuntimeParent* parent, RDOCalc* _oper ):
		RDOCalcUnary( parent, _oper ),
		m_round( false )
	{
	}
	void needRound()
	{
		m_round = true;
	}

private:
	bool m_round;

	virtual RDOValue& doCalc( RDORuntime* runtime )
	{
		m_value = m_round ? RDOValue( m_oper->calcValue( runtime ).getInt() ) : m_oper->calcValue( runtime );
		return m_value;
	}
};

class RDOCalcCheckDiap: public RDOCalcUnary
{
public:
	RDOCalcCheckDiap( RDORuntimeParent* parent, const RDOValue& min_value, const RDOValue& max_value, RDOCalc* oper ):
		RDOCalcUnary( parent, oper ),
		m_min_value( min_value ),
		m_max_value( max_value )
	{
	}

private:
	RDOValue m_min_value;
	RDOValue m_max_value;

	virtual RDOValue& doCalc( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- Последовательности
// ----------------------------------------------------------------------------
class RDOCalcSeqInit: public RDOCalc
{
public:
	RDOCalcSeqInit( RDORuntimeParent* parent, int base, RandGenerator* gen ):
		RDOCalc( parent ),
		m_base( base ),
		m_gen( gen )
	{
	}
	virtual ~RDOCalcSeqInit();

	void setBase( int base )
	{
		m_base = base;
	}

private:
	int            m_base;
	RandGenerator* m_gen;

	virtual RDOValue& doCalc( RDORuntime* runtime );
};

class RDOCalcSeqNext: public RDOFunCalc
{
public:
	bool   m_res_real;
	bool   m_diap;
	double m_diap_min;
	double m_diap_max;

	RDOCalcSeqNext( RDORuntimeParent* parent ):
		RDOFunCalc( parent ),
		m_res_real( true ),
		m_diap( false ),
		m_diap_min( 0 ),
		m_diap_max( 0 )
	{
	}

protected:
	virtual RDOValue getNextValue( RDORuntime* runtime ) = 0;
};

template< class T >
class RDOCalcRandomDistribution: public RDOCalcSeqNext
{
public:
	RDOCalcRandomDistribution( RDORuntimeParent* parent, T* gen ):
		RDOCalcSeqNext( parent ),
		m_gen( gen )
	{
	}
	virtual ~RDOCalcRandomDistribution()
	{
		delete m_gen;
	}

protected:
	T* m_gen;

private:
	virtual RDOValue& doCalc( RDORuntime* runtime )
	{
		RDOValue res = getNextValue( runtime );
		if ( m_diap ) {
			if ( res < m_diap_min ) {
				m_value = m_res_real ? m_diap_min : RDOValue(m_diap_min > 0 ? m_diap_min + 0.5 : m_diap_min - 0.5).getInt();
				return m_value;
			}
			if ( res > m_diap_max ) {
				m_value = m_res_real ? m_diap_max : RDOValue(m_diap_max > 0 ? m_diap_max + 0.5 : m_diap_max - 0.5).getInt();
				return m_value;
			}
			m_value = m_res_real ? res : RDOValue(res > 0 ? res + 0.5 : res - 0.5).getInt();
			return m_value;
			// В новом РДО была сделана попытка выбирать новое случайное число, если текущее вышло за диапазон. Но при этом смешается среднее (оно и в другом случае может смещаться imho). Для совместимости оставим первый вариант.
//			for ( int i = 0; i < 1000; i++ ) {
//				if ( res >= diap_min && res <= diap_max ) return res_real ? res : static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
//				res = gen->next( runtime->getFuncArgument(0), runtime->getFuncArgument(1) );
//			}
//			runtime->error( "Не удается получить значение, попадающее в назначенный диапазон", this );
//			return res_real ? diap_min : static_cast<int>(diap_min);
		} else {
			m_value = m_res_real ? res : RDOValue(res > 0 ? res + 0.5 : res - 0.5).getInt();
			return m_value;
		}
	}
};

#define DEFINE_RANDON_DISTRIBUTION( CalcName, Distribution ) \
class RDOCalcSeqNext##CalcName: public RDOCalcRandomDistribution<Distribution> \
{ \
public: \
	RDOCalcSeqNext##CalcName( RDORuntimeParent* parent, Distribution* gen ): \
		RDOCalcRandomDistribution<Distribution>( parent, gen ) \
	{ \
	} \
 \
private: \
	virtual RDOValue getNextValue( RDORuntime* runtime ); \
};

DEFINE_RANDON_DISTRIBUTION( Uniform    , RandGeneratorUniform     );
DEFINE_RANDON_DISTRIBUTION( Normal     , RandGeneratorNormal      );
DEFINE_RANDON_DISTRIBUTION( Exponential, RandGeneratorExponential );
DEFINE_RANDON_DISTRIBUTION( ByHist     , RandGeneratorCommonNext  );

// ----------------------------------------------------------------------------
// ---------- RDOCalcFuncParam
// ----------------------------------------------------------------------------
class RDOCalcFuncParam: public RDOCalc
{
public:
	RDOCalcFuncParam( RDORuntimeParent* parent, int param_number, const RDOSrcInfo& _src_info ):
		RDOCalc( parent ),
		m_param_number( param_number )
	{
		setSrcInfo( _src_info );
	}

private:
	int m_param_number;

	virtual RDOValue& doCalc( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetConst / RDOCalcSetConst
// ----------------------------------------------------------------------------
class RDOCalcGetConst: public RDOCalc
{
public:
	RDOCalcGetConst( RDORuntimeParent* parent, int number ):
		RDOCalc( parent ),
		m_number( number )
	{
	}

private:
	int m_number;
	virtual RDOValue& doCalc( RDORuntime* runtime );
};

class RDOCalcSetConst: public RDOCalc
{
public:
	RDOCalcSetConst( RDORuntimeParent* parent, int number, RDOCalc* calc ):
		RDOCalc( parent ),
		m_number( number ),
		m_calc( calc )
	{
		m_value = 0;
		if ( m_calc ) setSrcInfo( m_calc->src_info() );
	}

private:
	int      m_number;
	RDOCalc* m_calc;

	virtual RDOValue& doCalc( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcInt (приведение к целому)
// ----------------------------------------------------------------------------
class RDOCalcInt: public RDOCalcUnary
{
public:
	RDOCalcInt( RDORuntimeParent* parent, RDOCalc* oper ): RDOCalcUnary( parent, oper ) {}

private:
	virtual RDOValue& doCalc( RDORuntime* runtime ) {
		RDOValue res = m_oper->calcValue( runtime );
		m_value = res > 0 ? RDOValue( (int)(res.getDouble() + 0.5) ) : RDOValue( (int)(res.getDouble() - 0.5) );
		return m_value;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcFunctionCall
// ----------------------------------------------------------------------------
class RDOCalcFunctionCall: public RDOCalc
{
public:
	RDOCalcFunctionCall( RDORuntimeParent* parent, RDOFunCalc* function ):
		RDOCalc( parent ),
		m_function( function )
	{
	}
	void addParameter( RDOCalc* calc )
	{
		m_parameters.push_back( calc );
	}
	void setFunctionCalc( RDOFunCalc* calc )
	{
		m_function = calc;
	}

private:
	std::vector< RDOCalc* > m_parameters;
	RDOFunCalc*             m_function;

	virtual RDOValue& doCalc( RDORuntime* runtime );
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateNumberedResource (RSS: создание нового временного ресурса или постоянного в начальный момент времени по индексу с параметрами)
// ----------------------------------------------------------------------------
class RDOCalcCreateNumberedResource: public RDOCalc
{
protected:
	int  type;
	bool traceFlag;
	std::vector< RDOValue > paramsCalcs;
	unsigned int  number;
	bool isPermanent;

	virtual RDOValue& doCalc( RDORuntime* runtime );

public:
	RDOCalcCreateNumberedResource( RDORuntimeParent* parent, int _type, bool _traceFlag, const std::vector< RDOValue >& _paramsCalcs, int _number, bool _isPermanent );
	virtual RDOResource* createResource( RDORuntime* runtime ) const;

	unsigned int getNumber() const { return number; }
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateProcessResource (SEIZE: создание нового ресурса процесса)
// ----------------------------------------------------------------------------
class RDOCalcCreateProcessResource: public RDOCalcCreateNumberedResource
{
private:
	virtual RDOResource* createResource( RDORuntime* runtime ) const;

public:
	RDOCalcCreateProcessResource( RDORuntimeParent* parent, int _type, bool _traceFlag, const std::vector< RDOValue >& _paramsCalcs, int _number, bool _isPermanent );
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
	RDOCalcCreateEmptyResource( RDORuntimeParent* parent, int _type, bool _traceFlag, const std::vector< RDOValue >& _params_default, int _rel_res_id );
	virtual RDOValue& doCalc( RDORuntime* runtime );
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
	RDOSelectResourceCalc( RDORuntimeParent* parent, int _rel_res_id, RDOCalc* _choice_calc = NULL, RDOCalc* _order_calc = NULL, Type _order_type = order_empty );
};

class RDOSelectResourceNonExistCalc: public RDOSelectResourceCalc
{
public:
	RDOSelectResourceNonExistCalc( RDORuntimeParent* parent, int _rel_res_id ):
		RDOSelectResourceCalc( parent, _rel_res_id, NULL, NULL )
	{
		m_value = 1;
	}

private:
	virtual RDOValue& doCalc( RDORuntime* runtime );
};

class RDOSelectResourceDirectCalc: public RDOSelectResourceCalc
{
public:
	RDOSelectResourceDirectCalc( RDORuntimeParent* parent, int _rel_res_id, int _res_id, RDOCalc* _choice_calc = NULL, RDOCalc* _order_calc = NULL, Type _order_type = order_empty ):
		RDOSelectResourceCalc( parent, _rel_res_id, _choice_calc, _order_calc, _order_type ),
		res_id( _res_id )
	{
	}
	virtual RDOValue& doCalc( RDORuntime* runtime );

protected:
	int res_id;

	virtual bool compare( const RDOCalc& calc ) const
	{
		const RDOSelectResourceDirectCalc* directCalc = dynamic_cast<const RDOSelectResourceDirectCalc*>(&calc);
		if ( !directCalc )
		{
			return false;
		}
		return rel_res_id == directCalc->rel_res_id && res_id == directCalc->res_id;
	}
};

class RDOSelectResourceByTypeCalc: public RDOSelectResourceCalc
{
protected:
	int resType;

public:
	RDOSelectResourceByTypeCalc( RDORuntimeParent* parent, int _rel_res_id, int _resType, RDOCalc* _choice_calc = NULL, RDOCalc* _order_calc = NULL, Type _order_type = order_empty ):
		RDOSelectResourceCalc( parent, _rel_res_id, _choice_calc, _order_calc, _order_type ),
		resType( _resType )
	{
	}
	virtual RDOValue& doCalc( RDORuntime* runtime );
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
	void getBest( std::vector< std::vector< int > >& allNumbs, unsigned int level, std::vector< int >& res, RDOValue& bestVal, RDORuntime* sim, bool& hasBest ) const;
	bool getFirst( std::vector< std::vector< int > >& allNumbs, unsigned int level, RDORuntime* sim ) const;

public:
	RDOSelectResourceCommonCalc( RDORuntimeParent* parent, const std::vector< RDOSelectResourceCommon* >& _resSelectors, bool _useCommonWithMax, RDOCalc* _choice_calc ):
		RDOCalc( parent ),
		resSelectors( _resSelectors ),
		useCommonWithMax( _useCommonWithMax ),
		choice_calc( _choice_calc )
	{
		if ( choice_calc ) setSrcInfo( choice_calc->src_info() );
	}
	virtual RDOValue& doCalc( RDORuntime* runtime );
};

class RDOSelectResourceDirectCommonCalc: public RDOSelectResourceDirectCalc, public RDOSelectResourceCommon
{
public:
	RDOSelectResourceDirectCommonCalc( RDORuntimeParent* parent, int _relNumb, int _resNumb, RDOCalc* _choice_calc = NULL, RDOCalc* _order_calc = NULL, Type _order_type = order_empty ):
		RDOSelectResourceDirectCalc( parent, _relNumb, _resNumb, _choice_calc, _order_calc, _order_type )
	{
	}
	std::vector<int> getPossibleNumbers( RDORuntime* sim ) const;
	virtual bool callChoice( RDORuntime* sim ) const;
};

class RDOSelectResourceByTypeCommonCalc: public RDOSelectResourceByTypeCalc, public RDOSelectResourceCommon
{
public:
	RDOSelectResourceByTypeCommonCalc( RDORuntimeParent* parent, int _relNumb, int _resType, RDOCalc* _choice_calc = NULL, RDOCalc* _order_calc = NULL, Type _order_type = order_empty ):
		RDOSelectResourceByTypeCalc( parent, _relNumb, _resType, _choice_calc, _order_calc, _order_type )
	{
	}
	std::vector<int> getPossibleNumbers( RDORuntime* sim ) const;
	virtual bool callChoice( RDORuntime* sim ) const;
};

} // namespace rdoRuntime

#endif // RDOCALC_H
