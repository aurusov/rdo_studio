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
#else  // not _MSC_VER
	#ifdef __GNUC__
		#define COMPILER_GCC
	#endif // not __GNUC__
#endif // _MSC_VER

#ifdef _WIN32
	#define OS_WINDOWS
	#ifdef _WIN64
		#define OS_WINDOWS_64
	#else  // not _WIN64
		#define OS_WINDOWS_32
	#endif // _WIN64
#elif  // not _WIN32
	#ifdef __linux__
		#define OS_LINUX
	#endif
#endif // _WIN32

#endif // _LIB_COMMON_PLATFORM_H_
