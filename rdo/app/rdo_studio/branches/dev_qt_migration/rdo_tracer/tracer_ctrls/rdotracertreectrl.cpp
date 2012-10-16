/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracertreectrl.cpp
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtCore/qprocess.h>
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdotracertreectrl.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracer.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerrestype.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerresource.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerpattern.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotraceroperation.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerresult.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_windows_base.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOTracerTreeCtrl
// --------------------------------------------------------------------------------
SCODE RDODropSource::GiveFeedback( DROPEFFECT dropEffect )
{
	return COleDropSource::GiveFeedback( dropEffect );
}

//! @todo qt
//BEGIN_MESSAGE_MAP( RDOTracerTreeCtrl, RDOTreeCtrl )
//	ON_WM_INITMENUPOPUP()
//	ON_COMMAND( ID_CHART_ADDTONEWCHART, OnAddToNewChart )
//	ON_UPDATE_COMMAND_UI( ID_CHART_ADDTONEWCHART, OnUpdateAddToNewChart )
//	ON_NOTIFY_REFLECT( TVN_BEGINDRAG, OnDragDrop )
//	ON_WM_RBUTTONDOWN()
//	ON_UPDATE_COMMAND_UI(ID_CHART_FINDINCHARTS, OnUpdateChartFindincharts)
//	ON_COMMAND(ID_CHART_FINDINCHARTS, OnChartFindincharts)
//END_MESSAGE_MAP()

Q_DECLARE_METATYPE(RDOTracerTreeItem*);

RDOTracerTreeCtrl::RDOTracerTreeCtrl(PTR(QWidget) pParent)
	: parent_type(pParent)
{
	setColumnCount    (1);
	setHeaderHidden   (true);
	setRootIsDecorated(false);

	m_iconList.reserve(IT_COUNT);
	m_iconList.push_back(QIcon(QString::fromUtf8(":/images/images/tree_chart_root.png")));
	m_iconList.push_back(QIcon(QString::fromUtf8(":/images/images/tree_chart_sub_root_1.png")));
	m_iconList.push_back(QIcon(QString::fromUtf8(":/images/images/tree_chart_sub_root_2.png")));
	m_iconList.push_back(QIcon(QString::fromUtf8(":/images/images/tree_chart_sub_root_3.png")));
	m_iconList.push_back(QIcon(QString::fromUtf8(":/images/images/tree_chart_value.png")));
	m_iconList.push_back(QIcon(QString::fromUtf8(":/images/images/tree_chart_erased.png")));

	rootItem.setTreeItem(new QTreeWidgetItem(this));
	rootItem.getTreeItem().setText(0, "Модель");
	rootItem.getTreeItem().setIcon(0, m_iconList[IT_ROOT]);

	createItem(rootItem.getTreeItem(), rtpItem, "Типы ресурсов", IT_SUB_ROOT_1);
	createItem(rootItem.getTreeItem(), patItem, "Образцы",       IT_SUB_ROOT_1);
	createItem(rootItem.getTreeItem(), pmvItem, "Результаты",    IT_SUB_ROOT_1);

	rootItem.getTreeItem().setExpanded(true);

	connect(
		this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
		this, SLOT(onTreeWidgetItemDoubleClicked(QTreeWidgetItem*, int))
	);

	//! @todo qt
	//popupMenu.CreatePopupMenu();

	//if (AfxGetMainWnd())
	//{
	//	CMenu* mainMenu = AfxGetMainWnd()->GetMenu();
	//	if (mainMenu)
	//	{
	//		rbool maximized = studioApp.getIMainWnd()->isMDIMaximazed();
	//		int delta = maximized ? 1 : 0;

	//		appendMenu( mainMenu->GetSubMenu( 6 + delta ), 0, &popupMenu );
	//		appendMenu( mainMenu->GetSubMenu( 6 + delta ), 1, &popupMenu );
	//	}
	//}
}

RDOTracerTreeCtrl::~RDOTracerTreeCtrl()
{
	source.Empty();
}

PTR(RDOTracerTreeItem) RDOTracerTreeCtrl::getIfItemIsDrawable(PTR(QTreeWidgetItem) pCtrlItem) const
{
	PTR(RDOTracerTreeItem) pRes = NULL;
	if (pCtrlItem)
	{
		PTR(RDOTracerTreeItem) pItem = pCtrlItem->data(0, Qt::UserRole).value<PTR(RDOTracerTreeItem)>();
		pRes = pItem && pItem->isDrawable()
			? pItem
			: NULL;
	}
	return pRes;
}

