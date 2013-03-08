/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdointerface.inl
 * author   : Урусов Андрей
 * date     : 12.08.2011
 * bref     : 
 * indent   : 4T
 */

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdodebug.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

template<class I>
FORCE_INLINE Interface<I>::Interface()
	: UnknownPointer(NULL, NULL)
{}

template<class I>
FORCE_INLINE Interface<I>::Interface(LPIGetUnknown pGetUnknown)
	: UnknownPointer(pGetUnknown ? pGetUnknown->GetUnknown()->QueryInterface(GetInterface<I>::ID) : UnknownPointer())
{}

template<class I>
FORCE_INLINE Interface<I>::Interface(PTR(void) pInterface, LPIUnknown pUnknown)
	: UnknownPointer(pInterface, pUnknown)
{}

template<class I>
FORCE_INLINE Interface<I>::Interface(CREF(this_type) aInterface)
	: UnknownPointer(aInterface)
{}

template<class I>
FORCE_INLINE REF(typename Interface<I>::this_type) Interface<I>::operator= (CREF(this_type) aInterface)
{
	parent_type::operator= (aInterface);
	return *this;
}

template<class I>
FORCE_INLINE Interface<I>::operator rbool() const
{
	return m_pInterface != NULL;
}

template<class I>
FORCE_INLINE PTR(I) Interface<I>::get()
{
	return static_cast<PTR(I)>(m_pInterface);
}

template<class I>
FORCE_INLINE CPTR(I) Interface<I>::get() const
{
	return static_cast<PTR(I)>(m_pInterface);
}

template<class I>
FORCE_INLINE PTR(I) Interface<I>::operator-> ()
{
	return get();
}

template<class I>
FORCE_INLINE CPTR(I) Interface<I>::operator-> () const
{
	return get();
}

FORCE_INLINE UnknownPointer::UnknownPointer()
	: m_pInterface(NULL)
	, m_pUnknown  (NULL)
{}

FORCE_INLINE UnknownPointer::UnknownPointer(REF(IUnknown) unknown)
	: m_pInterface(NULL    )
	, m_pUnknown  (&unknown)
{
	if (m_pUnknown)
		m_pUnknown->AddRef();
}

FORCE_INLINE UnknownPointer::UnknownPointer(LPIGetUnknown pGetUnknown)
	: m_pInterface(NULL)
{
	m_pUnknown = pGetUnknown ? pGetUnknown->GetUnknown() : NULL;
	if (m_pUnknown)
		m_pUnknown->AddRef();
}

FORCE_INLINE UnknownPointer::UnknownPointer(PTR(void) pInterface, LPIUnknown pUnknown)
	: m_pInterface(pInterface)
	, m_pUnknown  (pUnknown  )
{
	if (m_pUnknown)
		m_pUnknown->AddRef();
}

FORCE_INLINE UnknownPointer::UnknownPointer(CREF(UnknownPointer) pointer)
	: m_pInterface(pointer.m_pInterface)
	, m_pUnknown  (pointer.m_pUnknown  )
{
	if (m_pUnknown)
		m_pUnknown->AddRef();
}

FORCE_INLINE UnknownPointer::~UnknownPointer()
{
	if (m_pUnknown)
		m_pUnknown->Release();
}

FORCE_INLINE rbool UnknownPointer::operator== (CREF(UnknownPointer) pointer) const
{
	return m_pUnknown == pointer.m_pUnknown;
}

inline REF(UnknownPointer) UnknownPointer::operator= (CREF(UnknownPointer) pointer)
{
	if (m_pUnknown)
		m_pUnknown->Release();

	m_pInterface = pointer.m_pInterface;
	m_pUnknown   = pointer.m_pUnknown;
	if (m_pUnknown)
		m_pUnknown->AddRef();

	return *this;
}

FORCE_INLINE UnknownPointer::operator rbool () const
{
	return m_pUnknown != NULL;
}

FORCE_INLINE UnknownPointer UnknownPointer::query_cast(ruint id)
{
	return m_pUnknown ? m_pUnknown->QueryInterface(id) : UnknownPointer();
}

