/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdosmart_ptr_wrapper.h
 * author   : Урусов Андрей
 * date     : 26.02.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOSMART_PTR_WRAPPER_H_
#define _RDOSMART_PTR_WRAPPER_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

class ISmartPtrWrapper
{
public:
	virtual void      destroy    () = 0;
	virtual PTR(void) getSmartPtr() = 0;
};
typedef PTR(ISmartPtrWrapper) LPISmartPtrWrapper;

#define DECLARE_ISmartPtrWrapper     \
public:                              \
	virtual void      destroy    (); \
	virtual PTR(void) getSmartPtr();

template<class T>
class smart_ptr_wrapper: public ISmartPtrWrapper
{
public:
	smart_ptr_wrapper(CREF(intrusive_ptr<T>) intrusive_ptr)
		: m_intrusive_ptr(intrusive_ptr)
	{}
	~smart_ptr_wrapper()
	{}
	void destroy()
	{
		delete this;
	}
	PTR(void) getSmartPtr()
	{
		return &m_intrusive_ptr;
	}

private:
	intrusive_ptr<T> m_intrusive_ptr;
};

CLOSE_RDO_NAMESPACE

#endif //! _RDOSMART_PTR_WRAPPER_H_