//! @todo qt
//void RDOTracerTreeCtrl::OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu )
//{
//	RDOTreeCtrl::OnInitMenuPopup( pPopupMenu, nIndex, bSysMenu );
//	CFrameWnd* pwndFrame = (CFrameWnd*)AfxGetMainWnd();
//	if( pwndFrame ) pwndFrame->SendMessage( WM_INITMENUPOPUP, WPARAM(pPopupMenu->m_hMenu), MAKELPARAM(nIndex, bSysMenu) );
//}
//
//void RDOTracerTreeCtrl::doDragDrop( RDOTracerTreeItem* item, CPoint point )
//{
//	UNUSED(point);
//
//	UINT format = tracer->getClipboardFormat();
//	if ( format ) {
//		RDOTracerSerie** ptr = (RDOTracerSerie**)::GlobalAlloc( LMEM_FIXED, sizeof( RDOTracerSerie* ) );
//		*ptr = (RDOTracerSerie*)item;
//		source.CacheGlobalData( CLIPFORMAT(format), ptr );
//		source.DoDragDrop( DROPEFFECT_COPY, NULL, &dropsource );
//		source.Empty();
//		// Dont call ::GlobalFree( ptr ), because
//		// COleDataSource::Empty() calls ::ReleaseStgMedium() for
//		// each allocated storage medium. By Microsoft's default
//		// STGMEDIUM::punkForRelease == NULL,
//		// so ::ReleaseStgMedium() calls ::GlobalFree()
//		// for each allocated STGMEDIUM::TYMED_HGLOBAL.
//		// ::GlobalFlags( ptr ) returns GMEM_INVALID_HANDLE
//		// if HGLOBAL is not a valid handle.
//	}
//}
//
//void RDOTracerTreeCtrl::OnDragDrop ( NMHDR * pNotifyStruct, LRESULT* result )
//{
//	LPNMTREEVIEW lpnmtv = (LPNMTREEVIEW)pNotifyStruct;
//	HTREEITEM hitem = lpnmtv->itemNew.hItem;
//	RDOTracerTreeItem* item = getIfItemIsDrawable( hitem );
//	if ( item  )
//		doDragDrop( item, lpnmtv->ptDrag );
//	*result = 0;
//}

void RDOTracerTreeCtrl::setModelName(CREF(tstring) modelName)
{
	rootItem.getTreeItem().setText(0, QString::fromStdString(rdo::format(ID_MODEL, modelName.c_str())));
}

void RDOTracerTreeCtrl::createItem(REF(QTreeWidgetItem) parent, REF(RDOTracerTreeItem) item, CREF(QString) name, IconType iconType)
{
	PTR(QTreeWidgetItem) pCtrlItem = new QTreeWidgetItem(&parent);
	pCtrlItem->setText(0, name);
	pCtrlItem->setIcon(0, m_iconList[iconType]);
	pCtrlItem->setData(0, Qt::UserRole, QVariant::fromValue(&item));
	item.setTreeItem(pCtrlItem);
}

void RDOTracerTreeCtrl::addResourceType(PTR(RDOTracerResType) pRTP)
{
	createItem(rtpItem.getTreeItem(), *pRTP, QString::fromStdString(pRTP->Name), IT_SUB_ROOT_2);
}

void RDOTracerTreeCtrl::addResource(PTR(RDOTracerResource) pRSS)
{
	PTR(RDOTracerResType) pRTP = pRSS->getType();
	createItem(pRTP->getTreeItem(), *pRSS, QString::fromStdString(pRSS->Name), IT_SUB_ROOT_3);

	int count = pRTP->getParamsCount();
	for (int i = 0; i < count; i++)
	{
		PTR(RDOTracerTreeItem) pParam = pRSS->getParam(i);
		ASSERT(pParam);
		createItem(pRSS->getTreeItem(), *pParam, QString::fromStdString(pRTP->getParamInfo(i)->Name), IT_VALUE);
	}
	updateResource(pRSS);
}

void RDOTracerTreeCtrl::updateResource(PTR(RDOTracerResource) pRSS)
{
	if (pRSS->isErased())
	{
		pRSS->getTreeItem().setIcon(0, m_iconList[IT_ERASED]);
	}
	else
	{
		pRSS->getTreeItem().setIcon(0, m_iconList[IT_SUB_ROOT_3]);
	}
}

void RDOTracerTreeCtrl::addPattern(PTR(RDOTracerPattern) pPAT)
{
	createItem(patItem.getTreeItem(), *pPAT, QString::fromStdString(pPAT->Name), IT_SUB_ROOT_2);
}

