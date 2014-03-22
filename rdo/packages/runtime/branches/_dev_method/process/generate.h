/*!
  \copyright (c) RDO-Team, 2012
  \file      generate.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     Процессный оператор GENERATE
  \indent    4T
*/

#ifndef _LIB_RUNTIME_PROCESS_GENERATE_H_
#define _LIB_RUNTIME_PROCESS_GENERATE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

OBJECT_INTERFACE(IInternalStatistics)
{
DECLARE_FACTORY(IInternalStatistics)
public:
	virtual void setTransCount(ruint count) = 0;

protected:
	IInternalStatistics()
	{}
	virtual ~IInternalStatistics()
	{}
};

CLOSE_RDO_RUNTIME_NAMESPACE

class IInternalStatisticsManager
{
public:
	virtual void setStatistics(CREF(rdo::runtime::LPIInternalStatistics) pStatistics) = 0;

protected:
	IInternalStatisticsManager()
	{}
	virtual ~IInternalStatisticsManager()
	{}
};

#define DECLARE_IInternalStatisticsManager \
	virtual void setStatistics(CREF(rdo::runtime::LPIInternalStatistics) pStatistics);

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOPROCGenerate
  \brief   Процессный блок GENERATE
*/
class RDOPROCGenerate: public RDOPROCBlock, public IBaseOperation, public IInternalStatisticsManager
{
DEFINE_IFACTORY(RDOPROCGenerate);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlock              )
	QUERY_INTERFACE       (IBaseOperation            )
	QUERY_INTERFACE       (IInternalStatisticsManager)
QUERY_INTERFACE_END

public:
	void calcNextTimeInterval(CREF(LPRDORuntime) pRuntime);

private:
	RDOPROCGenerate(LPIPROCProcess process, CREF(LPRDOCalc) pTime, CREF(LPRDOCalc) pCreateAndGoOnTransactCalc, boost::optional<ruint> maxCreateTransactCount = boost::optional<ruint>());

	double                 timeNext;
	LPRDOCalc              m_pTimeCalc;
	LPRDOCalc              m_pCreateAndGoOnTransactCalc;
	boost::optional<ruint> m_maxCreateTransactCount;
	ruint                  m_createdTransactCount;
	LPIInternalStatistics  m_pStatistics;

	void onMakePlaned(CREF(LPRDORuntime) pRuntime);

	DECLARE_IBaseOperation;
	DECLARE_IInternalStatisticsManager;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/process/generate.inl"

#endif // _LIB_RUNTIME_PROCESS_GENERATE_H_
