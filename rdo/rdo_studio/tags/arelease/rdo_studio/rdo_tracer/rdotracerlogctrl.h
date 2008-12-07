#ifndef RDOTRACERLOGCTRL_H
#define RDOTRACERLOGCTRL_H
#pragma once

#include <rdologctrl.h>
#include "rdotracerlogstyle.h"

#pragma warning( disable : 4786 )

#include <map>

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOTracerLogCtrl
// ----------------------------------------------------------------------------
class RDOTracerLogCtrl: public RDOLogCtrl
{
DECLARE_DYNCREATE( RDOTracerLogCtrl )

protected:
	typedef map< int, RDOColorPair > RDOColorMap;
	RDOColorMap subitemColors;
	bool addingSubitems;
	RDOColorPair itemColor;
	bool getItemColors( const int index, COLORREF& textColor, COLORREF& backColor ) const;
	void showFindError( CString& findStr );

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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	//{{AFX_VIRTUAL(RDOTracerLogCtrl)
	//}}AFX_VIRTUAL

public:
	RDOTracerLogCtrl();
	virtual ~RDOTracerLogCtrl();

	void addStringToLog( const CString logStr );

	void setStyle( const RDOTracerLogStyle& style, const bool needRedraw = true );

	void clear();

	void startTrace();

	bool getShowMenu() const             { return bShowMenu;  };
	void setShowMenu( const bool value ) { bShowMenu = value; };

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDOTRACERLOGCTRL_H
