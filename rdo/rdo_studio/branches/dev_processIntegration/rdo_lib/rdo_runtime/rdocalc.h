/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdocalc.h
 * author   : Александ Барс, Урусов Андрей, Дмитрий Лущан
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
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_common/smart_ptr/interface_ptr.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_random_distribution.h"
#include "rdo_lib/rdo_runtime/rdo_memory.h"
#include "rdo_lib/rdo_runtime/isReturn.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalc
// ----------------------------------------------------------------------------
OBJECT(RDOCalc) IS INSTANCE_OF(RDOSrcInfo) IS INSTANCE_OF(RDOCalcIsReturn)
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
private:              \
	REF(RDOValue) doCalc(PTR(RDORuntime) runtime);

#define CALC_SUB(TYPE, PARENT) \
PREDECLARE_POINTER(TYPE);      \
class TYPE: public PARENT

#define CALC(TYPE) CALC_SUB(TYPE, RDOCalc)

// ----------------------------------------------------------------------------
// ---------- RDOCalcConst
// ----------------------------------------------------------------------------
CALC(RDOCalcConst)
{
DECLARE_FACTORY(RDOCalcConst)
public:
	CREF(RDOValue) getValue() const;

private:
	RDOCalcConst(CREF(RDOValue) value);
	DECALRE_ICalc;
};

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

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTempResParamFRM (Параметры временного ресурса для FRM)
// ----------------------------------------------------------------------------
CALC_SUB(RDOCalcGetTempResParamFRM, RDOCalcGetResParam)
	AND IMPLEMENTATION_OF(INotify)
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

	DECALRE_ICalc;
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

	DECALRE_ICalc;
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

	DECALRE_ICalc;
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

	DECALRE_ICalc;
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

	DECALRE_ICalc;
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

	DECALRE_ICalc;
};

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

	DECALRE_ICalc;
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

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTimeNow
// ----------------------------------------------------------------------------
CALC(RDOCalcGetTimeNow)
{
DECLARE_FACTORY(RDOCalcGetTimeNow)
DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetSeconds
// ----------------------------------------------------------------------------
CALC(RDOCalcGetSeconds)
{
DECLARE_FACTORY(RDOCalcGetSeconds)
DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTermNow
// ----------------------------------------------------------------------------
CALC(RDOCalcGetTermNow)
{
DECLARE_FACTORY(RDOCalcGetTermNow)
DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcIf
// ----------------------------------------------------------------------------
CALC(RDOCalcIf)
{
DECLARE_FACTORY(RDOCalcIf)
private:
	RDOCalcIf(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pStatement);

	LPRDOCalc m_pCondition;
	LPRDOCalc m_pStatement;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcIfElse
// ----------------------------------------------------------------------------
CALC(RDOCalcIfElse)
{
DECLARE_FACTORY(RDOCalcIfElse)
private:
	RDOCalcIfElse(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pIfStatement, CREF(LPRDOCalc) pElseStatement);

	LPRDOCalc m_pCondition;
	LPRDOCalc m_pIfStatement;
	LPRDOCalc m_pElseStatement;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcFor
// ----------------------------------------------------------------------------
CALC(RDOCalcFor)
{
DECLARE_FACTORY(RDOCalcFor)
private:
	RDOCalcFor(CREF(LPRDOCalc) pDeclaration, CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pExpression, CREF(LPRDOCalc) pStatement);

	LPRDOCalc m_pDeclaration;
	LPRDOCalc m_pCondition;
	LPRDOCalc m_pExpression;
	LPRDOCalc m_pStatement;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateLocalVariable
// ----------------------------------------------------------------------------
CALC(RDOCalcCreateLocalVariable)
{
DECLARE_FACTORY(RDOCalcCreateLocalVariable)
private:
	RDOCalcCreateLocalVariable(CREF(tstring) name);

	tstring m_name;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcSetLocalVariable
// ----------------------------------------------------------------------------
CALC(RDOCalcSetLocalVariable)
{
DECLARE_FACTORY(RDOCalcSetLocalVariable)
private:
	RDOCalcSetLocalVariable(CREF(tstring) name, CREF(LPRDOCalc) pCalc);

	tstring   m_name;
	LPRDOCalc m_pCalc;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetLocalVariable
// ----------------------------------------------------------------------------
CALC(RDOCalcGetLocalVariable)
{
DECLARE_FACTORY(RDOCalcGetLocalVariable)
private:
	RDOCalcGetLocalVariable(CREF(tstring) name);

	tstring m_name;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcLocalVariableList
// ----------------------------------------------------------------------------
CALC(RDOCalcLocalVariableList)
{
	DECLARE_FACTORY(RDOCalcLocalVariableList)
public:
	typedef std::vector<LPRDOCalc> CalcLocalVariableList;

	void addCalcLocalVariable(CREF(LPRDOCalc) pCalc);

private:
	RDOCalcLocalVariableList();

	CalcLocalVariableList m_calcLocalVariableList;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcOpenBrace
// ----------------------------------------------------------------------------
CALC(RDOCalcOpenBrace)
{
DECLARE_FACTORY(RDOCalcOpenBrace)
private:
	RDOCalcOpenBrace();

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcCloseBrace
// ----------------------------------------------------------------------------
CALC(RDOCalcCloseBrace)
{
DECLARE_FACTORY(RDOCalcCloseBrace)
private:
	RDOCalcCloseBrace();

	DECALRE_ICalc;
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

// ----------------------------------------------------------------------------
// ---------- RDOCalcFunList
// ----------------------------------------------------------------------------
CALC_SUB(RDOCalcFunList, RDOFunCalc)
{
	DECLARE_FACTORY(RDOCalcFunList)
public:
	typedef std::vector<LPRDOCalc> CalcFunList;

	void addFunCalc(CREF(LPRDOCalc) pCalc);

private:
	RDOCalcFunList();

	CalcFunList m_calcFunList;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcFunBreak
// ----------------------------------------------------------------------------
CALC(RDOCalcFunBreak)
{
	DECLARE_FACTORY(RDOCalcFunBreak)
private:
	RDOCalcFunBreak();

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcFunReturn
// ----------------------------------------------------------------------------
CALC(RDOCalcFunReturn)
{
	DECLARE_FACTORY(RDOCalcFunReturn)
private:
	RDOCalcFunReturn(CREF(LPRDOCalc) pReturn);

	LPRDOCalc m_pReturn;
	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcList
// ----------------------------------------------------------------------------
CALC(RDOCalcList)
{
DECLARE_FACTORY(RDOCalcList)
public:
	typedef std::vector<LPRDOCalc> CalcList;

	void addCalc(CREF(LPRDOCalc) pCalc);

private:
	RDOCalcList();

	CalcList m_calcList;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcNoChange
// ----------------------------------------------------------------------------
CALC(RDOCalcNoChange)
{
DECLARE_FACTORY(RDOCalcNoChange)
private:
	RDOCalcNoChange();
	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOFunListCalc
// ----------------------------------------------------------------------------
// Функция типа список
// ----------------------------------------------------------------------------
CALC_SUB(RDOFuncTableCalc, RDOFunCalc)
{
DECLARE_FACTORY(RDOFuncTableCalc)
public:
	void addResultCalc(CREF(LPRDOCalcConst) res)
	{
		m_results.push_back( res );
	}

private:
	RDOFuncTableCalc(CREF(LPRDOCalc) pArgCalc)
		: m_pArgCalc(pArgCalc)
	{}

	std::vector<LPRDOCalcConst> m_results;
	LPRDOCalc                   m_pArgCalc;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOFunListCalc
// ----------------------------------------------------------------------------
// Функция типа список
// ----------------------------------------------------------------------------
CALC_SUB(RDOFunListCalc, RDOFunCalc)
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

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOFunAlgorithmicCalc
// ----------------------------------------------------------------------------
// Алгоритмическая функция
// ----------------------------------------------------------------------------
CALC_SUB(RDOFunAlgorithmicCalc, RDOFunCalc)
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

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOFunAlgorithmicDiapCalc
// ----------------------------------------------------------------------------
// Алгоритмическая функция с проверкой на диапазон возвращаемого значения
// ----------------------------------------------------------------------------
CALC_SUB(RDOFunAlgorithmicDiapCalc, RDOFunAlgorithmicCalc)
{
DECLARE_FACTORY(RDOFunAlgorithmicDiapCalc)
private:
	RDOFunAlgorithmicDiapCalc(CREF(RDOValue) min_value, CREF(RDOValue) max_value)
		: m_min_value          (min_value)
		, m_max_value          (max_value)
	{}

	RDOValue m_min_value;
	RDOValue m_max_value;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcGroup
// ----------------------------------------------------------------------------
CALC_SUB(RDOFunCalcGroup, RDOFunCalc)
{
protected:
	int        m_nResType;
	LPRDOCalc  m_pCondition;

	RDOFunCalcGroup(int nResType, CREF(LPRDOCalc) pCondition)
		: m_nResType  (nResType  )
		, m_pCondition(pCondition)
	{}
};

#define DEFINE_CALC_GROUP(CalcName) \
CALC_SUB(RDOFunCalc##CalcName, RDOFunCalcGroup) \
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

CALC_SUB(RDOFunCalcSelect, RDOFunCalcGroup)
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
	DECALRE_ICalc;
};

CALC_SUB(RDOFunCalcSelectBase, RDOFunCalc)
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
CALC_SUB(RDOFunCalcSelect##CalcName, RDOFunCalcSelectBase) \
{ \
DECLARE_FACTORY(RDOFunCalcSelect##CalcName) \
private: \
	RDOFunCalcSelect##CalcName(CREF(LPRDOFunCalcSelect) pSelect, CREF(LPRDOCalc) pCondition) \
		: RDOFunCalcSelectBase(pSelect, pCondition) \
	{} \
	DECALRE_ICalc; \
};

#define DEFINE_CALC_SELECT_METHOD(CalcName) \
CALC_SUB(RDOFunCalcSelect##CalcName, RDOFunCalcSelectBase) \
{ \
DECLARE_FACTORY(RDOFunCalcSelect##CalcName) \
private: \
	RDOFunCalcSelect##CalcName(CREF(LPRDOFunCalcSelect) pSelect) \
		: RDOFunCalcSelectBase(pSelect, NULL) \
	{} \
	DECALRE_ICalc; \
};

DEFINE_CALC_SELECT_GROUP ( Exist     );
DEFINE_CALC_SELECT_GROUP ( NotExist  );
DEFINE_CALC_SELECT_GROUP ( ForAll    );
DEFINE_CALC_SELECT_GROUP ( NotForAll );
DEFINE_CALC_SELECT_METHOD( Empty     );
DEFINE_CALC_SELECT_METHOD( Size      );

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

	DECALRE_ICalc;
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
	DECALRE_ICalc;
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

	DECALRE_ICalc;
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

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateNumberedResource (RSS: создание нового временного ресурса или постоянного в начальный момент времени по индексу с параметрами)
// ----------------------------------------------------------------------------
CALC(RDOCalcCreateNumberedResource)
{
DECLARE_FACTORY(RDOCalcCreateNumberedResource)
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

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateProcessResource (SEIZE: создание нового ресурса процесса)
// ----------------------------------------------------------------------------
CALC_SUB(RDOCalcCreateProcessResource, RDOCalcCreateNumberedResource)
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

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdocalc.inl"

#endif //! _RDOCALC_H_
