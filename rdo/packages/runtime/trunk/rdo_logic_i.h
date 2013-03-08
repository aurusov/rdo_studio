/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_logic_i.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      30.07.2009
  \brief     Интерфейс ILogic
  \indent    4T
*/

#ifndef _LIB_RUNTIME_LOGIC_I_H_
#define _LIB_RUNTIME_LOGIC_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
#include "utils/smart_ptr/intrusive_ptr.h"
#include "simulator/runtime/rdo_runtime_interface_registrator.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
PREDECLARE_POINTER(RDORuntime);
PREDECLARE_POINTER(RDOCalc);
CLOSE_RDO_RUNTIME_NAMESPACE

/*!
  \interface ILogic
  \brief     Логика в РДО - правила логического вывода для контейнеров IBaseOperationContainer
*/
class ILogic
{
public:
	virtual void init             (CREF(rdo::runtime::LPRDORuntime) pRuntime      ) = 0;
	virtual void setCondition     (CREF(rdo::runtime::LPRDOCalc)    pCondition    ) = 0;
	virtual void setMultithreading(rbool                            multithreading) = 0;
};

#define DECLARE_ILogic                                                              \
	virtual void init             (CREF(rdo::runtime::LPRDORuntime) pRuntime      ); \
	virtual void setCondition     (CREF(rdo::runtime::LPRDOCalc)    pCondition    ); \
	virtual void setMultithreading(rbool                            multithreading);

/*!
  \interface IBaseOperationContainer
  \brief     Иерархические контейнеры активностей. См. паттерн "Компоновщик"
*/
class IBaseOperationContainer
{
public:
	typedef  LPIBaseOperation      Item;
	typedef  std::vector<Item>     List;
	typedef  List::iterator        Iterator;
	typedef  List::const_iterator  CIterator;

	virtual Iterator              begin ()                = 0;
	virtual Iterator              end   ()                = 0;
	virtual CIterator             begin () const          = 0;
	virtual CIterator             end   () const          = 0;
	virtual void                  append(CREF(Item) item) = 0;
	virtual rbool                 empty () const          = 0;
	virtual REF(LPIBaseOperation) back  ()                = 0;
	virtual void                  clear ()                = 0;
};

#define DECLARE_IBaseOperationContainer                    \
	virtual Iterator              begin ();                \
	virtual Iterator              end   ();                \
	virtual CIterator             begin () const;          \
	virtual CIterator             end   () const;          \
	virtual void                  append(CREF(Item) item); \
	virtual rbool                 empty () const;          \
	virtual REF(LPIBaseOperation) back  ();                \
	virtual void                  clear ();

#endif // _LIB_RUNTIME_LOGIC_I_H_
