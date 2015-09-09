#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

// compilers
#ifdef _MSC_VER
    #define COMPILER_VISUAL_STUDIO
#elif defined( __MINGW32__ )
    #define COMPILER_GCC
    #define COMPILER_MINGW
#elif defined( __GNUC__ )
    #define COMPILER_GCC
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
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
    #define OST_MACOSX
#else
    #error Unknown OS!
#endif

// architecture
#if defined( __amd64__ ) || defined( _M_X64 )
    #define ARCHITECTURE_AMD64
#elif defined( __i386__ ) || defined( _M_IX86 )
    #define ARCHITECTURE_X86
#elif defined( __arm__ ) || defined( __thumb__ ) || defined ( __TARGET_ARCH_ARM ) || defined ( __TARGET_ARCH_THUMB )
    #define ARCHITECTURE_ARM
#else
    #error Unknown architecture!
#endif
