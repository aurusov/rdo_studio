/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_ctrl_view.h
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_TRACER_CTRLSTRACER_CTRL_VIEW_H_
#define _RDO_STUDIO_MFC_TRACER_CTRLSTRACER_CTRL_VIEW_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qabstractscrollarea.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/tracer/ctrls/tracer_ctrl.h"
#include "app/rdo_studio_mfc/src/tracer/ctrls/tracer_ctrl_view_style.h"
#include "app/rdo_studio_mfc/src/help_context_i.h"
// --------------------------------------------------------------------------------

namespace rdoTracer {
class RDOTracerBase;
}

namespace rdoTracerLog {

// --------------------------------------------------------------------------------
// -------------------- RDOTracerLogCtrlView
// --------------------------------------------------------------------------------
class RDOTracerLogCtrlView
	: public RDOLogCtrl
	, public IHelpContext
{
protected:
	typedef std::map<int, RDOLogColorPair*> RDOColorMap;

	RDOColorMap      subitemColors;
	rbool            addingSubitems;
	RDOLogColorPair* itemColor;

	virtual rbool getItemColors(int index, RDOLogColorPair* &colors) const;
	void showFindError(REF(tstring) findStr);

	CMenu popupMenu;
	rbool bShowMenu;

private:
	DECLARE_IHelpContext;

	afx_msg void OnFind();
	afx_msg void OnFindNext();
	afx_msg void OnFindPrev();
	afx_msg LRESULT OnFindReplaceMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateFindNextPrev(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFind(CCmdUI* pCmdUI);
	//! todo qt
	//afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	//afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);
	afx_msg void OnUpdateCoordStatusBar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModifyStatusBar(CCmdUI* pCmdUI);
	
public:
	RDOTracerLogCtrlView(PTR(QAbstractScrollArea) pParent);
	virtual ~RDOTracerLogCtrlView();

	virtual void addStringToLog(CREF(tstring) logStr);

	virtual void setStyle(RDOTracerLogStyle* style, const rbool needRedraw = true);

	virtual void clear();

	rbool getShowMenu() const      { return bShowMenu;  };
	void  setShowMenu(rbool value) { bShowMenu = value; };

};

// --------------------------------------------------------------------------------
// -------------------- RDOTracerLogCtrl
// --------------------------------------------------------------------------------
class RDOTracerLogCtrl: public QAbstractScrollArea
{
public:
	RDOTracerLogCtrl(PTR(QWidget) pParent);

	REF(RDOTracerLogCtrlView) view()
	{
		return *static_cast<PTR(RDOTracerLogCtrlView)>(viewport());
	}

private:
	virtual rbool viewportEvent(PTR(QEvent)      pEvent);
	virtual void  focusInEvent (PTR(QFocusEvent) pEvent);
	virtual void  focusOutEvent(PTR(QFocusEvent) pEvent);
	virtual void  keyPressEvent(PTR(QKeyEvent)   pEvent);
};

}; // namespace rdoTracerLog

#endif // _RDO_STUDIO_MFC_TRACER_CTRLSTRACER_CTRL_VIEW_H_
