/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracer.h
  \author    Захаров Павел
  \date      01.04.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_TRACER_RDOTRACER_H_
#define _RDO_STUDIO_MFC_TRACER_RDOTRACER_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerbase.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOTracer
// --------------------------------------------------------------------------------
namespace rdoTracer {

class RDOTracer: public RDOTracerBase
{
protected:
	void proc( RDOThread::RDOMessageInfo& msg );

public:
	RDOTracer();
	virtual ~RDOTracer();

	void setRuntimeMode( const rdo::runtime::RunTimeMode value );
};

}; // namespace rdoTracer

// --------------------------------------------------------------------------------
extern rdoTracer::RDOTracer* tracer;

#endif // _RDO_STUDIO_MFC_TRACER_RDOTRACER_H_
