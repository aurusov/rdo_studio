/*
 * copyright: (c) RDO-Team, 2011
 * filename : static_assert.h
 * author   : Урусов Андрей
 * date     : 08.03.2010
 * bref     : основан на классе CompileTimeError из Александреску
 * indent   : 4T
 */

#ifndef _STATIC_ASSERT_H_
#define _STATIC_ASSERT_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

template <rbool>
struct CompileTimeError;

template <>
struct CompileTimeError<true> {};

#define STATIC_ASSERT(expr)                 \
	rdo::CompileTimeError<((expr) != 0)>(); \

CLOSE_RDO_NAMESPACE

#endif //! _STATIC_ASSERT_H_