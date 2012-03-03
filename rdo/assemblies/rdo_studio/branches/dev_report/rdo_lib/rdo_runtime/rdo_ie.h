#ifndef RDO_IE_H
#define RDO_IE_H

#include "rdo.h"
#include "rdotrace.h"
#include "rdo_pattern.h"
#include "rdo_activity.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOIrregEvent
// ----------------------------------------------------------------------------
class RDOIrregEvent: public RDOActivityPattern<RDOPatternIrregEvent>
{
friend class RDOTrace;

public:
	RDOIrregEvent( RDORuntime* runtime, RDOPatternIrregEvent* pattern, bool trace, const std::string& name );

private:
	double  m_time;

	virtual void     onStart         ( RDOSimulator* sim );
	virtual void     onStop          ( RDOSimulator* sim );
	virtual bool     onCheckCondition( RDOSimulator* sim );
	virtual BOResult onDoOperation   ( RDOSimulator* sim );
	virtual void     onMakePlaned    ( RDOSimulator* sim, void* param = NULL );

	void convertEvent( RDOSimulator* sim );

	double getNextTimeInterval( RDOSimulator* sim );
	virtual void onBeforeIrregularEvent( RDOSimulator* sim );
	virtual void onAfterIrregularEvent( RDOSimulator* sim );
};

} // namespace rdoRuntime

#endif // RDO_IE_H
