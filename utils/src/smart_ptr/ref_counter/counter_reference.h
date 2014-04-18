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
// --------------------------------------------------------------------------------

namespace rdo {

class counter_reference
{
template<class T> friend class intrusive_ptr;
template<class T> friend class RefCounter;
public:
	//! Инициализиреут счетчик нулём
	counter_reference();
	virtual ~counter_reference();

private:
	std::size_t m_intrusive_counter;
};

} // namespace rdo

#include "utils/src/smart_ptr/ref_counter/counter_reference.inl"

#endif // _UTILS_SMART_PTR_COUNTER_REFERENCE_H_
