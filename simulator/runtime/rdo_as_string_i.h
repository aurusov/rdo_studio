/*!
  \copyright (c) RDO-Team, 2014
  \file      rdo_as_string_i.h
  \author    Богачев Павел <bogachev-pa@gmail.com>
  \date      18.04.2014
  \brief     Интерфейс для получения объекта как строки
  \details
  \indent    4T
*/

#ifndef _LIB_RUNTIME_AS_STRING_I_H
#define _LIB_RUNTIME_AS_STRING_I_H

// ----------------------------------------------------------------------- INCLUDES
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \interface IAsString
  \brief     Предоставляет метод asString()
*/
PREDECLARE_OBJECT_INTERFACE(IAsString)
struct IAsString
{
DECLARE_FACTORY(IAsString);
public:
	virtual std::string asString() const = 0;

protected:
	IAsString()          {}
	virtual ~IAsString() {}
};

#define DECLARE_IAsString \
	std::string asString() const;

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_AS_STRING_I_H
