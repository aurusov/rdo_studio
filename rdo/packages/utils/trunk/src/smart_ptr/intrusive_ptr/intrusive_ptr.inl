/*!
  \copyright (c) RDO-Team, 2011
  \file      intrusive_ptr.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      08.06.2010
  \brief
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdomacros.h"
#include "utils/src/smart_ptr/factory/factory.h"
#include "utils/src/smart_ptr/factory/factory.inl"
#include "utils/src/smart_ptr/ref_counter/ref_counter.h"
#include "utils/src/smart_ptr/ref_counter/ref_counter.inl"
// --------------------------------------------------------------------------------

namespace rdo {

template<class T>
FORCE_INLINE intrusive_ptr<T>::intrusive_ptr()
	: m_object(NULL)
{}

template<class T>
FORCE_INLINE intrusive_ptr<T>::intrusive_ptr(PTR(T) object)
	: m_object(object)
{
	if (m_object)
		addref();
}

template<class T>
FORCE_INLINE intrusive_ptr<T>::intrusive_ptr(CREF(this_type) sptr)
	: m_object(sptr.m_object)
{
	if (m_object)
		addref();
}

template <class T>
template <class P>
inline intrusive_ptr<T>::intrusive_ptr(CREF(interface_ptr<P>) pInterface)
{
	m_object = dynamic_cast<PTR(T)>(pInterface.m_pInterface);
	if (m_object)
		addref();
}

template<class T>
FORCE_INLINE intrusive_ptr<T>::~intrusive_ptr()
{
	if (m_object)
		release();
}

template<class T>
FORCE_INLINE REF(typename intrusive_ptr<T>::this_type) intrusive_ptr<T>::operator= (CREF(this_type) sptr)
{
	if (m_object)
		release();

	m_object = sptr.m_object;

	if (m_object)
		addref();

	return *this;
}

template<class T>
template<class P>
FORCE_INLINE rbool intrusive_ptr<T>::operator== (CREF(intrusive_ptr<P>) sptr) const
{
	return &m_object->m_intrusive_counter == &sptr.m_object->m_intrusive_counter;
}

template<class T>
template<class P>
FORCE_INLINE rbool intrusive_ptr<T>::operator!= (CREF(intrusive_ptr<P>) sptr) const
{
	return !operator==(sptr);
}

template<class T>
template<class P>
FORCE_INLINE rbool intrusive_ptr<T>::compare(CREF(intrusive_ptr<P>) sptr) const
{
	return *m_object == *sptr.m_object;
}

template<class T>
FORCE_INLINE intrusive_ptr<T>::operator rbool () const
{
	return m_object != NULL;
}

template<class T>
FORCE_INLINE PTR(T) intrusive_ptr<T>::operator-> () const
{
	return m_object;
}

template<class T>
FORCE_INLINE PTR(T) intrusive_ptr<T>::operator-> ()
{
	return m_object;
}

template<class T>
FORCE_INLINE CREF(T) intrusive_ptr<T>::operator* () const
{
	return *m_object;
}

template<class T>
FORCE_INLINE REF(T) intrusive_ptr<T>::operator* ()
{
	return *m_object;
}

template<class T>
template<class P>
inline intrusive_ptr<T>::operator intrusive_ptr<P>() const
{
	return intrusive_ptr<P>(m_object);
}

template<class T>
template<class P>
inline intrusive_ptr<P> intrusive_ptr<T>::object_parent_cast() const
{
	return intrusive_ptr<P>(m_object);
}

template<class T>
template<class P>
inline intrusive_ptr<P> intrusive_ptr<T>::object_static_cast() const
{
	return intrusive_ptr<P>(static_cast<PTR(P)>(m_object));
}

template<class T>
template<class P>
inline intrusive_ptr<P> intrusive_ptr<T>::object_dynamic_cast() const
{
	return intrusive_ptr<P>(dynamic_cast<PTR(P)>(m_object));
}

template<class T>
template<class P>
inline interface_ptr<P> intrusive_ptr<T>::interface_cast() const
{
	return interface_ptr<P>(static_cast<PTR(P)>(m_object), static_cast<LPIRefCounter>(m_object));
}

template<class T>
template<class P>
inline interface_ptr<P> intrusive_ptr<T>::interface_dynamic_cast() const
{
	PTR(P) pInterface = dynamic_cast<PTR(P)>(m_object);
	return interface_ptr<P>(pInterface, dynamic_cast<LPIRefCounter>(pInterface));
}

template<class T>
FORCE_INLINE PTR(T) intrusive_ptr<T>::get()
{
	return m_object;
}

template<class T>
FORCE_INLINE CPTR(T) intrusive_ptr<T>::get() const
{
	return m_object;
}

template<class T>
FORCE_INLINE void intrusive_ptr<T>::addref()
{
	counter()++;
}

template<class T>
FORCE_INLINE void intrusive_ptr<T>::release()
{
	counter()--;
	if (counter() == 0)
	{
		if (m_object)
		{
			Factory<T>::destroy(m_object);
			m_object = NULL;
		}
	}
}

template<class T>
FORCE_INLINE rbool intrusive_ptr<T>::owner() const
{
	return m_object->m_intrusive_counter == 1;
}

template<class T>
FORCE_INLINE REF(ruint) intrusive_ptr<T>::counter()
{
	return m_object->m_intrusive_counter;
}

} // namespace rdo
