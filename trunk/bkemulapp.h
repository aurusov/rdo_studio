#ifndef BKEMULAPP_H
#define BKEMULAPP_H

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

// --------------------------------------------------------------
// ---------- BKEmulApp
// --------------------------------------------------------------
class BKMainFrame;

class BKEmulApp: public CWinApp
{
public:
	BKEmulApp();

	BKMainFrame* mainFrame;

protected:
	//{{AFX_MSG(BKEmulApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(BKEmulApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL
};

// --------------------------------------------------------------
extern BKEmulApp enulApp;

//{{AFX_INSERT_LOCATION}}

#endif // BKEMULAPP_H
