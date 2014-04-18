/*!
  \copyright (c) RDO-Team, 2011
  \file      interface_ptr.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      12.06.2010
  \brief     
  \indent    4T
*/

#ifndef _UTILS_SMART_PTR_INTERFACE_PTR_H_
#define _UTILS_SMART_PTR_INTERFACE_PTR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/ref_counter/ref_counter_i.h"
// --------------------------------------------------------------------------------

namespace rdo {

template<class T>
class interface_ptr
{
template <typename P> friend class intrusive_ptr;
public:
	typedef interface_ptr<T> this_type;

	interface_ptr ();
	interface_ptr (T* pInterface, LPIRefCounter pCounter);
	interface_ptr (const this_type& sptr);
	~interface_ptr();

	this_type& operator= (const this_type& sptr);

	operator bool() const;
	const T* operator->() const;
	T* operator->();

private:
	T* m_pInterface;
	LPIRefCounter m_pCounter;
};

#define DECLARE_OBJECT_INTERFACE(TYPE)    typedef rdo::interface_ptr<TYPE> LP##TYPE;
#define PREDECLARE_OBJECT_INTERFACE(TYPE) struct TYPE; DECLARE_OBJECT_INTERFACE(TYPE);

} // namespace rdo

#include "utils/src/smart_ptr/interface_ptr/interface_ptr.inl"

#endif // _UTILS_SMART_PTR_INTERFACE_PTR_H_
