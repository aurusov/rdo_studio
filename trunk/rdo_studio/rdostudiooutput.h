#ifndef RDOSTUDIOOUTPUT_H
#define RDOSTUDIOOUTPUT_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudiodockwnd.h"
#include "edit_ctrls/rdobaseeditstyle.h"
#include "edit_ctrls/rdologeditstyle.h"
#include "rdo_edit/rdoeditoreditstyle.h"

#include <rdotabctrl.h>
#include <rdosimwin.h>

class RDOBuildEdit;
class RDODebugEdit;
class RDOFindEdit;
namespace rdoEditor {
	class RDOEditorEdit;
}

// ----------------------------------------------------------------------------
// ---------- RDOStudioOutput
// ----------------------------------------------------------------------------
class RDOStudioOutput: public RDOStudioDockWnd
{
private:
	RDOTabCtrl tab;
	RDOLogEditStyle               buildStyle;
	rdoBaseEdit::RDOBaseEditStyle debugStyle;
	rdoEditor::RDOEditorEditStyle resultsStyle;
	RDOLogEditStyle               findStyle;

	RDOBuildEdit*             build;
	RDODebugEdit*             debug;
	CWnd*                     tracer;
	rdoEditor::RDOEditorEdit* results;
	RDOFindEdit*              find;

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

	void appendStringToBuild( const string& str, const rdoModelObjects::RDOFileType fileType = rdoModelObjects::PAT, const int lineNumber = -1, const bool error = true ) const;
	void appendStringToDebug( const string& str ) const;
	void appendStringToFind( const string& str, const rdoModelObjects::RDOFileType fileType = rdoModelObjects::PAT, const int lineNumber = -1 ) const;

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
