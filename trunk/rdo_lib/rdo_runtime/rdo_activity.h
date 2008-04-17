#ifndef RDO_ACTIVITY_H
#define RDO_ACTIVITY_H

#include "rdo_pattern.h"
#include "rdotrace.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOActivity
// ----------------------------------------------------------------------------
class RDOActivity
{
public:
	void addParamCalc( RDOCalc* calc )
	{
		m_paramsCalcs.push_back( calc );
	}
	int getResByRelRes( const int rel_res_id ) const
	{
		if ( m_relResID.size() <= rel_res_id ) {
			return 0;
		}
		return m_relResID.at( rel_res_id ); 
	}
	void setRelRes( int rel_res_id, int res_id )
	{
		if ( m_relResID.size() <= rel_res_id ) {
			m_relResID.resize( rel_res_id + 1 );
		}
		m_relResID[rel_res_id] = res_id; 
	}

protected:
	RDOActivity( const std::string& name ):
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
		stream << m_oprName << " " << m_pattern->traceId() << std::endl;
	}

protected:
	RDOActivityPattern( T* pattern, const std::string& name ):
		RDOActivity( name ),
		m_pattern( pattern )
	{
	}
	virtual ~RDOActivityPattern() {}

	T* m_pattern;
};

} // namespace rdoRuntime

#endif // RDO_ACTIVITY_H
