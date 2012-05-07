/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_ie.h
  \author    
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDO_IE_H_
#define _RDO_IE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/rdo_pattern.h"
#include "simulator/runtime/rdo_activity.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPatternIrregEvent
// --------------------------------------------------------------------------------
class RDOPatternIrregEvent: public RDOPattern
{
friend class RDOIrregEvent;

public:
	RDOPatternIrregEvent(rbool trace);

	void addConvertorCalc  (CREF(LPRDOCalc) pCalc            ) { m_convertor.push_back(pCalc);        }
	void addConvertorStatus(RDOResource::ConvertStatus status) { m_convertorStatus.push_back(status); }
	void addEraseCalc      (CREF(LPRDOCalc) pCalc            ) { m_erase.push_back(pCalc);            }
	void setTime           (CREF(LPRDOCalc) timeCalc         ) { m_timeCalc = timeCalc;	              }

	void convertEvent(CREF(LPRDORuntime) pRuntime)
	{
		preSelectRelRes(pRuntime);
		runCalcs(m_convertor, pRuntime);
	}
	void convertErase(CREF(LPRDORuntime) pRuntime)
	{
		runCalcs(m_erase, pRuntime);
	}

	double getNextTimeInterval(CREF(LPRDORuntime) pRuntime);

private:
	LPRDOCalc         m_timeCalc;
	CalcList          m_convertor;
	ConvertStatusList m_convertorStatus;
	CalcList          m_erase;
};

// --------------------------------------------------------------------------------
// -------------------- RDOIrregEvent
// --------------------------------------------------------------------------------
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
	RDOIrregEvent(PTR(RDOPatternIrregEvent) pPattern, rbool trace, CREF(tstring) name);

	double  m_time;

	void   convertEvent       (CREF(LPRDORuntime) pRuntime);
	double getNextTimeInterval(CREF(LPRDORuntime) pRuntime);

	virtual void onBeforeIrregularEvent(CREF(LPRDORuntime) pRuntime);
	virtual void onAfterIrregularEvent (CREF(LPRDORuntime) pRuntime);

	DECLARE_IBaseOperation;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _RDO_IE_H_
