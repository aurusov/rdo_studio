/**
 @file      rdoprocess_i.h
 @authors   Урусов Андрей, Лущан Дмитрий
 @date      31.07.2009
 @brief     RDOProcess interface
 @indent    4T
 @copyright (c) RDO-Team, 2009
 */

#ifndef _RDOPROCESS_I_H_
#define _RDOPROCESS_I_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdointerface.h"
#include "rdo_common/namespace.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE
PREDECLARE_POINTER(RDOPROCTransact);
PREDECLARE_POINTER(RDOResourceType);
CLOSE_RDO_RUNTIME_NAMESPACE

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
#define DECLARE_IPROCBlock \
	virtual TransactIt     transactFind (CREF(LPTransact) pTransact); \
	virtual TransactIt     transactEnd  ();                           \
	virtual void           transactGoIn (CREF(LPTransact) pTransact); \
	virtual void           transactGoOut(CREF(LPTransact) pTransact); \
	virtual LPIPROCProcess getProcess   () const;

class IPROCProcess
{
public:
	virtual void  insertChild(LPIPROCProcess                      pProcess ) = 0;
	virtual void  setParent  (LPIPROCProcess                      pProcess ) = 0;
	virtual void  next       (CREF(rdoRuntime::LPRDOPROCTransact) pTransact) = 0;
	virtual rdoRuntime::LPRDOResourceType getTranType() const                = 0;
};
#define DECLARE_IPROCProcess                                                  \
	virtual void  insertChild(LPIPROCProcess                      pProcess ); \
	virtual void  setParent  (LPIPROCProcess                      pProcess ); \
	virtual void  next       (CREF(rdoRuntime::LPRDOPROCTransact) pTransact); \
	virtual rdoRuntime::LPRDOResourceType getTranType() const;

#endif //! _RDOPROCESS_I_H_
