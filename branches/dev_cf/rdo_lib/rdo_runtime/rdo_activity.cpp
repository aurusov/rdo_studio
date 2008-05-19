#include "pch.h"
#include "rdo_activity.h"
#include "rdo_runtime.h"
#include "rdocalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOActivity
// ----------------------------------------------------------------------------
void RDOActivity::setPatternParameters(RDOSimulator *sim) 
{
	RDORuntime* runtime = static_cast<RDORuntime*>(sim);
	int size = m_paramsCalcs.size();
	for ( int i = 0; i < size; i++ )
	{
		m_paramsCalcs.at(i)->calcValue( runtime );
	}
}

void RDOActivity::getRelevantResources( RDOSimulator* sim, std::list< RDOResource* >& rel_res_list )
{
	rel_res_list.clear();
	int size = m_relResID.size();
	for ( int i = 0; i < size; i++ )
	{
		rel_res_list.push_back( static_cast<RDORuntime*>(sim)->getResourceByID( m_relResID[i] ) );
	}
}

void RDOActivity::updateConvertStatus( RDOSimulator* sim, const std::vector< RDOResource::ConvertStatus >& status_list )
{
	updateRelRes( sim );
	int i = 0;
	std::list< RDOResource* >::iterator it = m_relevantResources.begin();
	while ( it != m_relevantResources.end() ) {
		RDOResource* res = *it;
		if ( res ) {
			res->setState( status_list[i] );
			switch ( status_list[i] ) {
				case RDOResource::CS_Create: {
					res->makeTemporary( true );
					break;
				}
			}
		}
		i++;
		it++;
	}
}

std::string RDOActivity::traceResourcesList( char prefix, RDOSimulatorTrace* sim )
{
	std::string res;
	for ( std::list< RDOResource* >::const_iterator i = m_relevantResources.begin(); i != m_relevantResources.end(); i++ ) {
		if ( *i ) {
			res += (*i)->traceResourceState( prefix, sim );
		}
	}
	return res;
}

std::string RDOActivity::traceResourcesListNumbers( RDOSimulatorTrace* sim, bool show_create_index )
{
	std::ostringstream res;
	res << m_relevantResources.size() << " ";
	for ( std::list< RDOResource* >::const_iterator i = m_relevantResources.begin(); i != m_relevantResources.end(); i++ ) {
#ifdef RDOSIM_COMPATIBLE
		if ( *i && (show_create_index || (!show_create_index && (*i)->getState() != RDOResource::CS_Create)) ) {
#else
		if ( *i ) {
#endif
			res << " " << (*i)->traceId();
		} else {
			res << " 0";
		}
	}
	return res.str();
}

void RDOActivity::incrementRelevantResourceReference( RDOSimulator* sim )
{
	for ( unsigned int i = 0; i < m_relResID.size(); i++ )
	{
		RDOResource* res = static_cast<RDORuntime*>(sim)->getResourceByID( m_relResID.at(i) );
		if ( res && (res->getState() == RDOResource::CS_Keep || res->getState() == RDOResource::CS_Create || res->getState() == RDOResource::CS_Erase ) ) res->incRef();
	}
}

void RDOActivity::decrementRelevantResourceReference( RDOSimulator* sim )
{
	for ( unsigned int i = 0; i < m_relResID.size(); i++ )
	{
		RDOResource* res = static_cast<RDORuntime*>(sim)->getResourceByID( m_relResID.at(i) );
		if ( res && (res->getState() == RDOResource::CS_Keep || res->getState() == RDOResource::CS_Create || res->getState() == RDOResource::CS_Erase ) ) res->decRef();
	}
}

} // namespace rdoRuntime
