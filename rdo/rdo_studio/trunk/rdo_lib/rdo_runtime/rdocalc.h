/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdocalc.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_H_
#define _RDOCALC_H_

// ====================================================================== INCLUDES
#include <list>
// ====================================================================== SYNOPSIS
#include <namespace.h>
#include "rdo_value.h"
#include "rdoruntime_object.h"
#include "rdo_random_distribution.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalc
// ----------------------------------------------------------------------------
class RDOCalc: public RDORuntimeObject, public RDOSrcInfo
{
public:
	RDOCalc(PTR(RDORuntimeParent) parent);
	virtual ~RDOCalc();

	REF(RDOValue) calcValue(PTR(RDORuntime) runtime);

	rbool operator== (CREF(RDOCalc) calc) const
	{
		return compare(calc);
	}

protected:
	RDOValue m_value;

	virtual REF(RDOValue) doCalc (PTR(RDORuntime) runtime) = 0;
	virtual rbool         compare(CREF(RDOCalc) calc) const
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
	RDOCalcConst(PTR(RDORuntimeParent) parent, CREF(RDOValue) val):
		RDOCalc( parent )
	{
		m_value = val;
	};

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime)
	{
		return m_value;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetResParam (Параметры постоянного ресурса)
// ----------------------------------------------------------------------------
class RDOCalcGetResParam: public RDOCalc
{
public:
	RDOCalcGetResParam(PTR(RDORuntimeParent) parent, int _resNumb, int _parNumb)
		: RDOCalc  (parent  )
		, m_resID  (_resNumb)
		, m_paramID(_parNumb)
	{}

protected:
	int m_resID;
	int m_paramID;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTempResParamFRM (Параметры временного ресурса для FRM)
// ----------------------------------------------------------------------------
class RDOCalcGetTempResParamFRM: public RDOCalcGetResParam, public INotify
{
public:
	RDOCalcGetTempResParamFRM(PTR(RDORuntime) runtime, int _resNumb, int _parNumb);

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);

	virtual void notify(ruint message, PTR(void) param)
	{
		if (m_resID == *reinterpret_cast<PTR(int)>(param))
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
	RDOCalcGetUnknowResParam(PTR(RDORuntimeParent) parent, CREF(tstring) resName, CREF(tstring) parName)
		: RDOCalc  (parent )
		, m_resName(resName)
		, m_parName(parName)
	{}

private:
	tstring m_resName;
	tstring m_parName;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetGroupResParam
// ----------------------------------------------------------------------------
class RDOCalcGetGroupResParam: public RDOCalc
{
public:
	RDOCalcGetGroupResParam(PTR(RDORuntimeParent) parent, int parNumb)
		: RDOCalc  (parent )
		, m_parNumb(parNumb)
	{}

private:
	int m_parNumb;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetRelevantResParam
// ----------------------------------------------------------------------------
class RDOCalcGetRelevantResParam: public RDOCalc
{
public:
	RDOCalcGetRelevantResParam(PTR(RDORuntimeParent) parent, int relNumb, int parNumb)
		: RDOCalc  (parent )
		, m_relNumb(relNumb)
		, m_parNumb(parNumb)
	{}

private:
	int m_relNumb;
	int m_parNumb;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOSetRelParamCalc
// ----------------------------------------------------------------------------
class RDOSetRelParamCalc: public RDOCalc
{
public:
	RDOSetRelParamCalc(PTR(RDORuntimeParent) parent, int relNumb, int parNumb, PTR(RDOCalc) calc)
		: RDOCalc  (parent )
		, m_relNumb(relNumb)
		, m_parNumb(parNumb)
		, m_calc   (calc   )
	{
		m_value = 1;
		if ( m_calc ) setSrcInfo( m_calc->src_info() );
	}

protected:
	int          m_relNumb;
	int          m_parNumb;
	PTR(RDOCalc) m_calc;

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOSetRelParamDiapCalc
// ----------------------------------------------------------------------------
class RDOSetRelParamDiapCalc: public RDOSetRelParamCalc
{
public:
	RDOSetRelParamDiapCalc(PTR(RDORuntimeParent) parent, int relNumb, int parNumb, PTR(RDOCalc) calc, RDOValue min_value, RDOValue max_value)
		: RDOSetRelParamCalc(parent, relNumb, parNumb, calc)
		, m_min_value       (min_value                     )
		, m_max_value       (max_value                     )
	{
		m_value = 1;
	}

private:
	RDOValue m_min_value;
	RDOValue m_max_value;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOSetResourceParamCalc
// ----------------------------------------------------------------------------
class RDOSetResourceParamCalc: public RDOCalc
{
public:
	RDOSetResourceParamCalc(PTR(RDORuntimeParent) parent, int resNumb, int parNumb, PTR(RDOCalc) calc)
		: RDOCalc  (parent )
		, m_resNumb(resNumb)
		, m_parNumb(parNumb)
		, m_calc   (calc   )
	{
		m_value = 1;
		if ( m_calc ) setSrcInfo( m_calc->src_info() );
	}

private:
	int          m_resNumb;
	int          m_parNumb;
	PTR(RDOCalc) m_calc;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcEraseRes
// ----------------------------------------------------------------------------
class RDOCalcEraseRes: public RDOCalc
{
public:
	RDOCalcEraseRes(PTR(RDORuntimeParent) parent, int rel_res_id, CREF(tstring) rel_res_name)
		: RDOCalc       (parent      )
		, m_rel_res_id  (rel_res_id  )
		, m_rel_res_name(rel_res_name)
	{
		m_value = 1;
	}
	tstring getName() const { return m_rel_res_name; }

private:
	int      m_rel_res_id;
	tstring  m_rel_res_name;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOSetPatternParamCalc
// ----------------------------------------------------------------------------
class RDOSetPatternParamCalc: public RDOCalc
{
public:
	RDOSetPatternParamCalc(PTR(RDORuntimeParent) parent, int parNumb, RDOValue val)
		: RDOCalc  (parent )
		, m_parNumb(parNumb)
		, m_val    (val    )
	{
		m_value = 0;
	}

private:
	int      m_parNumb;
	RDOValue m_val;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcPatParam
// ----------------------------------------------------------------------------
class RDOCalcPatParam: public RDOCalc
{
public:
	RDOCalcPatParam(PTR(RDORuntimeParent) parent, int numberOfParam)
		: RDOCalc        (parent       )
		, m_numberOfParam(numberOfParam)
	{}

private:
	int m_numberOfParam;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTimeNow
// ----------------------------------------------------------------------------
class RDOCalcGetTimeNow: public RDOCalc
{
public:
	RDOCalcGetTimeNow(PTR(RDORuntimeParent) parent)
		: RDOCalc(parent)
	{}

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetSeconds
// ----------------------------------------------------------------------------
class RDOCalcGetSeconds: public RDOCalc
{
public:
	RDOCalcGetSeconds(PTR(RDORuntimeParent) parent)
		: RDOCalc(parent)
	{}

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTermNow
// ----------------------------------------------------------------------------
class RDOCalcGetTermNow: public RDOCalc
{
public:
	RDOCalcGetTermNow(PTR(RDORuntimeParent) parent)
		: RDOCalc(parent)
	{}

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOFunCalc
// ----------------------------------------------------------------------------
class RDOFunCalc: public RDOCalc
{
protected:
	RDOFunCalc(PTR(RDORuntimeParent) parent)
		: RDOCalc(parent)
	{}
};

// ----------------------------------------------------------------------------
// ---------- RDOFunListCalc
// ----------------------------------------------------------------------------
// Функция типа список
// ----------------------------------------------------------------------------
class RDOFuncTableCalc: public RDOFunCalc
{
public:
	RDOFuncTableCalc(PTR(RDORuntimeParent) parent, PTR(RDOCalc) argCalc)
		: RDOFunCalc(parent )
		, m_argCalc (argCalc)
	{}

	void addResultCalc(PTR(RDOCalcConst) res)
	{
		m_results.push_back( res );
	}

private:
	std::vector<PTR(RDOCalcConst)> m_results;
	PTR(RDOCalc)                   m_argCalc;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime)
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
	RDOFunListCalc(PTR(RDORuntimeParent) parent, PTR(RDOCalcConst) default_value)
		: RDOFunCalc     (parent       )
		, m_default_value(default_value)
	{}

	void addCase(PTR(RDOCalc) case_calc, PTR(RDOCalcConst) result_calc)
	{
		m_cases  .push_back(case_calc  ); 
		m_results.push_back(result_calc);
	}

private:
	std::vector<PTR(RDOCalc)>       m_cases;
	std::vector<PTR(RDOCalcConst)>  m_results;
	PTR(RDOCalcConst)               m_default_value;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime)
	{
		size_t size = m_cases.size();
		for (ruint i = 0; i < size; i++)
		{
			PTR(RDOCalc) cas = m_cases[i];
			if (cas->calcValue( runtime ).getAsBool())
			{
				return m_results[i]->calcValue(runtime);
			}
		}
		return m_default_value->calcValue(runtime);
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
	RDOFunAlgorithmicCalc(PTR(RDORuntimeParent) parent)
		: RDOFunCalc(parent)
	{
		m_value = 0;
	}
	void addCalcIf(PTR(RDOCalc) cond, PTR(RDOCalc) act)
	{
		m_conditions.push_back(cond);
		m_actions   .push_back(act );
	}

protected:
	std::vector<PTR(RDOCalc)> m_conditions;
	std::vector<PTR(RDOCalc)> m_actions;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOFunAlgorithmicDiapCalc
// ----------------------------------------------------------------------------
// Алгоритмическая функция с проверкой на диапазон возвращаемого значения
// ----------------------------------------------------------------------------
class RDOFunAlgorithmicDiapCalc: public RDOFunAlgorithmicCalc
{
public:
	RDOFunAlgorithmicDiapCalc(PTR(RDORuntimeParent) parent, CREF(RDOValue) min_value, CREF(RDOValue) max_value)
		: RDOFunAlgorithmicCalc(parent   )
		, m_min_value          (min_value)
		, m_max_value          (max_value)
	{}

private:
	RDOValue m_min_value;
	RDOValue m_max_value;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcGroup
// ----------------------------------------------------------------------------
class RDOFunCalcGroup: public RDOFunCalc
{
protected:
	int           m_nResType;
	PTR(RDOCalc)  m_condition;

	RDOFunCalcGroup(PTR(RDORuntimeParent) parent, int nResType, PTR(RDOCalc) condition)
		: RDOFunCalc (parent   )
		, m_nResType (nResType )
		, m_condition(condition)
	{}
};

#define DEFINE_CALC_GROUP( CalcName ) \
class RDOFunCalc##CalcName: public RDOFunCalcGroup \
{ \
public: \
	RDOFunCalc##CalcName(PTR(RDORuntimeParent) parent, int nResType, PTR(RDOCalc) condition) \
		: RDOFunCalcGroup(parent, nResType, condition) \
	{} \
 \
private: \
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime); \
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
	mutable std::list<PTR(RDOResource)> res_list;
	void prepare(PTR(RDORuntime) sim) const;

	RDOFunCalcSelect(PTR(RDORuntimeParent) parent, int nResType, PTR(RDOCalc) condition)
		: RDOFunCalcGroup(parent, nResType, condition)
	{
		m_value = 1;
	}

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

class RDOFunCalcSelectBase: public RDOFunCalc
{
public:
	RDOFunCalcSelectBase(PTR(RDORuntimeParent) parent, PTR(RDOFunCalcSelect) select, PTR(RDOCalc) condition)
		: RDOFunCalc (parent   )
		, m_select   (select   )
		, m_condition(condition)
	{}

protected:
	PTR(RDOFunCalcSelect) m_select;
	PTR(RDOCalc)          m_condition;
};

#define DEFINE_CALC_SELECT_GROUP(CalcName) \
class RDOFunCalcSelect##CalcName: public RDOFunCalcSelectBase \
{ \
public: \
	RDOFunCalcSelect##CalcName(PTR(RDORuntimeParent) parent, PTR(RDOFunCalcSelect) select, PTR(RDOCalc) condition) \
		: RDOFunCalcSelectBase(parent, select, condition) \
	{} \
 \
private: \
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime); \
};

#define DEFINE_CALC_SELECT_METHOD( CalcName ) \
class RDOFunCalcSelect##CalcName: public RDOFunCalcSelectBase \
{ \
public: \
	RDOFunCalcSelect##CalcName(PTR(RDORuntimeParent) parent, PTR(RDOFunCalcSelect) select) \
		: RDOFunCalcSelectBase(parent, select, NULL) \
	{} \
 \
private: \
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime); \
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
	RDOFunCalc##CalcName(PTR(RDORuntimeParent) parent) \
		: RDOFunCalc( parent ) \
	{} \
\
private: \
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime); \
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
	RDOCalcBinary(PTR(RDORuntimeParent) parent, PTR(RDOCalc) left, PTR(RDOCalc) right)
		: RDOCalc(parent)
		, m_left (left  )
		, m_right(right )
	{}

	PTR(RDOCalc)      getLeft() const              { return m_left; }
	PTR(RDOCalcConst) getRightAsConst() const      { return dynamic_cast<PTR(RDOCalcConst)>(m_right); }
	void              setRight(PTR(RDOCalc) right) { m_right = right; }

protected:
	PTR(RDOCalc)  m_left;
	PTR(RDOCalc)  m_right;
};

#define DEFINE_BINARY_CALC( CalcName, CalcOpr ) \
class RDOCalc##CalcName: public RDOCalcBinary \
{ \
public: \
	RDOCalc##CalcName(PTR(RDORuntimeParent) parent, PTR(RDOCalc) const left, PTR(RDOCalc) const right) \
		: RDOCalcBinary(parent, left, right) \
	{ \
		setSrcInfo( getStaticSrcInfo(m_left, m_right) ); \
	} \
	static RDOSrcInfo getStaticSrcInfo(CPTRC(RDOCalc) left, CPTRC(RDOCalc) right) \
	{ \
		RDOSrcInfo src_info; \
		src_info.setSrcInfo(left->src_info(), CalcOpr, right->src_info()); \
		return src_info; \
	} \
 \
private: \
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime); \
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
	RDOCalcPlusEnumSafe(PTR(RDORuntimeParent) parent, PTR(RDOCalc) _left, PTR(RDOCalc) _right)
		: RDOCalcPlus(parent, _left, _right)
	{}

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

class RDOCalcMultEnumSafe: public RDOCalcMult
{
public:
	RDOCalcMultEnumSafe(PTR(RDORuntimeParent) parent, PTR(RDOCalc) _left, PTR(RDOCalc) _right)
		: RDOCalcMult(parent, _left, _right)
	{}

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- Логические функции
// ----------------------------------------------------------------------------
class RDOCalcAnd: public RDOCalcBinary
{
public:
	RDOCalcAnd(PTR(RDORuntimeParent) parent, PTR(RDOCalc) left, PTR(RDOCalc) right)
		: RDOCalcBinary(parent, left, right)
	{
		m_value_true  = 1;
		m_value_false = 0;
		setSrcInfo( getStaticSrcInfo(m_left, m_right) );
	}
	static RDOSrcInfo getStaticSrcInfo(CPTR(RDOCalc) left, CPTR(RDOCalc) right)
	{
		RDOSrcInfo src_info;
		src_info.setSrcInfo(left->src_info(), _T(" and "), right->src_info());
		return src_info;
	}

private:
	RDOValue m_value_true;
	RDOValue m_value_false;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

class RDOCalcOr: public RDOCalcBinary
{
public:
	RDOCalcOr(PTR(RDORuntimeParent) parent, PTR(RDOCalc) left, PTR(RDOCalc) right)
		: RDOCalcBinary(parent, left, right)
	{
		m_value_true  = 1;
		m_value_false = 0;
		setSrcInfo( getStaticSrcInfo(m_left, m_right) );
	}
	static RDOSrcInfo getStaticSrcInfo(CPTR(RDOCalc) left, CPTR(RDOCalc) right)
	{
		RDOSrcInfo src_info;
		src_info.setSrcInfo(left->src_info(), _T(" or "), right->src_info());
		return src_info;
	}

private:
	RDOValue m_value_true;
	RDOValue m_value_false;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

class RDOCalcNot: public RDOCalc
{
public:
	RDOCalcNot(PTR(RDORuntimeParent) parent, PTR(RDOCalc) calc)
		: RDOCalc(parent)
		, m_calc (calc  )
	{}

private:
	PTR(RDOCalc) m_calc;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
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
	PTR(RDOCalc) m_oper;

	RDOCalcUnary(PTR(RDORuntimeParent) parent, PTR(RDOCalc) oper)
		: RDOCalc(parent)
		, m_oper (oper  )
	{
		if (m_oper) setSrcInfo(m_oper->src_info());
	}
};

// ----------------------------------------------------------------------------
// ---------- Унарные операции
// ----------------------------------------------------------------------------
class RDOCalcUMinus: public RDOCalcUnary
{
public:
	RDOCalcUMinus(PTR(RDORuntimeParent) parent, PTR(RDOCalc) _oper)
		: RDOCalcUnary(parent, _oper)
	{}

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime)
	{
		m_value = -m_oper->calcValue(runtime);
		return m_value;
	}
};

class RDOCalcDoubleToInt: public RDOCalcUnary
{
public:
	RDOCalcDoubleToInt(PTR(RDORuntimeParent) parent, PTR(RDOCalc) _oper)
		: RDOCalcUnary(parent, _oper)
	{}

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime)
	{
		m_value = m_oper->calcValue( runtime ).getInt();
		return m_value;
	}
};

class RDOCalcDoubleToIntByResult: public RDOCalcUnary
{
public:
	RDOCalcDoubleToIntByResult(PTR(RDORuntimeParent) parent, PTR(RDOCalc) _oper)
		: RDOCalcUnary(parent, _oper)
		, m_round     (false        )
	{}

	void needRound()
	{
		m_round = true;
	}

private:
	rbool m_round;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime)
	{
		m_value = m_round ? RDOValue(m_oper->calcValue(runtime).getInt()) : m_oper->calcValue(runtime);
		return m_value;
	}
};

class RDOCalcCheckDiap: public RDOCalcUnary
{
public:
	RDOCalcCheckDiap(PTR(RDORuntimeParent) parent, CREF(RDOValue) min_value, CREF(RDOValue) max_value, PTR(RDOCalc) oper)
		: RDOCalcUnary(parent, oper)
		, m_min_value (min_value   )
		, m_max_value (max_value   )
	{}

private:
	RDOValue m_min_value;
	RDOValue m_max_value;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- Последовательности
// ----------------------------------------------------------------------------
class RDOCalcSeqInit: public RDOCalc
{
public:
	RDOCalcSeqInit(PTR(RDORuntimeParent) parent, int base, PTR(RandGenerator) gen)
		: RDOCalc(parent)
		, m_base (base  )
		, m_gen  (gen   )
	{}
	virtual ~RDOCalcSeqInit();

	void setBase(int base)
	{
		m_base = base;
	}

private:
	int                m_base;
	PTR(RandGenerator) m_gen;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

class RDOCalcSeqNext: public RDOFunCalc
{
public:
	rbool   m_res_real;
	rbool   m_diap;
	double  m_diap_min;
	double  m_diap_max;

	RDOCalcSeqNext(PTR(RDORuntimeParent) parent)
		: RDOFunCalc(parent)
		, m_res_real(true  )
		, m_diap    (false )
		, m_diap_min(0     )
		, m_diap_max(0     )
	{}

protected:
	virtual RDOValue getNextValue(PTR(RDORuntime) runtime) = 0;
};

template<class T>
class RDOCalcRandomDistribution: public RDOCalcSeqNext
{
public:
	RDOCalcRandomDistribution(PTR(RDORuntimeParent) parent, PTR(T) gen)
		: RDOCalcSeqNext(parent)
		, m_gen         (gen   )
	{}
	virtual ~RDOCalcRandomDistribution()
	{
		delete m_gen;
	}

protected:
	PTR(T) m_gen;

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime)
	{
		RDOValue res = getNextValue(runtime);
		if (m_diap)
		{
			if (res < m_diap_min)
			{
				m_value = m_res_real ? m_diap_min : RDOValue(m_diap_min > 0 ? m_diap_min + 0.5 : m_diap_min - 0.5).getInt();
				return m_value;
			}
			if (res > m_diap_max)
			{
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
		}
		else
		{
			m_value = m_res_real ? res : RDOValue(res > 0 ? res + 0.5 : res - 0.5).getInt();
			return m_value;
		}
	}
};

#define DEFINE_RANDON_DISTRIBUTION( CalcName, Distribution ) \
class RDOCalcSeqNext##CalcName: public RDOCalcRandomDistribution<Distribution> \
{ \
public: \
	RDOCalcSeqNext##CalcName(PTR(RDORuntimeParent) parent, PTR(Distribution) gen) \
		: RDOCalcRandomDistribution<Distribution>(parent, gen) \
	{} \
 \
private: \
	virtual RDOValue getNextValue(PTR(RDORuntime) runtime); \
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
	RDOCalcFuncParam(PTR(RDORuntimeParent) parent, int param_number, CREF(RDOSrcInfo) _src_info)
		: RDOCalc       (parent      )
		, m_param_number(param_number)
	{
		setSrcInfo( _src_info );
	}

private:
	int m_param_number;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetConst / RDOCalcSetConst
// ----------------------------------------------------------------------------
class RDOCalcGetConst: public RDOCalc
{
public:
	RDOCalcGetConst(PTR(RDORuntimeParent) parent, int number)
		: RDOCalc (parent)
		, m_number(number)
	{}

private:
	int m_number;
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

class RDOCalcSetConst: public RDOCalc
{
public:
	RDOCalcSetConst(PTR(RDORuntimeParent) parent, int number, PTR(RDOCalc) calc)
		: RDOCalc (parent)
		, m_number(number)
		, m_calc  (calc  )
	{
		m_value = 0;
		if (m_calc) setSrcInfo(m_calc->src_info());
	}

private:
	int          m_number;
	PTR(RDOCalc) m_calc;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcInt (приведение к целому)
// ----------------------------------------------------------------------------
class RDOCalcInt: public RDOCalcUnary
{
public:
	RDOCalcInt(PTR(RDORuntimeParent) parent, PTR(RDOCalc) oper)
		: RDOCalcUnary(parent, oper)
	{}

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime)
	{
		RDOValue res = m_oper->calcValue(runtime);
		m_value = res > 0 ? RDOValue((int)(res.getDouble() + 0.5)) : RDOValue((int)(res.getDouble() - 0.5));
		return m_value;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcFunctionCall
// ----------------------------------------------------------------------------
class RDOCalcFunctionCall: public RDOCalc
{
public:
	RDOCalcFunctionCall(PTR(RDORuntimeParent) parent, PTR(RDOFunCalc) function)
		: RDOCalc   (parent  )
		, m_function(function)
	{}

	void addParameter(PTR(RDOCalc) calc)
	{
		m_parameters.push_back(calc);
	}
	void setFunctionCalc(PTR(RDOFunCalc) calc)
	{
		m_function = calc;
	}

private:
	std::vector<PTR(RDOCalc)>  m_parameters;
	PTR(RDOFunCalc)            m_function;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateNumberedResource (RSS: создание нового временного ресурса или постоянного в начальный момент времени по индексу с параметрами)
// ----------------------------------------------------------------------------
class RDOCalcCreateNumberedResource: public RDOCalc
{
protected:
	int                    type;
	rbool                  traceFlag;
	std::vector<RDOValue>  paramsCalcs;
	ruint                  number;
	rbool                  isPermanent;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);

public:
	RDOCalcCreateNumberedResource(PTR(RDORuntimeParent) parent, int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _paramsCalcs, int _number, rbool _isPermanent);
	virtual PTR(RDOResource) createResource(PTR(RDORuntime) runtime) const;

	ruint getNumber() const { return number; }
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateProcessResource (SEIZE: создание нового ресурса процесса)
// ----------------------------------------------------------------------------
class RDOCalcCreateProcessResource: public RDOCalcCreateNumberedResource
{
private:
	virtual PTR(RDOResource) createResource(PTR(RDORuntime) runtime) const;

public:
	RDOCalcCreateProcessResource(PTR(RDORuntimeParent) parent, int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _paramsCalcs, int _number, rbool _isPermanent);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateEmptyResource (создание нового временного ресурса с пустым списком параметров)
// ----------------------------------------------------------------------------
class RDOCalcCreateEmptyResource: public RDOCalc
{
private:
	int                    type;
	rbool                  traceFlag;
	std::vector<RDOValue>  params_default;
	int                    rel_res_id;

public:
	RDOCalcCreateEmptyResource(PTR(RDORuntimeParent) parent, int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _params_default, int _rel_res_id);
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- Выбор ресурсов
// ----------------------------------------------------------------------------
class RDOSelectResourceCalc: public RDOCalc
{
public:
	enum Type
	{
		order_empty = 0,
		order_first,
		order_with_min, 
		order_with_max
	};

protected:
	int           rel_res_id;
	PTR(RDOCalc)  choice_calc;
	PTR(RDOCalc)  order_calc;
	Type          order_type;
	RDOSelectResourceCalc(PTR(RDORuntimeParent) parent, int _rel_res_id, PTR(RDOCalc) _choice_calc = NULL, PTR(RDOCalc) _order_calc = NULL, Type _order_type = order_empty);
};

class RDOSelectResourceNonExistCalc: public RDOSelectResourceCalc
{
public:
	RDOSelectResourceNonExistCalc(PTR(RDORuntimeParent) parent, int _rel_res_id)
		: RDOSelectResourceCalc(parent, _rel_res_id, NULL, NULL)
	{
		m_value = 1;
	}

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

class RDOSelectResourceDirectCalc: public RDOSelectResourceCalc
{
public:
	RDOSelectResourceDirectCalc(PTR(RDORuntimeParent) parent, int _rel_res_id, int _res_id, PTR(RDOCalc) _choice_calc = NULL, PTR(RDOCalc) _order_calc = NULL, Type _order_type = order_empty)
		: RDOSelectResourceCalc(parent, _rel_res_id, _choice_calc, _order_calc, _order_type)
		, res_id               (_res_id                                                    )
	{}

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);

protected:
	int res_id;

	virtual rbool compare(CREF(RDOCalc) calc) const
	{
		CPTR(RDOSelectResourceDirectCalc) directCalc = dynamic_cast<CPTR(RDOSelectResourceDirectCalc)>(&calc);
		if (!directCalc)
		{
			return false;
		}
		return rel_res_id == directCalc->rel_res_id && res_id == directCalc->res_id;
	}
};

class RDOSelectResourceByTypeCalc: public RDOSelectResourceCalc
{
public:
	RDOSelectResourceByTypeCalc(PTR(RDORuntimeParent) parent, int _rel_res_id, int _resType, PTR(RDOCalc) _choice_calc = NULL, PTR(RDOCalc) _order_calc = NULL, Type _order_type = order_empty)
		: RDOSelectResourceCalc(parent, _rel_res_id, _choice_calc, _order_calc, _order_type)
		, resType              (_resType                                                   )
	{}

protected:
	int resType;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

class RDOSelectResourceCommon
{
public:
	virtual std::vector<int> getPossibleNumbers(PTR(RDORuntime) sim) const = 0;
	virtual rbool            callChoice        (PTR(RDORuntime) sim) const = 0;
};

class RDOSelectResourceCommonCalc: public RDOCalc
{
public:
	RDOSelectResourceCommonCalc(PTR(RDORuntimeParent) parent, CREF(std::vector<PTR(RDOSelectResourceCommon)>) _resSelectors, rbool _useCommonWithMax, PTR(RDOCalc) _choice_calc)
		: RDOCalc         (parent           )
		, resSelectors    (_resSelectors    )
		, useCommonWithMax(_useCommonWithMax)
		, choice_calc     (_choice_calc     )
	{
		if (choice_calc) setSrcInfo(choice_calc->src_info());
	}

private:
	PTR(RDOCalc)                               choice_calc;
	std::vector<PTR(RDOSelectResourceCommon)>  resSelectors;
	rbool                                      useCommonWithMax;

	void  getBest ( REF(std::vector< std::vector<int> >) allNumbs, ruint level, REF(std::vector<int>) res, REF(RDOValue) bestVal, PTR(RDORuntime) sim, REF(rbool) hasBest) const;
	rbool getFirst( REF(std::vector< std::vector<int> >) allNumbs, ruint level, PTR(RDORuntime) sim) const;
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

class RDOSelectResourceDirectCommonCalc: public RDOSelectResourceDirectCalc, public RDOSelectResourceCommon
{
public:
	RDOSelectResourceDirectCommonCalc(PTR(RDORuntimeParent) parent, int _relNumb, int _resNumb, PTR(RDOCalc) _choice_calc = NULL, PTR(RDOCalc) _order_calc = NULL, Type _order_type = order_empty)
		: RDOSelectResourceDirectCalc(parent, _relNumb, _resNumb, _choice_calc, _order_calc, _order_type)
	{}

	std::vector<int> getPossibleNumbers(PTR(RDORuntime) sim) const;
	virtual rbool    callChoice        (PTR(RDORuntime) sim) const;
};

class RDOSelectResourceByTypeCommonCalc: public RDOSelectResourceByTypeCalc, public RDOSelectResourceCommon
{
public:
	RDOSelectResourceByTypeCommonCalc(PTR(RDORuntimeParent) parent, int _relNumb, int _resType, PTR(RDOCalc) _choice_calc = NULL, PTR(RDOCalc) _order_calc = NULL, Type _order_type = order_empty)
		: RDOSelectResourceByTypeCalc(parent, _relNumb, _resType, _choice_calc, _order_calc, _order_type)
	{}

	std::vector<int> getPossibleNumbers(PTR(RDORuntime) sim) const;
	virtual rbool    callChoice        (PTR(RDORuntime) sim) const;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdocalc.inl"

#endif //! _RDOCALC_H_
