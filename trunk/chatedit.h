#ifndef CHATEDIT_H
#define CHATEDIT_H

#if _MSC_VER > 1000
#pragma once
#endif

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

//{{AFX_INSERT_LOCATION}}

#endif // CHATEDIT_H
