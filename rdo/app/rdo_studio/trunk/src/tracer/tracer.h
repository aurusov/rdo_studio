/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer.h
  \author    Захаров Павел
  \date      01.04.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_H_
#define _RDO_STUDIO_TRACER_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
#include "simulator/service/src/rdosimwin.h"
#include "app/rdo_studio/src/tracer/tracer_base.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- Tracer
// --------------------------------------------------------------------------------
namespace rdo
{
	namespace gui
	{
		namespace tracer
		{

			class Tracer: public TracerBase
			{
			protected:
				void proc(RDOThread::RDOMessageInfo& msg);

			public:
				Tracer();
				virtual ~Tracer();

				void setRuntimeMode(const rdo::runtime::RunTimeMode value);
			};

		}
	}
} // namespace rdo::gui::tracer

// --------------------------------------------------------------------------------
extern rdo::gui::tracer::Tracer* g_pTracer;

#endif // _RDO_STUDIO_TRACER_H_
