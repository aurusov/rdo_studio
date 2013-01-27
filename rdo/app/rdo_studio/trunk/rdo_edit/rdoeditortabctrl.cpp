/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditortabctrl.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_edit/rdoeditortabctrl.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_windows_base.h"
#include "app/rdo_studio_mfc/src/edit/view_base.h"
// --------------------------------------------------------------------------------

using namespace rdoEditor;

// --------------------------------------------------------------------------------
// -------------------- RDOEditorTabCtrl
// --------------------------------------------------------------------------------
RDOEditorTabCtrl::RDOEditorTabCtrl(PTR(QWidget) pParent, PTR(RDOStudioEditBaseView) pView)
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

	setCurrentRDOItem(rdoModelObjects::RTP);
}

RDOEditorTabCtrl::~RDOEditorTabCtrl()
{}

void RDOEditorTabCtrl::createPage(PTR(RDOStudioEditBaseView) pView, CREF(QString) name)
{
	PTR(context_type) pPage = new context_type(this, pView);

	ASSERT(pPage);

	addTab(pPage, name);

	pPage->setEditorStyle(&studioApp.getStyle()->style_editor);
	pPage->show();
	//! @todo qt
	//pPage->getContext().setPopupMenu(&pView->popupMenu);

	group.insert(pPage);
	pPage->setGroup(&group);
}

rdoModelObjects::RDOFileType RDOEditorTabCtrl::indexToType(int index) const
{
	switch (index)
	{
		case  0: return rdoModelObjects::RTP;
		case  1: return rdoModelObjects::RSS;
		case  2: return rdoModelObjects::EVN;
		case  3: return rdoModelObjects::PAT;
		case  4: return rdoModelObjects::DPT;
		case  5: return rdoModelObjects::PRC;
		case  6: return rdoModelObjects::FRM;
		case  7: return rdoModelObjects::FUN;
		case  8: return rdoModelObjects::SMR;
		case  9: return rdoModelObjects::PMD;
		default: return rdoModelObjects::PAT;
	}
}

int RDOEditorTabCtrl::typeToIndex(rdoModelObjects::RDOFileType type) const
{
	switch (type)
	{
		case rdoModelObjects::RTP: return 0;
		case rdoModelObjects::RSS: return 1;
		case rdoModelObjects::EVN: return 2;
		case rdoModelObjects::PAT: return 3;
		case rdoModelObjects::DPT: return 4;
		case rdoModelObjects::PRC: return 5;
		case rdoModelObjects::FRM: return 6;
		case rdoModelObjects::FUN: return 7;
		case rdoModelObjects::SMR: return 8;
		case rdoModelObjects::PMD: return 9;
	}
	return -1;
}

void RDOEditorTabCtrl::setCurrentRDOItem(rdoModelObjects::RDOFileType type)
{
	int index = typeToIndex(type);
	if (index != -1)
		setCurrentIndex(index);
}

PTR(RDOEditorTabCtrl::context_type) RDOEditorTabCtrl::getItemEdit(rdoModelObjects::RDOFileType type) const
{
	int index = typeToIndex(type);
	return index != -1
		? getItemEdit(index)
		: NULL;
}

PTR(RDOEditorTabCtrl::context_type) RDOEditorTabCtrl::getCurrentEdit() const
{
	return static_cast<PTR(context_type)>(currentWidget());
}

PTR(RDOEditorTabCtrl::context_type) RDOEditorTabCtrl::getItemEdit(int index) const
{
	return static_cast<PTR(context_type)>(widget(index));
}
