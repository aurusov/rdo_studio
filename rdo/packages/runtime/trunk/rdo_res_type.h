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
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDORuntime;
class RDOPROCTransact;

//! @todo сделать классы \ref RDOResourceType, \ref RDOResourceTypeTransact и \ref RDOResourceTypeProccess шаблонными

/*!
  \class   RDOResourceType
  \brief   Тип ресурсов для создания обычных ресурсов РДО
  \details Создает ресурсы, которые могут быть релевантны активностям и
           событиям, но не могут использоваться в процессах
*/
OBJECT(RDOResourceType)
	IS  IMPLEMENTATION_OF(IResourceType     )
	AND INSTANCE_OF      (RDORuntimeObject  )
	AND INSTANCE_OF      (RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceType);
friend class RDOCalcCreateResource;
friend class RDOPROCGenerate;
private:
	/*!
	  \brief Конструктор
	  \param number - Целочисленный идентификатор
	*/
	RDOResourceType(ruint number);
	virtual ~RDOResourceType();

	DECLARE_IResourceType;
};

/*!
  \class   RDOResourceTypeTransact
  \brief   Тип ресурсов для создания ресуров-транзактов
  \details Создает ресурсы, которые могут быть релевантны активностям и 
           событиям, а также обслуживать транзакты в процессах
*/
OBJECT(RDOResourceTypeTransact)
	IS  IMPLEMENTATION_OF(IResourceType     )
	AND INSTANCE_OF      (RDORuntimeObject  )
	AND INSTANCE_OF      (RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceTypeTransact);
private:
	/*!
	  \brief Конструктор
	  \param number - Целочисленный идентификатор
	*/
	RDOResourceTypeTransact(ruint number);
	virtual ~RDOResourceTypeTransact();

	DECLARE_IResourceType;
};

/*!
  \class   RDOResourceTypeProccess
  \brief   Тип ресурсов для создания ресурсов-ресурсов (который свободен/занят с точки зрения процесснорго подхода)
  \details Создает ресурсы, которые могут быть релевантны активностям и 
           событиям, а также становиться ресурсам (свободен/занят) в процессах
*/
OBJECT(RDOResourceTypeProccess)
	IS  IMPLEMENTATION_OF(IResourceType     )
	AND INSTANCE_OF      (RDORuntimeObject  )
	AND INSTANCE_OF      (RDOTraceableObject)
{
DECLARE_FACTORY(RDOResourceTypeProccess);
private:
	/*!
	  \brief Конструктор
	  \param number - Целочисленный идентификатор
	*/
	RDOResourceTypeProccess(ruint number);
	virtual ~RDOResourceTypeProccess();

	DECLARE_IResourceType;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_RES_TYPE_H_
