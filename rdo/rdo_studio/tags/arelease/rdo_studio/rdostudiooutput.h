#ifndef RDOSTUDIOOUTPUT_H
#define RDOSTUDIOOUTPUT_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <rdotabctrl.h>

#include "rdostudiodockwnd.h"
#include "rdo_edit/rdoeditorscieditstyle.h"
#include "rdo_edit/rdoeditorscilogstyle.h"
#include "rdo_edit/rdoeditoreditstyle.h"

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

public:
	RDOStudioOutput();
	virtual ~RDOStudioOutput();

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
