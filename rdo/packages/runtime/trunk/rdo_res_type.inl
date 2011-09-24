/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_res_type.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      24.09.2011
  \brief     Типы ресурсов в rdoRuntime
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/factory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

template <class T>
inline RDOResourceTypeBase<T>::RDOResourceTypeBase(ruint number)
	: RDORuntimeObject  (                                        )
	, RDOTraceableObject(false, number, rdo::toString(number + 1))
{}

template <class T>
inline RDOResourceTypeBase<T>::~RDOResourceTypeBase()
{}

template <class T>
inline LPRDOResource RDOResourceTypeBase<T>::createRes(CREF(LPRDORuntime) pRuntime, CREF(std::vector<RDOValue>) paramsCalcs, rbool traceFlag, rbool permanentFlag)
{
	LPRDOResourceType pResType(this);
	ASSERT(pResType);
	LPIResourceType pIResType = pResType.interface_cast<IResourceType>();
	ASSERT(pIResType);
	return rdo::Factory<T>::create(pRuntime, paramsCalcs, pIResType, pRuntime->getResourceId(), this->getTraceID(), traceFlag, permanentFlag);
}

CLOSE_RDO_RUNTIME_NAMESPACE
