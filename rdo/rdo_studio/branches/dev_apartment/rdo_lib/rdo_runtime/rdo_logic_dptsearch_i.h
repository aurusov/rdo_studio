/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic_dptsearch_i.h
 * author   : Урусов Андрей
 * date     : 08.08.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_LOGIC_DPTSEARCH_I_H_
#define _RDO_LOGIC_DPTSEARCH_I_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdointerface.h"
// ===============================================================================

class IDPTSearchLogic
{
public:
	virtual void addActivity(LPIDPTSearchActivity activity) = 0;
};
#define DECLARE_IDPTSearchLogic \
	virtual void addActivity(LPIDPTSearchActivity activity);

#endif //! _RDO_LOGIC_DPTSEARCH_I_H_
