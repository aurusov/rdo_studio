#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/set_operation_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOCalcGetResourceHelper
{
public:
	static bool getResource(const LPRDORuntime& pRuntime, std::size_t resourceID, RDOValue& result);
};

PREDECLARE_POINTER(RDOCalcGetResourceByID);
class RDOCalcGetResourceByID: public RDOCalc
{
DECLARE_FACTORY(RDOCalcGetResourceByID)
private:
	RDOCalcGetResourceByID(const std::size_t& resourceID);

	std::size_t m_resourceID;

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcGetResourceParam);
class RDOCalcGetResourceParam: public RDOCalc
{
DECLARE_FACTORY(RDOCalcGetResourceParam)
private:
	RDOCalcGetResourceParam(const LPRDOCalc& pResource, std::size_t paramID);

	LPRDOCalc m_pResource;
	int m_paramID;

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcGetUnknowResParam);
class RDOCalcGetUnknowResParam: public RDOCalc
{
DECLARE_FACTORY(RDOCalcGetUnknowResParam)
private:
	RDOCalcGetUnknowResParam(const std::string& resName, const std::string& parName);

	std::string m_resName;
	std::string m_parName;

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOSetResourceParamCalc);
class RDOSetResourceParamCalc: public RDOCalc
{
DECLARE_FACTORY(RDOSetResourceParamCalc)
private:
	RDOSetResourceParamCalc(std::size_t resourceID, std::size_t paramID, const LPRDOCalc& pCalc);

	std::size_t m_resourceID;
	std::size_t m_paramID;
	LPRDOCalc m_pCalc;

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcSetResourceTrace);
class RDOCalcSetResourceTrace: public RDOCalc
{
DECLARE_FACTORY(RDOCalcSetResourceTrace)
private:
	RDOCalcSetResourceTrace(const LPRDOCalc& getResource, bool traceValue);

	LPRDOCalc m_getResource;
	bool  m_traceValue;

	DECLARE_ICalc;
};

template <SetOperationType::Type setOperationType>
class RDOSetResourceParam: public RDOCalc
{
public:
	RDOSetResourceParam(const LPRDOCalc& getResource, const std::size_t paramID, const LPRDOCalc& pCalc = NULL);
	virtual ~RDOSetResourceParam();

protected:
	LPRDOCalc m_getResource;
	std::size_t m_paramID;
	LPRDOCalc m_pCalc;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/resource/calc_resource-inl.h"
