#ifndef CHATEDIT_H
#define CHATEDIT_H
#pragma once

// ----------------------------------------------------------------------------
// ---------- CChatEdit
// ----------------------------------------------------------------------------
class CChatEdit: public CEdit
{
protected:
	//{{AFX_VIRTUAL(CChatEdit)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatEdit)
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CChatEdit();
	~CChatEdit();
};

#endif // CHATEDIT_H
