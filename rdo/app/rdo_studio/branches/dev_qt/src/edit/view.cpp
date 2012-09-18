/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/edit/view.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/edit/view.h"
#include "app/rdo_studio_mfc/src/edit/document.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_windows_base.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoredit.h"
#include "app/rdo_studio_mfc/resource.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditor;

// --------------------------------------------------------------------------------
// -------------------- RDOStudioEditView
// --------------------------------------------------------------------------------
RDOStudioEditView::RDOStudioEditView(QWidget* pParent)
	: RDOStudioEditBaseView(pParent)
{
	edit = new RDOEditorEdit( this );
	edit->setEditorStyle( &studioApp.getStyle()->style_editor );
	edit->Create( NULL, NULL, WS_VISIBLE, CRect(0, 0, 100, 100), &m_thisCWnd, 0 );
	//! @todo qt
	//edit->setPopupMenu(m_pPopupMenu);
}

RDOStudioEditView::~RDOStudioEditView()
{
	delete edit;
}

//! todo qt
//BOOL RDOStudioEditView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
//{
//	if ( edit->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
//	return RDOStudioEditBaseView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
//}
//
//void RDOStudioEditView::OnSetFocus(CWnd* pOldWnd)
//{
//	RDOStudioEditBaseView::OnSetFocus( pOldWnd );
//	edit->SetFocus();
//}
//
//void RDOStudioEditView::OnSize(UINT nType, int cx, int cy) 
//{
//	RDOStudioEditBaseView::OnSize(nType, cx, cy);
//	
//	CRect r;
//	GetClientRect( r );
//	edit->MoveWindow( r );
//}

RDOEditorEdit* RDOStudioEditView::getEdit() const
{
	return edit;
}
