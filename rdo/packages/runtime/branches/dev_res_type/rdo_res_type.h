/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_res_type.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      07.06.2011
  \brief     Типы ресурсов в rdo::runtime
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RES_TYPE_H_
#define _LIB_RUNTIME_RES_TYPE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/rdo_res_type_i.h"
#include "simulator/runtime/rdo_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Описывает РДО-тип ресурса (RTP), который суть фабрика для РДО-ресурсов
//! tparam T - ресурс, который будет создаваться данной фабрикой
template <class T>
class RDOResourceTypeBase: public RDOResourceListObject
{
DECLARE_FACTORY(RDOResourceTypeBase<T>);
public:
	typedef  T  value_type;

private:
	//! Конструктор
	//! \param number - Целочисленный идентификатор
	RDOResourceTypeBase(ruint number, rdo::runtime::LPRDORuntime pRuntime);
	virtual ~RDOResourceTypeBase();

	DECLARE_IResourceType;
};

class RDOResourceListObject
	: public RDOType
	, public IResourceType
	, public RDORuntimeObject
	, public RDOTraceableObject
{
friend class rdo::Factory<RDOResourceListObject>;

public:
	ResCIterator res_begin() const;
	ResCIterator res_end  () const;

	void eraseRes(CREF(rdo::runtime::LPRDOResource) pResource);

protected:
	RDOResourceListObject(ruint number, rdo::runtime::LPRDORuntime pRuntime);
	virtual ~RDOResourceListObject();

	typedef  std::list<rdo::runtime::LPRDOResource> ResourceList;
	ResourceList m_resourceList;
};

typedef  rdo::intrusive_ptr<RDOResourceListObject>  LPRDOResourceListObject;

//! Тип ресурсов для создания обычных ресурсов РДО
//! \details Создает ресурсы, которые могут быть релевантны активностям и
//!          событиям, но не могут использоваться в процессах
typedef  RDOResourceTypeBase<RDOResource>     RDOResourceType;
typedef  rdo::intrusive_ptr<RDOResourceType>  LPRDOResourceType;

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_res_type.inl"

#endif // _LIB_RUNTIME_RES_TYPE_H_
