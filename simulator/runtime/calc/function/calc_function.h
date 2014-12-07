#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_const.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOFunCalc);
class RDOFunCalc: public RDOCalc
{
public:
	virtual void addRetCalc(const LPRDOCalc& pCalc);

protected:
	RDOFunCalc();
};

PREDECLARE_POINTER(RDOFuncTableCalc);
class RDOFuncTableCalc: public RDOFunCalc
{
DECLARE_FACTORY(RDOFuncTableCalc)
public:
	void addResultCalc(const LPRDOCalcConst& pResult);

private:
	RDOFuncTableCalc(const LPRDOCalc& pArgument);

	typedef  std::vector<LPRDOCalcConst>  ResultList;

	ResultList  m_pResultList;
	LPRDOCalc   m_pArgument;

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOFunListCalc);
class RDOFunListCalc: public RDOFunCalc
{
DECLARE_FACTORY(RDOFunListCalc)
public:
	void addCase(const LPRDOCalc& pCase, const LPRDOCalcConst& pResult);

private:
	RDOFunListCalc(const LPRDOCalcConst& pDefaultValue);

	typedef  std::vector<LPRDOCalcConst>  ResultList;

	RDOCalcList     m_caseList;
	ResultList      m_resultList;
	LPRDOCalcConst  m_pDefaultValue;

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOFunAlgorithmicCalc);
class RDOFunAlgorithmicCalc: public RDOFunCalc
{
DECLARE_FACTORY(RDOFunAlgorithmicCalc)
public:
	void addCalcIf(const LPRDOCalc& pCondition, const LPRDOCalc& pAction);

protected:
	RDOFunAlgorithmicCalc();

	RDOCalcList  m_conditionList;
	RDOCalcList  m_actionList;

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcFuncParam);
class RDOCalcFuncParam: public RDOCalc
{
DECLARE_FACTORY(RDOCalcFuncParam)
private:
	RDOCalcFuncParam(std::size_t paramID, const RDOSrcInfo& src_info);

	std::size_t m_paramID;

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcGetConst);
class RDOCalcGetConst: public RDOCalc
{
DECLARE_FACTORY(RDOCalcGetConst)
private:
	RDOCalcGetConst(std::size_t constantID);

	std::size_t m_constantID;

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcSetConst);
class RDOCalcSetConst: public RDOCalc
{
DECLARE_FACTORY(RDOCalcSetConst)
private:
	RDOCalcSetConst(std::size_t constantID, const LPRDOCalc& pCalc);

	std::size_t m_constantID;
	LPRDOCalc m_pCalc;

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcFunctionCaller);
class RDOCalcFunctionCaller: public RDOCalc
{
DECLARE_FACTORY(RDOCalcFunctionCaller)
public:
	void addParameter   (const LPRDOCalc& pParam   );
	void setFunctionCalc(const LPRDOCalc& pFunction);

	const LPRDOCalc& function() const;

private:
	RDOCalcFunctionCaller(const LPRDOCalc& pFunction);

	RDOCalcList  m_paramList;
	LPRDOCalc    m_pFunction;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE
