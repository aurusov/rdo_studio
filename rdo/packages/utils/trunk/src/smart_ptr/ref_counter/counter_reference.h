/*!
  \copyright (c) RDO-Team, 2011
  \file      counter_reference.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      12.06.2010
  \brief     
  \indent    4T
*/

#ifndef _UTILS_SMART_PTR_COUNTER_REFERENCE_H_
#define _UTILS_SMART_PTR_COUNTER_REFERENCE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/namespace.h"
#include "utils/src/common/rdomacros.h"
#include "utils/src/common/rdotypes.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

class counter_reference
{
template<class T> friend class intrusive_ptr;
template<class T> friend class RefCounter;
public:
	//! Инициализиреут счетчик нулём
	counter_reference();
	virtual ~counter_reference();

private:
	ruint m_intrusive_counter;
};

CLOSE_RDO_NAMESPACE

#include "utils/src/smart_ptr/ref_counter/counter_reference.inl"

#endif // _UTILS_SMART_PTR_COUNTER_REFERENCE_H_