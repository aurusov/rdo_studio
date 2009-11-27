#ifndef RDOLINK_H
#define RDOLINK_H
#pragma once

#include <afxwin.h>

// ----------------------------------------------------------------------------
// ---------- RDOLink
// ----------------------------------------------------------------------------
#define NM_RDOLINK_CLICK 1

class RDOLink: public CWnd
{
protected:
	bool mustInit;
	void init();

	HCURSOR cursor;

	COLORREF textColor;
	COLORREF bgColor;
	bool bold;

	bool useTextColor;
	bool useBgColor;

	CFont fontNormal;
	CFont fontOver;
	bool isMouseCaptionOver;

	//{{AFX_MSG(RDOLink)
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnMouseMove( UINT nFlags, CPoint point );
	afx_msg void OnPaint();
	afx_msg void OnKillFocus( CWnd* pNewWnd );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOLink)
	public:
	virtual BOOL Create( const CString& text, const RECT& rect, CWnd* pParentWnd, UINT nID );
	//}}AFX_VIRTUAL

public:
	RDOLink();
	virtual ~RDOLink();

	CSize calcSize();
	void autoSize();

	void setTextColor( const COLORREF color );
	void setBgColor( const COLORREF color );
	void setFontBold( const bool value );
};

#endif // RDOLINK_H
