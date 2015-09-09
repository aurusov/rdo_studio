#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

namespace rdo {

class IDGenerator
{
public:
    typedef std::size_t ID;

    IDGenerator()
        : m_id(0)
    {}

    ID get()
    {
        return m_id++;
    }

private:
    ID m_id;
};

} // namespace rdo
