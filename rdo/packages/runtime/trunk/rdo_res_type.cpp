/*!
  \copyright (c) RDO-Team, 2013
  \file      rdo_res_type.cpp
  \author    Александровский Кирилл (k.alexandrovsky@gmail.com)
  \date      13.05.2013
  \brief     Типы ресурсов в rdo::runtime
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

RDOResourceTypeList::RDOResourceTypeList(ruint number, CREF(rdo::runtime::LPRDORuntime) pRuntime)
: RDOType           (t_pointer)
, RDOTraceableObject(false, number, rdo::toString(number + 1))
{
	rdo::intrusive_ptr<RDOResourceTypeList> pThis(this);
	ASSERT(pThis);
	pRuntime->addResType(pThis);
}

RDOResourceTypeList::~RDOResourceTypeList()
{}

void RDOResourceTypeList::eraseRes(CREF(rdo::runtime::LPRDOResource) pResource)
{
	m_resourceList.remove(pResource);
}

IResourceType::ResCIterator RDOResourceTypeList::res_begin() const
{
	return m_resourceList.begin();
}

IResourceType::ResCIterator RDOResourceTypeList::res_end() const
{
	return m_resourceList.end();
}

CLOSE_RDO_RUNTIME_NAMESPACE
