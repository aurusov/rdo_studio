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
#include "utils/platform.h"
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
#define  CLASS(A)            class A
//! Объявление структуры
#define STRUCT(A)            struct A
//! Объявление интерфейса
#define S_INTERFACE(A)       struct A
//! Наследование
#define IS  ,
//! Наследование
#define AND ,
//! Реализация интерфейса A
#define IMPLEMENTATION_OF(A) public A
//! Объект типа A
#define INSTANCE_OF(A)       public A
//! Класс-потомок от B
#define  CLASS_PARENT_OF(A, B)  CLASS(A): IMPLEMENTATION_OF(B)
//! Структура-потомок от B
#define STRUCT_PARENT_OF(A, B) STRUCT(A): IMPLEMENTATION_OF(B)

//! Итерирование контейнера
#define STL_FOR_ALL(CONTAINER, IT)                       \
typedef BOOST_TYPEOF(CONTAINER) CONTAINER_TYPE_FOR_##IT; \
for (CONTAINER_TYPE_FOR_##IT::iterator IT = CONTAINER.begin(); IT != CONTAINER.end(); ++IT)

//! Итерирование константного контейнера
#define STL_FOR_ALL_CONST(CONTAINER, IT)                 \
typedef BOOST_TYPEOF(CONTAINER) CONTAINER_TYPE_FOR_##IT; \
for (CONTAINER_TYPE_FOR_##IT::const_iterator IT = CONTAINER.begin(); IT != CONTAINER.end(); ++IT)

//! Автоматическое определение типа для паременной
#define AUTO BOOST_AUTO

#ifdef COMPILER_VISUAL_STUDIO
	#define NO_V_TABLE __declspec(novtable)
#else
	#define NO_V_TABLE
#endif

#endif // _UTILS_MACROS_H_
