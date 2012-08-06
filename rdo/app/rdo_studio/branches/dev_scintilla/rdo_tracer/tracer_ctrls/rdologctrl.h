/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdologctrl.h
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOLOGCTRL_H_
#define _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOLOGCTRL_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdologstyle.h"
// --------------------------------------------------------------------------------

namespace rdoTracerLog {

// --------------------------------------------------------------------------------
// -------------------- RDOLogCtrl
// --------------------------------------------------------------------------------
#define WM_LOGSELCHANGE WM_USER + 1

typedef std::list< tstring > stringList;

class RDOLogCtrl: public CWnd  
{
friend class RDOLogCtrlFindInList;

DECLARE_DYNAMIC( RDOLogCtrl )

protected:
	CMutex mutex;
	
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
	rbool hasFocus;
	int selectedLine;
	int fullRepaintLines;
	rbool focusOnly;

	stringList strings;
	int stringsCount;
	stringList::iterator yPos_iterator;
	void setYPosIterator( const int prev_yPos );
	stringList::iterator findString( int index );
	stringList::reverse_iterator reverse_findString( int index );
	stringList::const_iterator const_findString( int index ) const;
	stringList::const_reverse_iterator const_reverse_findString( int index ) const;

	int   firstFoundLine;
	int   posFind;
	rbool bHaveFound;
	rbool bSearchDown;
	rbool bMatchCase;
	rbool bMatchWholeWord;
	tstring findStr;
	void find( int& result, const rbool searchDown, const rbool matchCase, const rbool matchWholeWord );

	RDOLogStyle*  logStyle;
	virtual rbool getItemColors( const int index, RDOLogColorPair* &colors ) const;
	virtual rbool getItemColors( CREF(tstring) item, RDOLogColorPair* &colors ) const;
	
	void  recalcWidth( const int newMaxStrWidth );
	void  updateScrollBars();
	rbool scrollVertically( int inc );
	rbool scrollHorizontally( int inc );
	
	rbool isVisible( const int index ) const;
	rbool isFullyVisible( const int index ) const;

	void  getLineRect( const int index, CRect* rect ) const;
	void  repaintLine ( const int index );

	rbool canCopy() const { return selectedLine != -1; };

	void  updateWindow();

	rbool drawLog;

	HDC   hdc;
	int   saved_hdc;
	HWND  hwnd;
	HFONT fontInit;
	HFONT hfontLog;
	void setFont( const rbool needRedraw = true );

protected:
	afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

private:
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

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
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

public:
	RDOLogCtrl( RDOLogStyle* style = NULL );
	virtual ~RDOLogCtrl();

	virtual void addStringToLog( const tstring logStr );
	
	rbool getFocusOnly() const { return focusOnly; }
	virtual void setFocusOnly( const rbool value ) { focusOnly = value; }

	virtual void getString( const int index, tstring& str ) const;
	virtual int getSelectedIndex() const;
	virtual void getSelected( tstring& str ) const;
	virtual rbool makeLineVisible( const int index );
	virtual void selectLine( const int index );
	virtual void copy();
	virtual void findNext()     { int res; find( res, bSearchDown, bMatchCase, bMatchWholeWord ); selectLine( res );  };
	virtual void findPrevious() { int res; find( res, !bSearchDown, bMatchCase, bMatchWholeWord ); selectLine( res ); };
	virtual void clear();
	
	virtual const RDOLogStyle& getStyle() const;
	virtual void setStyle( RDOLogStyle* style, const rbool needRedraw = true );

	void setText( tstring text );

	void  setDrawLog( const rbool value );
	rbool getDrawLog() const { return drawLog; };

	int getStringsCount() const { return stringsCount; };
};

}; // namespace rdoTracerLog

#endif // _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOLOGCTRL_H_
