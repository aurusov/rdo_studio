#ifndef CHATUSERLISTCTRL_H
#define CHATUSERLISTCTRL_H
#pragma once

#include <rdotreectrl.h>

// ----------------------------------------------------------------------------
// ---------- CChatUserListCtrl
// ----------------------------------------------------------------------------
class CChatUser;

class CChatUserListCtrl: public RDOTreeCtrl
{
private:
	CImageList imageList;

	HTREEITEM findUser( const CChatUser* const user );

protected:
	//{{AFX_VIRTUAL(CChatUserListCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatUserListCtrl)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CChatUserListCtrl();
	virtual ~CChatUserListCtrl();

	void addUser( const CChatUser* const user );
	void deleteUser( const CChatUser* const user );
	void updateUserName( const CChatUser* const user );
	void updateUserStatus( const CChatUser* const user );
};

#endif // CHATUSERLISTCTRL_H
