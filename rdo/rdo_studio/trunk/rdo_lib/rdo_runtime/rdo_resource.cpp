/*!
  \copyright (c) RDO-Team, 2008
  \file      rdo_resource.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      16.04.2008
  \brief     RDOResource implementation
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_runtime/pch.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOResource
// --------------------------------------------------------------------------------
RDOResource::RDOResource(CREF(LPRDORuntime) pRuntime, CREF(std::vector<RDOValue>) paramsCalcs, LPIResourceType pResType, ruint resID, ruint typeID, rbool trace, rbool temporary)
	: RDORuntimeObject   (                                      )
	, RDOTraceableObject (trace, resID, rdo::toString(resID + 1))
	, m_state            (RDOResource::CS_None                  )
	, m_type             (typeID                                )
	, m_referenceCount   (0                                     )
	, m_resType          (pResType                              )
	, m_temporary        (temporary                             )
{
	appendParams(paramsCalcs.begin(), paramsCalcs.end());
	pRuntime->insertNewResource(this);
}

/// @todo копирующий конструктор не используется - нужен ли он?
RDOResource::RDOResource(CREF(LPRDORuntime) pRuntime, CREF(RDOResource) copy)
	: RDORuntimeObject   (                 )
	, RDOTraceableObject (copy.traceable(), copy.getTraceID(), copy.traceId())
	, m_type             (copy.m_type      )
	, m_state            (copy.m_state     )
	, m_typeId           (copy.m_typeId    )
	, m_params           (copy.m_params    )
	, m_referenceCount   (0                )
	, m_resType          (copy.m_resType   )
	, m_temporary        (copy.m_temporary )
{
	appendParams(copy.m_params.begin(), copy.m_params.end());
	pRuntime->insertNewResource(this);
/// @todo посмотреть history и принять решение и комментарии
//	getRuntime()->incrementResourceIdReference( getTraceID() );
}

RDOResource::~RDOResource()
{
	/// @todo Дима, поставь тут breakpoint и посмотри на this
	//getRuntime()->fireMessage(RDORuntime::RO_BEFOREDELETE, (void*)getTraceID());
	//getRuntime()->onResourceErase(this);
}

rbool RDOResource::operator!= (RDOResource &other)
{
	if (m_type != other.m_type) return true;
	if (m_params.size() != other.m_params.size()) return true;

	int size = m_params.size();
	for (int i = 0; i < size; ++i)
	{
		if (m_params.at(i) != other.m_params.at(i)) return true;
	}
	return false;
}

LPRDOResource RDOResource::clone(CREF(LPRDORuntime) pRuntime) const
{
	return rdo::Factory<RDOResource>::create(pRuntime, m_params, m_resType, getTraceID(), m_type, traceable(), m_temporary);
}

tstring RDOResource::getTypeId()
{
	std::ostringstream str;
	str << m_type;
	return str.str();
}

tstring RDOResource::traceParametersValue()
{
	std::ostringstream str;
	if(m_params.size() > 0)
	{
		std::vector<RDOValue>::iterator end = m_params.end();
		for(std::vector<RDOValue>::iterator it = m_params.begin();;)
		{
#ifdef RDOSIM_COMPATIBLE
			std::ostringstream _str;
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
	std::ostringstream res;
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

CLOSE_RDO_RUNTIME_NAMESPACE
