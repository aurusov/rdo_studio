#ifndef RDOPLUGINMFC_H
#define RDOPLUGINMFC_H

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPluginMFC
// ----------------------------------------------------------------------------
class RDOPluginMFC: public CWinApp
{
public:
	RDOPluginMFC();

	//{{AFX_VIRTUAL(RDOPluginMFC)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOPluginMFC)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern RDOPluginMFC pluginMFCApp;

//{{AFX_INSERT_LOCATION}}

#endif // RDOPLUGINMFC_H
