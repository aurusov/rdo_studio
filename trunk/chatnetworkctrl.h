#ifndef CHATNETWORKCTRL_H
#define CHATNETWORKCTRL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <rdotreectrl.h>

// Using code from Megavarnan Selvaraj for tooltip

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

	static UINT shellExecute( LPVOID pParam );

public:
	CChatNetworkCtrl();
	virtual ~CChatNetworkCtrl();

protected:
	//{{AFX_VIRTUAL(CChatNetworkCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatNetworkCtrl)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNetworkOpen();
	afx_msg void OnNetworkInfo();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATNETWORKCTRL_H
