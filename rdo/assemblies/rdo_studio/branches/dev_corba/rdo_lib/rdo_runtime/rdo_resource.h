#ifndef RDO_RESOURCE_H
#define RDO_RESOURCE_H

#include "rdotrace.h"
#include "rdoruntime_object.h"

namespace rdoRuntime
{

class RDORuntime;

// ----------------------------------------------------------------------------
// ---------- RDOResourceType
// ----------------------------------------------------------------------------
class RDOResourceType: public RDORuntimeObject, public RDOTraceableObject
{
public:
	RDOResourceType( RDORuntimeParent* parent );
};

// ----------------------------------------------------------------------------
// ---------- RDOResource
// ----------------------------------------------------------------------------
class RDOResource: public RDORuntimeObject, public RDOTraceableObject, public RDORuntimeContainer
{
public:
	enum ConvertStatus {
		CS_None = 0,
		CS_Keep,
		CS_Create,
		CS_Erase,
		CS_NonExist,
		CS_NoChange
	};

	RDOResource( RDORuntime* rt, int id, unsigned int type, bool trace );
	RDOResource( const RDOResource& copy );
	virtual ~RDOResource();

	void setRuntime(RDORuntime* runtime) { RDORuntimeContainer::setRuntime(runtime); }

	virtual std::string whoAreYou()      { return "rdoRes";     }
	void makeTemporary( bool value )     { m_temporary = value; }
	ConvertStatus getState() const       { return m_state;      }
	void setState( ConvertStatus value ) { m_state = value;     }
	std::string traceResourceState( char prefix, RDOSimulatorTrace* sim );

	bool checkType( unsigned int type ) const
	{
		return m_type == type;
	}

	virtual const RDOValue& getParam( unsigned int index ) const
	{
		return m_params.at( index );
	}
	virtual void setParam( unsigned int index, const RDOValue& value )
	{
		if ( m_params.size() <= index ) {
			m_params.resize( index + 1 );
		}
		setState(CS_Keep);
		m_params.at(index) = value;
	}
	virtual ruint paramsCount() const
	{
		return m_params.size();
	}
	virtual void appendParams( const std::vector< RDOValue >::const_iterator& from_begin, const std::vector< RDOValue >::const_iterator& from_end )
	{
		m_params.insert( m_params.end(), from_begin, from_end );
	}

	std::string getTypeId();
	std::string traceParametersValue();
	bool operator!= ( RDOResource& other );

	bool canFree() const { return m_referenceCount == 0; }
	void incRef()        { m_referenceCount++;           }
	void decRef()        { m_referenceCount--;           }

protected:
	std::vector< RDOValue > m_params;
	bool                    m_temporary;
	ConvertStatus           m_state;

private:
	unsigned int            m_type;
	unsigned int            m_referenceCount;

	std::string             m_typeId;

	std::string traceTypeId()
	{
		return m_typeId.empty() ? (m_typeId = getTypeId()) : m_typeId;
	}
};

} // namespace rdoRuntime

#endif // RDO_RESOURCE_H
