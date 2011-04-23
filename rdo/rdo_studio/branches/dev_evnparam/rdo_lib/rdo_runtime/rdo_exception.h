/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_exception.h
 * author   : Урусов Андрей
 * date     : 22.06.2008
 * bref     : Определение исключений
 * indent   : 4T
 */

#ifndef _RDO_EXCEPTION_H_
#define _RDO_EXCEPTION_H_

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOException
// ----------------------------------------------------------------------------
class RDOException
{
public:
	RDOException(const std::string& message): m_message(message) {}
	virtual std::string getType() const = 0;
	
	const std::string& message() const { return m_message; }

private:
	std::string m_message;
};

class RDORuntimeException: public RDOException
{
public:
	RDORuntimeException( const std::string& message ): RDOException( message ) {}
	virtual std::string getType() const { return "RDO Runtime Error"; }
};

class RDOTypeException: public RDORuntimeException
{
public:
	RDOTypeException( const std::string& message = "" ): RDORuntimeException( message ) {}
	virtual std::string getType() const { return "RDOType Error"; }
};

class RDOValueException: public RDORuntimeException
{
public:
	RDOValueException( const std::string& message = "" ): RDORuntimeException( message ) {}
	virtual std::string getType() const { return "RDOValue Error"; }
};

class RDOInternalException: public RDORuntimeException
{
public:
	RDOInternalException( const std::string& message ): RDORuntimeException( message ) {}
	virtual std::string getType() const { return "RDO Internal Error"; }
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_EXCEPTION_H_
