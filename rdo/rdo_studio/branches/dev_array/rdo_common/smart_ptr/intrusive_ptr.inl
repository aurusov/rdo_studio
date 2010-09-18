/*
 * copyright: (c) RDO-Team, 2010
 * filename : intrusive_ptr.inl
 * author   : Урусов Андрей
 * date     : 08.06.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_NAMESPACE

inline counter_reference::counter_reference()
	: m_intrusive_counter(0)
{}

template<class T>
inline intrusive_ptr<T>::intrusive_ptr()
	: m_object(NULL)
{}

template<class T>
inline intrusive_ptr<T>::intrusive_ptr(PTR(T) object)
	: m_object(object)
{
	if (m_object)
		addref();
}

template<class T>
inline intrusive_ptr<T>::intrusive_ptr(CREF(this_type) sptr)
	: m_object(sptr.m_object)
{
	if (m_object)
		addref();
}

template<class T>
inline intrusive_ptr<T>::~intrusive_ptr()
{
	if (m_object)
		release();
}

template<class T>
inline REF(typename intrusive_ptr<T>::this_type) intrusive_ptr<T>::operator= (CREF(this_type) sptr)
{
	if (m_object)
		release();

	m_object = sptr.m_object;

	if (m_object)
		addref();

	return *this;
}

//template<class T>
//inline PTR(T) intrusive_ptr<T>::get()
//{
//	return m_object;
//}
//
//template<class T>
//inline CPTR(T) intrusive_ptr<T>::get() const
//{
//	return m_object;
//}

template<class T>
inline intrusive_ptr<T>::operator rbool () const
{
	return m_object != NULL;
}

template<class T>
inline CPTR(T) intrusive_ptr<T>::operator-> () const
{
	return m_object;
}

template<class T>
inline PTR(T) intrusive_ptr<T>::operator-> ()
{
	return m_object;
}

template<class T>
template<class P>
inline intrusive_ptr<P> intrusive_ptr<T>::cast() const
{
	return intrusive_ptr<P>(static_cast<PTR(P)>(m_object));
}

template<class T>
inline void intrusive_ptr<T>::addref()
{
	counter()++;
}

template<class T>
inline void intrusive_ptr<T>::release()
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
inline REF(ruint) intrusive_ptr<T>::counter()
{
	return static_cast<PTR(counter_reference)>(m_object)->m_intrusive_counter;
}

CLOSE_RDO_NAMESPACE
