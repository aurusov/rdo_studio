/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdoprocess_i.h
 * @authors   Урусов Андрей, Лущан Дмитрий
 * @date      31.07.2009
 * @brief     RDOProcess interface
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_PROCESS_I_
#define _LIB_RUNTIME_PROCESS_I_

// **************************************************************************** PCH
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/rdointerface.h"
#include "rdo_common/namespace.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE
PREDECLARE_POINTER(RDOPROCTransact);
PREDECLARE_POINTER(RDOResourceType);
CLOSE_RDO_RUNTIME_NAMESPACE

/******************************************************************************//**
 * @interface IPROCBlock
 * @brief     unknown
 *********************************************************************************/
class IPROCBlock
{
public:
	typedef  rdoRuntime::LPRDOPROCTransact  LPTransact;
	typedef  std::list<LPTransact>          TransactList;
	typedef  TransactList::iterator         TransactIt;

	virtual TransactIt     transactFind (CREF(LPTransact) pTransact) = 0;
	virtual TransactIt     transactEnd  ()                           = 0;
	virtual void           transactGoIn (CREF(LPTransact) pTransact) = 0;
	virtual void           transactGoOut(CREF(LPTransact) pTransact) = 0;
	virtual LPIPROCProcess getProcess   () const                     = 0;
};

/******************************************************************************//**
 * @def     DECLARE_IPROCBlock
 * @brief   unknown
 *********************************************************************************/
#define DECLARE_IPROCBlock \
	virtual TransactIt     transactFind (CREF(LPTransact) pTransact); \
	virtual TransactIt     transactEnd  ();                           \
	virtual void           transactGoIn (CREF(LPTransact) pTransact); \
	virtual void           transactGoOut(CREF(LPTransact) pTransact); \
	virtual LPIPROCProcess getProcess   () const;

/******************************************************************************//**
 * @interface IPROCProcess
 * @brief     unknown
 *********************************************************************************/
class IPROCProcess
{
public:
	virtual void  insertChild(LPIPROCProcess                      pProcess ) = 0;
	virtual void  setParent  (LPIPROCProcess                      pProcess ) = 0;
	virtual void  next       (CREF(rdoRuntime::LPRDOPROCTransact) pTransact) = 0;
	virtual CREF(rdoRuntime::LPRDOResourceType) getTranType() const          = 0;
};

/******************************************************************************//**
 * @def     DECLARE_IPROCProcess
 * @brief   unknown
 *********************************************************************************/
#define DECLARE_IPROCProcess                                                  \
	virtual void  insertChild(LPIPROCProcess                      pProcess ); \
	virtual void  setParent  (LPIPROCProcess                      pProcess ); \
	virtual void  next       (CREF(rdoRuntime::LPRDOPROCTransact) pTransact); \
	virtual CREF(rdoRuntime::LPRDOResourceType) getTranType() const;

#endif // _LIB_RUNTIME_PROCESS_I_
