#ifndef CHATUSERLISTCTRL_H
#define CHATUSERLISTCTRL_H
#pragma once

#include <rdotreectrl.h>

// Using code from Megavarnan Selvaraj for tooltip

// ----------------------------------------------------------------------------
// ---------- CChatUserListCtrl
// ----------------------------------------------------------------------------
class CChatUser;

class CChatUserListCtrl: public RDOTreeCtrl
{
private:
	CImageList imageList;

	HTREEITEM findUser( const CChatUser* const user );

	TCHAR* m_pchTip ;
	WCHAR* m_pwchTip;
	virtual int OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;

protected:
	//{{AFX_VIRTUAL(CChatUserListCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatUserListCtrl)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	//}}AFX_MSG
	afx_msg BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	DECLARE_MESSAGE_MAP()

public:
	CChatUserListCtrl();
	virtual ~CChatUserListCtrl();

	void addUser( const CChatUser* const user );
	void deleteUser( const CChatUser* const user );
	void updateUserName( const CChatUser* const user );
	void updateUserStatus( const CChatUser* const user );
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATUSERLISTCTRL_H
