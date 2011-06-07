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

LPRDOResource RDOResourceType::createRes(PTR(RDORuntime) rt, ruint id, bool trace) const
{
	return new RDOResource(rt, id, this->getTraceID(), trace);
}

// ----------------------------------------------------------------------------
// ---------- RDOResourceTypeTransact
// ----------------------------------------------------------------------------
RDOResourceTypeTransact::RDOResourceTypeTransact(rsint number, RDORuntimeParent* parent)
	: RDORuntimeObject  (parent)
	, RDOTraceableObject(false )
	, m_id              (number)
{}

LPRDOResource RDOResourceTypeTransact::createRes(PTR(RDORuntime) rt, ruint id, bool trace) const
{
	return new RDOPROCTransact(rt, id, this->getTraceID(), trace);
}

// ----------------------------------------------------------------------------
// ---------- RDOResourceTypeProccess
// ----------------------------------------------------------------------------
RDOResourceTypeProccess::RDOResourceTypeProccess(rsint number, RDORuntimeParent* parent)
	: RDORuntimeObject  (parent)
	, RDOTraceableObject(false )
	, m_id              (number)
{}

LPRDOResource RDOResourceTypeProccess::createRes(PTR(RDORuntime) rt, ruint id, bool trace) const
{
	return new RDOPROCResource(rt, id, this->getTraceID(), trace);
}

CLOSE_RDO_RUNTIME_NAMESPACE
