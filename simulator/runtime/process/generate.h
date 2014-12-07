#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_OBJECT_INTERFACE(IInternalStatistics)
struct IInternalStatistics: public rdo::RefCounter<IInternalStatistics>
{
DECLARE_FACTORY(IInternalStatistics)
public:
	virtual void setTransCount(std::size_t count) = 0;

protected:
	IInternalStatistics()
	{}
	virtual ~IInternalStatistics()
	{}
};

CLOSE_RDO_RUNTIME_NAMESPACE

class IInternalStatisticsManager: public virtual rdo::counter_reference
{
DECLARE_FACTORY(IInternalStatisticsManager)
public:
	virtual void setStatistics(const rdo::runtime::LPIInternalStatistics& pStatistics) = 0;

protected:
	IInternalStatisticsManager()
	{}
	virtual ~IInternalStatisticsManager()
	{}
};
DECLARE_POINTER(IInternalStatisticsManager)

#define DECLARE_IInternalStatisticsManager \
	virtual void setStatistics(const rdo::runtime::LPIInternalStatistics& pStatistics);

OPEN_RDO_RUNTIME_NAMESPACE

class RDOPROCGenerate: public RDOPROCBlock, public IInternalStatisticsManager
{
DECLARE_FACTORY(RDOPROCGenerate);
public:
	void calcNextTimeInterval(const LPRDORuntime& pRuntime);

private:
	RDOPROCGenerate(LPIPROCProcess process, const LPRDOCalc& pTime, const LPRDOCalc& pCreateAndGoOnTransactCalc, boost::optional<std::size_t> maxCreateTransactCount = boost::optional<std::size_t>());

	double timeNext;
	LPRDOCalc m_pTimeCalc;
	LPRDOCalc m_pCreateAndGoOnTransactCalc;
	boost::optional<std::size_t> m_maxCreateTransactCount;
	std::size_t m_createdTransactCount;
	LPIInternalStatistics m_pStatistics;

	void onMakePlaned(const LPRDORuntime& pRuntime);

	DECLARE_IBaseOperation;
	DECLARE_IInternalStatisticsManager;
};

CLOSE_RDO_RUNTIME_NAMESPACE
