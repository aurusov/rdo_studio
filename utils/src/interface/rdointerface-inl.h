/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdointerface-inl.h
 * author   : Урусов Андрей
 * date     : 12.08.2011
 * bref     :
 * indent   : 4T
 */

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/debug/rdodebug.h"
// --------------------------------------------------------------------------------

namespace rdo {

template<class I>
inline Interface<I>::Interface()
	: UnknownPointer(NULL, NULL)
{}

template<class I>
inline Interface<I>::Interface(LPIGetUnknown pGetUnknown)
	: UnknownPointer(pGetUnknown ? pGetUnknown->GetUnknown()->QueryInterface(GetInterface<I>::ID) : UnknownPointer())
{}

template<class I>
inline Interface<I>::Interface(void* pInterface, LPIUnknown pUnknown)
	: UnknownPointer(pInterface, pUnknown)
{}

template<class I>
inline Interface<I>::Interface(const this_type& aInterface)
	: UnknownPointer(aInterface)
{}

template<class I>
inline typename Interface<I>::this_type& Interface<I>::operator= (const this_type& aInterface)
{
	parent_type::operator= (aInterface);
	return *this;
}

template<class I>
inline Interface<I>::operator bool() const
{
	return m_pInterface != NULL;
}

template<class I>
inline I* Interface<I>::get()
{
	return static_cast<I*>(m_pInterface);
}

template<class I>
inline const I* Interface<I>::get() const
{
	return static_cast<I*>(m_pInterface);
}

template<class I>
inline I* Interface<I>::operator->()
{
	return get();
}

template<class I>
inline const I* Interface<I>::operator->() const
{
	return get();
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

inline UnknownPointer UnknownPointer::query_cast(std::size_t id) const
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
inline IFactory<T>::Counter::Counter()
	: m_counter(0)
{}

template <class T>
inline IFactory<T>::Counter::operator UnknownPointer()
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
inline UnknownPointer IFactory<T>::Counter::QueryInterface(std::size_t id)
{
	void* pInterface = m_pObject->QueryInterface(id);
	return pInterface ? UnknownPointer(pInterface, this) : UnknownPointer();
}

template <class T>
inline IFactory<T>::Object::Object()
	: T()
{
	create();
}

template <class T>
inline IFactory<T>::Object::~Object()
{}

template <class T>
inline LPIUnknown IFactory<T>::Object::GetUnknown()
{
	return &m_counter;
}

template <class T>
inline void IFactory<T>::Object::create()
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
inline UnknownPointer IFactory<T>::create(const P1& p1)
{
	return init(new Object(p1));
}

template <class T>
template <typename P1, typename P2>
inline UnknownPointer IFactory<T>::create(const P1& p1, const P2& p2)
{
	return init(new Object(p1, p2));
}

template <class T>
template <typename P1, typename P2, typename P3>
inline UnknownPointer IFactory<T>::create(const P1& p1, const P2& p2, const P3& p3)
{
	return init(new Object(p1, p2, p3));
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4>
inline UnknownPointer IFactory<T>::create(const P1& p1, const P2& p2, const P3& p3, const P4& p4)
{
	return init(new Object(p1, p2, p3, p4));
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4, typename P5>
inline UnknownPointer IFactory<T>::create(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5)
{
	return init(new Object(p1, p2, p3, p4, p5));
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline UnknownPointer IFactory<T>::create(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6)
{
	return init(new Object(p1, p2, p3, p4, p5, p6));
}

template <class T>
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
inline UnknownPointer IFactory<T>::create(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7)
{
	return init(new Object(p1, p2, p3, p4, p5, p6, p7));
}

template <class T>
inline void IFactory<T>::destroy(T* pObject)
{
	delete pObject;
}

template <class T>
inline UnknownPointer IFactory<T>::init(Object* pObject)
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

} // namespace rdo
