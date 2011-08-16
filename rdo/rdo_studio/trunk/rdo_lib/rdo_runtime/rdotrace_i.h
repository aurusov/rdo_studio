/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdotrace_i.h
 * @author    Урусов Андрей
 * @date      28.07.2009
 * @brief     Интерфейс ITrace
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_TRACE_I_H_
#define _LIB_RUNTIME_TRACE_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/rdointerface.h"
// --------------------------------------------------------------------------------

/******************************************************************************//**
 * @interface ITrace
 * @brief     Интерфейс ITrace
 *********************************************************************************/
class ITrace
{
public:
	virtual rbool traceable() const      = 0;
	virtual void  setTrace (rbool trace) = 0;

	virtual ruint getTraceID() const                 = 0;
	virtual void  setTraceID(ruint id)               = 0;
	virtual void  setTraceID(ruint id, ruint str_id) = 0;

	virtual REF(tstring) traceId() const = 0;
};

/******************************************************************************//**
 * @def     DECLARE_ITrace
 * @brief   Декларация интерфейса ITrace
 *********************************************************************************/
#define DECLARE_ITrace                                       \
	virtual rbool        traceable (           ) const;      \
	virtual void         setTrace  (rbool trace);            \
	virtual ruint        getTraceID(           ) const;      \
	virtual void         setTraceID(ruint id   );            \
	virtual void         setTraceID(ruint id, ruint str_id); \
	virtual REF(tstring) traceId   (           ) const ;

/******************************************************************************//**
 * @interface IPokazTraceValue
 * @brief     Интерфейс IPokazTraceValue
 *********************************************************************************/
class IPokazTraceValue
{
public:
	virtual tstring traceValue() const = 0;
};

/******************************************************************************//**
 * @def     DECLARE_IPokazTraceValue
 * @brief   Декларация интерфейса IPokazTraceValue
 *********************************************************************************/
#define DECLARE_IPokazTraceValue \
	virtual tstring traceValue() const;

/******************************************************************************//**
 * @interface IPokazTrace
 * @brief     Интерфейс IPokazTrace
 *********************************************************************************/
class IPokazTrace
{
public:
	virtual void tracePokaz() = 0;
};

/******************************************************************************//**
 * @def     DECLARE_IPokazTrace
 * @brief   Декларация интерфейса IPokazTrace
 *********************************************************************************/
#define DECLARE_IPokazTrace \
	virtual void tracePokaz();

#endif // _LIB_RUNTIME_TRACE_I_H_
