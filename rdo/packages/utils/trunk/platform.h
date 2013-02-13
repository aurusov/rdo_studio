/*!
  \copyright (c) RDO-Team, 2011
  \file      platform.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      04.09.2011
  \brief     Определение платформы и компилятора
  \indent    4T
*/

#ifndef _UTILS_PLATFORM_H_
#define _UTILS_PLATFORM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

// compilers
#ifdef _MSC_VER
	#define COMPILER_VISUAL_STUDIO
#elif defined( __GNUC__ )
	#define COMPILER_GCC
#elif defined( __MINGW32__ )
	#define COMPILER_GCC
	#define COMPILER_MINGW
#else
	#error Unknown compiler!
#endif

// operating systems
#ifdef _WIN32
	#define OST_WINDOWS
	#ifdef _WIN64
		#define OST_WINDOWS_64
	#else
		#define OST_WINDOWS_32
	#endif
#elif defined( __linux__ )
	#define OST_LINUX
#else
	#error Unknown OS!
#endif

// architecture
#if defined( __amd64__ ) || defined( _M_X64 )
	#define ARCHITECTURE_AMD64
#elif defined( __i386__ ) || defined( _M_IX86 )
	#define ARCHITECTURE_X86
#else
	#error Unknown architecture!
#endif

#endif // _UTILS_PLATFORM_H_
