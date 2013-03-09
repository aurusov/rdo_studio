/*!
  \copyright (c) RDO-Team, 2011
  \file      rdotrace.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      06.08.2011
  \brief     Трассировщик
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdotrace_i.h"
#include "simulator/runtime/rdo_runtime_interface_registrator.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOEndL
// --------------------------------------------------------------------------------
inline void RDOEndL::onEndl()
{}

inline rdo::ostream &operator << (rdo::ostream &stream, RDOEndL& rdoEndL)
{
	rdoEndL.onEndl();
	return stream;
}

// --------------------------------------------------------------------------------
// -------------------- RDOTrace
// --------------------------------------------------------------------------------
inline RDOTrace::RDOTrace()
	: m_isNullTracer    (true )
	, m_canWriteToStream(false)
{}

inline RDOTrace::~RDOTrace()
{}

inline rbool RDOTrace::canTrace() const
{
	return !isNull() && canWrite();
}

inline void  RDOTrace::startWriting()
{
	m_canWriteToStream = true;
}

inline void  RDOTrace::stopWriting()
{
	m_canWriteToStream = false;
}

inline rbool RDOTrace::canWrite() const
{
	return m_canWriteToStream;
}

inline rbool RDOTrace::isNull() const
{
	return m_isNullTracer;
}

inline REF(rdo::ostream) RDOTrace::getOStream()
{
	return m_emptyOut;
}

inline REF(RDOEndL) RDOTrace::getEOL()
{
	return m_emptyEndL;
}

// --------------------------------------------------------------------------------
// -------------------- RDOTraceableObject
// --------------------------------------------------------------------------------
inline RDOTraceableObject::RDOTraceableObject(rbool trace, ruint id, tstring str)
	: m_trace (trace)
	, m_id    (id   )
	, m_str_id(str  )
{}

inline RDOTraceableObject::~RDOTraceableObject()
{}

inline rbool RDOTraceableObject::traceable() const
{
	return m_trace;
}

inline void RDOTraceableObject::setTrace(rbool trace)
{
	m_trace = trace;
}

inline ruint RDOTraceableObject::getTraceID() const
{
	return m_id;
}

inline void RDOTraceableObject::setTraceID(ruint id)
{
	setTraceID(id, id);
}

inline void RDOTraceableObject::setTraceID(ruint id, ruint str_id)
{
	m_id     = id;
	m_str_id = rdo::toString(str_id);
}

inline REF(tstring) RDOTraceableObject::traceId() const
{
	if (m_str_id.empty())
	{
		m_str_id = rdo::toString(m_id);
	}
	return m_str_id;
}

CLOSE_RDO_RUNTIME_NAMESPACE
