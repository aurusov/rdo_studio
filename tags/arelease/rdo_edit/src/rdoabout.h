#ifndef RDOABOUT_H
#define RDOABOUT_H
#pragma once

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

	virtual BOOL OnInitDialog();

	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnMouseMove( UINT nFlags, CPoint point );
	afx_msg void OnPaint();
	afx_msg void OnEMailLinkLButtonDown( NMHDR* pNotifyStruct, LRESULT* result );
	afx_msg void OnWebSiteLinkLButtonDown( NMHDR* pNotifyStruct, LRESULT* result );
	DECLARE_MESSAGE_MAP()

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

	void changeColor( const BYTE r, const BYTE g, const BYTE b );
};

#endif // RDOABOUT_H
