/*!
  \copyright (c) RDO-Team, 2011
  \file      counter_reference.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      12.06.2010
  \brief
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/ref_counter/counter_reference.h"
// --------------------------------------------------------------------------------

namespace rdo {

counter_reference::counter_reference()
	: m_intrusive_counter(0)
{}

counter_reference::~counter_reference()
{}

} // namespace rdo
