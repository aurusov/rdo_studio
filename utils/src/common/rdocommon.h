#pragma once

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <sstream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/model_objects.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
    #pragma warning(disable : 4786)
#endif

namespace rdo
{
    std::string format(const char* str, ...);

    template< class T >
    inline std::string toString(const T& value)
    {
        std::stringstream str;
        str << value;
        return str.str();
    }

    int roundDouble(double val);

} // namespace rdo
