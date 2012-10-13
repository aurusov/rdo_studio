/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/child_frm.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_CHILD_FRM_H_
#define _RDO_STUDIO_MFC_CHILD_FRM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChildFrame
// --------------------------------------------------------------------------------
class RDOStudioChildFrame: public CMDIChildWnd
{
	DECLARE_DYNCREATE(RDOStudioChildFrame)

public:
	RDOStudioChildFrame();
	virtual ~RDOStudioChildFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

protected:
	DECLARE_MESSAGE_MAP()
};

#endif // _RDO_STUDIO_MFC_CHILD_FRM_H_
