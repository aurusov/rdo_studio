/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerlogctrl.h
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERLOGCTRL_H_
#define _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERLOGCTRL_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdologctrl.h"
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdotracerlogstyle.h"
// --------------------------------------------------------------------------------

namespace rdoTracer {
class RDOTracerBase;
}

namespace rdoTracerLog {

// --------------------------------------------------------------------------------
// -------------------- RDOTracerLogCtrl
// --------------------------------------------------------------------------------
class RDOTracerLogCtrl: public RDOLogCtrl
{
DECLARE_DYNCREATE( RDOTracerLogCtrl )

protected:
	typedef std::map< int, RDOLogColorPair* > RDOColorMap;
	RDOColorMap subitemColors;
	bool addingSubitems;
	RDOLogColorPair* itemColor;
	virtual bool getItemColors( const int index, RDOLogColorPair* &colors ) const;
	void showFindError( tstring& findStr );

	CMenu popupMenu;
	bool bShowMenu;

private:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnFind();
	afx_msg void OnFindNext();
	afx_msg void OnFindPrev();
	afx_msg void OnCopy();
	afx_msg LRESULT OnFindReplaceMsg( WPARAM wParam, LPARAM lParam );
	afx_msg void OnCanCopy( CCmdUI* pCmdUI );
	afx_msg void OnUpdateFindNextPrev( CCmdUI* pCmdUI );
	afx_msg void OnUpdateFind( CCmdUI* pCmdUI );
	afx_msg void OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu );
	afx_msg void OnContextMenu( CWnd* pWnd, CPoint pos );
	afx_msg void OnHelpKeyword();
	afx_msg void OnUpdateCoordStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModifyStatusBar( CCmdUI *pCmdUI );
	DECLARE_MESSAGE_MAP()
	
public:
	RDOTracerLogCtrl();
	virtual ~RDOTracerLogCtrl();

	virtual void addStringToLog( const tstring logStr );

	virtual void setStyle( RDOTracerLogStyle* style, const bool needRedraw = true );

	virtual void clear();

	bool getShowMenu() const             { return bShowMenu;  };
	void setShowMenu( const bool value ) { bShowMenu = value; };

};

}; // namespace rdoTracerLog

#endif // _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERLOGCTRL_H_
