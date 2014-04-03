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
#include "utils/src/smart_ptr/ref_counter/ref_counter_i.h"
// --------------------------------------------------------------------------------

namespace rdo {

template <class T>
class RefCounter: public IRefCounter
{
public:
	DECLARE_IRefCounter;
};

} // namespace rdo

#endif // _UTILS_SMART_PTR_REF_COUNTER_H_
