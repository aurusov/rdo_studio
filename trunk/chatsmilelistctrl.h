#ifndef CHATSMILELISTCTRL_H
#define CHATSMILELISTCTRL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "chatsmile.h"

// ----------------------------------------------------------------------------
// ---------- CChatSmileListCtrl
// ----------------------------------------------------------------------------
class CChatSmileListCtrl: public CWnd
{
private:
	CChatSmileList list;
	int smile_max_width;
	int text_max_width;
	int text_focus_height;
	std::vector< int > heights;
	int prev_line_from;
	int prev_line_to;
	int xPos;
	int yPos;
	int xMax;
	int yMax;
	int xPageSize;
	int yPageSize;
	CRect newClientRect;
	bool hasFocus;
	int selectedLine;
	bool horzScrollBarVisible;
	bool vertScrollBarVisible;
	CFont font;

	bool updateScrollBars();
	bool scrollVertically( int inc );
	bool scrollHorizontally( int inc );

	int getStringsSumHeight();
	int getHeightBeforeLine( const int index );

public:
	CChatSmileListCtrl();
	virtual ~CChatSmileListCtrl();

protected:
	//{{AFX_VIRTUAL(CChatSmileListCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatSmileListCtrl)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/*
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
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()
};
*/

//{{AFX_INSERT_LOCATION}}

#endif // CHATSMILELISTCTRL_H
