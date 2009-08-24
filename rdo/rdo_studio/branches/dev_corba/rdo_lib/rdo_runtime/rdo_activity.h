#ifndef RDO_ACTIVITY_H
#define RDO_ACTIVITY_H

#include "rdo.h"
#include "rdo_resource.h"
#include "rdo_model_interface.h"
#include "rdo_activity_interface.h"
#include <rdostream.h>

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOActivity
// ----------------------------------------------------------------------------
class RDOActivity: public RDOTraceableObject, public IActivity, public IActivityTrace, CAST_TO_UNKNOWN
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOTraceableObject)
	QUERY_INTERFACE(IActivity)
	QUERY_INTERFACE(IActivityTrace)
QUERY_INTERFACE_END

protected:
	RDOActivity( RDORuntimeParent* parent, bool trace, CREF(tstring) name ):
		RDOTraceableObject( trace ),
		m_oprName( name )
	{}
	virtual ~RDOActivity() {}

	std::string               m_oprName;
	std::list< RDOResource* > m_relevantResources; // Список релевантных ресурсов
	std::vector< int >        m_relResID;          // Содержит список id ресурсов, которые стали релевантными образцу
	std::vector< RDOCalc* >   m_paramsCalcs;

	void setPatternParameters( RDOSimulator* sim );
	void getRelevantResources( RDOSimulator* sim, std::list< RDOResource* >& rel_res_list );
	void incrementRelevantResourceReference( RDOSimulator* sim );
	void decrementRelevantResourceReference( RDOSimulator* sim );

	void updateRelRes( RDOSimulator* sim )
	{
		getRelevantResources( sim, m_relevantResources );
	}
	void updateConvertStatus( RDOSimulator* sim, const std::vector< RDOResource::ConvertStatus >& status_list );

private:
	DECLARE_IActivity;
	DECLARE_IActivityTrace;
};

// ----------------------------------------------------------------------------
// ---------- RDOActivityPattern
// ----------------------------------------------------------------------------
template< class T >
class RDOActivityPattern: public RDOActivity, public IModelStructure, public IActivityPatternTrace
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOActivity)
	QUERY_INTERFACE(IModelStructure)
	QUERY_INTERFACE(IActivityPatternTrace)
QUERY_INTERFACE_END

protected:
	RDOActivityPattern( RDORuntimeParent* parent, T* pattern, bool trace, CREF(tstring) name ):
		RDOActivity( parent, trace, name ),
		m_pattern( pattern )
	{
	}
	virtual ~RDOActivityPattern() {}

	T* m_pattern;

private:
	void writeModelStructure(REF(std::ostream) stream) const
	{
		stream << m_oprName << " " << tracePatternId() << std::endl;
	}
	CREF(tstring) tracePatternId() const
	{
		return m_pattern->traceId();
	}
};

} // namespace rdoRuntime

#endif // RDO_ACTIVITY_H
