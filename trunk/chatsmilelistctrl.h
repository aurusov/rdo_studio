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
friend class CChatSmileThread;

private:

	struct Item {
		CChatSmile* smile;
		std::string info;
		int         height;
	};
	std::vector< Item* > list;

	int smile_max_width;
	int text_max_width;
	int text_focus_height;
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

	bool isVisible( const int index );
	bool isFirstLineVisible();
	bool isLastLineVisible();
	void selectLine( const int index );
	void repaintLine( const int index );

	int findLine( const int _yPos );
	int getStringsSumHeight();
	int getHeightBeforeLine( const int index );

	void copy();

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
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATSMILELISTCTRL_H
