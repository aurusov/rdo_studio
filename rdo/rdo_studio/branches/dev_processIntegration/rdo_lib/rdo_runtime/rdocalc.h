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
#include "rdo_lib/rdo_runtime/rdo_res_type.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalc
// ----------------------------------------------------------------------------
OBJECT(RDOCalc) IS INSTANCE_OF(RDOSrcInfo)
{
DECLARE_FACTORY(RDOCalc)

public:
	REF(RDOValue) calcValue(PTR(RDORuntime) pRuntime);

	virtual rbool compare(CREF(LPRDOCalc) pCalc) const
	{
		return false;
	}

protected:
	RDOCalc();
	virtual ~RDOCalc();

	RDOValue m_value;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) pRuntime) = 0;
};

#define DECALRE_ICalc \
private:              \
	REF(RDOValue) doCalc(PTR(RDORuntime) pRuntime);

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
	virtual REF(RDOValue) doCalc(PTR(RDORuntime) pRuntime);

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
// ---------- RDOFunCalc
// ----------------------------------------------------------------------------
CALC(RDOFunCalc)
{
public:
	virtual void addRetCalc(CREF(LPRDOCalc) pCalc)
	{}

protected:
	RDOFunCalc()
	{}
};

// ----------------------------------------------------------------------------
// ---------- RDOFuncTableCalc
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
 	virtual REF(RDOValue) doCalc(PTR(RDORuntime) pRuntime); \
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
	mutable std::list<LPRDOResource> res_list;
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
// ---------- RDOCalcCreateNumberedResource (рудимент для rdo_converter)
// ----------------------------------------------------------------------------
CALC(RDOCalcCreateNumberedResource)
{
DECLARE_FACTORY(RDOCalcCreateNumberedResource)
public:
	ruint getNumber() const {NEVER_REACH_HERE;}
	virtual PTR(RDOResource) createResource(PTR(RDORuntime) pRuntime) const {NEVER_REACH_HERE; return 0;}

protected:
	RDOCalcCreateNumberedResource(int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _paramsCalcs, int _number, rbool _isPermanent);

	int                    m_pType;
	rbool                  traceFlag;
	std::vector<RDOValue>  paramsCalcs;
	ruint                  number;
	rbool                  isPermanent;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateProcessResource (рудимент для rdo_converter)
// ----------------------------------------------------------------------------
CALC(RDOCalcCreateProcessResource)
{
DECLARE_FACTORY(RDOCalcCreateProcessResource)
public:
	ruint getNumber() const {NEVER_REACH_HERE;}
	virtual PTR(RDOResource) createResource(PTR(RDORuntime) pRuntime) const {NEVER_REACH_HERE; return 0;}

protected:
	RDOCalcCreateProcessResource(rsint _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _paramsCalcs, int _number, rbool _isPermanent);

	rsint                  m_pType;
	rbool                  traceFlag;
	std::vector<RDOValue>  paramsCalcs;
	ruint                  number;
	rbool                  isPermanent;

	DECALRE_ICalc;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateResource (создание нового ресурса)
// ----------------------------------------------------------------------------
CALC(RDOCalcCreateResource)
{
DECLARE_FACTORY(RDOCalcCreateResource)
private:
	//! relResID == 0 для ресурсов, создаваемых при инициализации модели
	RDOCalcCreateResource(LPRDOResourceType pType, CREF(std::vector<RDOValue>) rParamsCalcs, rbool traceFlag, rbool permanentFlag, ruint relResID = 0);

	LPRDOResourceType      m_pResType;
	std::vector<RDOValue>  m_paramsCalcs;
	rbool                  m_traceFlag;
	rbool                  m_permanentFlag;
	ruint                  m_relResID;

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdocalc.inl"

#endif //! _RDOCALC_H_
