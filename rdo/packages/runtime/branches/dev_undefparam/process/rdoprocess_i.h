/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoprocess_i.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      31.07.2009
  \brief     RDOProcess interface
  \indent    4T
*/

#ifndef _LIB_RUNTIME_PROCESS_I_H_
#define _LIB_RUNTIME_PROCESS_I_H_

// ----------------------------------------------------------------------- INCLUDES
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
#include "simulator/runtime/rdo_res_type_i.h"
#include "simulator/runtime/rdo_runtime_interface_registrator.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
PREDECLARE_POINTER(RDOPROCTransact);
CLOSE_RDO_RUNTIME_NAMESPACE

/*!
  \interface IPROCBlock
  \brief     Интерфейс IPROCBlock
*/
class IPROCBlock
{
public:
	typedef  rdo::runtime::LPRDOPROCTransact  LPTransact;
	typedef  std::list<LPTransact>            TransactList;
	typedef  TransactList::iterator           TransactIt;

	virtual TransactIt     transactFind (CREF(LPTransact) pTransact) = 0;
	virtual TransactIt     transactEnd  ()                           = 0;
	virtual void           transactGoIn (CREF(LPTransact) pTransact) = 0;
	virtual void           transactGoOut(CREF(LPTransact) pTransact) = 0;
	virtual LPIPROCProcess getProcess   () const                     = 0;
};

#define DECLARE_IPROCBlock \
	virtual TransactIt     transactFind (CREF(LPTransact) pTransact); \
	virtual TransactIt     transactEnd  ();                           \
	virtual void           transactGoIn (CREF(LPTransact) pTransact); \
	virtual void           transactGoOut(CREF(LPTransact) pTransact); \
	virtual LPIPROCProcess getProcess   () const;

/*!
  \interface IPROCProcess
  \brief     Интерфейс IPROCProcess
*/
class IPROCProcess
{
public:
	virtual void  insertChild(LPIPROCProcess                        pProcess ) = 0;
	virtual void  setParent  (LPIPROCProcess                        pProcess ) = 0;
	virtual void  next       (CREF(rdo::runtime::LPRDOPROCTransact) pTransact) = 0;
	virtual rdo::runtime::LPIResourceType getTranType() const                  = 0;
};

#define DECLARE_IPROCProcess                                                    \
	virtual void  insertChild(LPIPROCProcess                        pProcess ); \
	virtual void  setParent  (LPIPROCProcess                        pProcess ); \
	virtual void  next       (CREF(rdo::runtime::LPRDOPROCTransact) pTransact); \
	virtual rdo::runtime::LPIResourceType getTranType() const;

#endif // _LIB_RUNTIME_PROCESS_I_H_
