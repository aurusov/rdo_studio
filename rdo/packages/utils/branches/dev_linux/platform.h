/*!
  \copyright (c) RDO-Team, 2011
  \file      platform.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      04.09.2011
  \brief     Определение платформы и компилятора
  \indent    4T
*/

#ifndef _UTILS_PLATFORM_H_
#define _UTILS_PLATFORM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

#ifdef _MSC_VER
	#define COMPILER_VISUAL_STUDIO
#else
	#ifdef __GNUC__
		#define COMPILER_GCC
	#endif
#endif

#ifdef __MINGW32__
	#define OST_LINUX
        #define COMPILER_MINGW
#else
	#ifdef _WIN32
		#define OST_WINDOWS
		#ifdef _WIN64
			#define OST_WINDOWS_64
		#else
			#define OST_WINDOWS_32
		#endif
	#else
		#ifdef __linux__
			#define OST_LINUX
		#endif
	#endif
#endif

#endif // _UTILS_PLATFORM_H_
