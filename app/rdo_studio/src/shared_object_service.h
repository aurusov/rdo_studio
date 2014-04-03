#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class RDOKernel;
class MainWindow;

namespace rdo
{
	namespace gui
	{
		namespace tracer
		{
			class Tracer;
		}

		struct ISharedObjectService
		{
			virtual RDOKernel*                 getKernel   () const = 0;
			virtual MainWindow*                getMainWndUI() const = 0;
			virtual rdo::gui::tracer::Tracer*  getTracer   () const = 0;
		};

	} // namespace gui
} // namespace gui
