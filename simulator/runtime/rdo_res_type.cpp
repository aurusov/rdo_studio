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

RDOResourceTypeList::RDOResourceTypeList(std::size_t number, const LPRDORuntime& pRuntime)
	: RDOType           (t_pointer)
	, RDOTraceableObject(false, number, rdo::toString(number + 1))
{
	rdo::intrusive_ptr<RDOResourceTypeList> pThis(this);
	ASSERT(pThis);
	pRuntime->addResType(pThis);
}

RDOResourceTypeList::~RDOResourceTypeList()
{}

void RDOResourceTypeList::setFactoryMethod(const Create& create)
{
	ASSERT(create);
	ASSERT(!m_create);
	m_create = create;
}

LPRDOResource RDOResourceTypeList::createRes(const LPRDORuntime& pRuntime, std::size_t resID, const std::vector<RDOValue>& paramsCalcs, bool traceFlag, bool permanentFlag, bool isNested)
{
	ASSERT(m_create);

	rdo::intrusive_ptr<RDOResourceTypeList> pThis(this);
	ASSERT(pThis);
	LPIResourceType pIResType = pThis.interface_cast<IResourceType>();
	ASSERT(pIResType);

	LPRDOResource resource = m_create(pRuntime, paramsCalcs, pIResType, resID, getTraceID(), traceFlag, permanentFlag, isNested);
	insertNewResource(pRuntime, resource);

	return resource;
}

void RDOResourceTypeList::insertNewResource(const LPRDORuntime& pRuntime, const LPRDOResource& pResource)
{
	ASSERT(pRuntime);
	ASSERT(pResource);

#ifdef _DEBUG
	for (const LPRDOResource& resource: m_resourceList)
	{
		ASSERT(resource->getTraceID() != pResource->getTraceID());
	}
#endif // _DEBUG

	m_resourceList.push_back(pResource);
	pRuntime->insertNewResource(pResource);
}

void RDOResourceTypeList::eraseRes(const LPRDOResource& pResource)
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

LPRDOResourceTypeList RDOResourceTypeList::clone(const LPRDORuntime& pRuntime) const
{
	LPRDOResourceTypeList type = rdo::Factory<RDOResourceTypeList>::create(getTraceID(), pRuntime);
	ASSERT(type);
	type->setFactoryMethod(m_create);
	for (const LPRDOResource& resource: m_resourceList)
	{
		type->insertNewResource(pRuntime, resource->clone(pRuntime));
	}
	return type;
}

CLOSE_RDO_RUNTIME_NAMESPACE
