/*!
  \copyright (c) RDO-Team, 2012
  \file      notify_i.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      08.01.2012
  \brief
  \indent    4T
*/

#ifndef _LIB_RUNTIME_NOTIFY_I_H_
#define _LIB_RUNTIME_NOTIFY_I_H_

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/ref_counter/counter_reference.h"
// --------------------------------------------------------------------------------

//! Получения уведомлений по подписке
class INotify: public virtual rdo::counter_reference
{
public:
	virtual void notify(std::size_t message, void* pParam) = 0;
};
#define DECLARE_INotify \
	virtual void notify(std::size_t message, void* pParam);

#endif // _LIB_RUNTIME_NOTIFY_I_H_
