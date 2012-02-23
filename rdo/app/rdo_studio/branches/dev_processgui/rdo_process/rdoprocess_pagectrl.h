#ifndef RDO_PROCESS_PAGECTRL_H
#define RDO_PROCESS_PAGECTRL_H

#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_string.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_pixmap.h"

#if _MSC_VER > 1000
#pragma once
#endif

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

protected:
	//{{AFX_VIRTUAL(RPPageCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RPPageCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
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
	bool    over_close_button;
	CRect getLabelBMPRect();
	CRect getCloseBMPRect();
	void  updateOver( const CPoint& point );

protected:
	//{{AFX_VIRTUAL(RPPageCtrlItemLabel)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RPPageCtrlItemLabel)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
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
	bool                fly;

	RPPageCtrlItem();
	virtual ~RPPageCtrlItem();

	void hide();
	void show();

public:
	void setPixmap( RPPixmap& bmp );

protected:
	//{{AFX_VIRTUAL(RPPageCtrlItem)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RPPageCtrlItem)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnListCtrlBeginDrag(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDO_PROCESS_PAGECTRL_H
