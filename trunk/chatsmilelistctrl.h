#ifndef CHATSMILELISTCTRL_H
#define CHATSMILELISTCTRL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "chatsmile.h"

// ----------------------------------------------------------------------------
// ---------- CChatSmileListCtrl
// ----------------------------------------------------------------------------
class CChatSmileListCtrl: public CListBox
{
private:
	CChatSmileList list;
	int smile_max_width;
	int text_focus_height;

public:
	CChatSmileListCtrl();
	virtual ~CChatSmileListCtrl();

protected:
	//{{AFX_VIRTUAL(CChatSmileListCtrl)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatSmileListCtrl)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATSMILELISTCTRL_H
