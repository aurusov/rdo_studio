/*!
  \copyright (c) RDO-Team, 2011
  \file      rdotrace_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.07.2009
  \brief     Интерфейс ITrace
  \indent    4T
*/

#ifndef _LIB_RUNTIME_TRACE_I_H_
#define _LIB_RUNTIME_TRACE_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

/*!
  \interface ITrace
  \brief     Интерфейс ITrace
*/
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

/*!
  \interface IResultTraceValue
  \brief     Интерфейс IResultTraceValue
*/
class IResultTraceValue: public virtual rdo::counter_reference
{
public:
	virtual std::string traceValue() const = 0;
};

#define DECLARE_IResultTraceValue \
	virtual std::string traceValue() const;

/*!
  \interface IResultTrace
  \brief     Интерфейс IResultTrace
*/
class IResultTrace: public virtual rdo::counter_reference
{
public:
	virtual void traceResult() = 0;
};
DECLARE_POINTER(IResultTrace);

#define DECLARE_IResultTrace \
	virtual void traceResult();

#endif // _LIB_RUNTIME_TRACE_I_H_
