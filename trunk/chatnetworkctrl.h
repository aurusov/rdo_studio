#ifndef CHATNETWORKCTRL_H
#define CHATNETWORKCTRL_H
#pragma once

#include <rdotreectrl.h>

// ----------------------------------------------------------------------------
// ---------- CChatNetworkCtrl
// ----------------------------------------------------------------------------
class CChatNetworkCtrl: public RDOTreeCtrl
{
private:
	CImageList imageList;

protected:
	//{{AFX_VIRTUAL(CChatNetworkCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatNetworkCtrl)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CChatNetworkCtrl();
	virtual ~CChatNetworkCtrl();
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATNETWORKCTRL_H
