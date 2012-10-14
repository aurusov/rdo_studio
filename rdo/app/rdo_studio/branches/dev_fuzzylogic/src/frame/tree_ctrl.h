/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/frame/tree_ctrl.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_FRAME_TREE_CTRL_H_
#define _RDO_STUDIO_MFC_FRAME_TREE_CTRL_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "ui/mfc_ctrls/rdotreectrl.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameTreeCtrl
// --------------------------------------------------------------------------------
class RDOStudioFrameTreeCtrl: public RDOTreeCtrl
{
private:
	CImageList imageList;

public:
	RDOStudioFrameTreeCtrl();
	virtual ~RDOStudioFrameTreeCtrl();

	void expand();

private:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnHelpKeyword();
	DECLARE_MESSAGE_MAP()
};

#endif // _RDO_STUDIO_MFC_FRAME_TREE_CTRL_H_
