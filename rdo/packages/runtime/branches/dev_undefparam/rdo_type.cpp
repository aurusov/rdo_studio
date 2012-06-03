/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_type.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      22.06.2008
  \brief     Базовый тип данных
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_type.h"
#include "utils/smart_ptr/factory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/// @todo внимание макрос!

/*!
  \def       DEFINE_ATOM_TYPE_DEFAULT_VALUE
  \brief     Макрос для определения значений по умолчанию для простых (атомарных) типов данных
*/
#define DEFINE_ATOM_TYPE_DEFAULT_VALUE(Type) rdo::intrusive_ptr<RDOType__##Type> g_##Type = rdo::Factory<RDOType__##Type>::create();

/*!
  \var       g_unknow
  \brief     Значение типа unknow по умолчанию
*/
DEFINE_ATOM_TYPE_DEFAULT_VALUE(unknow);

/*!
  \var       g_void
  \brief     Значение типа void по умолчанию
*/
DEFINE_ATOM_TYPE_DEFAULT_VALUE(void);

/*!
  \var       g_identificator
  \brief     Значение типа identificator по умолчанию
*/
DEFINE_ATOM_TYPE_DEFAULT_VALUE(identificator);

/*!
  \var       g_int
  \brief     Значение типа int по умолчанию
*/
DEFINE_ATOM_TYPE_DEFAULT_VALUE(int);

/*!
  \var       g_real
  \brief     Значение типа real по умолчанию
*/
DEFINE_ATOM_TYPE_DEFAULT_VALUE(real);

/*!
  \var       g_bool
  \brief     Значение типа bool по умолчанию
*/
DEFINE_ATOM_TYPE_DEFAULT_VALUE(bool);

/*!
  \var       g_string
  \brief     Значение типа string по умолчанию
*/
DEFINE_ATOM_TYPE_DEFAULT_VALUE(string);

CLOSE_RDO_RUNTIME_NAMESPACE
