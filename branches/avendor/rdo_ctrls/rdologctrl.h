#ifndef RDOLOGCTRL_H
#define RDOLOGCTRL_H
#pragma once

#include "rdologstyle.h"

// ----------------------------------------------------------------------------
// ---------- RDOStringList
// ----------------------------------------------------------------------------
class RDOStringList: public CStringList
{
friend class RDOLogCtrl;
	
protected:
	bool scan( char*& wildCards, char*&str ) const;
public:
	RDOStringList( int blockSize = 10 );
	virtual ~RDOStringList();
	
	bool match( const CString& wildCards, const CString& str, const bool matchCase, const bool matchWholeWord ) const;
	int findNext( const CString& findWhat, const int findFrom, const int findTo, const bool searchDown, const bool matchCase, const bool matchWholeWord ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOLogCtrl
// ----------------------------------------------------------------------------
#define WM_LOGSELCHANGE WM_USER + 1

class RDOLogCtrl: public CWnd  
{
DECLARE_DYNAMIC( RDOLogCtrl )

protected:
	CFont fontLog;
	void setFont( const RDOFont& font, const bool needRedraw = true );
	int lineHeight;
	int charWidth;
	int maxStrWidth;
	int xPos;
	int yPos;
	int xMax;
	int yMax;
	int xPageSize;
	int yPageSize;
	CRect clipRect;
	CRect prevClientRect;
	CRect newClientRect;
	CRect prevWindowRect;
	int lastViewableLine;
	bool hasFocus;
	int selectedLine;
	int fullRepaintLines;
	bool focusOnly;

	int  firstFoundLine;
	bool bHaveFound;
	bool bSearchDown;
	bool bMatchCase;
	bool bMatchWholeWord;
	CString findStr;
	int find( const bool searchDown, const bool matchCase, const bool matchWholeWord );

	RDOStringList logStrings;

	RDOLogStyle* logStyle;
	virtual bool getItemColors( const int index, COLORREF& textColor, COLORREF& backColor ) const;
	virtual bool getItemColors( const CString& item, COLORREF& textColor, COLORREF& backColor ) const;
	
	void recalcWidth( const int newMaxStrWidth );
	void updateScrollBars();
	bool scrollVertically( int inc );
	bool scrollHorizontally( int inc );
	
	bool isVisible( const int index ) const;
	bool isFullyVisible( const int index ) const;

	void getLineRect( const int index, CRect* rect ) const;
	void repaintLine ( const int index );

	bool canCopy() const { return selectedLine != -1; };

	//{{AFX_MSG(RDOLogCtrl)
	afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnPaint();
	afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	afx_msg void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus( CWnd* pOldWnd );
	afx_msg void OnKillFocus( CWnd* pNewWnd );
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg UINT OnGetDlgCode() { return DLGC_WANTARROWS; };
	afx_msg BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOLogCtrl)
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
	//}}AFX_VIRTUAL

public:
	RDOLogCtrl( RDOLogStyle* style = NULL );
	virtual ~RDOLogCtrl();

	virtual void addStringToLog( const CString logStr );
	
	bool getFocusOnly() const { return focusOnly; }
	virtual void setFocusOnly( const bool value ) { focusOnly = value; }

	const RDOStringList& getLogStrings() const;
	virtual CString getString( const int index ) const;
	virtual int getSelectedIndex() const;
	virtual CString getSelected() const;
	virtual bool makeLineVisible( const int index );
	virtual void selectLine( const int index );
	virtual void copy();
	virtual void findNext()     { selectLine( find( bSearchDown, bMatchCase, bMatchWholeWord ) );  };
	virtual void findPrevious() { selectLine( find( !bSearchDown, bMatchCase, bMatchWholeWord ) ); };
	virtual void clear();
	
	virtual const RDOLogStyle& getStyle() const;
	virtual void setStyle( const RDOLogStyle& style, const bool needRedraw = true );
	
	//Must be called when moving main frame window
	//void storeWindowRect();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDOLOGCTRL_H
