/*!
  \copyright (c) RDO-Team, 2011
  \file      ref_counter.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      21.09.2011
  \brief     
  \indent    4T
*/

#ifndef _UTILS_SMART_PTR_REF_COUNTER_H_
#define _UTILS_SMART_PTR_REF_COUNTER_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/ref_counter_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

template <class T>
class RefCounter: public IRefCounter
{
public:
	DECLARE_IRefCounter;
};

CLOSE_RDO_NAMESPACE

#endif // _UTILS_SMART_PTR_REF_COUNTER_H_
