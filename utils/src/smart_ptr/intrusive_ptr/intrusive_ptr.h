#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/ref_counter/counter_reference.h"
#include "utils/src/smart_ptr/interface_ptr/interface_ptr.h"
#include "utils/src/smart_ptr/ref_counter/ref_counter_i.h"
// --------------------------------------------------------------------------------

namespace rdo {

template<class T>
class intrusive_ptr
{
template <typename P> friend class intrusive_ptr;
template <class W>    friend class smart_ptr_wrapper;
public:
	typedef T                object_type;
	typedef intrusive_ptr<T> this_type;

	intrusive_ptr();
	intrusive_ptr(T* object);
	intrusive_ptr(const this_type& sptr);
	template <class P>
	intrusive_ptr(const interface_ptr<P>& pInterface);
	~intrusive_ptr();

	this_type& operator=(const this_type& sptr);

	template <class P>
	bool operator==(const intrusive_ptr<P>& sptr) const;
	template <class P>
	bool operator!=(const intrusive_ptr<P>& sptr) const;

	template<class P>
	bool compare(const intrusive_ptr<P>& sptr) const;

	operator bool() const;
	T* operator->() const;
	T* operator->();
	const T& operator*() const;
	T& operator*();

	template <class P>
	operator intrusive_ptr<P>() const;

	template <class P>
	intrusive_ptr<P> object_parent_cast() const;

	template <class P>
	intrusive_ptr<P> object_static_cast() const;

	template <class P>
	intrusive_ptr<P> object_dynamic_cast() const;

	template <class P>
	interface_ptr<P> interface_cast() const;

	template <class P>
	interface_ptr<P> interface_dynamic_cast() const;

	// TODO переместить в protected
	T* get();
	const T* get() const;

	bool owner() const;

protected:
	void addref();
	void release();

private:
	T* m_object;

	std::size_t& counter();
};

#define DECLARE_POINTER(TYPE)    typedef rdo::intrusive_ptr<TYPE> LP##TYPE;
#define PREDECLARE_POINTER(TYPE) class TYPE; DECLARE_POINTER(TYPE);

} // namespace rdo

#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr-inl.h"
