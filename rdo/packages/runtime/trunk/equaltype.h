/*!
  \copyright (c) RDO-Team, 2011
  \file      equaltype.h
  \author    Чирков Михаил
  \date      03.12.2010
  \brief     Типы операторов сравнения
  \indent    4T
*/

#ifndef _LIB_RUNTIME_EQUAL_TYPE_H_
#define _LIB_RUNTIME_EQUAL_TYPE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \enum    EqualType
  \brief   Типы сравнений
*/
enum EqualType
{
	ET_UNDEFINED,
	ET_NOCHANGE,
	ET_EQUAL,
	ET_PLUS,
	ET_MINUS,
	ET_MULTIPLY,
	ET_DIVIDE,
	ET_INCR,
	ET_DECR
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_EQUAL_TYPE_H_
