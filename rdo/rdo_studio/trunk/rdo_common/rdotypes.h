/*!
  \copyright (c) RDO-Team, 2011
  \file      rdotypes.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      18.04.2009
  \brief     Объявление базовый типов
  \indent    4T
*/

#ifndef _LIB_COMMON_RDOTYPES_H_
#define _LIB_COMMON_RDOTYPES_H_

// ----------------------------------------------------------------------- INCLUDES
#ifdef WIN32
#include <tchar.h>
#else // not WIN32
#define _T(x)       x
#endif // WIN32

#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

//! Тип символа
typedef  char                       tchar;

//! Тип строки
typedef  std::basic_string<tchar>   tstring;

//! Булевский тип
typedef  bool                       rbool;

//! Целый знаковый тип
typedef  signed int                 rsint;

//! Целый беззнаковый тип
typedef  unsigned int               ruint;

//! Целый знаковый 64-х битный тип
typedef  signed long long int       rsint64;

//! Целый беззнаковый 64-х битный тип
typedef  unsigned long long int     ruint64;

//! Тип байта
typedef  unsigned char              rbyte;

#endif // _LIB_COMMON_RDOTYPES_H_
