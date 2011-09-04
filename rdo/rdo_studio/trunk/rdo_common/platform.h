/*!
  \copyright (c) RDO-Team, 2011
  \file      platform.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      04.09.2011
  \brief     Определение платформы и компилятора
  \indent    4T
*/

#ifndef _LIB_COMMON_PLATFORM_H_
#define _LIB_COMMON_PLATFORM_H_

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

#ifdef _WIN32
	#ifndef OS_WINDOWS
		#define OS_WINDOWS
	#endif
	#ifdef _WIN64
		#define OS_WINDOWS_64
	#else
		#define OS_WINDOWS_32
	#endif
#elif
	#ifdef __linux__
		#define OS_LINUX
	#endif
#endif

#endif // _LIB_COMMON_PLATFORM_H_
