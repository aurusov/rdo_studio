#ifndef BKEMULCHILDVIEW_H
#define BKEMULCHILDVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif

// --------------------------------------------------------------
// ---------- BKChildView
// --------------------------------------------------------------
class CDisplay;

class BKChildView: public CWnd
{
private:
	CDisplay* display;

	int   bytePerPixel;
	int   pitch;
	DWORD rBits, gBits, bBits;
	DWORD rZero, gZero, bZero;
	DWORD rColor, gColor, bColor, grayColor;
	CRect windowRect;
	CRect screenRect;

	HRESULT initDirectDraw();
	HRESULT lockSurface() const;
	HRESULT unlockSurface() const;
	HRESULT displayFrame() const;
	HRESULT restoreSurfaces() const;

	DWORD getColor( const COLORREF color ) const;

	void draw( const BYTE* bk_video_from, int count_byte, BYTE BK_byte_X = 0, BYTE BK_line_Y = 0 ) const;

public:
	BKChildView();
	virtual ~BKChildView();

	void updateMonitor() const;
	void updateScrolling( BYTE delta ) const;
	void updateVideoMemoryByte( WORD address ) const;
	void updateVideoMemoryWord( WORD address ) const;

	void updateBounds();

protected:
	//{{AFX_MSG(BKChildView)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(BKChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
};

//{{AFX_INSERT_LOCATION}}

#endif // BKEMULCHILDVIEW_H
