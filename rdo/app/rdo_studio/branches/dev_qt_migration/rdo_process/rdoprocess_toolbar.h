/*!
  \copyright (c) RDO-Team, 2006-2012
  \file      rdoprocess_toolbar.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Шувалов Андрей (dronbas@gmail.com)
  \date      12.04.2006
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_PROCESS_TOOLBAR_H_
#define _RDO_STUDIO_MFC_RDO_PROCESS_TOOLBAR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_process/rp_ctrl/rdoprocess_toolbar.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RPCtrlToolbarMFC
// --------------------------------------------------------------------------------
class RPToolbarToolTip: public CToolBar
{
friend class RPCtrlToolbarMFC;
private:
	CToolTipCtrl toolTip;
	std::map< unsigned int, rp::string > button_text;
	void updateToolTips();
	virtual BOOL PreTranslateMessage( MSG* pMsg ) {
		if ( pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST ) {
			toolTip.RelayEvent( pMsg );
		}
		return CToolBar::PreTranslateMessage( pMsg );
	}

	afx_msg void OnSize( UINT nType, int cx, int cy );
	DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------------------------
// -------------------- RPCtrlToolbarMFC
// --------------------------------------------------------------------------------
class RPPixmap;

class RPCtrlToolbarMFC: public RPCtrlToolbar
{
private:
	std::list< RPPixmap* > bitmaps;

public:
	RPToolbarToolTip toolbar;
	RPCtrlToolbarMFC( CWnd* parent );
	~RPCtrlToolbarMFC();
	virtual int insertButton( rpMethod::RPMethod* method, char* xpm[], const rp::string& tool_tip = "" );
	virtual void setCaption( const rp::string& caption );
};

// --------------------------------------------------------------------------------
// -------------------- RDOProcToolBar
// --------------------------------------------------------------------------------
class RDOProcToolBar: public CToolBar
{
public:
	virtual void init( CWnd* parent, unsigned int tbResID, unsigned int tbDisabledImageResID = -1 );

protected:
	CImageList disabledImage;
};

// --------------------------------------------------------------------------------
// -------------------- RPToolBar
// --------------------------------------------------------------------------------
class RPToolBar: public RDOProcToolBar
{
public:
	RPToolBar();
	virtual ~RPToolBar();

	COLORREF color_brush;
	COLORREF color_pen;
	COLORREF color_text;
	rbool    empty_brush;
	rbool    empty_pen;
	rbool    empty_text;

private:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

#endif // _RDO_STUDIO_MFC_RDO_PROCESS_TOOLBAR_H_
