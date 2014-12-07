#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
#include "simulator/service/src/simulator.h"
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
