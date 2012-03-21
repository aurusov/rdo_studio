/*!
  \copyright (c) RDO-Team, 2012
  \file      controller_console_options.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      21.03.2011
  \brief     Консольная версия RDO
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_console/test/controller_console_options.h"
// --------------------------------------------------------------------------------

template<class T>
rbool RDOControllerConsoleOptions::getParameter(REF(T) name, CREF(tstring) parameter)
{
	if(m_variables.count(parameter))
	{
		///! @todo added control input data
		name = m_variables[parameter].as<T>();
		return true;
	}
	return false;
}
