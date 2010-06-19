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
#include "rdo_common/namespace.h"
#include "rdo_common/smart_ptr/interface_ptr.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_random_distribution.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalc
// ----------------------------------------------------------------------------
OBJECT(RDOCalc) IS INSTANCE_OF(RDOSrcInfo)
{
DECLARE_FACTORY(RDOCalc)

public:
	REF(RDOValue) calcValue(PTR(RDORuntime) runtime);

	virtual rbool compare(CREF(LPRDOCalc) pCalc) const
	{
		return false;
	}

protected:
	RDOCalc();
	virtual ~RDOCalc();

	RDOValue m_value;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime) = 0;
};

#define DECALRE_ICalc \
	REF(RDOValue) doCalc(PTR(RDORuntime) runtime);

#define CALC(TYPE) class TYPE: public RDOCalc

// ----------------------------------------------------------------------------
// ---------- RDOCalcConst
// ----------------------------------------------------------------------------
CALC(RDOCalcConst)
{
DECLARE_FACTORY(RDOCalcConst)
private:
	RDOCalcConst(CREF(RDOValue) value);
	DECALRE_ICalc;
};
DECLARE_POINTER(RDOCalcConst);

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetResParam (Параметры постоянного ресурса)
// ----------------------------------------------------------------------------
CALC(RDOCalcGetResParam)
{
DECLARE_FACTORY(RDOCalcGetResParam)
protected:
	RDOCalcGetResParam(int _resNumb, int _parNumb)
		: m_resID  (_resNumb)
		, m_paramID(_parNumb)
	{}

	int m_resID;
	int m_paramID;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTempResParamFRM (Параметры временного ресурса для FRM)
// ----------------------------------------------------------------------------
class RDOCalcGetTempResParamFRM: public RDOCalcGetResParam, public INotify
{
DECLARE_FACTORY(RDOCalcGetTempResParamFRM)
private:
	RDOCalcGetTempResParamFRM(int _resNumb, int _parNumb);
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
CALC(RDOCalcGetUnknowResParam)
{
DECLARE_FACTORY(RDOCalcGetUnknowResParam)
private:
	RDOCalcGetUnknowResParam(CREF(tstring) resName, CREF(tstring) parName)
		: m_resName(resName)
		, m_parName(parName)
	{}

	tstring m_resName;
	tstring m_parName;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetGroupResParam
// ----------------------------------------------------------------------------
CALC(RDOCalcGetGroupResParam)
{
DECLARE_FACTORY(RDOCalcGetGroupResParam)
private:
	RDOCalcGetGroupResParam(int parNumb)
		: m_parNumb(parNumb)
	{}

	int m_parNumb;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetRelevantResParam
// ----------------------------------------------------------------------------
CALC(RDOCalcGetRelevantResParam)
{
DECLARE_FACTORY(RDOCalcGetRelevantResParam)
private:
	RDOCalcGetRelevantResParam(int relNumb, int parNumb)
		: m_relNumb(relNumb)
		, m_parNumb(parNumb)
	{}

	int m_relNumb;
	int m_parNumb;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOSetRelParamDiapCalc
// ----------------------------------------------------------------------------
CALC(RDOSetRelParamDiapCalc)
{
DECLARE_FACTORY(RDOSetRelParamDiapCalc)
private:
	RDOSetRelParamDiapCalc(int relNumb, int parNumb, CREF(RDOValue) min_value, CREF(RDOValue) max_value, CREF(LPRDOCalc) pCalc)
		: m_relNumb  (relNumb  )
		, m_parNumb  (parNumb  )
		, m_pCalc    (pCalc    )
		, m_min_value(min_value)
		, m_max_value(max_value)
	{
		m_value = true;
		if (m_pCalc)
			setSrcInfo(m_pCalc->src_info());
	}

	int          m_relNumb;
	int          m_parNumb;
	LPRDOCalc    m_pCalc;
	RDOValue     m_min_value;
	RDOValue     m_max_value;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOSetResourceParamCalc
// ----------------------------------------------------------------------------
CALC(RDOSetResourceParamCalc)
{
DECLARE_FACTORY(RDOSetResourceParamCalc)
private:
	RDOSetResourceParamCalc(int resNumb, int parNumb, CREF(LPRDOCalc) pCalc)
		: m_resNumb(resNumb)
		, m_parNumb(parNumb)
		, m_pCalc  (pCalc   )
	{
		m_value = true;
		if (m_pCalc)
			setSrcInfo(m_pCalc->src_info());
	}

	int        m_resNumb;
	int        m_parNumb;
	LPRDOCalc  m_pCalc;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcEraseRes
// ----------------------------------------------------------------------------
CALC(RDOCalcEraseRes)
{
DECLARE_FACTORY(RDOCalcEraseRes)
public:
	tstring getName() const { return m_rel_res_name; }

private:
	RDOCalcEraseRes(int rel_res_id, CREF(tstring) rel_res_name)
		: m_rel_res_id  (rel_res_id  )
		, m_rel_res_name(rel_res_name)
	{
		m_value = 1;
	}

	int      m_rel_res_id;
	tstring  m_rel_res_name;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};
DECLARE_POINTER(RDOCalcEraseRes);

// ----------------------------------------------------------------------------
// ---------- RDOSetPatternParamCalc
// ----------------------------------------------------------------------------
CALC(RDOSetPatternParamCalc)
{
DECLARE_FACTORY(RDOSetPatternParamCalc)
private:
	RDOSetPatternParamCalc(int parNumb, RDOValue val)
		: m_parNumb(parNumb)
		, m_val    (val    )
	{
		m_value = 0;
	}

	int      m_parNumb;
	RDOValue m_val;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcPatParam
// ----------------------------------------------------------------------------
CALC(RDOCalcPatParam)
{
DECLARE_FACTORY(RDOCalcPatParam)
private:
	RDOCalcPatParam(int numberOfParam)
		: m_numberOfParam(numberOfParam)
	{}

	int m_numberOfParam;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTimeNow
// ----------------------------------------------------------------------------
CALC(RDOCalcGetTimeNow)
{
DECLARE_FACTORY(RDOCalcGetTimeNow)

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetSeconds
// ----------------------------------------------------------------------------
CALC(RDOCalcGetSeconds)
{
DECLARE_FACTORY(RDOCalcGetSeconds)
private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTermNow
// ----------------------------------------------------------------------------
CALC(RDOCalcGetTermNow)
{
DECLARE_FACTORY(RDOCalcGetTermNow)
private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOFunCalc
// ----------------------------------------------------------------------------
CALC(RDOFunCalc)
{
protected:
	RDOFunCalc()
	{}
};
DECLARE_POINTER(RDOFunCalc);

// ----------------------------------------------------------------------------
// ---------- RDOFunListCalc
// ----------------------------------------------------------------------------
// Функция типа список
// ----------------------------------------------------------------------------
class RDOFuncTableCalc: public RDOFunCalc
{
DECLARE_FACTORY(RDOFuncTableCalc)
public:
	void addResultCalc(PTR(RDOCalcConst) res)
	{
		m_results.push_back( res );
	}

private:
	RDOFuncTableCalc(CREF(LPRDOCalc) pArgCalc)
		: m_pArgCalc(pArgCalc)
	{}

	std::vector<PTR(RDOCalcConst)> m_results;
	LPRDOCalc                      m_pArgCalc;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime)
	{
		int index = m_pArgCalc->calcValue( runtime ).getInt();
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
DECLARE_FACTORY(RDOFunListCalc)
public:
	void addCase(CREF(LPRDOCalc) pCaseCalc, CREF(LPRDOCalcConst) pResultCalc)
	{
		m_cases  .push_back(pCaseCalc  ); 
		m_results.push_back(pResultCalc);
	}

private:
	RDOFunListCalc(CREF(LPRDOCalcConst) pDefaultValue)
		: m_pDefaultValue(pDefaultValue)
	{}

	std::vector<LPRDOCalc>       m_cases;
	std::vector<LPRDOCalcConst>  m_results;
	LPRDOCalcConst               m_pDefaultValue;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime)
	{
		size_t size = m_cases.size();
		for (ruint i = 0; i < size; i++)
		{
			LPRDOCalc cas = m_cases[i];
			if (cas->calcValue( runtime ).getAsBool())
			{
				return m_results[i]->calcValue(runtime);
			}
		}
		return m_pDefaultValue->calcValue(runtime);
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOFunAlgorithmicCalc
// ----------------------------------------------------------------------------
// Алгоритмическая функция
// ----------------------------------------------------------------------------
class RDOFunAlgorithmicCalc: public RDOFunCalc
{
DECLARE_FACTORY(RDOFunAlgorithmicCalc)
public:
	void addCalcIf(CREF(LPRDOCalc) pCond, CREF(LPRDOCalc) pAct)
	{
		m_conditions.push_back(pCond);
		m_actions   .push_back(pAct );
	}

protected:
	RDOFunAlgorithmicCalc()
	{
		m_value = 0;
	}

	std::vector<LPRDOCalc> m_conditions;
	std::vector<LPRDOCalc> m_actions;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOFunAlgorithmicDiapCalc
// ----------------------------------------------------------------------------
// Алгоритмическая функция с проверкой на диапазон возвращаемого значения
// ----------------------------------------------------------------------------
class RDOFunAlgorithmicDiapCalc: public RDOFunAlgorithmicCalc
{
DECLARE_FACTORY(RDOFunAlgorithmicDiapCalc)
private:
	RDOFunAlgorithmicDiapCalc(CREF(RDOValue) min_value, CREF(RDOValue) max_value)
		: m_min_value          (min_value)
		, m_max_value          (max_value)
	{}

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
	int        m_nResType;
	LPRDOCalc  m_pCondition;

	RDOFunCalcGroup(int nResType, CREF(LPRDOCalc) pCondition)
		: m_nResType  (nResType  )
		, m_pCondition(pCondition)
	{}
};

#define DEFINE_CALC_GROUP( CalcName ) \
class RDOFunCalc##CalcName: public RDOFunCalcGroup \
{ \
DECLARE_FACTORY(RDOFunCalc##CalcName) \
private: \
	RDOFunCalc##CalcName(int nResType, CREF(LPRDOCalc) pCondition) \
		: RDOFunCalcGroup(nResType, pCondition) \
	{} \
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
DECLARE_FACTORY(RDOFunCalcSelect)
public:
	mutable std::list<PTR(RDOResource)> res_list;
	void prepare(PTR(RDORuntime) sim);

private:
	RDOFunCalcSelect(int nResType, CREF(LPRDOCalc) pCondition)
		: RDOFunCalcGroup(nResType, pCondition)
	{
		m_value = 1;
	}
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};
DECLARE_POINTER(RDOFunCalcSelect);

class RDOFunCalcSelectBase: public RDOFunCalc
{
protected:
	RDOFunCalcSelectBase(CREF(LPRDOFunCalcSelect) pSelect, CREF(LPRDOCalc) pCondition)
		: m_pSelect   (pSelect   )
		, m_pCondition(pCondition)
	{}

	LPRDOFunCalcSelect m_pSelect;
	LPRDOCalc          m_pCondition;
};

#define DEFINE_CALC_SELECT_GROUP(CalcName) \
class RDOFunCalcSelect##CalcName: public RDOFunCalcSelectBase \
{ \
DECLARE_FACTORY(RDOFunCalcSelect##CalcName) \
private: \
	RDOFunCalcSelect##CalcName(CREF(LPRDOFunCalcSelect) pSelect, CREF(LPRDOCalc) pCondition) \
		: RDOFunCalcSelectBase(pSelect, pCondition) \
	{} \
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime); \
};

#define DEFINE_CALC_SELECT_METHOD( CalcName ) \
class RDOFunCalcSelect##CalcName: public RDOFunCalcSelectBase \
{ \
DECLARE_FACTORY(RDOFunCalcSelect##CalcName) \
private: \
	RDOFunCalcSelect##CalcName(CREF(LPRDOFunCalcSelect) pSelect) \
		: RDOFunCalcSelectBase(pSelect, NULL) \
	{} \
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
DECLARE_FACTORY(RDOFunCalc##CalcName) \
private: \
	RDOFunCalc##CalcName() \
	{} \
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
CALC(RDOCalcBinary)
{
public:
	LPRDOCalc      getLeft        () const                 { return m_pLeft;                              }
	LPRDOCalcConst getRightAsConst() const                 { return m_pRight.object_cast<RDOCalcConst>(); }
	void           setRight       (CREF(LPRDOCalc) pRight) { m_pRight = pRight;                           }

protected:
	RDOCalcBinary(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
		: m_pLeft (pLeft )
		, m_pRight(pRight)
	{}

	LPRDOCalc  m_pLeft;
	LPRDOCalc  m_pRight;
};

#define DEFINE_BINARY_CALC( CalcName, CalcOpr ) \
class RDOCalc##CalcName: public RDOCalcBinary \
{ \
DECLARE_FACTORY(RDOCalc##CalcName) \
public: \
	static RDOSrcInfo getStaticSrcInfo(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight) \
	{ \
		RDOSrcInfo src_info; \
		src_info.setSrcInfo(pLeft->src_info(), CalcOpr, pRight->src_info()); \
		return src_info; \
	} \
 \
protected: \
	RDOCalc##CalcName(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight) \
		: RDOCalcBinary(pLeft, pRight) \
	{ \
		setSrcInfo(getStaticSrcInfo(m_pLeft, m_pRight)); \
	} \
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
DECLARE_FACTORY(RDOCalcPlusEnumSafe)
private:
	RDOCalcPlusEnumSafe(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
		: RDOCalcPlus(pLeft, pRight)
	{}
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

class RDOCalcMultEnumSafe: public RDOCalcMult
{
DECLARE_FACTORY(RDOCalcMultEnumSafe)
private:
	RDOCalcMultEnumSafe(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
		: RDOCalcMult(pLeft, pRight)
	{}
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- Логические функции
// ----------------------------------------------------------------------------
class RDOCalcAnd: public RDOCalcBinary
{
DECLARE_FACTORY(RDOCalcAnd)
public:
	static RDOSrcInfo getStaticSrcInfo(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
	{
		RDOSrcInfo src_info;
		src_info.setSrcInfo(pLeft->src_info(), _T(" and "), pRight->src_info());
		return src_info;
	}

private:
	RDOCalcAnd(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
		: RDOCalcBinary(pLeft, pRight)
	{
		m_value_true  = 1;
		m_value_false = 0;
		setSrcInfo( getStaticSrcInfo(m_pLeft, m_pRight) );
	}
	RDOValue m_value_true;
	RDOValue m_value_false;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

class RDOCalcOr: public RDOCalcBinary
{
DECLARE_FACTORY(RDOCalcOr)
public:
	static RDOSrcInfo getStaticSrcInfo(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
	{
		RDOSrcInfo src_info;
		src_info.setSrcInfo(pLeft->src_info(), _T(" or "), pRight->src_info());
		return src_info;
	}

private:
	RDOCalcOr(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
		: RDOCalcBinary(pLeft, pRight)
	{
		m_value_true  = 1;
		m_value_false = 0;
		setSrcInfo( getStaticSrcInfo(m_pLeft, m_pRight) );
	}

	RDOValue m_value_true;
	RDOValue m_value_false;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

CALC(RDOCalcNot)
{
DECLARE_FACTORY(RDOCalcNot)
private:
	RDOCalcNot(CREF(LPRDOCalc) pCalc)
		: m_pCalc(pCalc)
	{}

	LPRDOCalc m_pCalc;

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
CALC(RDOCalcUnary)
{
protected:
	RDOCalcUnary(CREF(LPRDOCalc) pOperation)
		: m_pOperation(pOperation)
	{
		if (m_pOperation)
			setSrcInfo(m_pOperation->src_info());
	}

	LPRDOCalc m_pOperation;
};

// ----------------------------------------------------------------------------
// ---------- Унарные операции
// ----------------------------------------------------------------------------
class RDOCalcUMinus: public RDOCalcUnary
{
DECLARE_FACTORY(RDOCalcUMinus)
private:
	RDOCalcUMinus(CREF(LPRDOCalc) pOper)
		: RDOCalcUnary(pOper)
	{}
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime)
	{
		m_value = -m_pOperation->calcValue(runtime);
		return m_value;
	}
};

class RDOCalcDoubleToInt: public RDOCalcUnary
{
DECLARE_FACTORY(RDOCalcDoubleToInt)
private:
	RDOCalcDoubleToInt(CREF(LPRDOCalc) pOper)
		: RDOCalcUnary(pOper)
	{}
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime)
	{
		m_value = m_pOperation->calcValue( runtime ).getInt();
		return m_value;
	}
};

class RDOCalcDoubleToIntByResult: public RDOCalcUnary
{
DECLARE_FACTORY(RDOCalcDoubleToIntByResult)
public:
	void needRound()
	{
		m_round = true;
	}

private:
	RDOCalcDoubleToIntByResult(CREF(LPRDOCalc) pOper)
		: RDOCalcUnary(pOper)
		, m_round     (false)
	{}

	rbool m_round;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime)
	{
		m_value = m_round ? RDOValue(m_pOperation->calcValue(runtime).getInt()) : m_pOperation->calcValue(runtime);
		return m_value;
	}
};

class RDOCalcCheckDiap: public RDOCalcUnary
{
DECLARE_FACTORY(RDOCalcCheckDiap)
private:
	RDOCalcCheckDiap(CREF(RDOValue) min_value, CREF(RDOValue) max_value, CREF(LPRDOCalc) pOper)
		: RDOCalcUnary(pOper    )
		, m_min_value (min_value)
		, m_max_value (max_value)
	{}

	RDOValue m_min_value;
	RDOValue m_max_value;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- Последовательности
// ----------------------------------------------------------------------------
CALC(RDOCalcSeqInit)
{
DECLARE_FACTORY(RDOCalcSeqInit)
public:
	void setBase(int base)
	{
		m_base = base;
	}

private:
	RDOCalcSeqInit(int base, PTR(RandGenerator) gen)
		: m_base(base)
		, m_gen (gen )
	{}
	virtual ~RDOCalcSeqInit();

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

protected:
	RDOCalcSeqNext()
		: m_res_real(true  )
		, m_diap    (false )
		, m_diap_min(0     )
		, m_diap_max(0     )
	{}

	virtual RDOValue getNextValue(PTR(RDORuntime) runtime) = 0;
};

template<class T>
class RDOCalcRandomDistribution: public RDOCalcSeqNext
{
protected:
	RDOCalcRandomDistribution(PTR(T) gen)
		: m_gen(gen)
	{}
	virtual ~RDOCalcRandomDistribution()
	{
		delete m_gen;
	}
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
DECLARE_FACTORY(RDOCalcSeqNext##CalcName) \
private: \
	RDOCalcSeqNext##CalcName(PTR(Distribution) gen) \
		: RDOCalcRandomDistribution<Distribution>(gen) \
	{} \
 \
	virtual RDOValue getNextValue(PTR(RDORuntime) runtime); \
};

DEFINE_RANDON_DISTRIBUTION( Uniform    , RandGeneratorUniform     );
DEFINE_RANDON_DISTRIBUTION( Normal     , RandGeneratorNormal      );
DEFINE_RANDON_DISTRIBUTION( Exponential, RandGeneratorExponential );
DEFINE_RANDON_DISTRIBUTION( ByHist     , RandGeneratorCommonNext  );

// ----------------------------------------------------------------------------
// ---------- RDOCalcFuncParam
// ----------------------------------------------------------------------------
CALC(RDOCalcFuncParam)
{
DECLARE_FACTORY(RDOCalcFuncParam)
private:
	RDOCalcFuncParam(int param_number, CREF(RDOSrcInfo) _src_info)
		: m_param_number(param_number)
	{
		setSrcInfo( _src_info );
	}

	int m_param_number;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetConst / RDOCalcSetConst
// ----------------------------------------------------------------------------
CALC(RDOCalcGetConst)
{
DECLARE_FACTORY(RDOCalcGetConst)
private:
	RDOCalcGetConst(int number)
		: m_number(number)
	{}

	int m_number;
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

CALC(RDOCalcSetConst)
{
DECLARE_FACTORY(RDOCalcSetConst)
private:
	RDOCalcSetConst(int number, CREF(LPRDOCalc) pCalc)
		: m_number(number)
		, m_pCalc (pCalc )
	{
		m_value = 0;
		if (m_pCalc) setSrcInfo(m_pCalc->src_info());
	}

	int        m_number;
	LPRDOCalc  m_pCalc;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcInt (приведение к целому)
// ----------------------------------------------------------------------------
class RDOCalcInt: public RDOCalcUnary
{
DECLARE_FACTORY(RDOCalcInt)
private:
	RDOCalcInt(CREF(LPRDOCalc) pOperation)
		: RDOCalcUnary(pOperation)
	{}

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime)
	{
		RDOValue res = m_pOperation->calcValue(runtime);
		m_value = res > 0 ? RDOValue((int)(res.getDouble() + 0.5)) : RDOValue((int)(res.getDouble() - 0.5));
		return m_value;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcFunctionCall
// ----------------------------------------------------------------------------
CALC(RDOCalcFunctionCall)
{
DECLARE_FACTORY(RDOCalcFunctionCall)
public:
	void addParameter(CREF(LPRDOCalc) pCalc)
	{
		m_parameters.push_back(pCalc);
	}
	void setFunctionCalc(CREF(LPRDOFunCalc) pFunction)
	{
		m_pFunction = pFunction;
	}

private:
	RDOCalcFunctionCall(CREF(LPRDOFunCalc) pFunction)
		: m_pFunction(pFunction)
	{}

	std::vector<LPRDOCalc>  m_parameters;
	LPRDOFunCalc            m_pFunction;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateNumberedResource (RSS: создание нового временного ресурса или постоянного в начальный момент времени по индексу с параметрами)
// ----------------------------------------------------------------------------
CALC(RDOCalcCreateNumberedResource)
{
public:
	virtual PTR(RDOResource) createResource(PTR(RDORuntime) runtime) const;

	ruint getNumber() const { return number; }

protected:
	RDOCalcCreateNumberedResource(int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _paramsCalcs, int _number, rbool _isPermanent);

	int                    type;
	rbool                  traceFlag;
	std::vector<RDOValue>  paramsCalcs;
	ruint                  number;
	rbool                  isPermanent;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateProcessResource (SEIZE: создание нового ресурса процесса)
// ----------------------------------------------------------------------------
class RDOCalcCreateProcessResource: public RDOCalcCreateNumberedResource
{
DECLARE_FACTORY(RDOCalcCreateProcessResource)
private:
	RDOCalcCreateProcessResource(int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _paramsCalcs, int _number, rbool _isPermanent);
	virtual PTR(RDOResource) createResource(PTR(RDORuntime) runtime) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateEmptyResource (создание нового временного ресурса с пустым списком параметров)
// ----------------------------------------------------------------------------
CALC(RDOCalcCreateEmptyResource)
{
DECLARE_FACTORY(RDOCalcCreateEmptyResource)
private:
	RDOCalcCreateEmptyResource(int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _params_default, int _rel_res_id);

	int                    type;
	rbool                  traceFlag;
	std::vector<RDOValue>  params_default;
	int                    rel_res_id;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

// ----------------------------------------------------------------------------
// ---------- Выбор ресурсов
// ----------------------------------------------------------------------------
CALC(RDOSelectResourceCalc)
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
	RDOSelectResourceCalc(int _rel_res_id, CREF(LPRDOCalc) _choice_calc, CREF(LPRDOCalc) _order_calc, Type _order_type = order_empty);

	int        rel_res_id;
	LPRDOCalc  choice_calc;
	LPRDOCalc  order_calc;
	Type       order_type;
};

class RDOSelectResourceNonExistCalc: public RDOSelectResourceCalc
{
DECLARE_FACTORY(RDOSelectResourceNonExistCalc)
private:
	RDOSelectResourceNonExistCalc(int _rel_res_id)
		: RDOSelectResourceCalc(_rel_res_id, NULL, NULL)
	{
		m_value = 1;
	}
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

class RDOSelectResourceDirectCalc: public RDOSelectResourceCalc
{
DECLARE_FACTORY(RDOSelectResourceDirectCalc)
protected:
	RDOSelectResourceDirectCalc(int _rel_res_id, int _res_id, CREF(LPRDOCalc) _choice_calc = NULL, CREF(LPRDOCalc) _order_calc = NULL, Type _order_type = order_empty)
		: RDOSelectResourceCalc(_rel_res_id, _choice_calc, _order_calc, _order_type)
		, res_id               (_res_id                                            )
	{}

	int res_id;

	virtual rbool compare(CREF(LPRDOCalc) pCalc) const
	{
		CPTR(RDOSelectResourceDirectCalc) directCalc = dynamic_cast<CPTR(RDOSelectResourceDirectCalc)>(pCalc.get());
		if (!directCalc)
		{
			return false;
		}
		return rel_res_id == directCalc->rel_res_id && res_id == directCalc->res_id;
	}

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

class RDOSelectResourceByTypeCalc: public RDOSelectResourceCalc
{
DECLARE_FACTORY(RDOSelectResourceByTypeCalc)
protected:
	RDOSelectResourceByTypeCalc(int _rel_res_id, int _resType, CREF(LPRDOCalc) pChoiceCalc = NULL, CREF(LPRDOCalc) pOrderCalc = NULL, Type _order_type = order_empty)
		: RDOSelectResourceCalc(_rel_res_id, pChoiceCalc, pOrderCalc, _order_type)
		, resType              (_resType                                         )
	{}

	int resType;

private:
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

OBJECT_INTERFACE(IRDOSelectResourceCommon)
{
public:
	virtual std::vector<int> getPossibleNumbers(PTR(RDORuntime) sim) const = 0;
	virtual rbool            callChoice        (PTR(RDORuntime) sim) const = 0;
};

CALC(RDOSelectResourceCommonCalc)
{
DECLARE_FACTORY(RDOSelectResourceCommonCalc)
private:
	typedef  std::vector<LPIRDOSelectResourceCommon>  SelectResourceCommonList;

	RDOSelectResourceCommonCalc(CREF(SelectResourceCommonList) _resSelectors, rbool _useCommonWithMax, CREF(LPRDOCalc) _choice_calc)
		: resSelectors    (_resSelectors    )
		, useCommonWithMax(_useCommonWithMax)
		, choice_calc     (_choice_calc     )
	{
		if (choice_calc) setSrcInfo(choice_calc->src_info());
	}

	LPRDOCalc                 choice_calc;
	SelectResourceCommonList  resSelectors;
	rbool                     useCommonWithMax;

	void  getBest ( REF(std::vector< std::vector<int> >) allNumbs, ruint level, REF(std::vector<int>) res, REF(RDOValue) bestVal, PTR(RDORuntime) sim, REF(rbool) hasBest) const;
	rbool getFirst( REF(std::vector< std::vector<int> >) allNumbs, ruint level, PTR(RDORuntime) sim) const;
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

class RDOSelectResourceDirectCommonCalc: public RDOSelectResourceDirectCalc, public IRDOSelectResourceCommon
{
DECLARE_FACTORY(RDOSelectResourceDirectCommonCalc)
public:
	std::vector<int> getPossibleNumbers(PTR(RDORuntime) sim) const;
	virtual rbool    callChoice        (PTR(RDORuntime) sim) const;

protected:
	RDOSelectResourceDirectCommonCalc(int _relNumb, int _resNumb, CREF(LPRDOCalc) _choice_calc = NULL, CREF(LPRDOCalc) _order_calc = NULL, Type _order_type = order_empty)
		: RDOSelectResourceDirectCalc(_relNumb, _resNumb, _choice_calc, _order_calc, _order_type)
	{}
};
DECLARE_POINTER(RDOSelectResourceDirectCommonCalc);

class RDOSelectResourceByTypeCommonCalc: public RDOSelectResourceByTypeCalc, public IRDOSelectResourceCommon
{
DECLARE_FACTORY(RDOSelectResourceByTypeCommonCalc)
public:
	std::vector<int> getPossibleNumbers(PTR(RDORuntime) sim) const;
	virtual rbool    callChoice        (PTR(RDORuntime) sim) const;

private:
	RDOSelectResourceByTypeCommonCalc(int _relNumb, int _resType, CREF(LPRDOCalc) pChoiceCalc = NULL, CREF(LPRDOCalc) pOrderCalc = NULL, Type _order_type = order_empty)
		: RDOSelectResourceByTypeCalc(_relNumb, _resType, pChoiceCalc, pOrderCalc, _order_type)
	{}
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdocalc.inl"

#endif //! _RDOCALC_H_
