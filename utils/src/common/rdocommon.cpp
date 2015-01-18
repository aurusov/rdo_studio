// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <stdarg.h>
#include <stdexcept>
#include <stdio.h>
#include <vector>

#ifdef COMPILER_VISUAL_STUDIO
#    include <windows.h>
#    include <io.h>
#else
#    include <stdarg.h>
#    include <wchar.h>
#endif
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
#    pragma warning(disable : 4786)
#endif

namespace rdo {

namespace
{

std::string format(const char* str, va_list& params)
{
    std::vector<char> s;
    s.resize(256);
    int size = -1;
    while (size == -1)
    {
#ifdef COMPILER_VISUAL_STUDIO
#    pragma warning(disable: 4996)
        size = _vsnprintf(&s[0], s.size(), str, params);
#    pragma warning(default: 4996)
#endif // COMPILER_VISUAL_STUDIO

#ifdef COMPILER_GCC
        size = vsnprintf(&s[0], s.size(), str, params);
#endif // COMPILER_GCC
        if (size == -1)
            s.resize(s.size() + 256);
    }
    s.resize(size);
    return std::string(s.begin(), s.end());
}

} // anonymous namespace

std::string format(const char* str, ...)
{
    va_list params;
    va_start(params, str);
    std::string res = format(str, params);
    va_end(params);
    return res;
}

int roundDouble(double val)
{
    if (val >= 0)
    {
        return (val - (int)val < 0.5) ? (int)val : (int)val + 1;
    }
    else
    {
        return (val - (int)val > - 0.5) ? (int)val : (int)val - 1;
    }
}

} // namespace rdo
