#ifndef RDOSTUDIOOUTPUT_H
#define RDOSTUDIOOUTPUT_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudiodockwnd.h"

#include <rdotabctrl.h>
#include <rdosimwin.h>

namespace rdoEditor {
	class RDOEditorEdit;
	class RDOEditorResults;
}
namespace rdoEditCtrl {
	class RDOBuildEdit;
	class RDODebugEdit;
	class RDOFindEdit;
}

// ----------------------------------------------------------------------------
// ---------- RDOStudioOutput
// ----------------------------------------------------------------------------
class RDOStudioOutput: public RDOStudioDockWnd
{
private:

	class Tab: public RDOTabCtrl {
	protected:
		virtual void changeCurrentItem();
	};

	Tab tab;

	rdoEditCtrl::RDOBuildEdit*   build;
	rdoEditCtrl::RDODebugEdit*   debug;
	CWnd*                        tracer;
	rdoEditor::RDOEditorResults* results;
	rdoEditCtrl::RDOFindEdit*    find;

	CMenu popupMenu;

public:
	RDOStudioOutput();
	virtual ~RDOStudioOutput();

	void showBuild();
	void showDebug();
	void showTracer();
	void showResults();
	void showFind();

	void clearBuild();
	void clearDebug();
	void clearResults();
	void clearFind();

	const rdoEditCtrl::RDOBuildEdit*   getBuild() const   { return build;   };
	const rdoEditCtrl::RDODebugEdit*   getDebug() const   { return debug;   };
	const CWnd*                        getTracer() const  { return tracer;  };
	const rdoEditor::RDOEditorResults* getResults() const { return results; };
	const rdoEditCtrl::RDOFindEdit*    getFind() const    { return find;    };

	void appendStringToBuild( const string& str, const rdoModelObjects::RDOFileType fileType = rdoModelObjects::PAT, const int lineNumber = -1, const bool error = true ) const;
	void appendStringToDebug( const string& str ) const;
	void appendStringToFind( const string& str, const rdoModelObjects::RDOFileType fileType = rdoModelObjects::PAT, const int lineNumber = -1, const int posInLine = 0 ) const;

	void updateLogConnection() const;
	void updateStyles() const;

public:
	//{{AFX_VIRTUAL(RDOStudioOutput)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioOutput)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOOUTPUT_H
