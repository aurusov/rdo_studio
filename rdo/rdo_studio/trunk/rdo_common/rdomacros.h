/*
  \copyright (c) RDO-Team, 2011
  \file      rdomacros.h
  \author    Урусов Андрей
  \date      13.06.2009
  \brief     Объявление стандартных макросов
  \indent    4T
 */

#ifndef _LIB_COMMON_MACROS_H_
#define _LIB_COMMON_MACROS_H_

// ====================================================================== INCLUDES
#include <boost/typeof/typeof.hpp>
// ====================================================================== SYNOPSIS
// ===============================================================================

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
#define STL_FOR_ALL(CONTAINER, IT)       for (BOOST_TYPEOF(CONTAINER)::iterator IT = CONTAINER.begin(); IT != CONTAINER.end(); ++IT)
//! Итерирование константного контейнера
#define STL_FOR_ALL_CONST(CONTAINER, IT) for (BOOST_TYPEOF(CONTAINER)::const_iterator IT = CONTAINER.begin(); IT != CONTAINER.end(); ++IT)

#endif //! _LIB_COMMON_MACROS_H_
