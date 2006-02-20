#ifndef RDO_PROCESS_PROJECTBAR_H
#define RDO_PROCESS_PROJECTBAR_H

#include "rdoprocess_dockwnd.h"
#include "rdoprocess_pagectrl.h"

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- RPProjectBar
// ----------------------------------------------------------------------------
class RPProjectBar: public RPDockWnd
{
protected:
	RPPageCtrl pagectrl;

public:
	RPProjectBar();
	virtual ~RPProjectBar();

	RPPageCtrlItem* prepareNewPage()                      { return pagectrl.prepareNewPage();  }
	void insertPage( CWnd* wnd, const rp::string& label ) { pagectrl.insertPage( wnd, label ); }
	void removePage( RPPageCtrlItem* page )               { pagectrl.removePage( page );       }
	void selectFirst()                                    { pagectrl.selectFirst();            }

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
