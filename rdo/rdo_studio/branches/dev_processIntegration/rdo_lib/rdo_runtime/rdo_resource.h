/**
 @file    rdo_resource.h
 @authors Урусов Андрей, Лущан Дмитрий
 @date    03.06.2011
 @brief   Ресурсы в runtime
 @indent  4T
 */

#ifndef RDO_RESOURCE_H
#define RDO_RESOURCE_H

#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"

OPEN_RDO_RUNTIME_NAMESPACE

class RDORuntime;

// ----------------------------------------------------------------------------
// ---------- RDOResource
// ----------------------------------------------------------------------------
OBJECT(RDOResource) IS INSTANCE_OF(RDORuntimeObject) AND INSTANCE_OF(RDOTraceableObject) AND INSTANCE_OF(RDORuntimeContainer)
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

	RDOResource(PTR(RDORuntime) runtime, LPIResourceType pResType, ruint resID, ruint typeID, bool trace);
	RDOResource(const RDOResource& copy);
	virtual ~RDOResource();

	CREF(LPRDOResource) clone();

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

	CREF(LPIResourceType) getResType() const {return m_resType;}
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
	ruint            m_type;
	ruint            m_referenceCount;
	LPIResourceType  m_resType;

	std::string      m_typeId;

	std::string traceTypeId()
	{
		return m_typeId.empty() ? (m_typeId = getTypeId()) : m_typeId;
	}
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // RDO_RESOURCE_H
