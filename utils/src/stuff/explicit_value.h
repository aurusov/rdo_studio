#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

namespace rdo {

template <class T>
class explicit_value
{
public:
    explicit explicit_value(const T& value)
        : m_value(value)
    {}

    operator T() const
    {
        return m_value;
    }

private:
    T m_value;
};

} // namespace rdo
