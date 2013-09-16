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
#include <boost/preprocessor/cat.hpp>
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
//! Объявление структуры
#define STRUCT(A)              struct A
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
//! Объект типа виртуальный A
#define INSTANCE_VIRTUAL_OF(A) public virtual A
//! Класс-потомок от B
#define CLASS_PARENT_OF(A, B)  CLASS(A): IMPLEMENTATION_OF(B)
//! Структура-потомок от B
#define STRUCT_PARENT_OF(A, B) STRUCT(A): IMPLEMENTATION_OF(B)

#ifdef UNUSED
	#undef UNUSED
#endif
//! Макрос подавляет предупреждения компилятора о неиспользуемых переменных
#define UNUSED(A) ((void)A);

//! Условно-уникальное имя, основанное на номере строки
#define RDO_UNIQUE_NAME(NAME) BOOST_PP_CAT(NAME, __LINE__)

//! Итерирование контейнера
#define STL_FOR_ALL(CONTAINER, IT)                                \
typedef BOOST_TYPEOF(CONTAINER) RDO_UNIQUE_NAME(CONTAINER_TYPE_); \
for (RDO_UNIQUE_NAME(CONTAINER_TYPE_)::iterator IT = CONTAINER.begin(); IT != CONTAINER.end(); ++IT)

//! Итерирование константного контейнера
#define STL_FOR_ALL_CONST(CONTAINER, IT)                          \
typedef BOOST_TYPEOF(CONTAINER) RDO_UNIQUE_NAME(CONTAINER_TYPE_); \
for (RDO_UNIQUE_NAME(CONTAINER_TYPE_)::const_iterator IT = CONTAINER.begin(); IT != CONTAINER.end(); ++IT)

//! Автоматическое определение типа для паременной
#define AUTO BOOST_AUTO

#ifdef COMPILER_VISUAL_STUDIO
	#define NO_V_TABLE   __declspec(novtable)
	#define FORCE_INLINE __forceinline
#else
	#define NO_V_TABLE
	#define FORCE_INLINE inline
#endif

#endif // _UTILS_MACROS_H_
