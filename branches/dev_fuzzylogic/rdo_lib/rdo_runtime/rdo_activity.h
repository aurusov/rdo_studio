#ifndef RDO_ACTIVITY_H
#define RDO_ACTIVITY_H

#include "rdo.h"
#include "rdo_resource.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOActivity
// ----------------------------------------------------------------------------
class RDOActivity: public RDOBaseOperation, public RDOTraceableObject
{
public:
	void addParamCalc( RDOCalc* calc )
	{
		m_paramsCalcs.push_back( calc );
	}
	int getResByRelRes( unsigned int rel_res_id ) const
	{
		if ( m_relResID.size() <= rel_res_id ) {
			return 0;
		}
		return m_relResID.at( rel_res_id ); 
	}
	void setRelRes( unsigned int rel_res_id, unsigned int res_id )
	{
		if ( m_relResID.size() <= rel_res_id ) {
			m_relResID.resize( rel_res_id + 1 );
		}
		m_relResID[rel_res_id] = res_id; 
	}

protected:
	RDOActivity( RDORuntimeParent* parent, bool trace, const std::string& name ):
		RDOBaseOperation( parent ),
		RDOTraceableObject( trace ),
		m_oprName( name )
	{
	}
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

	std::string traceResourcesList( char prefix, RDOSimulatorTrace* sim );
	std::string traceResourcesListNumbers( RDOSimulatorTrace* sim, bool show_create_index = true );
};

// ----------------------------------------------------------------------------
// ---------- RDOActivityPattern
// ----------------------------------------------------------------------------
template< class T >
class RDOActivityPattern: public RDOActivity
{
public:
	void writeModelStructure( std::stringstream& stream )
	{
		stream << m_oprName << " " << tracePatternId() << std::endl;
	}
	const std::string& tracePatternId() const
	{
		return m_pattern->traceId();
	}

protected:
	RDOActivityPattern( RDORuntimeParent* parent, T* pattern, bool trace, const std::string& name ):
		RDOActivity( parent, trace, name ),
		m_pattern( pattern )
	{
	}
	virtual ~RDOActivityPattern() {}

	T* m_pattern;
};

} // namespace rdoRuntime

#endif // RDO_ACTIVITY_H
