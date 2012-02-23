#ifndef RDOSTUDIOWORKSPACE_H
#define RDOSTUDIOWORKSPACE_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "app/rdo_studio_mfc/src/dock_wnd.h"
#include "app/rdo_studio_mfc/src/frame/tree_ctrl.h"
#include "ui/mfc_ctrls/rdotabctrl.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_pagectrl.h"
// --------------------------------------------------------------------------------
// -------------------- RDOStudioWorkspace
// --------------------------------------------------------------------------------
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
