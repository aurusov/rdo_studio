/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdosmart_ptr_unknow_factory.h
 * author   : Урусов Андрей
 * date     : 25.02.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOSMART_PTR_UNKNOW_FACTORY_H_
#define _RDOSMART_PTR_UNKNOW_FACTORY_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdosmart_ptr_unknow.h"
#include "rdo_common/rdodebug.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

class UFactory
{
public:
	template <class T>
	static usmart_ptr create()
	{
		return usmart_ptr(new T());
	}
	template <class T, typename P1>
	static usmart_ptr create(CREF(P1) p1)
	{
		return usmart_ptr(new T(p1));
	}
	template <class T, typename P1, typename P2>
	static usmart_ptr create(CREF(P1) p1, CREF(P2) p2)
	{
		return usmart_ptr(new T(p1, p2));
	}
	template <class T, typename P1, typename P2, typename P3>
	static usmart_ptr create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3)
	{
		return usmart_ptr(new T(p1, p2, p3));
	}
	template <class T, typename P1, typename P2, typename P3, typename P4>
	static usmart_ptr create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4)
	{
		return usmart_ptr(new T(p1, p2, p3, p4));
	}
	template <class T, typename P1, typename P2, typename P3, typename P4, typename P5>
	static usmart_ptr create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5)
	{
		return usmart_ptr(new T(p1, p2, p3, p4, p5));
	}
	template <class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static usmart_ptr create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6)
	{
		return usmart_ptr(new T(p1, p2, p3, p4, p5, p6));
	}
	template <class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static usmart_ptr create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6, CREF(P7) p7)
	{
		return usmart_ptr(new T(p1, p2, p3, p4, p5, p6, p7));
	}

private:
	static void destroy(usmart_ptr object)
	{
		delete object.get<void>();
	}
};

CLOSE_RDO_NAMESPACE

#define DEFINE_UFACTORY  friend class rdo::UFactory; friend class rdo::usmart_ptr;

#endif //! _RDOSMART_PTR_UNKNOW_FACTORY_H_
