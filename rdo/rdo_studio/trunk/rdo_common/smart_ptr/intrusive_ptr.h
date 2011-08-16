/*
 * copyright: (c) RDO-Team, 2010
 * filename : intrusive_ptr.h
 * author   : Урусов Андрей
 * date     : 08.06.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_INTRUSIVE_PTR_H_
#define _RDO_INTRUSIVE_PTR_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
#include "rdo_common/smart_ptr/counter_reference.h"
#include "rdo_common/smart_ptr/interface_ptr.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

template<class T>
class intrusive_ptr
{
template <typename P> friend class intrusive_ptr;
public:
	typedef T                object_type;
	typedef intrusive_ptr<T> this_type;

	 intrusive_ptr();
	 intrusive_ptr(PTR (T)         object);
	 intrusive_ptr(CREF(this_type) sptr  );
	~intrusive_ptr();

	REF(this_type) operator= (CREF(this_type) sptr);

	//! Сравнивает по указателям
	template <class P>
	rbool operator== (CREF(intrusive_ptr<P>) sptr) const;
	template <class P>
	rbool operator!= (CREF(intrusive_ptr<P>) sptr) const;

	//! Сравнивает по значениям
	template<class P>
	rbool compare(CREF(intrusive_ptr<P>) sptr) const;

	operator rbool     () const;
	 PTR(T) operator-> () const;
	 PTR(T) operator-> ();

	template <class P>
	operator intrusive_ptr<P>() const;

	template <class P>
	intrusive_ptr<P> object_parent_cast() const;

	template <class P>
	intrusive_ptr<P> object_static_cast() const;

	template <class P>
	intrusive_ptr<P> object_dynamic_cast() const;

	template <class P>
	interface_ptr<P> interface_cast();

	template <class P>
	interface_ptr<P> interface_dynamic_cast();

	/// @todo переместить в protected
	PTR(T)   get();
	CPTR(T)  get() const;

protected:
	void  addref ();
	void  release();
	rbool owner  () const;

private:
	PTR(T) m_object;

	REF(ruint) counter();
};

//! \class Factory<T>
//! Фабрика, используется для создания и автоматического удаления объектов
//! \tparam T - Тип создаваемого объекта
template <class T>
class Factory
{
friend class intrusive_ptr<T>;
friend class CounterReferenceReal<T>;
public:
	//! Создает объект класса Т
	//! \result Умный указатель на объект
	inline static intrusive_ptr<T> create()
	{
		PTR(T) pObject = new T();
		return intrusive_ptr<T>(pObject);
	}

	//! Создает объект класса Т
	//! \param p1 - параметр конструктора
	//! \result Умный указатель на объект
	template <typename P1>
	static intrusive_ptr<T> create(CREF(P1) p1)
	{
		PTR(T) pObject = new T(p1);
		return intrusive_ptr<T>(pObject);
	}

	//! Создает объект класса Т
	//! \param p1 - параметр конструктора
	//! \param p2 - параметр конструктора
	//! \result Умный указатель на объект
	template <typename P1, typename P2>
	static intrusive_ptr<T> create(CREF(P1) p1, CREF(P2) p2)
	{
		PTR(T) pObject = new T(p1, p2);
		return intrusive_ptr<T>(pObject);
	}

	//! Создает объект класса Т
	//! \param p1 - параметр конструктора
	//! \param p2 - параметр конструктора
	//! \param p3 - параметр конструктора
	//! \result Умный указатель на объект
	template <typename P1, typename P2, typename P3>
	static intrusive_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3)
	{
		PTR(T) pObject = new T(p1, p2, p3);
		return intrusive_ptr<T>(pObject);
	}

	//! Создает объект класса Т
	//! \param p1 - параметр конструктора
	//! \param p2 - параметр конструктора
	//! \param p3 - параметр конструктора
	//! \param p4 - параметр конструктора
	//! \result Умный указатель на объект
	template <typename P1, typename P2, typename P3, typename P4>
	static intrusive_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4)
	{
		PTR(T) pObject = new T(p1, p2, p3, p4);
		return intrusive_ptr<T>(pObject);
	}

	//! Создает объект класса Т
	//! \param p1 - параметр конструктора
	//! \param p2 - параметр конструктора
	//! \param p3 - параметр конструктора
	//! \param p4 - параметр конструктора
	//! \param p5 - параметр конструктора
	//! \result Умный указатель на объект
	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	static intrusive_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5)
	{
		PTR(T) pObject = new T(p1, p2, p3, p4, p5);
		return intrusive_ptr<T>(pObject);
	}

	//! Создает объект класса Т
	//! \param p1 - параметр конструктора
	//! \param p2 - параметр конструктора
	//! \param p3 - параметр конструктора
	//! \param p4 - параметр конструктора
	//! \param p5 - параметр конструктора
	//! \param p6 - параметр конструктора
	//! \result Умный указатель на объект
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static intrusive_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6)
	{
		PTR(T) pObject = new T(p1, p2, p3, p4, p5, p6);
		return intrusive_ptr<T>(pObject);
	}

	//! Создает объект класса Т
	//! \param p1 - параметр конструктора
	//! \param p2 - параметр конструктора
	//! \param p3 - параметр конструктора
	//! \param p4 - параметр конструктора
	//! \param p5 - параметр конструктора
	//! \param p6 - параметр конструктора
	//! \param p7 - параметр конструктора
	//! \result Умный указатель на объект
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static intrusive_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6, CREF(P7) p7)
	{
		PTR(T) pObject = new T(p1, p2, p3, p4, p5, p6, p7);
		return intrusive_ptr<T>(pObject);
	}

	//! Создает объект класса Т
	//! \param p1 - параметр конструктора
	//! \param p2 - параметр конструктора
	//! \param p3 - параметр конструктора
	//! \param p4 - параметр конструктора
	//! \param p5 - параметр конструктора
	//! \param p6 - параметр конструктора
	//! \param p7 - параметр конструктора
	//! \param p8 - параметр конструктора
	//! \result Умный указатель на объект
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	static intrusive_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6, CREF(P7) p7, CREF(P8) p8)
	{
		PTR(T) pObject = new T(p1, p2, p3, p4, p5, p6, p7, p8);
		return intrusive_ptr<T>(pObject);
	}

	//! Создает объект класса Т
	//! \param p1 - параметр конструктора
	//! \param p2 - параметр конструктора
	//! \param p3 - параметр конструктора
	//! \param p4 - параметр конструктора
	//! \param p5 - параметр конструктора
	//! \param p6 - параметр конструктора
	//! \param p7 - параметр конструктора
	//! \param p8 - параметр конструктора
	//! \param p9 - параметр конструктора
	//! \result Умный указатель на объект
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
	static intrusive_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6, CREF(P7) p7, CREF(P8) p8, CREF(P9) p9)
	{
		PTR(T) pObject = new T(p1, p2, p3, p4, p5, p6, p7, p8, p9);
		return intrusive_ptr<T>(pObject);
	}

private:
	static void destroy(PTR(T) object)
	{
		delete object;
	}
};

#define DECLARE_FACTORY(TYPE)    friend class rdo::Factory<TYPE>;
#define DECLARE_POINTER(TYPE)    typedef rdo::intrusive_ptr<TYPE> LP##TYPE;
#define PREDECLARE_POINTER(TYPE) class TYPE; DECLARE_POINTER(TYPE);
#define OBJECT(TYPE)             PREDECLARE_POINTER(TYPE) class TYPE: public rdo::counter_reference

CLOSE_RDO_NAMESPACE

#include "rdo_common/smart_ptr/intrusive_ptr.inl"

#endif //! _RDO_INTRUSIVE_PTR_H_
