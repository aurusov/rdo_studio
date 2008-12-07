#ifndef RDO_PROCESS_APP_H
#define RDO_PROCESS_APP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPROCApp
// ----------------------------------------------------------------------------
class RDOPROCApp: public CWinApp
{
protected:
	HMENU m_hMDIMenu;
	HACCEL m_hMDIAccel;

public:
	RDOPROCApp();

	//{{AFX_VIRTUAL(RDOPROCApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOPROCApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDO_PROCESS_APP_H
