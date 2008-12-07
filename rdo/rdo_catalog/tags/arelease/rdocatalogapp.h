#ifndef RDOCATALOGAPP_H
#define RDOCATALOGAPP_H

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <rdolink.h>
#include "resource.h"

// ----------------------------------------------------------------------------
// ---------- RDOCatalogApp
// ----------------------------------------------------------------------------
class RDOCatalogApp: public CWinApp
{
public:
	RDOCatalogApp();
	virtual ~RDOCatalogApp();

	//{{AFX_VIRTUAL(RDOCatalogApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOCatalogApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// ----------------------------------------------------------------------------
// ---------- RDOAboutDlg
// ----------------------------------------------------------------------------
class RDOAboutDlg: public CDialog
{
public:
	RDOAboutDlg();
	virtual ~RDOAboutDlg();

protected:
	//{{AFX_DATA(RDOAboutDlg)
	enum { IDD = IDD_ABOUT };
	RDOLink	m_web;
	RDOLink	m_email;
	CString	m_caption;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(RDOAboutDlg)
	protected:
	virtual void DoDataExchange( CDataExchange* pDX );
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOAboutDlg)
	afx_msg void OnAboutEmail();
	afx_msg void OnAboutWeb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// ----------------------------------------------------------------------------
extern RDOCatalogApp catalog;

//{{AFX_INSERT_LOCATION}}

#endif // RDOCATALOGAPP_H
