#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOException
{
public:
    RDOException(const std::string& message);
    virtual std::string getType() const = 0;

    const std::string& message() const;

private:
    std::string m_message;
};

class RDORuntimeException: public RDOException
{
public:
    RDORuntimeException(const std::string& message);
    virtual std::string getType() const;
};

class RDOTypeException: public RDORuntimeException
{
public:
    RDOTypeException(const std::string& message = "");
    virtual std::string getType() const;
};

class RDOValueException: public RDORuntimeException
{
public:
    RDOValueException(const std::string& message = "");
    virtual std::string getType() const;
};

class RDOUndefinedException: public RDORuntimeException
{
public:
    RDOUndefinedException();
    virtual std::string getType() const;
};

class RDOInternalException: public RDORuntimeException
{
public:
    RDOInternalException(const std::string& message);
    virtual std::string getType() const;
};

CLOSE_RDO_RUNTIME_NAMESPACE
