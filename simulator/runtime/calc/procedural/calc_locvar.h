#ifndef _LIB_RUNTIME_CALC_PROCEDURAL_LOCVAR_H_
#define _LIB_RUNTIME_CALC_PROCEDURAL_LOCVAR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/set_operation_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOCalcCreateLocalVariable);
class RDOCalcCreateLocalVariable: public RDOCalc
{
DECLARE_FACTORY(RDOCalcCreateLocalVariable)
private:
	RDOCalcCreateLocalVariable(const std::string& name, const LPRDOCalc& pValueCalc);

	std::string m_name;
	LPRDOCalc  m_pValueCalc;

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcGetLocalVariable);
class RDOCalcGetLocalVariable: public RDOCalc
{
DECLARE_FACTORY(RDOCalcGetLocalVariable)
private:
	RDOCalcGetLocalVariable(const std::string& name);

	std::string m_name;

	DECLARE_ICalc;
};

class RDOCalcSetLocalVariable: public RDOCalc
{
DECLARE_FACTORY(RDOCalcSetLocalVariable)
private:
	RDOCalcSetLocalVariable(const std::string& name, LPRDOCalc pCalc = NULL);
	virtual ~RDOCalcSetLocalVariable();

	std::string m_name;
	LPRDOCalc m_pCalc;

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcLocalVariableList);
class RDOCalcLocalVariableList: public RDOCalc
{
DECLARE_FACTORY(RDOCalcLocalVariableList)
public:
	typedef std::vector<LPRDOCalc> CalcLocalVariableList;

	void addCalcLocalVariable(const LPRDOCalc& pCalc);

private:
	RDOCalcLocalVariableList();

	CalcLocalVariableList m_calcLocalVariableList;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_PROCEDURAL_LOCVAR_H_
