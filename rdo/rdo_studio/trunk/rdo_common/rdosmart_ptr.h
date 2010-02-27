/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdosmart_ptr.h
 * author   : Урусов Андрей
 * date     : 12.07.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOSMART_PTR_H_
#define _RDOSMART_PTR_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

template<class T>
class smart_ptr
{
friend class usmart_ptr;
public:
	typedef T            object_type;
	typedef smart_ptr<T> this_type;

	smart_ptr ();
	smart_ptr ( PTR(T)         obj );
	smart_ptr (CREF(this_type) sptr);
	~smart_ptr();

	REF(this_type) operator= (CREF(this_type) sptr);
	PTR(T)   get();
	CPTR(T)  get() const;

	operator rbool    () const;
	PTR(T) operator-> ();

	rbool owner() const;

protected:
	void addref ();
	void release();

private:
	PTR(ruint) m_counter;
	PTR(T)     m_object;

	rbool       inited           () const;
	CREF(ruint) counter          () const;
	REF(ruint)  counter          ();
	void        allocateCounter  ();
	void        deallocateCounter();
	void        clear            ();
};

CLOSE_RDO_NAMESPACE

#include "rdo_common/rdosmart_ptr.inl"

#endif //! _RDOSMART_PTR_H_
