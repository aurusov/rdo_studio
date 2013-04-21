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
#include "utils/smart_ptr/intrusive_ptr.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

template <class T>
inline RDOResourceTypeBase<T>::RDOResourceTypeBase(ruint number, rdo::runtime::LPRDORuntime pRuntime)
	: RDOType              (t_pointer       )
	, RDOResourceListObject(number, pRuntime)
{
	rdo::intrusive_ptr<RDOResourceTypeBase<T> > pThis(this);
	ASSERT(pThis);
	LPIResourceType pIResType = pThis.template interface_cast<IResourceType>();
	ASSERT(pIResType);

	pRuntime->addResType(number, pIResType);
}

template <class T>
inline RDOResourceTypeBase<T>::~RDOResourceTypeBase()
{}

template <class T>
inline LPRDOResource RDOResourceTypeBase<T>::createRes(CREF(LPRDORuntime) pRuntime, ruint resID, CREF(std::vector<RDOValue>) paramsCalcs, rbool traceFlag, rbool permanentFlag)
{
	rdo::intrusive_ptr<RDOResourceTypeBase<T> > pThis(this);
	ASSERT(pThis);
	LPIResourceType pIResType = pThis.template interface_cast<IResourceType>();
	ASSERT(pIResType);

	rdo::intrusive_ptr<T> pResource = rdo::Factory<T>::create(pRuntime, paramsCalcs, pIResType, resID, this->getTraceID(), traceFlag, permanentFlag);
	ASSERT(pResource);
	m_resourceList.push_back(pResource);
	pRuntime->insertNewResource(pResource);

	return pResource;
}

inline RDOResourceListObject::RDOResourceListObject(ruint number, rdo::runtime::LPRDORuntime pRuntime)
	: RDORuntimeObject  (                                        )
	, RDOTraceableObject(false, number, rdo::toString(number + 1))
{}

inline RDOResourceListObject::~RDOResourceListObject()
{}

inline void RDOResourceListObject::eraseRes(CREF(rdo::runtime::LPRDOResource) pResource)
{
	m_resourceList.remove(pResource);
}

inline IResourceType::ResCIterator RDOResourceListObject::res_begin() const
{
	return m_resourceList.begin();
}

inline IResourceType::ResCIterator RDOResourceListObject::res_end() const
{
	return m_resourceList.end();
}

CLOSE_RDO_RUNTIME_NAMESPACE
