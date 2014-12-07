#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_res_type_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
PREDECLARE_POINTER(RDOPROCTransact);
CLOSE_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(IPROCProcess)
class IPROCBlock: public virtual rdo::counter_reference
{
public:
	typedef  rdo::runtime::LPRDOPROCTransact  LPTransact;
	typedef  std::list<LPTransact>            TransactList;
	typedef  TransactList::iterator           TransactIt;

	virtual TransactIt     transactFind (const LPTransact& pTransact) = 0;
	virtual TransactIt     transactEnd  ()                            = 0;
	virtual void           transactGoIn (const LPTransact& pTransact) = 0;
	virtual void           transactGoOut(const LPTransact& pTransact) = 0;
	virtual LPIPROCProcess getProcess   () const                      = 0;
};
DECLARE_POINTER(IPROCBlock)

#define DECLARE_IPROCBlock \
	virtual TransactIt     transactFind (const LPTransact& pTransact); \
	virtual TransactIt     transactEnd  ();                            \
	virtual void           transactGoIn (const LPTransact& pTransact); \
	virtual void           transactGoOut(const LPTransact& pTransact); \
	virtual LPIPROCProcess getProcess   () const;

class IPROCProcess: public virtual rdo::counter_reference
{
public:
	virtual void  insertChild(LPIPROCProcess                         pProcess ) = 0;
	virtual void  setParent  (LPIPROCProcess                         pProcess ) = 0;
	virtual void  next       (const rdo::runtime::LPRDOPROCTransact& pTransact) = 0;
	virtual rdo::runtime::LPIResourceType getTranType() const                   = 0;
};

#define DECLARE_IPROCProcess                                                     \
	virtual void  insertChild(LPIPROCProcess                         pProcess ); \
	virtual void  setParent  (LPIPROCProcess                         pProcess ); \
	virtual void  next       (const rdo::runtime::LPRDOPROCTransact& pTransact); \
	virtual rdo::runtime::LPIResourceType getTranType() const;
