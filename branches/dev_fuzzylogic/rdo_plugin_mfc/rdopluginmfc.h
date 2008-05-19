#ifndef RDOPLUGINMFC_H
#define RDOPLUGINMFC_H

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#define RDOPLUGIN_EXPORTS
#include <rdoplugin.h>

#include <afxmt.h>

// ----------------------------------------------------------------------------
// ---------- RDOPluginMFC
// ----------------------------------------------------------------------------
class RDOPluginMFCMainFrame;

class RDOPluginMFC: public CWinApp
{
public:
	RDOPluginMFC();

	rdoPlugin::Studio studio;
	RDOPluginMFCMainFrame* frame;
//	CMutex closeMutex;

protected:
	//{{AFX_VIRTUAL(RDOPluginMFC)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOPluginMFC)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern RDOPluginMFC pluginMFCApp;

//{{AFX_INSERT_LOCATION}}

#endif // RDOPLUGINMFC_H
