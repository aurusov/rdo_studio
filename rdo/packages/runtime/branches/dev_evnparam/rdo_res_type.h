/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_res_type.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      07.06.2011
  \brief     Типы ресурсов в rdoRuntime
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
CLASS_PARENT_OF(RDOResourceTypeBase, RDOType)
	IS  IMPLEMENTATION_OF(IResourceType     )
	AND INSTANCE_OF      (RDORuntimeObject  )
	AND INSTANCE_OF      (RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceTypeBase<T>);
public:
	typedef  T  value_type;

private:
	//! Конструктор
	//! \param number - Целочисленный идентификатор
	RDOResourceTypeBase(ruint number);
	virtual ~RDOResourceTypeBase();

	DECLARE_IResourceType;
};

//! Тип ресурсов для создания обычных ресурсов РДО
//! \details Создает ресурсы, которые могут быть релевантны активностям и
//!          событиям, но не могут использоваться в процессах
typedef  RDOResourceTypeBase<RDOResource>     RDOResourceType;
typedef  rdo::intrusive_ptr<RDOResourceType>  LPRDOResourceType;

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_res_type.inl"

#endif // _LIB_RUNTIME_RES_TYPE_H_
