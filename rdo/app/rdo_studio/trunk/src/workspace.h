/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      workspace.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_WORKSPACE_H_
#define _RDO_STUDIO_MFC_WORKSPACE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "ui/mfc_ctrls/rdotabctrl.h"
#include "app/rdo_studio_mfc/src/dock_wnd.h"
#include "app/rdo_studio_mfc/src/frame/tree_ctrl.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_pagectrl.h"
// --------------------------------------------------------------------------------

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

#endif // _RDO_STUDIO_MFC_WORKSPACE_H_
