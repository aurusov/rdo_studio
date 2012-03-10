/*!
  \copyright (c) RDO-Team, 2006-2012
  \file      rdoprocess_pagectrl.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Шувалов Андрей (dronbas@gmail.com)
  \date      20.02.2006
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_PROCESS_PAGECTRL_H_
#define _RDO_STUDIO_MFC_RDO_PROCESS_PAGECTRL_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_string.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_pixmap.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RPPageCtrl
// --------------------------------------------------------------------------------
const UINT RP_PAGECTRL_DELETEITEM  = ::RegisterWindowMessage( "RP_PAGECTRL_DELETEITEM" );

class RPPageCtrlItem;

class RPPageCtrl: public CWnd
{
friend class RPPageCtrlItem;
friend class RPPageCtrlItemLabel;

protected:
	std::list< RPPageCtrlItem* > items;
	std::list< RPPageCtrlItem* >::const_iterator it_current;
	int label_height;

	ruint fly_timer;
	void startFly();
	void repaint();

	ruint over_timer;
	void startOver();

public:
	RPPageCtrl();
	virtual ~RPPageCtrl();

	RPPageCtrlItem* prepareNewPage();
	RPPageCtrlItem* insertPage( CWnd* wnd, const rp::string& label );
	void removePage( RPPageCtrlItem* page );

	void selectFirst();

private:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------------------------
// -------------------- RPPageCtrlItemLabel
// --------------------------------------------------------------------------------
class RPPageCtrlItemLabel: public CWnd
{
friend class RPPageCtrl;
friend class RPPageCtrlItem;

protected:
	RPPageCtrlItemLabel();
	virtual ~RPPageCtrlItemLabel();

	CBitmap label_bmp;
	CBitmap close_bmp;
	CBitmap close_bmp_over;
	rbool   over_close_button;
	CRect getLabelBMPRect();
	CRect getCloseBMPRect();
	void  updateOver( const CPoint& point );

private:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------------------------
// -------------------- RPPageCtrlItem
// --------------------------------------------------------------------------------
class RPPageCtrlItem: public CWnd
{
friend class RPPageCtrl;

protected:
	RPPageCtrlItemLabel label;
	CWnd*               wnd;
	rbool               fly;

	RPPageCtrlItem();
	virtual ~RPPageCtrlItem();

	void hide();
	void show();

public:
	void setPixmap( RPPixmap& bmp );

private:
	virtual BOOL DestroyWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnListCtrlBeginDrag(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
};

#endif // _RDO_STUDIO_MFC_RDO_PROCESS_PAGECTRL_H_
