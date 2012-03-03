#ifndef RDO_PROCESS_TOOLBAR_H
#define RDO_PROCESS_TOOLBAR_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "app/rdo_studio_mfc/rdo_process/rp_ctrl/rdoprocess_toolbar.h"

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
	//{{AFX_MSG(RPToolBar)
	afx_msg void OnSize( UINT nType, int cx, int cy );
	//}}AFX_MSG
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
// -------------------- RDOToolBar
// --------------------------------------------------------------------------------
class RDOProcToolBar: public CToolBar
{
protected:
	CImageList disabledImage;

public:
	virtual void init( CWnd* parent, unsigned int tbResID, unsigned int tbDisabledImageResID = -1 );
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
	bool     empty_brush;
	bool     empty_pen;
	bool     empty_text;

protected:
	//{{AFX_VIRTUAL(RPToolBar)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RPToolBar)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDO_PROCESS_TOOLBAR_H
