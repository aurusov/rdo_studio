#ifndef RDOTRACEREXCEPTION_H
#define RDOTRACEREXCEPTION_H
#pragma once

// ----------------------------------------------------------------------------
// ---------- RDOTracerException
// ----------------------------------------------------------------------------
class RDOTracerException
{
public:
	std::string message;
	virtual std::string getMessage() { return message; };
	RDOTracerException( const char* _message ) : message( _message ) { };
};

#endif // RDOTRACEREXCEPTION_H