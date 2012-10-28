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
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

template <class T>
inline RDOResourceTypeBase<T>::RDOResourceTypeBase(ruint number)
	: RDOType           (t_pointer                               )
	, RDORuntimeObject  (                                        )
	, RDOTraceableObject(false, number, rdo::toString(number + 1))
{}

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
	pRuntime->insertNewResource(pResource);

	return pResource;
}

CLOSE_RDO_RUNTIME_NAMESPACE
