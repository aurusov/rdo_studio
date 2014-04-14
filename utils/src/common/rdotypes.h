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
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_GCC
#   include <stdint.h>
#endif // COMPILER_GCC
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

//! Тип байта
typedef  unsigned char               rbyte;

#endif // _UTILS_RDOTYPES_H_
