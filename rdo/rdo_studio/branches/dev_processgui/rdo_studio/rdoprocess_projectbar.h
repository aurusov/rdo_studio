#ifndef RDO_PROCESS_PROJECTBAR_H
#define RDO_PROCESS_PROJECTBAR_H

//#include "rdo_studio/rdo_process/rdoprocess_dockwnd.h"
#include "rdo_studio/rdostudiodockwnd.h"
#include "rdo_studio/rdo_process/rdoprocess_pagectrl.h"

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- RPProjectBar
// ----------------------------------------------------------------------------
class RPProjectBar: public RDOStudioDockWnd
{
protected:
	RPPageCtrl pagectrl;

public:
	RPProjectBar();
	virtual ~RPProjectBar();

	RPPageCtrlItem* prepareNewPage()                                 { return pagectrl.prepareNewPage();         }
	RPPageCtrlItem* insertPage( CWnd* wnd, const rp::string& label ) { return pagectrl.insertPage( wnd, label ); }
	void removePage( RPPageCtrlItem* page )                          { pagectrl.removePage( page );              }
	void selectFirst()                                               { pagectrl.selectFirst();                   }

protected:
	//{{AFX_VIRTUAL(RPProjectBar)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RPProjectBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDO_PROCESS_PROJECTBAR_H
