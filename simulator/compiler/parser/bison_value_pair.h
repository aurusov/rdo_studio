#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <utility>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/compiler/parser/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- BisonValuePair
// --------------------------------------------------------------------------------
template <class T1, class T2>
class BisonValuePair: public rdo::counter_reference
{
public:
    const T1& getFirst () const;
    const T2& getSecond() const;

private:
    typedef  BisonValuePair<T1, T2>  this_type;
    DECLARE_FACTORY(this_type);

    BisonValuePair(const T1& first, const T2& second);
    virtual ~BisonValuePair();

    typedef  std::pair<T1, T2>  Value;

    Value m_value;
};

CLOSE_RDO_PARSER_NAMESPACE

#include "simulator/compiler/parser/bison_value_pair-inl.h"
