#ifndef RDOSTUDIOWORKSPACE_H
#define RDOSTUDIOWORKSPACE_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdo_studio/rdostudiodockwnd.h"
#include "rdo_studio/rdostudioframetreectrl.h"
#include "rdo_ctrls/rdotabctrl.h"
#include "rdo_studio/rdo_process/rdoprocess_pagectrl.h"
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
	
	RPPageCtrl* pagectrl;	
	RPPageCtrlItem* prepareNewPage()                                 { return pagectrl->prepareNewPage();         }
	RPPageCtrlItem* insertPage( CWnd* wnd, const rp::string& label ) { return pagectrl->insertPage( wnd, label ); }
	void removePage( RPPageCtrlItem* page )                          { pagectrl->removePage( page );              }
	void selectFirst()                                               { pagectrl->selectFirst();                   }

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
