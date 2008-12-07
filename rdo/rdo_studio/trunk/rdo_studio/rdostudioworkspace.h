#ifndef RDOSTUDIOWORKSPACE_H
#define RDOSTUDIOWORKSPACE_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudiodockwnd.h"
#include "rdostudioframetreectrl.h"

#include <rdotabctrl.h>

// ----------------------------------------------------------------------------
// ---------- RDOStudioWorkspace
// ----------------------------------------------------------------------------
class RDOStudioWorkspace: public RDOStudioDockWnd
{
friend class RDOStudioFrameManager;
private:
	RDOTabCtrl tab;
	RDOStudioFrameTreeCtrl* frames;

public:
	RDOStudioWorkspace();
	virtual ~RDOStudioWorkspace();

protected:
	//{{AFX_VIRTUAL(RDOStudioWorkspace)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioWorkspace)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOWORKSPACE_H
