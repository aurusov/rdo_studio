#ifndef CHATUSERSTREECTRL_H
#define CHATUSERSTREECTRL_H
#pragma once

#include "rdotreectrl.h"

// ----------------------------------------------------------------------------
// ---------- CChatUsersTreeCtrl
// ----------------------------------------------------------------------------
class CChatUser;

class CChatUsersTreeCtrl: public RDOTreeCtrl
{
DECLARE_DYNCREATE( CChatUsersTreeCtrl )
private:
	CImageList imageList;

protected:
	virtual BOOL DestroyWindow();

	HTREEITEM findUser( const CChatUser* const user );

	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
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
