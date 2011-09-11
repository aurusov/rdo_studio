/*!
  \copyright (c) RDO-Team, 2011
  \file      interface_ptr.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      12.06.2010
  \brief     
  \indent    4T
*/

#ifndef _LIB_COMMON_SMART_PTR_INTERFACE_PTR_H_
#define _LIB_COMMON_SMART_PTR_INTERFACE_PTR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdomacros.h"
#include "utils/rdotypes.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

template <class T>
class Factory;

template<class T>
class intrusive_ptr;

struct NO_V_TABLE ICounterReference
{
	virtual void addref () = 0;
	virtual void release() = 0;
};

typedef PTR(ICounterReference) LPICounterReference;

template <class T>
class CounterReferenceReal: public ICounterReference
{
public:
	void addref ();
	void release();
};

template<class T>
class interface_ptr
{
public:
	typedef interface_ptr<T> this_type;

	interface_ptr ();
	interface_ptr (PTR (T) pInterface, LPICounterReference pCounter);
	interface_ptr (CREF(this_type) sptr);
	~interface_ptr();

	REF(this_type) operator= (CREF(this_type) sptr);

	operator rbool      () const;
	CPTR(T)  operator-> () const;
	 PTR(T)  operator-> ();

private:
	PTR(T)              m_pInterface;
	LPICounterReference m_pCounter;
};

#define DECLARE_OBJECT_INTERFACE(TYPE)    typedef rdo::interface_ptr<TYPE> LP##TYPE;
#define PREDECLARE_OBJECT_INTERFACE(TYPE) struct NO_V_TABLE TYPE; DECLARE_OBJECT_INTERFACE(TYPE);
#define OBJECT_INTERFACE(TYPE)            PREDECLARE_OBJECT_INTERFACE(TYPE) struct TYPE: public rdo::CounterReferenceReal<TYPE>

CLOSE_RDO_NAMESPACE

#include "utils/smart_ptr/interface_ptr.inl"

#endif // _LIB_COMMON_SMART_PTR_INTERFACE_PTR_H_
