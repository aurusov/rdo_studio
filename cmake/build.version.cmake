// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <string>
#include <boost/format.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

#pragma once

namespace rdo
{
    namespace version
    {
        static const std::string g_appName("${APP_NAME}");
        static const std::string g_version("${PROJECT_VERSION_LONG}");
        static const std::string g_site   ("${ORG_WEBSITE}");

#ifdef RDOSIM_COMPATIBLE
        static const std::string g_comp("-comp");
#else
        static const std::string g_comp("");
#endif

#ifdef OST_WINDOWS
        static const std::string g_systemOS("win");
#elif defined(OST_MACOSX)
                static const std::string g_systemOS("mac");
#elif defined(OST_LINUX)
        static const std::string g_systemOS("linux");
#endif

#ifdef ARCHITECTURE_AMD64
        static const std::string g_systemArchitectures("AMD64");
#elif defined(ARCHITECTURE_X86)
        static const std::string g_systemArchitectures("x86");
#elif defined(ARCHITECTURE_ARM)
        static const std::string g_systemArchitectures("arm");
#endif

#ifdef RDO_LICENSE_ACADEMIC
        static const std::string g_academic("не для коммерческого использования");
#else
        static const std::string g_academic("");
#endif

#ifdef RDO_LIMIT_RES
        static const std::string g_limitRes("ограниченная версия");
#else
        static const std::string g_limitRes("");
#endif

        static const std::string g_versionName = boost::str(boost::format("%1% %2%-version %3% %4%-%5%%6%")
            % g_appName
            % g_comp
            % g_version
            % g_systemOS
            % g_systemArchitectures
            % (!g_academic.empty() || !g_limitRes.empty()
                ? "\n(" + g_academic + (!g_academic.empty() && !g_limitRes.empty() ? ", " : "") + g_limitRes + ")"
                : ""
            )
        );
    }
}
