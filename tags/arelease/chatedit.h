#ifndef CHATEDIT_H
#define CHATEDIT_H
#pragma once

// ----------------------------------------------------------------------------
// ---------- CChatEdit
// ----------------------------------------------------------------------------
class CChatEdit: public CEdit
{
protected:
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );

	DECLARE_MESSAGE_MAP()

public:
	CChatEdit();
	~CChatEdit();
};

#endif // CHATEDIT_H
