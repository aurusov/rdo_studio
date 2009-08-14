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
	typedef  rdoRuntime::RDOPROCTransact  Transact;
	typedef  std::list<PTR(Transact)>     TransactList;
	typedef  TransactList::iterator       TransactIt;

	virtual TransactIt     transactFind (PTR(Transact) transact) = 0;
	virtual TransactIt     transactEnd  ()                       = 0;
	virtual void           transactGoIn (PTR(Transact) transact) = 0;
	virtual void           transactGoOut(PTR(Transact) transact) = 0;
	virtual LPIPROCProcess getProcess   () const                 = 0;
};
#define DECLARE_IPROCBlock \
	virtual TransactIt     transactFind (PTR(Transact) transact); \
	virtual TransactIt     transactEnd  ();                       \
	virtual void           transactGoIn (PTR(Transact) transact); \
	virtual void           transactGoOut(PTR(Transact) transact); \
	virtual LPIPROCProcess getProcess   () const;

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
