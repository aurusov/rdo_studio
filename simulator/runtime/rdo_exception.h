/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_exception.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      22.06.2008
  \brief     Определение исключений
  \indent    4T
*/

#ifndef _LIB_RUNTIME_EXCEPTION_H_
#define _LIB_RUNTIME_EXCEPTION_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class     RDOException
  \brief     Ошибка
*/
class RDOException
{
public:
	RDOException(const std::string& message);
	virtual std::string getType() const = 0;
	
	const std::string& message() const;

private:
	std::string m_message;
};

/*!
  \class     RDORuntimeException
  \brief     Ошибка времени выполнения
*/
class RDORuntimeException: public RDOException
{
public:
	RDORuntimeException(const std::string& message);
	virtual std::string getType() const;
};

/*!
  \class     RDOTypeException
  \brief     Ошибка в типе
*/
class RDOTypeException: public RDORuntimeException
{
public:
	RDOTypeException(const std::string& message = "");
	virtual std::string getType() const;
};

/*!
  \class     RDOValueException
  \brief     Ошибка в значении
*/
class RDOValueException: public RDORuntimeException
{
public:
	RDOValueException(const std::string& message = "");
	virtual std::string getType() const;
};

/*!
  \class     RDOValueException
  \brief     Ошибка - значение не определено
*/
class RDOUndefinedException: public RDORuntimeException
{
public:
	RDOUndefinedException();
	virtual std::string getType() const;
};

/*!
  \class     RDOInternalException
  \brief     Внутренняя ошибка
*/
class RDOInternalException: public RDORuntimeException
{
public:
	RDOInternalException(const std::string& message);
	virtual std::string getType() const;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_exception-inl.h"

#endif // _LIB_RUNTIME_EXCEPTION_H_
