#ifndef RDOABOUT_H
#define RDOABOUT_H
#pragma once

#include <afxwin.h>

#include "rdolink.h"

// ----------------------------------------------------------------------------
// ---------- RDOAbout
// ----------------------------------------------------------------------------
class RDOAbout: public CDialog
{
protected:
	int dx;
	int dy;
	bool isMouseDown;

	CFont fontCaption;
	CFont fontBMSTU;
	CFont fontTel;
	CFont fontEmail;
	CFont fontWebSite;
	CRect rCaption;
	CRect rBMSTU;
	CRect rTel;
	CRect rEmail;
	CRect rEmailLink;
	CRect rWebSite;
	CRect rWebSiteLink;
	CRect rBottom;
	CRect rButtonOK;

	RDOLink emailLink;
	RDOLink webSiteLink;
	RDOLink buttonOK;

	int getCheckedColor( const int color );

	//{{AFX_MSG(RDOAbout)
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnMouseMove( UINT nFlags, CPoint point );
	afx_msg void OnPaint();
	afx_msg void OnEMailLinkLButtonDown( NMHDR* pNotifyStruct, LRESULT* result );
	afx_msg void OnWebSiteLinkLButtonDown( NMHDR* pNotifyStruct, LRESULT* result );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOAbout)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

public:
	RDOAbout();
	virtual ~RDOAbout();

	int width;

	CString caption;
	CString BMSTU;
	CString tel;
	CString email;
	CString webSite;
	bool captionBold;
	bool BMSTUBold;
	bool telBold;
	bool emailBold;
	bool webSiteBold;

	CString emailLinkText;
	CString webSiteLinkText;
	CString buttonOkText;
	bool emailLinkBold;
	bool webSiteLinkBold;
	bool buttonOkBold;

	HICON hPixmap;

	COLORREF bgPixmap;
	COLORREF fgCaption;
	COLORREF bgCaption;
	COLORREF fgInfo;
	COLORREF bgInfo;
	COLORREF fgLink;
	COLORREF fgButtonOK;
	COLORREF bgButtonOK;

	virtual int DoModal();

	void changeColor( const int r, const int g, const int b );
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOABOUT_H