template<class I>
inline Interface<I> UnknownPointer::query_cast()
{
	UnknownPointer pointer = m_pUnknown ? m_pUnknown->QueryInterface(GetInterface<I>::ID) : UnknownPointer();
	return Interface<I>(pointer.m_pInterface, pointer.m_pUnknown);
}

template<class I>
inline UnknownPointer::operator Interface<I> ()
{
	return query_cast<I>();
}

FORCE_INLINE UnknownPointer UnknownPointer::query_cast(ruint id) const
{
	return m_pUnknown ? m_pUnknown->QueryInterface(id) : UnknownPointer();
}

template<class I>
inline Interface<I> UnknownPointer::query_cast() const
{
	UnknownPointer pointer = m_pUnknown ? m_pUnknown->QueryInterface(GetInterface<I>::ID) : UnknownPointer();
	return Interface<I>(pointer.m_pInterface, pointer.m_pUnknown);
}

template<class I>
inline UnknownPointer::operator Interface<I> () const
{
	return query_cast<I>();
}

template <class T>
FORCE_INLINE IFactory<T>::Counter::Counter()
	: m_counter(0)
{}

template <class T>
FORCE_INLINE IFactory<T>::Counter::operator UnknownPointer()
{
	return UnknownPointer(NULL, this);
}

template <class T>
inline void IFactory<T>::Counter::AddRef()
{
	m_counter++;
}

template <class T>
inline void IFactory<T>::Counter::Release()
{
	m_counter--;
	if (m_counter == 0)
	{
		if (m_pObject)
		{
			IFactory<T>::destroy(m_pObject);
		}
	}
}

template <class T>
inline UnknownPointer IFactory<T>::Counter::QueryInterface(ruint id)
{
	PTR(void) pInterface = m_pObject->QueryInterface(id);
	return pInterface ? UnknownPointer(pInterface, this) : UnknownPointer();
}

template <class T>
FORCE_INLINE IFactory<T>::Object::Object()
	: T()
{
	create();
}

template <class T>
FORCE_INLINE IFactory<T>::Object::~Object()
{}

template <class T>
FORCE_INLINE LPIUnknown IFactory<T>::Object::GetUnknown()
{
	return &m_counter;
}

template <class T>
FORCE_INLINE void IFactory<T>::Object::create()
{
	m_counter.m_pObject = this;
}

template <class T>
inline UnknownPointer IFactory<T>::create()
{
	return init(new Object());
}

template <class T>
template <typename P1>
inline UnknownPointer IFactory<T>::create(CREF(P1) p1)
{
	return init(new Object(p1));
}

template <class T>
template <typename P1, typename P2>
inline UnknownPointer IFactory<T>::create(CREF(P1) p1, CREF(P2) p2)
{
	return init(new Object(p1, p2));
}

template <class T>
template <typename P1, typename P2, typename P3>
inline UnknownPointer IFactory<T>::create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3)
{
	return init(new Object(p1, p2, p3));
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4>
inline UnknownPointer IFactory<T>::create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4)
{
	return init(new Object(p1, p2, p3, p4));
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4, typename P5>
inline UnknownPointer IFactory<T>::create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5)
{
	return init(new Object(p1, p2, p3, p4, p5));
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline UnknownPointer IFactory<T>::create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6)
{
	return init(new Object(p1, p2, p3, p4, p5, p6));
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
inline UnknownPointer IFactory<T>::create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6, CREF(P7) p7)
{
	return init(new Object(p1, p2, p3, p4, p5, p6, p7));
}

template <class T>
FORCE_INLINE void IFactory<T>::destroy(PTR(T) pObject)
{
	delete pObject;
}

template <class T>
inline UnknownPointer IFactory<T>::init(PTR(Object) pObject)
{
	UnknownPointer uPointer(pObject ? *pObject->GetUnknown() : UnknownPointer());
	LPIInit iInit = uPointer;
	if (iInit)
	{
		if (!iInit->init())
		{
			iInit    = 0;
			uPointer = 0;
			return UnknownPointer();
		}
	}
	return uPointer;
}

CLOSE_RDO_NAMESPACE
