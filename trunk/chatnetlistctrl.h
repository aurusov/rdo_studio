#ifndef CHATNETLISTCTRL_H
#define CHATNETLISTCTRL_H
#pragma once

#include <rdotreectrl.h>

// ----------------------------------------------------------------------------
// ---------- CChatNetListCtrl
// ----------------------------------------------------------------------------
class CChatNetListCtrl: public RDOTreeCtrl
{
private:
	CImageList imageList;

protected:
	//{{AFX_VIRTUAL(CChatNetListCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatNetListCtrl)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CChatNetListCtrl();
	virtual ~CChatNetListCtrl();
};

#endif // CHATNETLISTCTRL_H
