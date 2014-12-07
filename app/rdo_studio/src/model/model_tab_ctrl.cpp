// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/model_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/model/model_tab_ctrl.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_window_base.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::model;

TabCtrl::TabCtrl(QWidget* pParent, QWidget* pView)
	: QTabWidget(pParent)
{
	createPage(pView, "RTP");
	createPage(pView, "RSS");
	createPage(pView, "EVN");
	createPage(pView, "PAT");
	createPage(pView, "DPT");
	createPage(pView, "PRC");
	createPage(pView, "FRM");
	createPage(pView, "FUN");
	createPage(pView, "SMR");
	createPage(pView, "PMD");

	setCurrentRDOItem(rdo::model::RTP);
}

TabCtrl::~TabCtrl()
{}

void TabCtrl::createPage(QWidget* pView, const QString& name)
{
	context_type* pPage = new context_type(this, pView);

	ASSERT(pPage);

	addTab(pPage, name);

	pPage->setEditorStyle(&g_pApp->getStyle()->style_editor);
	pPage->show();
	//! @todo qt
	//pPage->getContext().setPopupMenu(&pView->popupMenu);

	m_group.insert(pPage);
	pPage->setGroup(&m_group);
}

rdo::model::FileType TabCtrl::indexToType(int index) const
{
	switch (index)
	{
		case  0: return rdo::model::RTP;
		case  1: return rdo::model::RSS;
		case  2: return rdo::model::EVN;
		case  3: return rdo::model::PAT;
		case  4: return rdo::model::DPT;
		case  5: return rdo::model::PRC;
		case  6: return rdo::model::FRM;
		case  7: return rdo::model::FUN;
		case  8: return rdo::model::SMR;
		case  9: return rdo::model::PMD;
		default: return rdo::model::PAT;
	}
}

int TabCtrl::typeToIndex(rdo::model::FileType type) const
{
	switch (type)
	{
		case rdo::model::RTP: return 0;
		case rdo::model::RSS: return 1;
		case rdo::model::EVN: return 2;
		case rdo::model::PAT: return 3;
		case rdo::model::DPT: return 4;
		case rdo::model::PRC: return 5;
		case rdo::model::FRM: return 6;
		case rdo::model::FUN: return 7;
		case rdo::model::SMR: return 8;
		case rdo::model::PMD: return 9;
		default: break;
	}
	return -1;
}

void TabCtrl::setCurrentRDOItem(rdo::model::FileType type)
{
	int index = typeToIndex(type);
	if (index != -1)
		setCurrentIndex(index);
}

TabCtrl::context_type* TabCtrl::getItemEdit(rdo::model::FileType type) const
{
	int index = typeToIndex(type);
	return index != -1
		? getItemEdit(index)
		: NULL;
}

TabCtrl::context_type* TabCtrl::getCurrentEdit() const
{
	return static_cast<context_type*>(currentWidget());
}

TabCtrl::context_type* TabCtrl::getItemEdit(int index) const
{
	return static_cast<context_type*>(widget(index));
}
