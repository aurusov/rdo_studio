#ifndef CHATEDIT_H
#define CHATEDIT_H
#pragma once

// ----------------------------------------------------------------------------
// ---------- CChatEdit
// ----------------------------------------------------------------------------
class CChatEdit: public CEdit
{
public:
	CChatEdit();
	~CChatEdit();

protected:
	//{{AFX_VIRTUAL(CChatEdit)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatEdit)
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // CHATEDIT_H
