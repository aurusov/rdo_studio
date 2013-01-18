/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/model/model_view.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
#include <QtGui/qevent.h>
#include <QtGui/qboxlayout.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/model/model_view.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_windows_base.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdofindedit.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditortabctrl.h"
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
// --------------------------------------------------------------------------------

using namespace rdoEditor;

// --------------------------------------------------------------------------------
// -------------------- RDOStudioModelView
// --------------------------------------------------------------------------------
RDOStudioModelView::RDOStudioModelView(PTR(QWidget) pParent)
	: parent_type(pParent)
	, m_pModel     (NULL)
	, m_pTabCtrl   (NULL)
	, m_pFindDialog(NULL)
{
	m_pTabCtrl = new RDOEditorTabCtrl(this, this);

	PTR(QVBoxLayout) pLayout = new QVBoxLayout(this);
	pLayout->setSpacing(0);
	pLayout->setContentsMargins(0, 0, 0, 0);
	pLayout->addWidget(m_pTabCtrl);

	RDOStudioMainFrame* pMainWindow = studioApp.getMainWndUI();
	ASSERT(pMainWindow);
	pMainWindow->actSearchFindInModel->setEnabled(true);
	connect(pMainWindow->actSearchFindInModel, SIGNAL(triggered(bool)), this, SLOT(onSearchFindInModel()));
}

RDOStudioModelView::~RDOStudioModelView()
{
	RDOStudioMainFrame* pMainWindow = studioApp.getMainWndUI();
	ASSERT(pMainWindow);
	pMainWindow->actSearchFindInModel->setEnabled(false);
	disconnect(pMainWindow->actSearchFindInModel, SIGNAL(triggered(bool)), this, SLOT(onSearchFindInModel()));
}

void RDOStudioModelView::setModel(PTR(RDOStudioModel) pModel)
{
	ASSERT(m_pModel != pModel);
	m_pModel = pModel;
}

void RDOStudioModelView::closeEvent(PTR(QCloseEvent) event)
{
	if (m_pModel)
	{
		if (m_pModel->closeModel())
		{
			event->accept();
		}
		else
		{
			event->ignore();
		}
	}
}

REF(rdoEditor::RDOEditorTabCtrl) RDOStudioModelView::getTab()
{
	return *m_pTabCtrl;
}

void RDOStudioModelView::onSearchFindInModel()
{
	m_findSettings.what = m_pTabCtrl->getCurrentEdit()->getWordForFind();

	if (!m_pFindDialog)
	{
		m_pFindDialog = new FindDialog(
			this,
			boost::bind(&RDOStudioModelView::onFindDlgFind, this, _1),
			boost::bind(&RDOStudioModelView::onFindDlgClose, this)
		);
	}

	m_pFindDialog->setSettings(m_findSettings);
	m_pFindDialog->show();
	m_pFindDialog->raise();
	m_pFindDialog->activateWindow();
}

void RDOStudioModelView::onFindDlgFind(CREF(FindDialog::Settings) settings)
{
	m_findSettings = settings;
	onSearchFindAll();
}

void RDOStudioModelView::onFindDlgClose()
{
	m_pFindDialog = NULL;
}

void RDOStudioModelView::onSearchFindAll()
{
	studioApp.getIMainWnd()->getDockFind().clear();
	studioApp.getIMainWnd()->getDockFind().raise();
	tstring findStr       = m_findSettings.what;
	rbool bMatchCase      = m_findSettings.matchCase;
	rbool bMatchWholeWord = m_findSettings.matchWholeWord;
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
				pEdit->setCurrentPos(pos);
				tstring newfindStr = pEdit->getCurrentWord();
				pos = pEdit->findPos(newfindStr, line, bMatchCase, bMatchWholeWord);
				line = pEdit->getLineFromPosition(pos);
				studioApp.getIMainWnd()->getDockFind().appendString(pEdit->getLine(line), m_pTabCtrl->indexToType(i), line, pos - pEdit->getPositionFromLine(line) + newfindStr.length());
				line++;
				count++;
			}
		}
	}
	m_pFindDialog = NULL;
	tstring s = count
		? rdo::format(ID_FINDINMODEL_ENDMSG_COUNT,    count)
		: rdo::format(ID_FINDINMODEL_ENDMSG_NOTFOUND, findStr.c_str());
	studioApp.getIMainWnd()->getDockFind().appendString(s);
}