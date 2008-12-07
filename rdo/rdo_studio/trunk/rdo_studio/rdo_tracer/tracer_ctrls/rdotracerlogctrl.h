#ifndef RDOTRACERLOGCTRL_H
#define RDOTRACERLOGCTRL_H
#pragma once

#include "rdologctrl.h"
#include "rdotracerlogstyle.h"

namespace rdoTracer {
class RDOTracerBase;
}

namespace rdoTracerLog {

// ----------------------------------------------------------------------------
// ---------- RDOTracerLogCtrl
// ----------------------------------------------------------------------------
class RDOTracerLogCtrl: public RDOLogCtrl
{
DECLARE_DYNCREATE( RDOTracerLogCtrl )

protected:
	typedef std::map< int, RDOLogColorPair* > RDOColorMap;
	RDOColorMap subitemColors;
	bool addingSubitems;
	RDOLogColorPair* itemColor;
	virtual bool getItemColors( const int index, RDOLogColorPair* &colors ) const;
	void showFindError( std::string& findStr );

	CMenu popupMenu;
	bool bShowMenu;
	
	//{{AFX_MSG(RDOTracerLogCtrl)
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
	//}}AFX_MSG
	afx_msg void OnUpdateCoordStatusBar( CCmdUI *pCmdUI );
	afx_msg void OnUpdateModifyStatusBar( CCmdUI *pCmdUI );
	DECLARE_MESSAGE_MAP()
	
	//{{AFX_VIRTUAL(RDOTracerLogCtrl)
	//}}AFX_VIRTUAL

public:
	RDOTracerLogCtrl();
	virtual ~RDOTracerLogCtrl();

	virtual void addStringToLog( const std::string logStr );

	virtual void setStyle( RDOTracerLogStyle* style, const bool needRedraw = true );

	virtual void clear();

	bool getShowMenu() const             { return bShowMenu;  };
	void setShowMenu( const bool value ) { bShowMenu = value; };

};

}; // namespace rdoTracerLog

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDOTRACERLOGCTRL_H
