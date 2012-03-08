#ifndef RDOPLUGINMFC_H
#define RDOPLUGINMFC_H

#define RDOPLUGIN_EXPORTS
#include "app/rdo_studio_mfc/plugins/common/rdoplugin.h"

#include <afxmt.h>

// --------------------------------------------------------------------------------
// -------------------- RDOPluginMFC
// --------------------------------------------------------------------------------
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
