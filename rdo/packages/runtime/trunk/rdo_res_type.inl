/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_res_type.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      24.09.2011
  \brief     Типы ресурсов в rdo::runtime
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

template <class T>
inline RDOResourceTypeListT<T>::RDOResourceTypeListT(ruint number, rdo::runtime::LPRDORuntime pRuntime)
	: RDOResourceTypeList(number, pRuntime)
{}

template <class T>
inline RDOResourceTypeListT<T>::~RDOResourceTypeListT()
{}

template <class T>
inline LPRDOResource RDOResourceTypeListT<T>::createRes(CREF(LPRDORuntime) pRuntime, ruint resID, CREF(std::vector<RDOValue>) paramsCalcs, rbool traceFlag, rbool permanentFlag)
{
	rdo::intrusive_ptr<RDOResourceTypeListT<T> > pThis(this);
	ASSERT(pThis);
	LPIResourceType pIResType = pThis.template interface_cast<IResourceType>();
	ASSERT(pIResType);

	rdo::intrusive_ptr<T> pResource = rdo::Factory<T>::create(pRuntime, paramsCalcs, pIResType, resID, this->getTraceID(), traceFlag, permanentFlag);
	ASSERT(pResource);
	insertNewResource(pRuntime, pResource);

	return pResource;
}

template <class T>
inline LPRDOResourceTypeList RDOResourceTypeListT<T>::cloneTypeOnly(CREF(LPRDORuntime) pRuntime) const
{
	LPRDOResourceTypeList type = rdo::Factory<RDOResourceTypeListT<T> >::create(getTraceID(), pRuntime);
	ASSERT(type);
	return type;
}

CLOSE_RDO_RUNTIME_NAMESPACE
