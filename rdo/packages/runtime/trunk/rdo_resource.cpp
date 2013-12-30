/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_resource.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      16.04.2008
  \brief     RDOResource implementation
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOResource
// --------------------------------------------------------------------------------
RDOResource::RDOResource(CREF(LPRDORuntime) pRuntime, CREF(ParamList) paramList, LPIResourceType pResType, ruint resID, ruint typeID, rbool trace, rbool temporary)
	: RDORuntimeObject   (                                      )
	, RDOTraceableObject (trace, resID, rdo::toString(resID + 1))
	, m_temporary        (temporary                             )
	, m_state            (RDOResource::CS_None                  )
	, m_type             (typeID                                )
	, m_referenceCount   (0                                     )
	, m_resType          (pResType                              )
{
	UNUSED(pRuntime);
	appendParams(paramList.begin(), paramList.end());
}

/// @todo копирующий конструктор не используется - нужен ли он?
RDOResource::RDOResource(CREF(LPRDORuntime) pRuntime, CREF(RDOResource) copy)
	: RDORuntimeObject   (                 )
	, RDOTraceableObject (copy.traceable(), copy.getTraceID(), copy.traceId())
	, m_paramList        (copy.m_paramList )
	, m_temporary        (copy.m_temporary )
	, m_state            (copy.m_state     )
	, m_type             (copy.m_type      )
	, m_referenceCount   (0                )
	, m_resType          (copy.m_resType   )
	, m_typeId           (copy.m_typeId    )
{
	UNUSED(pRuntime);
	appendParams(copy.m_paramList.begin(), copy.m_paramList.end());
/// @todo посмотреть history и принять решение о комментарии
//	getRuntime()->incrementResourceIdReference( getTraceID() );
}

RDOResource::~RDOResource()
{}

bool RDOResource::operator== (const RDOResource& other) const
{
	if (m_type != other.m_type)
		return false;

	if (m_paramList.size() != other.m_paramList.size())
		return false;

	int size = m_paramList.size();
	for (int i = 0; i < size; ++i)
	{
		if (m_paramList[i] != other.m_paramList[i])
			return false;
	}
	return true;
}

LPRDOResource RDOResource::clone(CREF(LPRDORuntime) pRuntime) const
{
	const LPRDOResourceTypeList& resourceType = pRuntime->getResType(m_type);
	ASSERT(resourceType);
	LPRDOResource pResource = rdo::Factory<RDOResource>::create(pRuntime, m_paramList, resourceType.interface_cast<IResourceType>(), getTraceID(), m_type, traceable(), m_temporary);
	ASSERT(pResource);
	return pResource;
}

tstring RDOResource::getTypeId()
{
	rdo::ostringstream str;
	str << m_type;
	return str.str();
}

tstring RDOResource::traceParametersValue()
{
	rdo::ostringstream str;
	if(m_paramList.size() > 0)
	{
		ParamList::iterator end = m_paramList.end();
		for (ParamList::iterator it = m_paramList.begin();;)
		{
#ifdef RDOSIM_COMPATIBLE
			rdo::ostringstream _str;
			_str << *it;
			tstring::size_type pos = _str.str().find("e");
			if (pos != tstring::npos)
			{
				tstring __str = _str.str();
				__str.erase(pos + 2, 1);
				str << __str.c_str();
			}
			else
			{
				str << _str.str().c_str();
			}
#else
			str << *it;
#endif
			if (++it == end)
				break;
			str << " ";
		}
	}
	return str.str();
}

tstring RDOResource::traceResourceState(char prefix, CREF(LPRDORuntime) pRuntime)
{
	rdo::ostringstream res;
	if (traceable() || (prefix != '\0'))
	{
		if (m_state == RDOResource::CS_NoChange || m_state == RDOResource::CS_NonExist)
			return "";

		if (prefix != '\0')
			res << prefix;

		switch (m_state)
		{
		case RDOResource::CS_Create:
			res << "RC ";
			break;
		case RDOResource::CS_Erase:
			res << "RE "
#ifdef RDOSIM_COMPATIBLE
				<< pRuntime->getCurrentTime() << " "
				<< traceTypeId()              << " "
				<< traceId()                  << std::endl;
			return res.str();
#else
				;
#endif
			break;
		default:
			res << "RK ";
			break;
		}

		res << pRuntime->getCurrentTime() << " "
			<< traceTypeId()              << " "
			<< traceId()                  << " "
			<< traceParametersValue()     << std::endl;
	}
	return res.str();
}

void RDOResource::setRuntime(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);

	/// @todo походу надо удалить метод
	NEVER_REACH_HERE;
}

tstring RDOResource::whoAreYou()
{
	return "rdoRes";
}

void RDOResource::makeTemporary(rbool value)
{
	m_temporary = value;
}

RDOResource::ConvertStatus RDOResource::getState() const
{
	return m_state;
}

void RDOResource::setState(RDOResource::ConvertStatus value)
{
	m_state = value;
}

rbool RDOResource::checkType(ruint type) const
{
	return m_type == type;
}

CREF(LPIResourceType) RDOResource::getResType() const
{
	return m_resType;
}

ruint RDOResource::getType() const
{
	return m_type;
}

CREF(RDOResource::ParamList) RDOResource::getParamList() const
{
	return m_paramList;
}

CREF(RDOValue) RDOResource::getParam(ruint index) const
{
	ASSERT(index < m_paramList.size());
	return m_paramList[index];
}

REF(RDOValue) RDOResource::getParamRaw(ruint index)
{
	ASSERT(index < m_paramList.size());
	setState(CS_Keep);
	return m_paramList[index];
}

void RDOResource::setParam(ruint index, CREF(RDOValue) value)
{
	ASSERT(index < m_paramList.size());
	setState(CS_Keep);
	m_paramList[index] = value;
}

ruint RDOResource::paramsCount() const
{
	return m_paramList.size();
}

void RDOResource::appendParams(CREF(ParamCIt) from_begin, CREF(ParamCIt) from_end)
{
	m_paramList.insert(m_paramList.end(), from_begin, from_end);
}

rbool RDOResource::canFree() const
{
	return m_referenceCount == 0;
}

void RDOResource::incRef()
{
	++m_referenceCount;
}

void RDOResource::decRef()
{
	--m_referenceCount;
}

tstring RDOResource::traceTypeId()
{
	return m_typeId.empty() ? (m_typeId = getTypeId()) : m_typeId;
}

CLOSE_RDO_RUNTIME_NAMESPACE
