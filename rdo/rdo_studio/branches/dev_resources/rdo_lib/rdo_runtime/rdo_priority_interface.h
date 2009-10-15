/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_priority_interface.h
 * author   : Урусов Андрей
 * date     : 01.08.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_PRIORITY_INTERFACE_H_
#define _RDO_PRIORITY_INTERFACE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <rdointerface.h>
// ===============================================================================

class IPriority
{
public:
	virtual PTR(rdoRuntime::RDOCalc) getPrior()                               = 0;
	virtual rbool                    setPrior(PTR(rdoRuntime::RDOCalc) prior) = 0;
};
#define DECLARE_IPriority \
	virtual PTR(rdoRuntime::RDOCalc) getPrior(); \
	virtual rbool                    setPrior(PTR(rdoRuntime::RDOCalc) prior);

#endif //! _RDO_PRIORITY_INTERFACE_H_
