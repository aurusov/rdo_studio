#pragma once

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <stdarg.h>
#include <vector>
#include <sstream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/model_objects.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
    #pragma warning(disable : 4786)
#endif

namespace rdo {

    std::string format(const char* str, ...);
    std::string format(const char* str, va_list& params);

    template< class T >
    inline std::string toString(const T& value)
    {
        std::stringstream str;
        str << value;
        return str.str();
    }

    template <class T>
    class vector: public std::vector<T>
    {
    public:
        vector()
        {}

        vector(const T& item)
        {
            std::vector<T>::push_back(item);
        }

        vector& operator() (const T& item)
        {
            std::vector<T>::push_back(item);
            return *this;
        }
    };

    int roundDouble(double val);

} // namespace rdo
