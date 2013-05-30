/*!
  \copyright (c) RDO-Team, 2012
  \file      /app/rdo_check_results/controller_console_options.inl
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      21.03.2011
  \brief     Консольная версия RDO
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
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
