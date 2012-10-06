/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      output.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_OUTPUT_H_
#define _RDO_STUDIO_MFC_OUTPUT_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qwidget.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/service/rdosimwin.h"
#include "ui/mfc_ctrls/rdotabctrl.h"
// --------------------------------------------------------------------------------

namespace rdoEditor {
	class RDOEditorEdit;
	class RDOEditorResults;
}
namespace rdoEditCtrl {
	class RDOBuildEdit;
	class RDODebugEdit;
	class RDOFindEdit;
}
namespace rdoTracerLog {
	class RDOTracerLogCtrl;
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioOutput
// --------------------------------------------------------------------------------
class RDOStudioOutput: public QWidget
{
public:
	RDOStudioOutput(PTR(QWidget) pParent);
	virtual ~RDOStudioOutput();

private:
	typedef  QWidget  parent_type;

	class Tab: public RDOTabCtrl
	{
	protected:
		virtual void changeCurrentItem();
	};

	CWnd                            m_thisCWnd;
	Tab                             tab;
	rdoEditCtrl::RDOBuildEdit*      build;
	rdoEditCtrl::RDODebugEdit*      debug;
	rdoTracerLog::RDOTracerLogCtrl* trace;
	rdoEditor::RDOEditorResults*    results;
	rdoEditCtrl::RDOFindEdit*       find;
	CMenu                           popupMenu;

	void resizeEvent(PTR(QResizeEvent) event);
};

#endif // _RDO_STUDIO_MFC_OUTPUT_H_
