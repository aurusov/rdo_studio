/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdotrace_interface.h
 * author   : Урусов Андрей
 * date     : 28.07.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOTRACE_INTERFACE_H_
#define _RDOTRACE_INTERFACE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <rdointerface.h>
// ===============================================================================

class ITrace
{
public:
	virtual rbool traceable() const      = 0;
	virtual void  setTrace (rbool trace) = 0;

	virtual ruint getTraceID() const                 = 0;
	virtual void  setTraceID(ruint id)               = 0;
	virtual void  setTraceID(ruint id, ruint str_id) = 0;

	virtual REF(std::string) traceId() const = 0;
};
#define DECLARE_ITrace \
	virtual rbool            traceable (           ) const;      \
	virtual void             setTrace  (rbool trace);            \
	virtual ruint            getTraceID(           ) const;      \
	virtual void             setTraceID(ruint id   );            \
	virtual void             setTraceID(ruint id, ruint str_id); \
	virtual REF(std::string) traceId   (           ) const ;


class IPokazTraceValue
{
public:
	virtual tstring traceValue() const = 0;
};
#define DECLARE_IPokazTraceValue \
	virtual tstring traceValue() const;


class IPokazTrace
{
public:
	virtual void tracePokaz() = 0;
};
#define DECLARE_IPokazTrace \
	virtual void tracePokaz();

#endif //! _RDOTRACE_INTERFACE_H_
