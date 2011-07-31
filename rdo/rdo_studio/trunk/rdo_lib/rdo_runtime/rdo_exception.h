/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_exception.h
 * @authors   Урусов Андрей
 * @date      22.06.2008
 * @brief     Определение исключений
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_EXCEPTION_H_
#define _LIB_RUNTIME_EXCEPTION_H_

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOException
// ********************************************************************************
class RDOException
{
public:
	RDOException(CREF(tstring) message): m_message(message) {}
	virtual tstring getType() const = 0;
	
	CREF(tstring) message() const { return m_message; }

private:
	tstring m_message;
};

class RDORuntimeException: public RDOException
{
public:
	RDORuntimeException( CREF(tstring) message ): RDOException( message ) {}
	virtual tstring getType() const { return "RDO Runtime Error"; }
};

class RDOTypeException: public RDORuntimeException
{
public:
	RDOTypeException( CREF(tstring) message = "" ): RDORuntimeException( message ) {}
	virtual tstring getType() const { return "RDOType Error"; }
};

class RDOValueException: public RDORuntimeException
{
public:
	RDOValueException( CREF(tstring) message = "" ): RDORuntimeException( message ) {}
	virtual tstring getType() const { return "RDOValue Error"; }
};

class RDOInternalException: public RDORuntimeException
{
public:
	RDOInternalException( CREF(tstring) message ): RDORuntimeException( message ) {}
	virtual tstring getType() const { return "RDO Internal Error"; }
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_EXCEPTION_H_
