#include "rdo_lib/rdo_runtime/pch.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdoprocess.h"

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOResourceType
// ----------------------------------------------------------------------------
RDOResourceType::RDOResourceType(rsint number, RDORuntimeParent* parent)
	: RDORuntimeObject  (parent)
	, RDOTraceableObject(false )
	, m_id              (number)
{}

LPRDOResource RDOResourceType::createRes(PTR(RDORuntime) runtime, bool trace) const
{
	return rdo::Factory<RDOResource>::create(runtime, runtime->getResourceId(), this->getTraceID(), trace);
}

// ----------------------------------------------------------------------------
// ---------- RDOResourceTypeTransact
// ----------------------------------------------------------------------------
RDOResourceTypeTransact::RDOResourceTypeTransact(rsint number, RDORuntimeParent* parent)
	: RDORuntimeObject  (parent)
	, RDOTraceableObject(false )
	, m_id              (number)
{}

LPRDOResource RDOResourceTypeTransact::createRes(PTR(RDORuntime) runtime, bool trace) const
{
	return rdo::Factory<RDOPROCTransact>::create(runtime, runtime->getResourceId(), this->getTraceID(), trace);
}

// ----------------------------------------------------------------------------
// ---------- RDOResourceTypeProccess
// ----------------------------------------------------------------------------
RDOResourceTypeProccess::RDOResourceTypeProccess(rsint number, RDORuntimeParent* parent)
	: RDORuntimeObject  (parent)
	, RDOTraceableObject(false )
	, m_id              (number)
{}

LPRDOResource RDOResourceTypeProccess::createRes(PTR(RDORuntime) runtime, bool trace) const
{
	return rdo::Factory<RDOPROCResource>::create(runtime, runtime->getResourceId(), this->getTraceID(), trace);
}

CLOSE_RDO_RUNTIME_NAMESPACE
