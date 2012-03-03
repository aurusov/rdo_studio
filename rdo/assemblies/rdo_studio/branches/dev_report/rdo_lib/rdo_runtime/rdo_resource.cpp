#include "pch.h"
#include "rdo_resource.h"
#include "rdo_runtime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOResourceType
// ----------------------------------------------------------------------------
RDOResourceType::RDOResourceType( RDORuntimeParent* parent ):
	RDORuntimeObject( parent ),
	RDOTraceableObject( false )
{
}

// ----------------------------------------------------------------------------
// ---------- RDOResource
// ----------------------------------------------------------------------------
RDOResource::RDOResource( RDORuntime* rt, int id, unsigned int type, bool trace ):
	RDORuntimeObject( NULL ),
	RDOTraceableObject( trace ),
	RDORuntimeContainer( rt ),
	m_state( RDOResource::CS_None ),
	m_temporary( false ),
	m_type( type ),
	m_referenceCount( 0 )
{
	unsigned int newID;
	if ( id == -1 )
	{
		// Для временного ресурса ищем дырку в нумерации
		newID = getRuntime()->getFreeResourceId();
	}
	else
	{
		// Вызываем для увеличения счетчика maxResourcesId постоянных ресурсов
		getRuntime()->getFreeResourceId( id );
		newID = id;
	}
	setTraceID( newID, newID + 1 );
}

RDOResource::RDOResource( const RDOResource& copy ):
	RDORuntimeObject( NULL ),
	RDOTraceableObject( copy.traceable() ),
	RDORuntimeContainer( copy.getRuntime() ),
	m_type( copy.m_type ),
	m_state( copy.m_state ),
	m_typeId( copy.m_typeId ),
	m_temporary( copy.m_temporary ),
	m_params( copy.m_params )
{
	setTraceID( copy.getTraceID() );
	m_referenceCount = 0;
//	getRuntime()->incrementResourceIdReference( getTraceID() );
}

RDOResource::~RDOResource()
{
	getRuntime()->fireMessage( this, RDORuntime::RO_BEFOREDELETE, (void*)getTraceID() );
	getRuntime()->onResourceErase( this );
}

bool RDOResource::operator!= (RDOResource &other)
{
	if ( m_type != other.m_type ) return true;
	if ( m_params.size() != other.m_params.size() ) return true;

	int size = m_params.size();
	for ( int i = 0; i < size; i++ ) {
		if ( m_params.at(i) != other.m_params.at(i) ) return true;
	}
	return false;
}

std::string RDOResource::getTypeId()
{
	std::ostringstream str;
	str << m_type;
	return str.str();
}

std::string RDOResource::traceParametersValue()
{
	std::ostringstream str;
	if(m_params.size() > 0)
	{
		std::vector<RDOValue>::iterator end = m_params.end();
		for(std::vector<RDOValue>::iterator it = m_params.begin();;)
		{
#ifdef RDOSIM_COMPATIBLE
			std::ostringstream _str;
			_str << *it;
			std::string::size_type pos = _str.str().find( "e" );
			if ( pos != std::string::npos ) {
				std::string __str = _str.str();
				__str.erase( pos + 2, 1 );
				str << __str.c_str();
			} else {
				str << _str.str().c_str();
			}
#else
			str << *it;
#endif
			if(++it == end)
				break;
			str << " ";
		}
	}
	return str.str();
}

std::string RDOResource::traceResourceState( char prefix, RDOSimulatorTrace* sim )
{
	std::ostringstream res;
	if ( traceable() || (prefix != '\0') ) {
		if ( m_state == RDOResource::CS_NoChange || m_state == RDOResource::CS_NonExist ) return "";
		if ( prefix != '\0' ) res << prefix;
		switch ( m_state ) {
			case RDOResource::CS_Create: res << "RC "; break;
			case RDOResource::CS_Erase : res << "RE "
#ifdef RDOSIM_COMPATIBLE
				<< sim->getCurrentTime() << " " << traceTypeId() << " " << traceId() << std::endl; return res.str();
#else
				;
#endif
				break;
			default                         : res << "RK "; break;
		}
		res << sim->getCurrentTime() << " " << traceTypeId() << " " << traceId() << " " << traceParametersValue() << std::endl;
	}
	return res.str();
}

} // namespace rdoRuntime
