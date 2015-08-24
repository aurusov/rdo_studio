#include "rdodebug.h"

#ifdef _DEBUG
#include <iostream>
#include "utils/src/locale/rdolocale.h"
#endif

#ifdef _DEBUG

void StdOutput(const std::string& message)
{
#ifdef COMPILER_VISUAL_STUDIO
    std::wcout << rdo::locale::convertToWStr(message) << std::endl;
#else
    std::cout << message << std::endl;
#endif
}

void StdError(const std::string& message)
{
#ifdef COMPILER_VISUAL_STUDIO
    std::wcerr << rdo::locale::convertToWStr(message) << std::endl;
#else
    std::cerr << message << std::endl;
#endif
}

#endif // _DEBUG
