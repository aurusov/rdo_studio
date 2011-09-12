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
#ifdef OST_WINDOWS
	#include <tchar.h>
#else
	#define _T(x) x
#endif

#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

//! Тип символа
typedef  signed char                tchar;

//! Тип байта
typedef  unsigned char              rbyte;

//! Тип строки
typedef  std::basic_string<tchar>   tstring;

//! Булевский тип
typedef  bool                       rbool;

//! Целый знаковый тип
typedef  signed int                 rsint;

//! Целый беззнаковый тип
typedef  unsigned int               ruint;

//! Целый знаковый длинный тип
typedef  signed long int            rslong;

//! Целый беззнаковый длинный тип
typedef  unsigned long int          rulong;

//! Целый знаковый 64-х битный тип
typedef  signed long long int       rsint64;

//! Целый беззнаковый 64-х битный тип
typedef  unsigned long long int     ruint64;

#endif // _UTILS_RDOTYPES_H_
