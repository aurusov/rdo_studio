/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/model/model_view.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <boost/bind.hpp>
#include <QEvent>
#include <QBoxLayout>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/model/model_view.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/editor/find_edit.h"
#include "app/rdo_studio/src/model/model_tab_ctrl.h"
#include "app/rdo_studio/src/main_window.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::model;

View::View(PTR(QWidget) pParent)
	: parent_type(pParent)
	, m_pModel     (NULL)
	, m_pTabCtrl   (NULL)
	, m_pFindDialog(NULL)
{
	m_pTabCtrl = new TabCtrl(this, this);

	PTR(QVBoxLayout) pLayout = new QVBoxLayout(this);
	pLayout->setSpacing(0);
	pLayout->setContentsMargins(0, 0, 0, 0);
	pLayout->addWidget(m_pTabCtrl);

	MainWindow* pMainWindow = g_pApp->getMainWndUI();
	ASSERT(pMainWindow);
	pMainWindow->actSearchFindInModel->setEnabled(true);
	connect(pMainWindow->actSearchFindInModel, SIGNAL(triggered(bool)), this, SLOT(onSearchFindInModel()));
}

View::~View()
{
	MainWindow* pMainWindow = g_pApp->getMainWndUI();
	ASSERT(pMainWindow);
	pMainWindow->actSearchFindInModel->setEnabled(false);
	disconnect(pMainWindow->actSearchFindInModel, SIGNAL(triggered(bool)), this, SLOT(onSearchFindInModel()));
}

void View::setModel(PTR(Model) pModel)
{
	ASSERT(m_pModel != pModel);
	m_pModel = pModel;
}

void View::closeEvent(PTR(QCloseEvent) event)
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

REF(TabCtrl) View::getTab()
{
	return *m_pTabCtrl;
}

void View::onSearchFindInModel()
{
	m_findSettings.what = m_pTabCtrl->getCurrentEdit()->getWordForFind();

	if (!m_pFindDialog)
	{
		m_pFindDialog = new FindDialog(
			this,
			boost::bind(&View::onFindDlgFind, this, _1),
			boost::bind(&View::onFindDlgClose, this)
		);
	}

	m_pFindDialog->setSettings(m_findSettings);
	m_pFindDialog->show();
	m_pFindDialog->raise();
	m_pFindDialog->activateWindow();
}

void View::onFindDlgFind(CREF(FindDialog::Settings) settings)
{
	m_findSettings = settings;
	onSearchFindAll();
}

void View::onFindDlgClose()
{
	m_pFindDialog = NULL;
}

void View::onSearchFindAll()
{
	g_pApp->getIMainWnd()->getDockFind().clear();
	g_pApp->getIMainWnd()->getDockFind().raise();
	QString findStr       = m_findSettings.what;
	rbool bMatchCase      = m_findSettings.matchCase;
	rbool bMatchWholeWord = m_findSettings.matchWholeWord;
	g_pApp->getIMainWnd()->getDockFind().getContext().setKeyword(findStr, bMatchCase);
	g_pApp->getIMainWnd()->getDockFind().appendString(QString::fromStdWString(L"Поиск '%1'...\r\n").arg(findStr));
	int count = 0;
	for (int i = 0; i < m_pTabCtrl->count(); i++)
	{
		PTR(editor::Model) pEdit = m_pTabCtrl->getItemEdit(i);
		int pos  = 0;
		int line = 0;
		while (pos != -1)
		{
			pos = pEdit->findPos(findStr, line, bMatchCase, bMatchWholeWord);
			if (pos != -1)
			{
				int endPos = pEdit->isEndOfWord(pos);
				ASSERT(endPos != -1);
				line = pEdit->getLineFromPosition(pos);
				g_pApp->getIMainWnd()->getDockFind().appendString(
					QString::fromLocal8Bit(pEdit->getLine(line).c_str()),
					m_pTabCtrl->indexToType(i),
					line,
					endPos - pEdit->getPositionFromLine(line)
				);
				line++;
				count++;
			}
		}
	}
	m_pFindDialog = NULL;

	QString s = count
		? QString(QString::fromStdWString(L"'%1' раз было найдено.\r\n").arg(count))
		: QString(QString::fromStdWString(L"Не получилось найти строчку '%1'.\r\n").arg(findStr));
	g_pApp->getIMainWnd()->getDockFind().appendString(s);
}
