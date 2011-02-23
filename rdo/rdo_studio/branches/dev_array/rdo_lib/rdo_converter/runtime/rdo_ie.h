/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdo_ie.h
 * author   : 
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_IE_H_
#define _RDO_IE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/rdo_pattern.h"
#include "rdo_lib/rdo_runtime/rdo_activity.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOPatternIrregEvent
// ----------------------------------------------------------------------------
class RDOPatternIrregEvent: public RDOPattern
{
friend class RDOIrregEvent;

public:
	RDOPatternIrregEvent(PTR(RDORuntime) rTime, rbool trace);

	void addConvertorCalc  (CREF(LPRDOCalc) pCalc            ) { m_convertor.push_back(pCalc);        }
	void addConvertorStatus(RDOResource::ConvertStatus status) { m_convertorStatus.push_back(status); }
	void addEraseCalc      (CREF(LPRDOCalc) pCalc            ) { m_erase.push_back(pCalc);            }
	void setTime           (CREF(LPRDOCalc) timeCalc         ) { m_timeCalc = timeCalc;	              }

	void convertEvent(PTR(RDORuntime) pRuntime)
	{
		preSelectRelRes(pRuntime);
		runCalcs(m_convertor, pRuntime);
	}
	void convertErase(PTR(RDORuntime) pRuntime)
	{
		runCalcs(m_erase, pRuntime);
	}

	double getNextTimeInterval(PTR(RDORuntime) pRuntime);

private:
	LPRDOCalc         m_timeCalc;
	CalcList          m_convertor;
	ConvertStatusList m_convertorStatus;
	CalcList          m_erase;
};

// ----------------------------------------------------------------------------
// ---------- RDOIrregEvent
// ----------------------------------------------------------------------------
class RDOIrregEvent: public IBaseOperation, public RDOActivityPattern<RDOPatternIrregEvent>
{
typedef RDOActivityPattern<RDOPatternIrregEvent> pattern_type;
DEFINE_IFACTORY(RDOIrregEvent);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(pattern_type)
	QUERY_INTERFACE(IBaseOperation)
QUERY_INTERFACE_END

friend class RDOTrace;

private:
	RDOIrregEvent(PTR(RDORuntime) pRuntime, PTR(RDOPatternIrregEvent) pPattern, rbool trace, CREF(tstring) name);

	double  m_time;

	void   convertEvent       (PTR(RDOSimulator) pSimulator);
	double getNextTimeInterval(PTR(RDOSimulator) pSimulator);

	virtual void onBeforeIrregularEvent(PTR(RDOSimulator) pSimulator);
	virtual void onAfterIrregularEvent (PTR(RDOSimulator) pSimulator);

	DECLARE_IBaseOperation;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_IE_H_
