// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <string>
#include <boost/format.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

#ifndef _RDO_BUILD_VERSION_H_
#define _RDO_BUILD_VERSION_H_

namespace rdo
{
	namespace version
	{
		static const std::string g_appName("${APP_NAME}");
		static const std::string g_version("2.0");
		static const std::string g_build  ("svn-${CMAKE_SVN_VERSION}");
		static const std::string g_site   ("${ORG_WEBSITE}");

#ifdef RDO_MT
		static const std::string g_mt("mt");
#else
		static const std::string g_mt("st");
#endif

#ifdef RDOSIM_COMPATIBLE
		static const std::string g_comp("-comp");
#else
		static const std::string g_comp("");
#endif

#ifdef OST_WINDOWS
		static const std::string g_systemOS("win");
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

		static const std::string g_versionName = boost::str(boost::format("%1% %2%%3%-version %4% (build %5%) %6%-%7%%8%")
			% g_appName
			% g_mt
			% g_comp
			% g_version
			% g_build
			% g_systemOS
			% g_systemArchitectures
			% (!g_academic.empty() || !g_limitRes.empty()
				? "\n(" + g_academic + (!g_academic.empty() && !g_limitRes.empty() ? ", " : "") + g_limitRes + ")"
				: ""
			)
		);
	}
}

#endif // _RDO_BUILD_VERSION_H_
