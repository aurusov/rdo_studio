// rdogui.h : main header file for the RDOGUI application
//

#if !defined(AFX_RDOGUI_H__8F30FB45_91D1_428D_8855_EE00130D795B__INCLUDED_)
#define AFX_RDOGUI_H__8F30FB45_91D1_428D_8855_EE00130D795B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRdoguiApp:
// See rdogui.cpp for the implementation of this class
//

class CRdoguiApp : public CWinApp
{
public:
	CRdoguiApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRdoguiApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CRdoguiApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RDOGUI_H__8F30FB45_91D1_428D_8855_EE00130D795B__INCLUDED_)
