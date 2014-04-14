/*!
  \copyright (c) RDO-Team, 2011
  \file      simtrace.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     Трассировка симулятора
  \indent    4T
*/

#ifndef _LIB_RUNTIME_SIM_TRACE_H_
#define _LIB_RUNTIME_SIM_TRACE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/rdo_simulator.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDODPTSearchTrace;
class RDODPTSearchTrace;
class RDOEvent;
class RDORule;
class RDOOperation;

PREDECLARE_POINTER(RDOSimulatorTrace);

/*!
  \class     RDOSimulatorTrace
  \brief     Трассировщик симулятора
*/
class RDOSimulatorTrace: public RDOSimulator
{
DECLARE_FACTORY(RDOSimulatorTrace)
public:
	enum { UNDEFINE_TIME = -1 };

	virtual void rdoInit();

	PTR(RDOTrace) getTracer() const;
	bool canTrace() const;

	double getTraceStartTime() const;
	void   setTraceStartTime(double value);

	double getTraceEndTime() const;
	void   setTraceEndTime(double value);

	virtual void onNewTimeNow();

	void memory_insert(std::size_t mem);
	void memory_remove(std::size_t mem);
	std::size_t memory_get() const;

	std::size_t getResourceId();
	void incrementResourceIdReference(int id);

	void freeOperationId(int id);
	int getFreeOperationId(); 
	void onResourceErase(CREF(LPRDOResource) pResource);

	int getFreeEventId();
	int getFreeActivityId();

	int getFreeDPTId();

protected:
	RDOSimulatorTrace();
	virtual ~RDOSimulatorTrace();

	void copyFrom(CREF(LPRDOSimulatorTrace) pOther);

	RDOTrace* m_tracer;

	int maxOperationId;

	virtual void preProcess();
	virtual void postProcess();
	void checkRSSDefinedResources();

	virtual std::list<LPRDOResource> getResourcesBeforeSim() const = 0;

private:
	double traceStartTime;
	double traceEndTime;

	std::size_t maxResourcesId;

	std::list<std::size_t> freeResourcesIds;
	typedef std::map<int, int> MAPII;
	MAPII resourcesIdsRefs;
	std::list<int> freeOperationsIds;

	void eraseFreeResourceId(std::size_t id);

	int m_ieCounter;
	int m_eventCounter;
	int m_activityCounter;
	int m_dptCounter;

	void addTemplateDecisionPoint(RDODPTSearchTrace *dp  );
	void addTemplateEvent        (RDOEvent          *ev  );
	void addTemplateRule         (RDORule           *rule);
	void addTemplateOperation    (RDOOperation      *op  );

	std::size_t memory_current;
	std::size_t memory_max;

	bool timeForTrace() const;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/simtrace.inl"

#endif // _LIB_RUNTIME_SIM_TRACE_H_
