/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_resource.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      16.04.2008
  \brief     Ресурсы в runtime
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RESOURCE_H_
#define _LIB_RUNTIME_RESOURCE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/operators.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDORuntime;
PREDECLARE_OBJECT_INTERFACE(IResourceType);

/*!
  \class   RDOResource
  \brief   "обычные" ресурсы моделей
  \details Ресурсы, которые могут быть релевантны активностям и
 * событиям, но не могут использоваться в процессах
*/
PREDECLARE_POINTER(RDOResource);
class RDOResource
	: public rdo::counter_reference
	, public RDORuntimeObject
	, public RDOTraceableObject
	, public boost::operators<RDOResource>
{
public:
	/*!
	  \enum  ConvertStatus
	  \brief статусы конверторов релевантных ресурсов
	*/
	enum ConvertStatus
	{
		CS_None = 0,
		CS_Keep,
		CS_Create,
		CS_Erase,
		CS_NonExist,
		CS_NoChange
	};

	typedef  std::vector<RDOValue>      ParamList;
	typedef  ParamList::const_iterator  ParamCIt;

	RDOResource(CREF(LPRDORuntime) pRuntime, CREF(ParamList) paramList, LPIResourceType pResType, ruint resID, ruint typeID, bool trace, bool temporary);
	RDOResource(CREF(LPRDORuntime) pRuntime, CREF(RDOResource) copy);
	virtual ~RDOResource();

	bool operator== (const RDOResource& other) const;

	ConvertStatus          getState    (                           ) const;
	CREF(RDOValue)         getParam    (ruint index                ) const;
	bool                   checkType   (ruint type                 ) const;
	bool                   canFree     (                           ) const;
	CREF(LPIResourceType)  getResType  (                           ) const;
	ruint                  getType     (                           ) const;
	virtual ruint          paramsCount (                           ) const;
	LPRDOResource          clone       (CREF(LPRDORuntime) pRuntime) const;
	CREF(ParamList)        getParamList(                           ) const;

	virtual void appendParams(CREF(ParamCIt) from_begin, CREF(ParamCIt) from_end);
	void setRuntime(CREF(LPRDORuntime) pRuntime);
	void makeTemporary(bool value);
	void setState(ConvertStatus value);
	std::string traceResourceState(char prefix, CREF(LPRDORuntime) pRuntime);
	REF(RDOValue) getParamRaw(ruint index);
	void setParam (ruint index, CREF(RDOValue) value);
	std::string getTypeId();
	std::string traceParametersValue();
	virtual std::string whoAreYou();
	void incRef();
	void decRef();

protected:
	ParamList m_paramList;
	bool m_temporary;
	ConvertStatus m_state;

private:
	ruint m_type;
	ruint m_referenceCount;
	LPIResourceType m_resType;
	std::string m_typeId;

	std::string traceTypeId();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_RESOURCE_H_
