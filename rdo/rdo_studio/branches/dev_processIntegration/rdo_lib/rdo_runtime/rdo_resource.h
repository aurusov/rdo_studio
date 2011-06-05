/**
 @file    rdo_resource.h
 @authors Урусов Андрей, Лущан Дмитрий
 @date    03.06.2011
 @brief   Ресурсы и типы ресурсов в runtime
 @indent  4T
 */

#ifndef RDO_RESOURCE_H
#define RDO_RESOURCE_H

#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdo_resource_i.h"
#include "rdo_lib/rdo_runtime/rdoprocess.h"

namespace rdoRuntime
{

class RDORuntime;
class RDOResource;

// ----------------------------------------------------------------------------
// ---------- RDOResourceType
// ----------------------------------------------------------------------------
OBJECT(RDOResourceType) IS INSTANCE_OF(IResourceType) AND INSTANCE_OF(RDORuntimeObject) AND INSTANCE_OF(RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceType);
public:
	RDOResourceType(rsint number, RDORuntimeParent* parent = 0);
	DECLARE_IResourceType;

private:
	rsint m_id;
};

// ----------------------------------------------------------------------------
// ---------- RDOResourceTypeTransact
// ----------------------------------------------------------------------------
OBJECT(RDOResourceTypeTransact) IS INSTANCE_OF(IResourceType) AND INSTANCE_OF(RDORuntimeObject) AND INSTANCE_OF(RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceTypeTransact);
public:
	RDOResourceTypeTransact(rsint number, RDORuntimeParent* parent = 0);
	DECLARE_IResourceType;

private:
	rsint m_id;
};

// ----------------------------------------------------------------------------
// ---------- RDOResourceTypeProccess
// ----------------------------------------------------------------------------
OBJECT(RDOResourceTypeProccess) IS INSTANCE_OF(IResourceType) AND INSTANCE_OF(RDORuntimeObject) AND INSTANCE_OF(RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceTypeProccess);
public:
	RDOResourceTypeProccess(rsint number, RDORuntimeParent* parent = 0);
	DECLARE_IResourceType;

private:
	rsint m_id;
};

// ----------------------------------------------------------------------------
// ---------- RDOResource
// ----------------------------------------------------------------------------
class RDOResource: public RDORuntimeObject, public RDOTraceableObject, public RDORuntimeContainer
{
friend class RDOResourceType;
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

	CREF(RDOValue) getParam(ruint index) const
	{
		ASSERT(index < m_params.size());
		return m_params[index];
	}
	REF(RDOValue) getParamRaw(ruint index)
	{
		ASSERT(index < m_params.size());
		setState(CS_Keep);
		return m_params[index];
	}
	void setParam(ruint index, CREF(RDOValue) value)
	{
		if (m_params.size() <= index)
		{
			m_params.resize(index + 1);
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
