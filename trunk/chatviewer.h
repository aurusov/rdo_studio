#ifndef CHATVIEWER_H
#define CHATVIEWER_H
#pragma once

#include "chatviewerstring.h"
#include "chatviewerstyle.h"

// ----------------------------------------------------------------------------
// ---------- CChatViewer
// ----------------------------------------------------------------------------
class CChatViewer: public CWnd
{
private:
	CChatStringList strings;
	CChatViewerStyle style;

	int xPos;
	int yPos;
	int xMax;
	int yMax;
	int xPageSize;
	int yPageSize;
	int maxStrWidth;
	CRect newClientRect;
	bool hasFocus;
	int selectedLine;
	bool horzScrollBarVisible;
	bool vertScrollBarVisible;

	void recalcWidth();
	bool updateScrollBars();
	bool scrollVertically( int inc );
	bool scrollHorizontally( int inc );

	bool isVisible( const int index );
	void selectLine( const int index );
	void repaintLine ( const int index );

	CChatString* getString( const int index ) const;
	int findLine( const int _yPos );
	int getStringsSumHeight();
	int getHeightBeforeLine( const int index );
	bool sizeChanged();
	int getStringsMaxWidth();
	int getStrHeight( const int index );

public:
	CChatViewer();
	virtual ~CChatViewer();

	void addString( CChatString* str );

	const CChatViewerStyle& getStyle() const;
	void setStyle( const CChatViewerStyle& _style, const bool needRedraw = true );

protected:
	//{{AFX_VIRTUAL(CChatViewer)
	protected:
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatViewer)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	afx_msg void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus( CWnd* pOldWnd );
	afx_msg void OnKillFocus( CWnd* pNewWnd );
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg UINT OnGetDlgCode() { return DLGC_WANTARROWS; };
	afx_msg BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATVIEWER_H
