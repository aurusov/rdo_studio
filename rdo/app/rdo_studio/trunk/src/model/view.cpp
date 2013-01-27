/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/model/view.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qevent.h>
#include <QtGui/qboxlayout.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/model/view.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_windows_base.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdofindedit.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditortabctrl.h"
#include "app/rdo_studio_mfc/resource.h"
// --------------------------------------------------------------------------------

using namespace rdoEditor;

// --------------------------------------------------------------------------------
// -------------------- RDOStudioModelView
// --------------------------------------------------------------------------------
static const UINT FINDINMODEL_MSG = ::RegisterWindowMessage( FINDMSGSTRING );

// ON_UPDATE_COMMAND_UI сделано

//! @todo qt
//BEGIN_MESSAGE_MAP(RDOStudioModelView, RDOStudioEditBaseView)
//	ON_WM_CREATE()
//	ON_WM_SETFOCUS()
//	ON_WM_SIZE()
//	ON_COMMAND(ID_SEARCH_FIND_INMODEL, OnSearchFindInModel)
//	ON_REGISTERED_MESSAGE( FINDINMODEL_MSG, OnFindInModelMsg )
//	ON_UPDATE_COMMAND_UI( ID_COORD_STATUSBAR          , OnUpdateCoordStatusBar )
//	ON_UPDATE_COMMAND_UI( ID_MODIFY_STATUSBAR         , OnUpdateModifyStatusBar )
//	ON_UPDATE_COMMAND_UI( ID_INSERTOVERWRITE_STATUSBAR, OnUpdateInsertOverwriteStatusBar )
//	ON_COMMAND(ID_FILE_PRINT, RDOStudioEditBaseView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, RDOStudioEditBaseView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, RDOStudioEditBaseView::OnFilePrintPreview)
//END_MESSAGE_MAP()

RDOStudioModelView::RDOStudioModelView(PTR(QWidget) pParent)
	: RDOStudioEditBaseView(pParent)
	, m_pModel  (NULL)
	, m_pTabCtrl(NULL)
{
	m_pTabCtrl = new RDOEditorTabCtrl(this, this);

	PTR(QVBoxLayout) pLayout = new QVBoxLayout(this);
	pLayout->setSpacing(0);
	pLayout->setContentsMargins(0, 0, 0, 0);
	pLayout->addWidget(m_pTabCtrl);
}

RDOStudioModelView::~RDOStudioModelView()
{}

void RDOStudioModelView::setModel(PTR(RDOStudioModel) pModel)
{
	ASSERT(m_pModel != pModel);
	m_pModel = pModel;
}

void RDOStudioModelView::closeEvent(PTR(QCloseEvent) event)
{
	if (model->saveModified())
	{
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

//! @todo qt
//BOOL RDOStudioModelView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
//{
//	if ( m_pTabCtrl->getCurrentEdit()->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
//	return RDOStudioEditBaseView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
//}
//
//void RDOStudioModelView::OnSetFocus(CWnd* pOldWnd)
//{
//	RDOStudioEditBaseView::OnSetFocus( pOldWnd );
//	m_pTabCtrl->SetFocus();
//}
//

REF(rdoEditor::RDOEditorTabCtrl) RDOStudioModelView::getTab()
{
	return *m_pTabCtrl;
}

void RDOStudioModelView::OnSearchFindInModel() 
{
	PTR(CFindReplaceDialog) pDlg = new CFindReplaceDialog();
	pDlg->Create(true, m_pTabCtrl->getCurrentEdit()->getWordForFind().c_str(), NULL, FR_HIDEUPDOWN, &m_thisCWnd);
}

LRESULT RDOStudioModelView::OnFindInModelMsg( WPARAM /*wParam*/, LPARAM lParam )
{
	PTR(CFindReplaceDialog) pDialog = CFindReplaceDialog::GetNotifier(lParam);

	if (!pDialog->IsTerminating())
	{
		studioApp.getIMainWnd()->getDockFind().clear();
		studioApp.getIMainWnd()->getDockFind().raise();
		tstring findStr       = pDialog->GetFindString();
		rbool bMatchCase      = pDialog->MatchCase() ? true : false;
		rbool bMatchWholeWord = pDialog->MatchWholeWord() ? true : false;
		studioApp.getIMainWnd()->getDockFind().getContext().setKeyword(findStr, bMatchCase);
		studioApp.getIMainWnd()->getDockFind().appendString(rdo::format(ID_FINDINMODEL_BEGINMSG, findStr.c_str()));
		int count = 0;
		for (int i = 0; i < m_pTabCtrl->count(); i++)
		{
			PTR(RDOEditorEdit) pEdit = m_pTabCtrl->getItemEdit(i);
			int pos  = 0;
			int line = 0;
			while (pos != -1)
			{
				pos = pEdit->findPos(findStr, line, bMatchCase, bMatchWholeWord);
				if (pos != -1)
				{
					line = pEdit->getLineFromPosition(pos);
					studioApp.getIMainWnd()->getDockFind().appendString(pEdit->getLine(line), m_pTabCtrl->indexToType(i), line, pos - pEdit->getPositionFromLine(line));
					line++;
					count++;
				}
			}
		}
		pDialog->SendMessage(WM_CLOSE);
		tstring s = count
			? rdo::format(ID_FINDINMODEL_ENDMSG_COUNT,    count)
			: rdo::format(ID_FINDINMODEL_ENDMSG_NOTFOUND, findStr.c_str());
		studioApp.getIMainWnd()->getDockFind().appendString(s);
	}
	return 0;
}

void RDOStudioModelView::OnUpdateCoordStatusBar(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
	PTR(RDOEditorEdit) pEdit = m_pTabCtrl->getCurrentEdit();
	pCmdUI->SetText(pEdit
		? rdo::format("%d: %d", pEdit->getCurrentColumnNumber() + 1, pEdit->getCurrentLineNumber() + 1).c_str()
		: ""
	);
}

void RDOStudioModelView::OnUpdateModifyStatusBar(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
	PTR(RDOEditorEdit) pEdit = m_pTabCtrl->getCurrentEdit();
	if (pEdit)
	{
		if (pEdit->isReadOnly())
		{
			pCmdUI->SetText(rdo::format(ID_STATUSBAR_READONLY).c_str());
		}
		else if (pEdit->isModify())
		{
			pCmdUI->SetText(rdo::format( ID_STATUSBAR_MODIFIED ).c_str());
		}
		else
		{
			pCmdUI->SetText("");
		}
	}
	else
	{
		pCmdUI->SetText("");
	}
}

void RDOStudioModelView::OnUpdateInsertOverwriteStatusBar(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
	PTR(RDOEditorEdit) pEdit = m_pTabCtrl->getCurrentEdit();
	if (pEdit && pEdit->isOverwrite())
	{
		pCmdUI->SetText(rdo::format(ID_STATUSBAR_OVERWRITE).c_str());
	}
	else
	{
		pCmdUI->SetText("");
	}
}
