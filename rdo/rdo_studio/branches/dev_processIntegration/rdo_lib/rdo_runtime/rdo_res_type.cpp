#include "rdo_lib/rdo_runtime/pch.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdoprocess.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOResourceType
// ----------------------------------------------------------------------------
RDOResourceType::RDOResourceType(rsint number, RDORuntimeParent* parent)
	: RDORuntimeObject  (parent)
	, RDOTraceableObject(false )
	, m_id              (number)
{}

RDOResourceType::~RDOResourceType()
{}

LPRDOResource RDOResourceType::createRes(PTR(RDORuntime) runtime, bool trace)
{
	LPRDOResourceType pResType(this);
	ASSERT(pResType);
	LPIResourceType pIResType = pResType.interface_cast<IResourceType>();
	ASSERT(pIResType);
	return rdo::Factory<RDOResource>::create(runtime, pIResType, runtime->getResourceId(), this->getTraceID(), trace);
}

// ----------------------------------------------------------------------------
// ---------- RDOResourceTypeTransact
// ----------------------------------------------------------------------------
RDOResourceTypeTransact::RDOResourceTypeTransact(rsint number, RDORuntimeParent* parent)
	: RDORuntimeObject  (parent)
	, RDOTraceableObject(false )
	, m_id              (number)
{}

RDOResourceTypeTransact::~RDOResourceTypeTransact()
{}

LPRDOResource RDOResourceTypeTransact::createRes(PTR(RDORuntime) runtime, bool trace)
{
	LPRDOResourceTypeTransact pResType(this);
	ASSERT(pResType);
	LPIResourceType pIResType = pResType.interface_cast<IResourceType>();
	ASSERT(pIResType);
	return rdo::Factory<RDOPROCTransact>::create(runtime, pIResType, runtime->getResourceId(), this->getTraceID(), trace);
}

// ----------------------------------------------------------------------------
// ---------- RDOResourceTypeProccess
// ----------------------------------------------------------------------------
RDOResourceTypeProccess::RDOResourceTypeProccess(rsint number, RDORuntimeParent* parent)
	: RDORuntimeObject  (parent)
	, RDOTraceableObject(false )
	, m_id              (number)
{}

RDOResourceTypeProccess::~RDOResourceTypeProccess()
{}

LPRDOResource RDOResourceTypeProccess::createRes(PTR(RDORuntime) runtime, bool trace)
{
	LPRDOResourceTypeProccess pResType(this);
	ASSERT(pResType);
	LPIResourceType pIResType = pResType.interface_cast<IResourceType>();
	ASSERT(pIResType);
	return rdo::Factory<RDOPROCResource>::create(runtime, pIResType, runtime->getResourceId(), this->getTraceID(), trace);
}

CLOSE_RDO_RUNTIME_NAMESPACE
