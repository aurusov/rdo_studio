/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoprocess_interface.h
 * author   : Урусов Андрей
 * date     : 31.07.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPROCESS_INTERFACE_H_
#define _RDOPROCESS_INTERFACE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <rdointerface.h>
#include <namespace.h>
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE
class RDOPROCTransact;
CLOSE_RDO_RUNTIME_NAMESPACE

class IPROCBlock
{
public:
	virtual void TransactGoIn (PTR(rdoRuntime::RDOPROCTransact) transact) = 0;
	virtual void TransactGoOut(PTR(rdoRuntime::RDOPROCTransact) transact) = 0;
};
#define DECLARE_IPROCBlock \
	virtual void TransactGoIn (PTR(rdoRuntime::RDOPROCTransact) transact); \
	virtual void TransactGoOut(PTR(rdoRuntime::RDOPROCTransact) transact);

class IPROCProcess
{
public:
	virtual void insertChild(LPIPROCProcess                   process ) = 0;
	virtual void setParent  (LPIPROCProcess                   process ) = 0;
	virtual void next       (PTR(rdoRuntime::RDOPROCTransact) transact) = 0;
};
#define DECLARE_IPROCProcess \
	virtual void insertChild(LPIPROCProcess                   process ); \
	virtual void setParent  (LPIPROCProcess                   process ); \
	virtual void next       (PTR(rdoRuntime::RDOPROCTransact) transact);

#endif //! _RDOPROCESS_INTERFACE_H_
