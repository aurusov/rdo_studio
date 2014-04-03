/*!
  \copyright (c) RDO-Team, 2011
  \file      intrusive_ptr_interface_wrapper.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      22.09.2011
  \brief     
  \indent    4T
*/

#ifndef _UTILS_SMART_PTR_INTRUSIVE_PTR_INTERFACE_WRAPPER_H_
#define _UTILS_SMART_PTR_INTRUSIVE_PTR_INTERFACE_WRAPPER_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "utils/src/smart_ptr/ref_counter/ref_counter_i.h"
// --------------------------------------------------------------------------------

namespace rdo {

//! Оборачивает умный указатель в интерфейс
//! tparam T - тип объекта, на который ссылается указатель
template <class T>
class intrusive_ptr_interface_wrapper: public rdo::intrusive_ptr<T>, public rdo::IRefCounter
{
public:
	intrusive_ptr_interface_wrapper(CREF(rdo::intrusive_ptr<T>) pPointer);
	DECLARE_IRefCounter;

private:
	typedef rdo::intrusive_ptr<T> parent_type;
};

} // namespace rdo

#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr_interface_wrapper.inl"

#endif // _UTILS_SMART_PTR_INTRUSIVE_PTR_INTERFACE_WRAPPER_H_
