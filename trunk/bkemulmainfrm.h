#ifndef BKEMULMAINFRM_H
#define BKEMULMAINFRM_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "ddutil.h"

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
	CRect backRect;
	CRect screenRect;
	bool  fullScreenMode;
	int   fullWindowWidth;
	int   fullWindowHeight;

	UINT timer;
	bool lock;
	bool canIteration;
	std::vector< WORD > updateVideoMemory;
	HRESULT initDirectDraw();
	HRESULT lockSurface( LPDIRECTDRAWSURFACE7 surface ) const;
	HRESULT unlockSurface( LPDIRECTDRAWSURFACE7 surface ) const;
	HRESULT displayFrame() const;
	HRESULT restoreSurfaces() const;

	DWORD getColor( const COLORREF color ) const;

	void draw( const BYTE* bk_video_from, int count_byte, BYTE BK_byte_X = 0, BYTE BK_line_Y = 0 ) const;

	void updateMonitor() const;
	void updateScrolling( BYTE delta );
	void updateBounds();
	void setSmallScreen();
	bool isFullScreenMode() const { return fullScreenMode; }
	void setFullScreenMode( const bool value );

	void setDefaultFontMenu();
	void setDefaultRomMenu();

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
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnViewFullScreen();
	afx_msg void OnUpdateViewFullScreen(CCmdUI* pCmdUI);
	afx_msg void OnViewColorMonitor();
	afx_msg void OnUpdateViewColorMonitor(CCmdUI* pCmdUI);
	afx_msg void OnEmulatorReset();
	afx_msg void OnEmulatorSoftReset();
	afx_msg void OnEmulatorPowerOn();
	afx_msg void OnEmulatorPoweOff();
	afx_msg void OnUpdateEmulatorPowerOn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEmulatorPoweOff(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEmulatorReset(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEmulatorSoftReset(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnFontClicked( UINT nID );
	afx_msg void OnRom();
	afx_msg void OnUpdateRom( CCmdUI* pCmdUI );
	afx_msg void OnRom120000( UINT nID );
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(BKMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
};

//{{AFX_INSERT_LOCATION}}

#endif // BKEMULMAINFRM_H
