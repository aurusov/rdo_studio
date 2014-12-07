#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class ITrace: public virtual rdo::counter_reference
{
public:
	virtual bool traceable() const = 0;
	virtual void setTrace (bool trace) = 0;

	virtual std::size_t getTraceID() const = 0;
	virtual void setTraceID(std::size_t id) = 0;
	virtual void setTraceID(std::size_t id, std::size_t str_id) = 0;

	virtual std::string& traceId() const = 0;
};
DECLARE_POINTER(ITrace)

#define DECLARE_ITrace                                           \
	virtual bool traceable() const;                              \
	virtual void setTrace(bool trace);                           \
	virtual std::size_t getTraceID() const;                      \
	virtual void setTraceID(std::size_t id);                     \
	virtual void setTraceID(std::size_t id, std::size_t str_id); \
	virtual std::string& traceId() const ;

class IResultTraceValue: public virtual rdo::counter_reference
{
public:
	virtual std::string traceValue() const = 0;
};

#define DECLARE_IResultTraceValue \
	virtual std::string traceValue() const;

class IResultTrace: public virtual rdo::counter_reference
{
public:
	virtual void traceResult() = 0;
};
DECLARE_POINTER(IResultTrace);

#define DECLARE_IResultTrace \
	virtual void traceResult();
