/*!
  \copyright (c) RDO-Team, 2011
  \file      rdocalc.inl
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.07.2009
  \brief     Вычислитель
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdomacros.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalc
// --------------------------------------------------------------------------------
inline rbool RDOCalc::compare(CREF(LPRDOCalc) pCalc) const
{
	UNUSED(pCalc);
	return false;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetResParam
// --------------------------------------------------------------------------------
inline RDOCalcGetResParam::RDOCalcGetResParam(int _resNumb, int _parNumb)
	: m_resID  (_resNumb)
	, m_paramID(_parNumb)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetTempResParamFRM
// --------------------------------------------------------------------------------
inline void RDOCalcGetTempResParamFRM::notify(ruint message, PTR(void) pParam)
{
	UNUSED(message);

	if (m_resID == *reinterpret_cast<PTR(int)>(pParam))
	{
		m_resID = -1;
	}
};

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetUnknowResParam
// --------------------------------------------------------------------------------
inline RDOCalcGetUnknowResParam::RDOCalcGetUnknowResParam(CREF(tstring) resName, CREF(tstring) parName)
	: m_resName(resName)
	, m_parName(parName)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetGroupResParam
// --------------------------------------------------------------------------------
inline RDOCalcGetGroupResParam::RDOCalcGetGroupResParam(int parNumb)
	: m_parNumb(parNumb)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOSetResourceParamCalc
// --------------------------------------------------------------------------------
inline RDOSetResourceParamCalc::RDOSetResourceParamCalc(int resNumb, int parNumb, CREF(LPRDOCalc) pCalc)
	: m_resNumb(resNumb)
	, m_parNumb(parNumb)
	, m_pCalc  (pCalc  )
{
	m_value = true;
	if (m_pCalc)
		setSrcInfo(m_pCalc->src_info());
}

// --------------------------------------------------------------------------------
// -------------------- RDOSetPatternParamCalc
// --------------------------------------------------------------------------------
inline RDOSetPatternParamCalc::RDOSetPatternParamCalc(int parNumb, RDOValue val)
	: m_parNumb(parNumb)
	, m_val    (val    )
{
	m_value = 0;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcPatParam
// --------------------------------------------------------------------------------
inline RDOCalcPatParam::RDOCalcPatParam(int numberOfParam)
	: m_numberOfParam(numberOfParam)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalc
// --------------------------------------------------------------------------------
inline RDOFunCalc::RDOFunCalc()
{}

inline void RDOFunCalc::addRetCalc(CREF(LPRDOCalc) pCalc)
{
	UNUSED(pCalc);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFuncTableCalc
// --------------------------------------------------------------------------------
inline RDOFuncTableCalc::RDOFuncTableCalc(CREF(LPRDOCalc) pArgCalc)
	: m_pArgCalc(pArgCalc)
{}

inline void RDOFuncTableCalc::addResultCalc(CREF(LPRDOCalcConst) res)
{
	m_results.push_back(res);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunListCalc
// --------------------------------------------------------------------------------
inline RDOFunListCalc::RDOFunListCalc(CREF(LPRDOCalcConst) pDefaultValue)
	: m_pDefaultValue(pDefaultValue)
{}

inline void RDOFunListCalc::addCase(CREF(LPRDOCalc) pCaseCalc, CREF(LPRDOCalcConst) pResultCalc)
{
	m_cases  .push_back(pCaseCalc  ); 
	m_results.push_back(pResultCalc);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunAlgorithmicCalc
// --------------------------------------------------------------------------------
inline RDOFunAlgorithmicCalc::RDOFunAlgorithmicCalc()
{
	m_value = 0;
}

inline void RDOFunAlgorithmicCalc::addCalcIf(CREF(LPRDOCalc) pCond, CREF(LPRDOCalc) pAct)
{
	m_conditions.push_back(pCond);
	m_actions   .push_back(pAct );
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunAlgorithmicDiapCalc
// --------------------------------------------------------------------------------
inline RDOFunAlgorithmicDiapCalc::RDOFunAlgorithmicDiapCalc(CREF(RDOValue) min_value, CREF(RDOValue) max_value)
	: m_min_value          (min_value)
	, m_max_value          (max_value)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcGroup
// --------------------------------------------------------------------------------
inline RDOFunCalcGroup::RDOFunCalcGroup(int nResType, CREF(LPRDOCalc) pCondition)
	: m_nResType  (nResType  )
	, m_pCondition(pCondition)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelect
// --------------------------------------------------------------------------------
inline RDOFunCalcSelect::RDOFunCalcSelect(int nResType, CREF(LPRDOCalc) pCondition)
	: RDOFunCalcGroup(nResType, pCondition)
{
	m_value = 1;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalcSelectBase
// --------------------------------------------------------------------------------
inline RDOFunCalcSelectBase::RDOFunCalcSelectBase(CREF(LPRDOFunCalcSelect) pSelect, CREF(LPRDOCalc) pCondition)
	: m_pSelect   (pSelect   )
	, m_pCondition(pCondition)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFuncParam
// --------------------------------------------------------------------------------
inline RDOCalcFuncParam::RDOCalcFuncParam(int param_number, CREF(RDOSrcInfo) _src_info)
	: m_param_number(param_number)
{
	setSrcInfo( _src_info );
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetConst
// --------------------------------------------------------------------------------
inline RDOCalcGetConst::RDOCalcGetConst(int number)
	: m_number(number)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcSetConst
// --------------------------------------------------------------------------------
inline RDOCalcSetConst::RDOCalcSetConst(int number, CREF(LPRDOCalc) pCalc)
	: m_number(number)
	, m_pCalc (pCalc )
{
	m_value = 0;
	if (m_pCalc) setSrcInfo(m_pCalc->src_info());
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFunctionCall
// --------------------------------------------------------------------------------
inline RDOCalcFunctionCall::RDOCalcFunctionCall(CREF(LPRDOFunCalc) pFunction)
	: m_pFunction(pFunction)
{}

inline void RDOCalcFunctionCall::addParameter(CREF(LPRDOCalc) pCalc)
{
	m_parameters.push_back(pCalc);
}

inline void RDOCalcFunctionCall::setFunctionCalc(CREF(LPRDOFunCalc) pFunction)
{
	m_pFunction = pFunction;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcConst
// --------------------------------------------------------------------------------
inline RDOCalcConst::RDOCalcConst(CREF(RDOValue) value)
{
	m_value = value;
}

inline CREF(RDOValue) RDOCalcConst::getValue() const
{
	return m_value;
}

inline REF(RDOValue) RDOCalcConst::doCalc(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);

	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
