#ifndef BKEMULMAINFRM_H
#define BKEMULMAINFRM_H

#if _MSC_VER > 1000
#pragma once
#endif

// --------------------------------------------------------------
// ---------- BKMainFrame
// --------------------------------------------------------------
class CDisplay;
namespace bkemul {
class BKEmulVideo;
}

class BKMainFrame: public CFrameWnd
{
DECLARE_DYNAMIC(BKMainFrame)

friend class bkemul::BKEmulVideo;

private:
	CDisplay* display;

	int   bytePerPixel;
	int   pitch;
	DWORD rBits, gBits, bBits;
	DWORD rZero, gZero, bZero;
	DWORD rColor, gColor, bColor, grayColor;
	CRect windowRect;
	CRect screenRect;
	bool  fullScreenMode;
	int   fullWindowWidth;
	int   fullWindowHeight;

	UINT timer;
	bool lock;
	std::vector< WORD > updateVideoMemory;
	HRESULT initDirectDraw();
	HRESULT lockSurface() const;
	HRESULT unlockSurface() const;
	HRESULT displayFrame() const;
	HRESULT restoreSurfaces() const;

	DWORD getColor( const COLORREF color ) const;

	void draw( const BYTE* bk_video_from, int count_byte, BYTE BK_byte_X = 0, BYTE BK_line_Y = 0 ) const;

	void updateMonitor() const;
	void updateScrolling( BYTE delta ) const;
	void updateBounds();
	bool isFullScreenMode() const { return fullScreenMode; }
	void setFullScreenMode( const bool value );

public:
	BKMainFrame();
	virtual ~BKMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//{{AFX_MSG(BKMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnViewFullScreen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(BKMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	//}}AFX_VIRTUAL
};

//{{AFX_INSERT_LOCATION}}

#endif // BKEMULMAINFRM_H
