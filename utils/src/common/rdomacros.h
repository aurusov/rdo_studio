/*!
  \copyright (c) RDO-Team, 2011
  \file      rdomacros.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.06.2009
  \brief     Объявление базовых макросов
  \indent    4T
*/

#ifndef _UTILS_MACROS_H_
#define _UTILS_MACROS_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/typeof/typeof.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

//! Ссылка на A
#define   REF(A)       A&
//! Константная ссылка на A
#define  CREF(A) const A&
//! Указатель на А
#define   PTR(A)       A*
//! Константный указатель на А
#define  CPTR(A) const A*
//! Константный указатель на константный объект типа А
#define CPTRC(A) const A* const

//! Объявление класса
#define  CLASS(A)              class A
//! Объявление интерфейса
#define S_INTERFACE(A)         struct A
//! Наследование
#define IS  ,
//! Наследование
#define AND ,
//! Реализация интерфейса A
#define IMPLEMENTATION_OF(A)   public A
//! Объект типа A
#define INSTANCE_OF(A)         public A

#ifdef UNUSED
	#undef UNUSED
#endif
//! Макрос подавляет предупреждения компилятора о неиспользуемых переменных
#define UNUSED(A) ((void)A);

#endif // _UTILS_MACROS_H_
