#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/compiler/parser/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ContextStack
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(Context);

PREDECLARE_POINTER(ContextStack);
class ContextStack: public rdo::counter_reference
{
DECLARE_FACTORY(ContextStack);
public:
    void                     push(LPContext pContext);
    template <class T> void  pop ()
    {
        ASSERT(top().object_dynamic_cast<T>());
        pop_not_safed();
    }

    LPContext top   () const;
    LPContext global() const;
    LPContext prev  (const LPContext& pContext) const;

    void pop_not_safed();

private:
    ContextStack();
    virtual ~ContextStack();

    typedef std::list<LPContext> Container;

    Container m_container;
};

CLOSE_RDO_PARSER_NAMESPACE
