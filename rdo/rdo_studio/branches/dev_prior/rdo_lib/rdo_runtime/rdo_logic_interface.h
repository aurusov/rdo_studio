/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic_interface.h
 * author   : Урусов Андрей
 * date     : 30.07.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_LOGIC_INTERFACE_H_
#define _RDO_LOGIC_INTERFACE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <namespace.h>
#include <rdointerface.h>
#include "rdo_runtime_interface_registrator.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE
class RDOSimulator;
class RDOCalc;
CLOSE_RDO_RUNTIME_NAMESPACE

class ILogic
{
public:
	virtual void init           (PTR (rdoRuntime::RDOSimulator) sim  ) = 0;
	virtual void setCondition   (PTR (rdoRuntime::RDOCalc)      calc ) = 0;
};
#define DECLARE_ILogic \
	virtual void init           (PTR (rdoRuntime::RDOSimulator) sim  ); \
	virtual void setCondition   (PTR (rdoRuntime::RDOCalc)      calc );

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

#endif //! _RDO_LOGIC_INTERFACE_H_
