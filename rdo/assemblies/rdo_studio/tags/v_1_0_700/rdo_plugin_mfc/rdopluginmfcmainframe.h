#ifndef RDOPLUGINMFCMAINFRAME_H
#define RDOPLUGINMFCMAINFRAME_H

#if _MSC_VER > 1000
#pragma once
#endif

#pragma warning( disable : 4786 )

#include <afxmt.h>
#include <list>
#include <string>

// ----------------------------------------------------------------------------
// ---------- RDOPluginMFCLogCtrl
// ----------------------------------------------------------------------------
typedef std::list< std::string > stringList;

class RDOPluginMFCLogCtrl: public CWnd  
{
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
	bool hasFocus;
	int selectedLine;
	int fullRepaintLines;
	bool focusOnly;

	stringList strings;
	int stringsCount;
	stringList::iterator yPos_iterator;
	void setYPosIterator( const int prev_yPos );
	stringList::iterator findString( int index );
	stringList::const_iterator const_findString( int index ) const;

	void recalcWidth( const int newMaxStrWidth );
	void updateScrollBars();
	bool scrollVertically( int inc );
	bool scrollHorizontally( int inc );
	
	bool isVisible( const int index ) const;
	bool isFullyVisible( const int index ) const;

	void getLineRect( const int index, CRect* rect ) const;
	void repaintLine ( const int index );

	bool canCopy() const { return selectedLine != -1; };

	void updateWindow();

	bool drawLog;

	HDC   hdc;
	int   saved_hdc;
	HWND  hwnd;
	HFONT font;

	//{{AFX_MSG(RDOPluginMFCLogCtrl)
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
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOPluginMFCLogCtrl)
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
	//}}AFX_VIRTUAL

public:
	RDOPluginMFCLogCtrl();
	virtual ~RDOPluginMFCLogCtrl();

	virtual void addStringToLog( std::string logStr );

	bool getFocusOnly() const { return focusOnly; }
	virtual void setFocusOnly( const bool value ) { focusOnly = value; }

	virtual void getString( const int index, std::string& str ) const;
	virtual int getSelectedIndex() const;
	virtual void getSelected( std::string& str ) const;
	virtual bool makeLineVisible( const int index );
	virtual void selectLine( const int index );
	virtual void clear();

	int getStringsCount() const { return stringsCount; };
};

// ----------------------------------------------------------------------------
// ---------- RDOPluginMFCMainFrame
// ----------------------------------------------------------------------------
class RDOPluginMFCMainFrame: public CFrameWnd
{
DECLARE_DYNAMIC(RDOPluginMFCMainFrame)

private:
	RDOPluginMFCLogCtrl log;

public:
	RDOPluginMFCMainFrame();
	virtual ~RDOPluginMFCMainFrame();

	void insertLine( const char* line );

protected:
	//{{AFX_VIRTUAL(RDOPluginMFCMainFrame)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOPluginMFCMainFrame)
	afx_msg void OnPluginClose();
	afx_msg void OnModelBuild();
	afx_msg void OnModelRun();
	afx_msg void OnModelStop();
	afx_msg void OnUpdateModelBuild(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelRun(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelStop(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnModelRunMaxSpeed();
	afx_msg void OnModelRunJump();
	afx_msg void OnModelRunSync();
	afx_msg void OnModelRunPause();
	afx_msg void OnUpdateModelRunMaxSpeed(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelRunJump(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelRunSync(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelRunPause(CCmdUI* pCmdUI);
	afx_msg void OnModelShowRateInc();
	afx_msg void OnModelShowRateIncFour();
	afx_msg void OnModelShowRateDecFour();
	afx_msg void OnModelShowRateDec();
	afx_msg void OnUpdateModelShowRateInc(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelShowRateIncFour(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelShowRateDecFour(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelShowRateDec(CCmdUI* pCmdUI);
	afx_msg void OnModelFrameNext();
	afx_msg void OnModelFramePrev();
	afx_msg void OnUpdateModelFrameNext(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelFramePrev(CCmdUI* pCmdUI);
	afx_msg void OnModelNew();
	afx_msg void OnModelOpen();
	afx_msg void OnModelSave();
	afx_msg void OnModelClose();
	afx_msg void OnUpdateModelSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelClose(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOPLUGINMFCMAINFRAME_H
