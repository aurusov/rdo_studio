#ifndef RDO_IE_H
#define RDO_IE_H

#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/rdo_pattern.h"
#include "rdo_lib/rdo_runtime/rdo_activity.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOPatternIrregEvent
// ----------------------------------------------------------------------------
class RDOPatternIrregEvent: public RDOPattern
{
friend class RDOIrregEvent;

public:
	RDOPatternIrregEvent( PTR(RDORuntime) rTime, bool trace );

	void addConvertorCalc  ( CREF(LPRDOCalc) pCalc             ) { m_convertor.push_back( pCalc );        }
	void addConvertorStatus( RDOResource::ConvertStatus status ) { m_convertorStatus.push_back( status ); }
	void addEraseCalc      ( CREF(LPRDOCalc) pCalc             ) { m_erase.push_back( pCalc );            }
	void setTime           ( CREF(LPRDOCalc) timeCalc          ) { m_timeCalc = timeCalc;	              }

	void convertEvent(PTR(RDORuntime) runtime)
	{
		preSelectRelRes(runtime);
		runCalcs(m_convertor, runtime);
	}
	void convertErase(PTR(RDORuntime) runtime)
	{
		runCalcs(m_erase, runtime);
	}

	double getNextTimeInterval( PTR(RDORuntime) runtime );

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
	RDOIrregEvent( RDORuntime* runtime, RDOPatternIrregEvent* pattern, bool trace, const std::string& name );

	double  m_time;

	void convertEvent( RDOSimulator* sim );

	double getNextTimeInterval( RDOSimulator* sim );
	virtual void onBeforeIrregularEvent( RDOSimulator* sim );
	virtual void onAfterIrregularEvent( RDOSimulator* sim );

	DECLARE_IBaseOperation;
};

} // namespace rdoRuntime

#endif // RDO_IE_H
