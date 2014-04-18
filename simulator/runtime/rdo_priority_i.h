/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_priority_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      01.08.2009
  \brief     Интерфейс IPriority
  \indent    4T
*/

#ifndef _LIB_RUNTIME_PRIORITY_I_H_
#define _LIB_RUNTIME_PRIORITY_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/interface/rdointerface.h"
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOCalc)

CLOSE_RDO_RUNTIME_NAMESPACE

/*!
  \interface IPriority
  \brief     Интерфейс IPriority
*/
class IPriority
{
public:
	virtual rdo::runtime::LPRDOCalc getPrior()                   = 0;
	virtual bool setPrior(const rdo::runtime::LPRDOCalc& pPrior) = 0;
};

#define DECLARE_IPriority                       \
	virtual rdo::runtime::LPRDOCalc getPrior(); \
	virtual bool setPrior(const rdo::runtime::LPRDOCalc& pPrior);

#endif // _LIB_RUNTIME_PRIORITY_I_H_
