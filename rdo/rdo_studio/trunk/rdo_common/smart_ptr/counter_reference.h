/*
 * copyright: (c) RDO-Team, 2011
 * file     : counter_reference.h
 * author   : Урусов Андрей
 * date     : 12.06.2010
 * brief    : 
 * indent   : 4T
 */

#ifndef _RDO_COUNTER_REFERENCE_H_
#define _RDO_COUNTER_REFERENCE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_NAMESPACE

class counter_reference
{
template<class T> friend class intrusive_ptr;
template<class T> friend class CounterReferenceReal;
public:
	//! Инициализиреут счетчик нулём
	counter_reference();

private:
	ruint m_intrusive_counter;
};

CLOSE_RDO_NAMESPACE

#include "rdo_common/smart_ptr/counter_reference.inl"

#endif //! _RDO_COUNTER_REFERENCE_H_
