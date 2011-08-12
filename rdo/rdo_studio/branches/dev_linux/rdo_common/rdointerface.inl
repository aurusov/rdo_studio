/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdointerface.inl
 * author   : Урусов Андрей
 * date     : 12.08.2011
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_NAMESPACE

template<class I>
inline Interface<I>::Interface()
	: UnknownPointer(NULL, NULL)
{}

template<class I>
inline Interface<I>::Interface(LPIGetUnknown get_smt_ptr)
	: UnknownPointer(get_smt_ptr ? get_smt_ptr->GetUnknown()->QueryInterface(GetInterface<I>::ID) : UnknownPointer())
{}

template<class I>
inline Interface<I>::Interface(PTR(void) aInterface, LPIUnknown smt_ptr)
	: UnknownPointer(aInterface, smt_ptr)
{}

template<class I>
inline Interface<I>::Interface(CREF(this_type) pInterface)
	: UnknownPointer(pInterface)
{}

template<class I>
inline REF(typename Interface<I>::this_type) Interface<I>::operator= (CREF(this_type) pointer)
{
	parent_type::operator= (pointer);
	return *this;
}

template<class I>
inline Interface<I>::operator rbool() const
{
	return m_interface != NULL;
}

template<class I>
inline PTR(I) Interface<I>::get()
{
	return static_cast<PTR(I)>(m_interface);
}

template<class I>
inline CPTR(I) Interface<I>::get() const
{
	return static_cast<PTR(I)>(m_interface);
}

template<class I>
inline PTR(I) Interface<I>::operator-> ()
{
	return get();
}

template<class I>
inline CPTR(I) Interface<I>::operator-> () const
{
	return get();
}

inline UnknownPointer::UnknownPointer()
	: m_interface(NULL)
	, m_smt_ptr  (NULL)
{}

inline UnknownPointer::UnknownPointer(REF(IUnknown) smt_ptr)
	: m_interface(NULL    )
	, m_smt_ptr  (&smt_ptr)
{
	if (m_smt_ptr)
		m_smt_ptr->AddRef();
}

inline UnknownPointer::UnknownPointer(LPIGetUnknown get_smt_ptr)
	: m_interface(NULL)
{
	m_smt_ptr = get_smt_ptr ? get_smt_ptr->GetUnknown() : NULL;
	if (m_smt_ptr)
		m_smt_ptr->AddRef();
}

inline UnknownPointer::UnknownPointer(PTR(void) aInterface, LPIUnknown smt_ptr)
	: m_interface(aInterface)
	, m_smt_ptr  (smt_ptr   )
{
	if (m_smt_ptr)
		m_smt_ptr->AddRef();
}

inline UnknownPointer::UnknownPointer(CREF(UnknownPointer) unknowPointer)
	: m_interface(unknowPointer.m_interface)
	, m_smt_ptr  (unknowPointer.m_smt_ptr  )
{
	if (m_smt_ptr)
		m_smt_ptr->AddRef();
}

inline UnknownPointer::~UnknownPointer()
{
	if (m_smt_ptr)
		m_smt_ptr->Release();
}

inline rbool UnknownPointer::operator== (CREF(UnknownPointer) pointer) const
{
	return m_smt_ptr == pointer.m_smt_ptr;
}

inline REF(UnknownPointer) UnknownPointer::operator= (CREF(UnknownPointer) pointer)
{
	if (m_smt_ptr)
		m_smt_ptr->Release();

	m_interface = pointer.m_interface;
	m_smt_ptr   = pointer.m_smt_ptr;
	if (m_smt_ptr)
		m_smt_ptr->AddRef();

	return *this;
}

inline UnknownPointer::operator rbool () const
{
	return m_smt_ptr != NULL;
}

inline UnknownPointer UnknownPointer::query_cast(ruint id)
{
	return m_smt_ptr ? m_smt_ptr->QueryInterface(id) : UnknownPointer();
}

template<class I>
inline Interface<I> UnknownPointer::query_cast()
{
	UnknownPointer pointer = m_smt_ptr ? m_smt_ptr->QueryInterface(GetInterface<I>::ID) : UnknownPointer();
	return Interface<I>(pointer.m_interface, pointer.m_smt_ptr);
}

template<class I>
inline UnknownPointer::operator Interface<I> ()
{
	return query_cast<I>();
}

inline UnknownPointer UnknownPointer::query_cast(ruint id) const
{
	return m_smt_ptr ? m_smt_ptr->QueryInterface(id) : UnknownPointer();
}

template<class I>
inline Interface<I> UnknownPointer::query_cast() const
{
	UnknownPointer pointer = m_smt_ptr ? m_smt_ptr->QueryInterface(GetInterface<I>::ID) : UnknownPointer();
	return Interface<I>(pointer.m_interface, pointer.m_smt_ptr);
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
		if (m_object)
		{
			IFactory<T>::destroy(m_object);
		}
	}
}

template <class T>
inline UnknownPointer IFactory<T>::Counter::QueryInterface(ruint id)
{
	PTR(void) pInterface = m_object->QueryInterface(id);
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
	m_counter.m_object = this;
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
inline void IFactory<T>::destroy(PTR(T) object)
{
	delete object;
}

template <class T>
inline UnknownPointer IFactory<T>::init(PTR(Object) object)
{
	UnknownPointer uPointer(object ? *object->GetUnknown() : UnknownPointer());
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
