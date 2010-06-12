/*
 * copyright: (c) RDO-Team, 2010
 * filename : interface_ptr.inl
 * author   : Урусов Андрей
 * date     : 12.06.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_NAMESPACE

template<class T>
inline interface_ptr<T>::interface_ptr()
	: m_pInterface(NULL)
	, m_pCounter  (NULL)
{}

template<class T>
inline interface_ptr<T>::interface_ptr(PTR(T) pInterface, PTR(ruint) pCounter)
	: m_pInterface(pInterface)
	, m_pCounter  (pCounter  )
{
	if (m_pInterface)
		addref();
}

template<class T>
inline interface_ptr<T>::interface_ptr(CREF(this_type) sptr)
	: m_pInterface(sptr.m_pInterface)
	, m_pCounter  (sptr.m_pCounter  )
{
	if (m_pInterface)
		addref();
}

template<class T>
inline interface_ptr<T>::~interface_ptr()
{
	if (m_pInterface)
		release();
}

template<class T>
inline REF(typename interface_ptr<T>::this_type) interface_ptr<T>::operator= (CREF(this_type) sptr)
{
	if (m_pInterface)
		release();

	m_pInterface = sptr.m_pInterface;
	m_pCounter   = sptr.m_pCounter;

	if (m_pInterface)
		addref();

	return *this;
}

template<class T>
inline interface_ptr<T>::operator rbool () const
{
	return m_pInterface != NULL;
}

template<class T>
inline CPTR(T) interface_ptr<T>::operator-> () const
{
	return m_pInterface;
}

template<class T>
inline PTR(T) interface_ptr<T>::operator-> ()
{
	return m_pInterface;
}

template<class T>
inline void interface_ptr<T>::addref()
{
	(*m_pCounter)++;
}

template<class T>
inline void interface_ptr<T>::release()
{
	(*m_pCounter)--;
	if ((*m_pCounter) == 0)
	{
		if (m_pInterface)
		{
			Factory<T>::destroy(m_pInterface);
			m_pInterface = NULL;
		}
	}
}

CLOSE_RDO_NAMESPACE
