#ifndef BKEMULCHILDVIEW_H
#define BKEMULCHILDVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif

// --------------------------------------------------------------
// ---------- BKChildView
// --------------------------------------------------------------
class CDisplay;
class CSurface;

class BKChildView: public CWnd
{
private:
	CDisplay* display;
	CSurface* surface;

	int   bytePerPixel;
	int   pitch;
	DWORD rBits, gBits, bBits;
	DWORD rZero, gZero, bZero;
	DWORD rColor, gColor, bColor, grayColor;

	HRESULT initDirectDraw();
	HRESULT displayFrame();
	HRESULT restoreSurfaces();
	bool lockSurface();
	void unlockSurface();
	DWORD getColor( const COLORREF color ) const;

public:
	BKChildView();
	virtual ~BKChildView();

	void updateBounds();

protected:
	//{{AFX_MSG(BKChildView)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(BKChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
};

//{{AFX_INSERT_LOCATION}}

#endif // BKEMULCHILDVIEW_H
