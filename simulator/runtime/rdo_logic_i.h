#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
PREDECLARE_POINTER(RDORuntime);
PREDECLARE_POINTER(RDOCalc);
CLOSE_RDO_RUNTIME_NAMESPACE

class ILogic: public virtual rdo::counter_reference
{
public:
    virtual void init             (const rdo::runtime::LPRDORuntime& pRuntime      ) = 0;
    virtual void setCondition     (const rdo::runtime::LPRDOCalc&    pCondition    ) = 0;
    virtual void setMultithreading(bool                              multithreading) = 0;
};
DECLARE_POINTER(ILogic)

#define DECLARE_ILogic                                                                \
    virtual void init             (const rdo::runtime::LPRDORuntime& pRuntime      ); \
    virtual void setCondition     (const rdo::runtime::LPRDOCalc&    pCondition    ); \
    virtual void setMultithreading(bool                              multithreading);

class IBaseOperationContainer: public virtual rdo::counter_reference
{
public:
    typedef  LPIBaseOperation      Item;
    typedef  std::vector<Item>     List;
    typedef  List::iterator        Iterator;
    typedef  List::const_iterator  CIterator;

    virtual Iterator          begin ()                 = 0;
    virtual Iterator          end   ()                 = 0;
    virtual CIterator         begin () const           = 0;
    virtual CIterator         end   () const           = 0;
    virtual void              append(const Item& item) = 0;
    virtual bool              empty () const           = 0;
    virtual LPIBaseOperation& back  ()                 = 0;
    virtual void              clear ()                 = 0;
};
DECLARE_POINTER(IBaseOperationContainer);

#define DECLARE_IBaseOperationContainer                 \
    virtual Iterator          begin ();                 \
    virtual Iterator          end   ();                 \
    virtual CIterator         begin () const;           \
    virtual CIterator         end   () const;           \
    virtual void              append(const Item& item); \
    virtual bool              empty () const;           \
    virtual LPIBaseOperation& back  ();                 \
    virtual void              clear ();
