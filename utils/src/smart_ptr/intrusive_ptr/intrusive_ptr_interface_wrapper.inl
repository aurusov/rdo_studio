/*!
  \copyright (c) RDO-Team, 2011
  \file      intrusive_ptr_interface_wrapper.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      22.09.2011
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "utils/src/smart_ptr/ref_counter/ref_counter_i.h"
// --------------------------------------------------------------------------------

namespace rdo {

// --------------------------------------------------------------------------------
// -------------------- RefCounter<T>
// --------------------------------------------------------------------------------
template <class T>
FORCE_INLINE intrusive_ptr_interface_wrapper<T>::intrusive_ptr_interface_wrapper(CREF(rdo::intrusive_ptr<T>) pPointer)
	: parent_type(pPointer)
{}

template <class T>
FORCE_INLINE void intrusive_ptr_interface_wrapper<T>::addref()
{
	parent_type::addref();
}

template <class T>
FORCE_INLINE void intrusive_ptr_interface_wrapper<T>::release()
{
	parent_type::release();
}

template <class T>
FORCE_INLINE rbool intrusive_ptr_interface_wrapper<T>::owner() const
{
	return parent_type::owner();
}

} // namespace rdo
