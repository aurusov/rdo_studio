/*!
  \copyright (c) RDO-Team, 2011
  \file      rdotypes.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      18.04.2009
  \brief     Объявление базовый типов
  \indent    4T
*/

#ifndef _UTILS_RDOTYPES_H_
#define _UTILS_RDOTYPES_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_VISUAL_STUDIO
#   include <tchar.h>
#endif // COMPILER_VISUAL_STUDIO
#ifdef COMPILER_GCC
#   include <stdint.h>
#   define _T(x) x
#endif // COMPILER_GCC

#include <list>
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

//! Тип символа
typedef  char                        tchar;

//! Тип строки
typedef  std::basic_string<tchar>    tstring;

//! ANSI-строка
typedef  std::basic_string<char>     astring;

//! Unicode-строка
typedef  std::basic_string<wchar_t>  wstring;

//! Булевский тип
typedef  bool                        rbool;

//! Целый знаковый тип
typedef  signed int                  rsint;

//! Целый беззнаковый тип
typedef  unsigned int                ruint;

//! Целый знаковый 32-х битный тип
#ifdef COMPILER_VISUAL_STUDIO
	typedef  long                    rsint32;
#elif defined COMPILER_GCC
	typedef  int32_t                 rsint32;
#endif

//! Целый беззнаковый 32-х битный тип
#ifdef COMPILER_VISUAL_STUDIO
	typedef  unsigned long           ruint32;
#elif defined COMPILER_GCC
	typedef  uint32_t                ruint32;
#endif

//! Целый знаковый 64-х битный тип
#ifdef COMPILER_VISUAL_STUDIO
	typedef  signed long long int    rsint64;
#elif defined COMPILER_GCC
	typedef  int64_t                 rsint64;
#endif

//! Целый беззнаковый 64-х битный тип
#ifdef COMPILER_VISUAL_STUDIO
	typedef  unsigned long long int  ruint64;
#elif defined COMPILER_GCC
	typedef  uint64_t                ruint64;
#endif

//! Тип байта
typedef  unsigned char               rbyte;

typedef std::list<tstring>           rtstringList;

#endif // _UTILS_RDOTYPES_H_
