#include "rdo_lib/rdo_runtime/pch.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdoprocess.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOResourceType
// ----------------------------------------------------------------------------
RDOResourceType::RDOResourceType(ruint number)
	: RDORuntimeObject  (                                        )
	, RDOTraceableObject(false, number, rdo::toString(number + 1))
{}

RDOResourceType::~RDOResourceType()
{}

LPRDOResource RDOResourceType::createRes(CREF(LPRDORuntime) pRuntime, CREF(std::vector<RDOValue>) paramsCalcs, rbool traceFlag, rbool permanentFlag)
{
	LPRDOResourceType pResType(this);
	ASSERT(pResType);
	LPIResourceType pIResType = pResType.interface_cast<IResourceType>();
	ASSERT(pIResType);
	return rdo::Factory<RDOResource>::create(pRuntime, paramsCalcs, pIResType, pRuntime->getResourceId(), this->getTraceID(), traceFlag, permanentFlag);
}

// ----------------------------------------------------------------------------
// ---------- RDOResourceTypeTransact
// ----------------------------------------------------------------------------
RDOResourceTypeTransact::RDOResourceTypeTransact(ruint number)
	: RDORuntimeObject  (                                        )
	, RDOTraceableObject(false, number, rdo::toString(number + 1))
{}

RDOResourceTypeTransact::~RDOResourceTypeTransact()
{}

LPRDOResource RDOResourceTypeTransact::createRes(CREF(LPRDORuntime) pRuntime, CREF(std::vector<RDOValue>) paramsCalcs, rbool traceFlag, rbool permanentFlag)
{
	LPRDOResourceTypeTransact pResType(this);
	ASSERT(pResType);
	LPIResourceType pIResType = pResType.interface_cast<IResourceType>();
	ASSERT(pIResType);
	return rdo::Factory<RDOPROCTransact>::create(pRuntime, paramsCalcs, pIResType, pRuntime->getResourceId(), this->getTraceID(), traceFlag, permanentFlag);
}

// ----------------------------------------------------------------------------
// ---------- RDOResourceTypeProccess
// ----------------------------------------------------------------------------
RDOResourceTypeProccess::RDOResourceTypeProccess(ruint number)
	: RDORuntimeObject  (                                        )
	, RDOTraceableObject(false, number, rdo::toString(number + 1))
{}

RDOResourceTypeProccess::~RDOResourceTypeProccess()
{}

LPRDOResource RDOResourceTypeProccess::createRes(CREF(LPRDORuntime) pRuntime, CREF(std::vector<RDOValue>) paramsCalcs, rbool traceFlag, rbool permanentFlag)
{
	LPRDOResourceTypeProccess pResType(this);
	ASSERT(pResType);
	LPIResourceType pIResType = pResType.interface_cast<IResourceType>();
	ASSERT(pIResType);
	return rdo::Factory<RDOPROCResource>::create(pRuntime, paramsCalcs, pIResType, pRuntime->getResourceId(), this->getTraceID(), traceFlag, permanentFlag);
}

CLOSE_RDO_RUNTIME_NAMESPACE
