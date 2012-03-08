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

	void appendStringToBuild( const std::string& str ) const;
	void appendStringToBuild( rdoSimulator::RDOSyntaxError::ErrorCode error_code, const std::string& str, const rdoModelObjects::RDOFileType fileType = rdoModelObjects::PAT, const int lineNumber = -1, const int posInLine = 0, const bool warning = true ) const;
	void appendStringToDebug( const std::string& str ) const;
	void appendStringToResults( const std::string& str ) const;
	void appendStringToFind( const std::string& str, const rdoModelObjects::RDOFileType fileType = rdoModelObjects::PAT, const int lineNumber = -1, const int posInLine = 0 ) const;

	void updateLogConnection() const;
	void updateStyles() const;

public:
	//{{AFX_VIRTUAL(RDOStudioOutput)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioOutput)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _RDO_STUDIO_MFC_OUTPUT_H_
