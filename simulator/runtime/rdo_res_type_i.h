/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_res_type_i.h
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \authors   Александровский Кирилл (k.alexandrovsky@gmail.com)
  \date      05.06.2011
  \brief     Интерфейс для типов ресурсов
  \details   Типы ресурсов - фабрики ресурсов. См. паттерн "Фабричный метод"
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RES_TYPE_I_H_
#define _LIB_RUNTIME_RES_TYPE_I_H_

// ----------------------------------------------------------------------- INCLUDES
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/interface_ptr/interface_ptr.h"
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime );
PREDECLARE_POINTER(RDOResource);

/*!
  \interface IResourceType
  \brief     Предоставляет фабричный метод createRes()
*/
PREDECLARE_OBJECT_INTERFACE(IResourceType)
struct IResourceType: public rdo::RefCounter<IResourceType>
{
DECLARE_FACTORY(IResourceType);
public:
	virtual rdo::runtime::LPRDOResource createRes(const LPRDORuntime& pRuntime, std::size_t resID, const std::vector<RDOValue>& paramsCalcs, bool traceFlag, bool permanentFlag) = 0;

	typedef  std::list<rdo::runtime::LPRDOResource>  ResList;
	typedef  ResList::const_iterator  ResCIterator;

	virtual ResCIterator res_begin() const = 0;
	virtual ResCIterator res_end() const = 0;

	virtual void eraseRes(const rdo::runtime::LPRDOResource& pResource) = 0;

	typedef  RDOResource  value_type;

protected:
	IResourceType()          {}
	virtual ~IResourceType() {}

};

#define DECLARE_IResourceType \
	rdo::runtime::LPRDOResource createRes(const LPRDORuntime& pRuntime, std::size_t resID, const std::vector<RDOValue>& paramsCalcs, bool traceFlag, bool permanentFlag);

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_RES_TYPE_I_H_
