/*!
  \copyright (c) RDO-Team, 2011
  \file      ref_counter_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      21.09.2011
  \brief     
  \indent    4T
*/

#ifndef _UTILS_SMART_PTR_REF_COUNTER_I_H_
#define _UTILS_SMART_PTR_REF_COUNTER_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/namespace.h"
#include "utils/src/common/rdomacros.h"
#include "utils/src/common/rdotypes.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

struct NO_V_TABLE IRefCounter
{
	virtual void  addref ()       = 0;
	virtual void  release()       = 0;
	virtual rbool owner  () const = 0;
};
#define DECLARE_IRefCounter \
	void  addref ();        \
	void  release();        \
	rbool owner  () const;

typedef PTR(IRefCounter) LPIRefCounter;

CLOSE_RDO_NAMESPACE

#endif // _UTILS_SMART_PTR_REF_COUNTER_I_H_
