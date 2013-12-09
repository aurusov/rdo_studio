/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_event_i.h
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      23.05.2010
  \brief     Интерфейс событий
  \indent    4T
*/

#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/interface/rdointerface.h"
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace runtime {

PREDECLARE_POINTER(RDORuntime)

}} // namespace rdo::runtime

class IEvent
{
public:
	virtual void onMakePlaned(const rdo::runtime::LPRDORuntime& pRuntime) = 0;

	virtual ~IEvent()
	{}
};

INTERFACE_PREDECLARATION(IEvent);
