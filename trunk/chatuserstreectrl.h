#ifndef CHATUSERSTREECTRL_H
#define CHATUSERSTREECTRL_H
#pragma once

#include <rdotreectrl.h>

// ----------------------------------------------------------------------------
// ---------- CChatUsersTreeCtrl
// ----------------------------------------------------------------------------
class CChatUser;

class CChatUsersTreeCtrl: public RDOTreeCtrl
{
private:
	CImageList imageList;

	HTREEITEM findUser( const CChatUser* const user );

protected:
	//{{AFX_VIRTUAL(CChatUsersTreeCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatUsersTreeCtrl)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CChatUsersTreeCtrl();
	virtual ~CChatUsersTreeCtrl();

	void addUser( const CChatUser* const user );
	void deleteUser( const CChatUser* const user );
	void updateUserName( const CChatUser* const user );
	void updateUserStatus( const CChatUser* const user );
};

#endif // CHATUSERSTREECTRL_H
