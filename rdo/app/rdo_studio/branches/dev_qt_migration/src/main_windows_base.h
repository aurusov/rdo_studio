/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      main_windows_base.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      01.09.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MAIN_WINDOWS_BASE_H_
#define _RDO_STUDIO_MAIN_WINDOWS_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoreditstyle.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorresultsstyle.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdobuildeditstyle.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdobaseeditstyle.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdofindeditstyle.h"
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdotracerlogstyle.h"
#include "app/rdo_studio_mfc/src/style.h"
#include "app/rdo_studio_mfc/src/chart/view_style.h"
#include "app/rdo_studio_mfc/src/frame/style.h"
#include "app/rdo_studio_mfc/src/dock/dock_build.h"
#include "app/rdo_studio_mfc/src/dock/dock_debug.h"
#include "app/rdo_studio_mfc/src/dock/dock_trace.h"
#include "app/rdo_studio_mfc/src/dock/dock_results.h"
#include "app/rdo_studio_mfc/src/dock/dock_find.h"
#include "app/rdo_studio_mfc/src/dock/dock_trace_tree.h"
#include "app/rdo_studio_mfc/src/dock/dock_frame.h"
#include "app/rdo_studio_mfc/src/dock/dock_process.h"
// --------------------------------------------------------------------------------

class MainWindowBase
{
public:
	rdoEditor::RDOEditorEditStyle    style_editor;
	rdoEditCtrl::RDOBuildEditStyle   style_build;
	rdoEditCtrl::RDOBaseEditStyle    style_debug;
	rdoTracerLog::RDOTracerLogStyle  style_trace;
	rdoEditor::RDOEditorResultsStyle style_results;
	rdoEditCtrl::RDOFindEditStyle    style_find;
	RDOStudioFrameStyle              style_frame;
	RDOStudioChartViewStyle          style_chart;

	rbool isMDIMaximazed() const { return true; }

	REF(DockBuild)      getDockBuild    () { ASSERT(m_pDockBuild    ); return *m_pDockBuild;     }
	REF(DockDebug)      getDockDebug    () { ASSERT(m_pDockDebug    ); return *m_pDockDebug;     }
	REF(DockTrace)      getDockTrace    () { ASSERT(m_pDockTrace    ); return *m_pDockTrace;     }
	REF(DockResults)    getDockResults  () { ASSERT(m_pDockResults  ); return *m_pDockResults;   }
	REF(DockFind)       getDockFind     () { ASSERT(m_pDockFind     ); return *m_pDockFind;      }
	REF(DockTraceTree)  getDockTraceTree() { ASSERT(m_pDockTraceTree); return *m_pDockTraceTree; }
	REF(DockFrame)      getDockFrame    () { ASSERT(m_pDockFrame    ); return *m_pDockFrame;     }
	REF(DockProcess)    getDockProcess  () { ASSERT(m_pDockProcess  ); return *m_pDockProcess;   }

	static rbool is_close_mode() { return close_mode; }

	virtual void updateAllStyles() = 0;

	virtual double getSpeed() const = 0;

	virtual void beginProgress   (const int lower = 0, const int upper = 100, const int step = 1 ) = 0;
	virtual void getProgressRange(int& lower, int& upper ) const = 0;
	virtual void setProgress     (const int pos)                 = 0;
	virtual int  getProgress     () const                        = 0;
	virtual void offsetProgress  (const int offset)              = 0;
	virtual void stepProgress    ()                              = 0;
	virtual void endProgress     ()                              = 0;

	virtual void update_start() = 0;
	virtual void update_stop () = 0;

	virtual void addSubWindow              (QWidget* pWidget) = 0;
	virtual void activateSubWindow         (QWidget* pWidget) = 0;
	virtual void connectOnActivateSubWindow(QObject* pObject) = 0;

protected:
	// ������������ ��� �������� ������. �������� ������.
	static rbool close_mode;

	PTR(DockBuild)      m_pDockBuild;
	PTR(DockDebug)      m_pDockDebug;
	PTR(DockTrace)      m_pDockTrace;
	PTR(DockResults)    m_pDockResults;
	PTR(DockFind)       m_pDockFind;
	PTR(DockTraceTree)  m_pDockTraceTree;
	PTR(DockFrame)      m_pDockFrame;
	PTR(DockProcess)    m_pDockProcess;
};

#endif // _RDO_STUDIO_MAIN_WINDOWS_BASE_H_