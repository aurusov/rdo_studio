#ifndef RDOLOGCTRL_H
#define RDOLOGCTRL_H
#pragma once

#include "rdologstyle.h"

namespace rdoTracerLog {

// ----------------------------------------------------------------------------
// ---------- RDOLogCtrl
// ----------------------------------------------------------------------------
#define WM_LOGSELCHANGE WM_USER + 1

typedef std::list< std::string > stringList;

class RDOLogCtrl: public CWnd  
{
friend class RDOLogCtrlFindInList;

DECLARE_DYNAMIC( RDOLogCtrl )

protected:
	CFont fontLog;
	void setFont( const bool needRedraw = true );
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

	stringList strings;
	int stringsCount;
	stringList::iterator findString( int index );
	stringList::reverse_iterator reverse_findString( int index );
	stringList::const_iterator const_findString( int index ) const;
	stringList::const_reverse_iterator const_reverse_findString( int index ) const;

	int  firstFoundLine;
	int  posFind;
	bool bHaveFound;
	bool bSearchDown;
	bool bMatchCase;
	bool bMatchWholeWord;
	std::string findStr;
	int find( const bool searchDown, const bool matchCase, const bool matchWholeWord );

	RDOLogStyle* logStyle;
	virtual bool getItemColors( const int index, COLORREF& textColor, COLORREF& backColor ) const;
	virtual bool getItemColors( const std::string& item, COLORREF& textColor, COLORREF& backColor ) const;
	
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

	virtual void addStringToLog( const std::string logStr );
	
	bool getFocusOnly() const { return focusOnly; }
	virtual void setFocusOnly( const bool value ) { focusOnly = value; }

	const stringList& getLogStrings() const;
	virtual std::string getString( const int index ) const;
	virtual int getSelectedIndex() const;
	virtual std::string getSelected() const;
	virtual bool makeLineVisible( const int index );
	virtual void selectLine( const int index );
	virtual void copy();
	virtual void findNext()     { selectLine( find( bSearchDown, bMatchCase, bMatchWholeWord ) );  };
	virtual void findPrevious() { selectLine( find( !bSearchDown, bMatchCase, bMatchWholeWord ) ); };
	virtual void clear();
	
	virtual const RDOLogStyle& getStyle() const;
	virtual void setStyle( RDOLogStyle* style, const bool needRedraw = true );

	void setText( std::string text );
	
	//Must be called when moving main frame window
	//void storeWindowRect();
};

}; // namespace rdoTracerLog

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDOLOGCTRL_H
