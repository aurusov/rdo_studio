#ifndef RDOSTUDIOOUTPUT_H
#define RDOSTUDIOOUTPUT_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudiodockwnd.h"
#include "rdo_edit/rdoeditorscieditstyle.h"
#include "rdo_edit/rdoeditorscilogstyle.h"
#include "rdo_edit/rdoeditoreditstyle.h"

#include <rdotabctrl.h>
#include <rdosimwin.h>

namespace rdoEditor {
	class RDOEditorSciEdit;
	class RDOEditorSciLog;
	class RDOEditorEdit;
}

// ----------------------------------------------------------------------------
// ---------- RDOStudioOutput
// ----------------------------------------------------------------------------
class RDOStudioOutput: public RDOStudioDockWnd
{
private:
	RDOTabCtrl tab;
	rdoEditor::RDOEditorSciLogStyle  buildStyle;
	rdoEditor::RDOEditorSciEditStyle debugStyle;
	rdoEditor::RDOEditorEditStyle    resultsStyle;
	rdoEditor::RDOEditorSciLogStyle  findStyle;

	rdoEditor::RDOEditorSciLog*  build;
	rdoEditor::RDOEditorSciEdit* debug;
	CWnd*                        tracer;
	rdoEditor::RDOEditorEdit*    results;
	rdoEditor::RDOEditorSciLog*  find;

	CMenu popupMenu;

	void appendString( const rdoEditor::RDOEditorSciEdit* const edit, const string& str ) const;

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

	void appendStringToBuild( const string& str, const rdoModelObjects::RDOFileType fileType = rdoModelObjects::PAT, const int lineNumber = -1 ) const;
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
