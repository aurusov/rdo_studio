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
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/service/rdosimwin.h"
#include "ui/mfc_ctrls/rdotabctrl.h"
#include "app/rdo_studio_mfc/src/dock_wnd.h"
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
class RDOStudioOutput: public RDOStudioDockWnd
{
private:

	class Tab: public RDOTabCtrl {
	protected:
		virtual void changeCurrentItem();
	};

	Tab tab;

	rdoEditCtrl::RDOBuildEdit*      build;
	rdoEditCtrl::RDODebugEdit*      debug;
	rdoTracerLog::RDOTracerLogCtrl* trace;
	rdoEditor::RDOEditorResults*    results;
	rdoEditCtrl::RDOFindEdit*       find;

	CMenu popupMenu;

public:
	typedef  rdo::service::simulation::RDOSyntaxMessage  RDOSyntaxMessage;

	RDOStudioOutput();
	virtual ~RDOStudioOutput();

	void showBuild();
	void showDebug();
	void showTrace();
	void showResults();
	void showFind();

	void clearBuild();
	void clearDebug();
	void clearResults();
	void clearFind();

	const rdoEditCtrl::RDOBuildEdit*      getBuild() const   { return build;   };
	const rdoEditCtrl::RDODebugEdit*      getDebug() const   { return debug;   };
	const rdoTracerLog::RDOTracerLogCtrl* getTrace() const   { return trace;   };
	      rdoEditor::RDOEditorResults*    getResults() const { return results; };
	const rdoEditCtrl::RDOFindEdit*       getFind() const    { return find;    };

	void appendStringToBuild( CREF(tstring) str ) const;
	void appendStringToBuild( CREF(RDOSyntaxMessage) error ) const;
	void appendStringToDebug( CREF(tstring) str ) const;
	void appendStringToResults( CREF(tstring) str ) const;
	void appendStringToFind( CREF(tstring) str, rdoModelObjects::RDOFileType fileType = rdoModelObjects::PAT, int lineNumber = -1, int posInLine = 0 ) const;

	void updateLogConnection() const;
	void updateStyles() const;

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};

#endif // _RDO_STUDIO_MFC_OUTPUT_H_
