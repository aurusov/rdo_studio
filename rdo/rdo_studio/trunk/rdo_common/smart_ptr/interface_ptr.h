/*
 * copyright: (c) RDO-Team, 2011
 * file     : interface_ptr.h
 * author   : Урусов Андрей
 * date     : 12.06.2010
 * brief    : 
 * indent   : 4T
 */

#ifndef _RDO_INTERFACE_PTR_H_
#define _RDO_INTERFACE_PTR_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

struct __declspec(novtable) ICounterReference
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
#define PREDECLARE_OBJECT_INTERFACE(TYPE) struct __declspec(novtable) TYPE; DECLARE_OBJECT_INTERFACE(TYPE);
#define OBJECT_INTERFACE(TYPE)            PREDECLARE_OBJECT_INTERFACE(TYPE) struct TYPE: public rdo::CounterReferenceReal<TYPE>

CLOSE_RDO_NAMESPACE

#include "rdo_common/smart_ptr/interface_ptr.inl"

#endif //! _RDO_INTERFACE_PTR_H_
