#ifndef RDO_EXCEPTION_H
#define RDO_EXCEPTION_H

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOException
// ----------------------------------------------------------------------------
class RDOException
{
public:
	RDOException( const std::string& message ): m_message( message ) {}
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

} // namespace rdoRuntime

#endif // RDO_EXCEPTION_H
