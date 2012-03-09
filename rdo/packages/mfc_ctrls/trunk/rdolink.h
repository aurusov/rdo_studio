/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdolink.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _UI_MFC_CTRLS_RDOLINK_H_
#define _UI_MFC_CTRLS_RDOLINK_H_

// ----------------------------------------------------------------------- INCLUDES
#include <afxwin.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOLink
// --------------------------------------------------------------------------------
#define NM_RDOLINK_CLICK 1

class RDOLink: public CWnd
{
protected:
	rbool mustInit;
	void init();

	HCURSOR cursor;

	COLORREF textColor;
	COLORREF bgColor;
	rbool bold;

	rbool useTextColor;
	rbool useBgColor;

	CFont fontNormal;
	CFont fontOver;
	rbool isMouseCaptionOver;

private:
	virtual BOOL Create( const CString& text, const RECT& rect, CWnd* pParentWnd, UINT nID );

	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnMouseMove( UINT nFlags, CPoint point );
	afx_msg void OnPaint();
	afx_msg void OnKillFocus( CWnd* pNewWnd );
	DECLARE_MESSAGE_MAP()

public:
	RDOLink();
	virtual ~RDOLink();

	CSize calcSize();
	void autoSize();

	void setTextColor( const COLORREF color );
	void setBgColor( const COLORREF color );
	void setFontBold( const rbool value );
};

#endif // _UI_MFC_CTRLS_RDOLINK_H_