void RDOTracerTreeCtrl::addOperation(PTR(RDOTracerOperationBase) pOPR)
{
	createItem(pOPR->getPattern()->getTreeItem(), *pOPR, QString::fromStdString(pOPR->getName()), IT_VALUE);
}

/*void RDOTracerTreeCtrl::addIrregularEvent(PTR(RDOTracerOperation) pOpr)
{
	addOperation(pOpr);
}*/

void RDOTracerTreeCtrl::addResult(PTR(RDOTracerResult) pPMV)
{
	createItem(pmvItem.getTreeItem(), *pPMV, QString::fromStdString(pPMV->getName()), IT_VALUE);
}

void RDOTracerTreeCtrl::deleteChildren(REF(RDOTracerTreeItem) parent)
{
	QList<PTR(QTreeWidgetItem)> children = parent.getTreeItem().takeChildren();
	BOOST_FOREACH(PTR(QTreeWidgetItem) item, children)
	{
		parent.getTreeItem().removeChild(item);
	}
}

void RDOTracerTreeCtrl::clear()
{
	deleteChildren(rtpItem);
	deleteChildren(patItem);
	deleteChildren(pmvItem);
	rootItem.getTreeItem().setText(0, "Модель");
}

void RDOTracerTreeCtrl::addToNewChart(PTR(QTreeWidgetItem) pCtrlItem) const
{
	PTR(RDOTracerTreeItem) pTreeItem = getIfItemIsDrawable(pCtrlItem);
	if (pTreeItem)
	{
		tracer->addSerieToChart(static_cast<PTR(RDOTracerSerie)>(pTreeItem));
	}
}

rbool RDOTracerTreeCtrl::findInCharts(PTR(QTreeWidgetItem) pCtrlItem) const
{
	PTR(RDOTracerTreeItem) pTreeItem = getIfItemIsDrawable(pCtrlItem);
	if (pTreeItem)
	{
		PTR(RDOTracerSerie) pSerie = static_cast<PTR(RDOTracerSerie)>(pTreeItem);
		return pSerie->activateFirstDoc();
	}
	return false;
}

PTR(QTreeWidgetItem) RDOTracerTreeCtrl::getSelected() const
{
	QList<PTR(QTreeWidgetItem)> selected = selectedItems();
	return selected.size() == 1
		? selected.front()
		: NULL;
}

void RDOTracerTreeCtrl::OnAddToNewChart()
{
	addToNewChart(getSelected());
}

void RDOTracerTreeCtrl::OnUpdateAddToNewChart( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(tracer->getDrawTrace() && getIfItemIsDrawable(getSelected()));
}

void RDOTracerTreeCtrl::onTreeWidgetItemDoubleClicked(QTreeWidgetItem* pCtrlItem, int)
{
	if (!tracer->getDrawTrace())
		return;

	if (!findInCharts(pCtrlItem))
	{
		addToNewChart(pCtrlItem);
	}
}

//! @todo qt
//void RDOTracerTreeCtrl::OnRButtonDown(UINT _nFlags, CPoint point)
//{
//	UNUSED(_nFlags);
//
//	UINT uFlags;
//	HTREEITEM hitem = HitTest( point, &uFlags );
//	if ( hitem && ( TVHT_ONITEM & uFlags ) ) {
//		SelectItem( hitem );
//	}
//	if ( GetFocus() != this )
//		SetFocus();
//	CPoint pos = point;
//	ClientToScreen( &pos );
//	if ( popupMenu.m_hMenu ) popupMenu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this );
//}

void RDOTracerTreeCtrl::OnUpdateChartFindincharts(CCmdUI* pCmdUI)
{
	rbool enable = false;
	if (tracer->getDrawTrace())
	{
		PTR(RDOTracerTreeItem) pTreeItem = getIfItemIsDrawable(getSelected());
		if (pTreeItem)
		{
			PTR(RDOTracerSerie) pSerie = static_cast<PTR(RDOTracerSerie)>(pTreeItem);
			enable = pSerie->isInOneOrMoreDocs();
		}
	}
	pCmdUI->Enable(enable);
}

void RDOTracerTreeCtrl::OnChartFindincharts()
{
	findInCharts(getSelected());
}

void RDOTracerTreeCtrl::onHelpContext()
{
	QByteArray ba;
	ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_model/work_model_chart.htm\n");
	studioApp.callQtAssistant(ba);
}
