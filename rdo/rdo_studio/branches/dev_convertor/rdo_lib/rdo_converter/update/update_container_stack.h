/*
 * copyright: (c) RDO-Team, 2010
 * filename : update_container_stack.h
 * author   : Урусов Андрей
 * date     : 12.12.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONVERTOR_UPDATE_CONTAINER_STACK_H_
#define _CONVERTOR_UPDATE_CONTAINER_STACK_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/stack.h"

#include "rdo_lib/rdo_converter/update/update_container.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- UpdateContainerStack
// ----------------------------------------------------------------------------
typedef rdo::stack<LPUpdateContainer> UpdateContainerStack;

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_UPDATE_CONTAINER_STACK_H_
