/*!
  \copyright (c) RDO-Team, 2006-2012
  \file      rdoprocess_childfrm.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Шувалов Андрей (dronbas@gmail.com)
  \date      10.01.2006
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_PROCESS_CHILDFRM_H_
#define _RDO_STUDIO_MFC_RDO_PROCESS_CHILDFRM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/child_frm.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RPChildFrame
// --------------------------------------------------------------------------------
class RPView;

class RPChildFrame: public RDOStudioChildFrame
{
DECLARE_DYNCREATE(RPChildFrame)

private:
	RPView* view;

public:
	RPChildFrame();
	virtual ~RPChildFrame();

	void setView( RPView* _view ) { view = _view; }

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg void OnFileClose();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	DECLARE_MESSAGE_MAP()
};

#endif // _RDO_STUDIO_MFC_RDO_PROCESS_CHILDFRM_H_
