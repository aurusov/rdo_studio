/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_logic_dptsearch_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      08.08.2009
  \brief     DPTSearch
  \indent    4T
*/

#ifndef _LIB_RUNTIME_LOGIC_DPTSEARCH_I_H_
#define _LIB_RUNTIME_LOGIC_DPTSEARCH_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/interface/rdointerface.h"
#include "simulator/runtime/rdo_dptsearch_activity_i.h"
// --------------------------------------------------------------------------------

/*!
  \interface IDPTSearchLogic
  \brief     Инерфейс IDPTSearchLogic
*/
class IDPTSearchLogic
{
public:
	virtual void addActivity(LPIDPTSearchActivity activity) = 0;
};

#define DECLARE_IDPTSearchLogic \
	virtual void addActivity(LPIDPTSearchActivity activity);

#endif // _LIB_RUNTIME_LOGIC_DPTSEARCH_I_H_
