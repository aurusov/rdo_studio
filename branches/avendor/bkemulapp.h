#ifndef BKEMULAPP_H
#define BKEMULAPP_H

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

// --------------------------------------------------------------
// ---------- BKApp
// --------------------------------------------------------------
class BKApp: public CWinApp
{
public:
	BKApp();

protected:
	//{{AFX_MSG(BKApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(BKApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL
};

//{{AFX_INSERT_LOCATION}}

#endif // BKEMULAPP_H
