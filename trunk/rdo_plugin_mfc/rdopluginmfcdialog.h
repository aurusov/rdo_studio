#ifndef RDOPLUGINMFCDIALOG_H
#define RDOPLUGINMFCDIALOG_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "resource.h"

// ----------------------------------------------------------------------------
// ---------- RDOPluginMFCDialog
// ----------------------------------------------------------------------------
class RDOPluginMFCDialog: public CDialog
{
public:
	RDOPluginMFCDialog( CWnd* pParent = NULL );
	virtual ~RDOPluginMFCDialog();

	//{{AFX_DATA(RDOPluginMFCDialog)
	enum { IDD = IDD_PLUGIN_DIALOG };
	//}}AFX_DATA


	//{{AFX_VIRTUAL(RDOPluginMFCDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnCancel();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOPluginMFCDialog)
	afx_msg void OnModelRun();
	afx_msg void OnUpdateModelRun(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOPLUGINMFCDIALOG_H
