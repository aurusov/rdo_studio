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

	TCHAR* m_pchTip ;
	WCHAR* m_pwchTip;
	virtual int OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;

protected:
	//{{AFX_VIRTUAL(CChatNetworkCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatNetworkCtrl)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	DECLARE_MESSAGE_MAP()

public:
	CChatNetworkCtrl();
	virtual ~CChatNetworkCtrl();
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATNETWORKCTRL_H
