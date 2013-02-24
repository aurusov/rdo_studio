/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_tree.cpp
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
#include <QtCore/qprocess.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/chart/chart_tree.h"
#include "app/rdo_studio/src/tracer/rdotracer.h"
#include "app/rdo_studio/src/tracer/rdotracerrestype.h"
#include "app/rdo_studio/src/tracer/rdotracerresource.h"
#include "app/rdo_studio/src/tracer/rdotracerpattern.h"
#include "app/rdo_studio/src/tracer/rdotraceroperation.h"
#include "app/rdo_studio/src/tracer/rdotracerresult.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_frm.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdo::gui::tracer;

SCODE RDODropSource::GiveFeedback(DROPEFFECT dropEffect)
{
	return COleDropSource::GiveFeedback(dropEffect);
}

//! @todo qt
//BEGIN_MESSAGE_MAP(ChartTree, RDOTreeCtrl)
//	ON_WM_INITMENUPOPUP()
//	ON_COMMAND(ID_CHART_ADDTONEWCHART, OnAddToNewChart)
//	ON_UPDATE_COMMAND_UI(ID_CHART_ADDTONEWCHART, OnUpdateAddToNewChart)
//	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnDragDrop)
//	ON_WM_RBUTTONDOWN()
//	ON_UPDATE_COMMAND_UI(ID_CHART_FINDINCHARTS, OnUpdateChartFindincharts)
//	ON_COMMAND(ID_CHART_FINDINCHARTS, OnChartFindincharts)
//	ON_COMMAND(ID_CHART_EXPORT, OnExportChart)
//	ON_UPDATE_COMMAND_UI(ID_CHART_EXPORT, OnUpdateExportChart)
//END_MESSAGE_MAP()

Q_DECLARE_METATYPE(const ChartTreeItem*);

ChartTree::ChartTree(PTR(QWidget) pParent)
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

	m_root    = rdo::Factory<ChartTreeItem>::create();
	m_rootRTP = rdo::Factory<ChartTreeItem>::create();
	m_rootPAT = rdo::Factory<ChartTreeItem>::create();
	m_rootPMV = rdo::Factory<ChartTreeItem>::create();

	m_root->setCtrlItem(new QTreeWidgetItem(this));
	m_root->getCtrlItem().setText(0, QString::fromStdWString(L"Модель"));
	m_root->getCtrlItem().setIcon(0, m_iconList[IT_ROOT]);

	createItem(m_root, m_rootRTP, QString::fromStdWString(L"Типы ресурсов"), IT_SUB_ROOT_1);
	createItem(m_root, m_rootPAT, QString::fromStdWString(L"Образцы"),       IT_SUB_ROOT_1);
	createItem(m_root, m_rootPMV, QString::fromStdWString(L"Результаты"),    IT_SUB_ROOT_1);

	m_root->getCtrlItem().setExpanded(true);

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

	//		appendMenu(mainMenu->GetSubMenu(6 + delta), 2, &popupMenu);
	//	}
	//}
}

ChartTree::~ChartTree()
{
	m_source.Empty();
}

LPChartTreeItem ChartTree::getIfItemIsDrawable(CPTR(QTreeWidgetItem) pCtrlItem) const
{
	LPChartTreeItem pRes;
	if (pCtrlItem)
	{
		PTR(ChartTreeItem) pItem = const_cast<PTR(ChartTreeItem)>(pCtrlItem->data(0, Qt::UserRole).value<CPTR(ChartTreeItem)>());
		pRes = pItem && pItem->isDrawable()
			? pItem
			: NULL;
	}
	return pRes;
}

//! @todo qt
//void ChartTree::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
//{
//	RDOTreeCtrl::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
//	CFrameWnd* pwndFrame = (CFrameWnd*)AfxGetMainWnd();
//	if(pwndFrame) pwndFrame->SendMessage(WM_INITMENUPOPUP, WPARAM(pPopupMenu->m_hMenu), MAKELPARAM(nIndex, bSysMenu));
//}
//
//void ChartTree::doDragDrop(ChartTreeItem* item, CPoint point)
//{
//	UNUSED(point);
//
//	UINT format = g_pTracer->getClipboardFormat();
//	if (format) {
//		TracerSerie** ptr = (TracerSerie**)::GlobalAlloc(LMEM_FIXED, sizeof(TracerSerie*));
//		*ptr = (TracerSerie*)item;
//		source.CacheGlobalData(CLIPFORMAT(format), ptr);
//		source.DoDragDrop(DROPEFFECT_COPY, NULL, &dropsource);
//		source.Empty();
//		// Dont call ::GlobalFree(ptr), because
//		// COleDataSource::Empty() calls ::ReleaseStgMedium() for
//		// each allocated storage medium. By Microsoft's default
//		// STGMEDIUM::punkForRelease == NULL,
//		// so ::ReleaseStgMedium() calls ::GlobalFree()
//		// for each allocated STGMEDIUM::TYMED_HGLOBAL.
//		// ::GlobalFlags(ptr) returns GMEM_INVALID_HANDLE
//		// if HGLOBAL is not a valid handle.
//	}
//}
//
//void ChartTree::OnDragDrop (NMHDR * pNotifyStruct, LRESULT* result)
//{
//	LPNMTREEVIEW lpnmtv = (LPNMTREEVIEW)pNotifyStruct;
//	HTREEITEM hitem = lpnmtv->itemNew.hItem;
//	ChartTreeItem* item = getIfItemIsDrawable(hitem);
//	if (item )
//		doDragDrop(item, lpnmtv->ptDrag);
//	*result = 0;
//}

void ChartTree::setModelName(CREF(QString) modelName)
{
	m_root->getCtrlItem().setText(0, QString::fromLocal8Bit("Модель : %1").arg(modelName));
}

void ChartTree::createItem(CREF(LPChartTreeItem) parent, CREF(LPChartTreeItem) item, CREF(QString) name, IconType iconType)
{
	PTR(QTreeWidgetItem) pCtrlItem = new QTreeWidgetItem(&parent->getCtrlItem());
	pCtrlItem->setText(0, name);
	pCtrlItem->setIcon(0, m_iconList[iconType]);
	//! @todo smart_ptr
	const ChartTreeItem* pRawItem = item.get();
	pCtrlItem->setData(0, Qt::UserRole, QVariant::fromValue(pRawItem));
	item->setCtrlItem(pCtrlItem);
}

void ChartTree::addResourceType(CREF(LPTracerResourceType) pRTP)
{
	ASSERT(pRTP);
	createItem(m_rootRTP, pRTP, pRTP->getName(), IT_SUB_ROOT_2);
}

void ChartTree::addResource(CREF(LPTracerResource) pRSS)
{
	LPTracerResourceType pRTP = pRSS->getType();
	ASSERT(pRTP);
	ASSERT(pRSS);
	createItem(pRTP, pRSS, pRSS->getName(), IT_SUB_ROOT_3);

	int count = pRTP->getParamsCount();
	for (int i = 0; i < count; i++)
	{
		LPChartTreeItem pParam = pRSS->getParam(i);
		ASSERT(pParam);
		createItem(pRSS, pParam, pRTP->getParamInfo(i)->getName(), IT_VALUE);
	}
	updateResource(pRSS);
}

void ChartTree::updateResource(CREF(LPTracerResource) pRSS)
{
	if (pRSS->isErased())
	{
		pRSS->getCtrlItem().setIcon(0, m_iconList[IT_ERASED]);
	}
	else
	{
		pRSS->getCtrlItem().setIcon(0, m_iconList[IT_SUB_ROOT_3]);
	}
}

void ChartTree::addPattern(CREF(LPTracerPattern) pPAT)
{
	ASSERT(pPAT);
	createItem(m_rootPAT, pPAT, pPAT->getName(), IT_SUB_ROOT_2);
}

void ChartTree::addOperation(CREF(LPTracerOperationBase) pOPR)
{
	createItem(pOPR->getPattern(), pOPR, pOPR->getName(), IT_VALUE);
}

void ChartTree::addResult(CREF(LPTracerResult) pPMV)
{
	createItem(m_rootPMV, pPMV, pPMV->getName(), IT_VALUE);
}

void ChartTree::deleteChildren(CREF(LPChartTreeItem) pParent)
{
	QList<PTR(QTreeWidgetItem)> children = pParent->getCtrlItem().takeChildren();
	BOOST_FOREACH(PTR(QTreeWidgetItem) item, children)
	{
		pParent->getCtrlItem().removeChild(item);
	}
}

void ChartTree::clear()
{
	deleteChildren(m_rootRTP);
	deleteChildren(m_rootPAT);
	deleteChildren(m_rootPMV);
	m_root->getCtrlItem().setText(0, QString::fromStdWString(L"Модель"));
}

void ChartTree::addToNewChart(PTR(QTreeWidgetItem) pCtrlItem) const
{
	LPTracerSerie pSerie = getIfItemIsDrawable(pCtrlItem).object_dynamic_cast<TracerSerie>();
	if (pSerie)
	{
		g_pTracer->addSerieToChart(pSerie);
	}
}

rbool ChartTree::findInCharts(PTR(QTreeWidgetItem) pCtrlItem) const
{
	LPTracerSerie pSerie = getIfItemIsDrawable(pCtrlItem).object_dynamic_cast<TracerSerie>();
	if (pSerie)
	{
		return pSerie->activateFirstDoc();
	}
	return false;
}

PTR(QTreeWidgetItem) ChartTree::getSelected() const
{
	QList<PTR(QTreeWidgetItem)> selected = selectedItems();
	return selected.size() == 1
		? selected.front()
		: NULL;
}

void ChartTree::OnAddToNewChart()
{
	addToNewChart(getSelected());
}

void ChartTree::OnUpdateAddToNewChart(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(g_pTracer->getDrawTrace() && getIfItemIsDrawable(getSelected()));
}

void ChartTree::onTreeWidgetItemDoubleClicked(QTreeWidgetItem* pCtrlItem, int)
{
	if (!g_pTracer->getDrawTrace())
		return;

	if (!findInCharts(pCtrlItem))
	{
		addToNewChart(pCtrlItem);
	}
}

//! @todo qt
//void ChartTree::OnRButtonDown(UINT _nFlags, CPoint point)
//{
//	UNUSED(_nFlags);
//
//	UINT uFlags;
//	HTREEITEM hitem = HitTest(point, &uFlags);
//	if (hitem && (TVHT_ONITEM & uFlags)) {
//		SelectItem(hitem);
//	}
//	if (GetFocus() != this)
//		SetFocus();
//	CPoint pos = point;
//	ClientToScreen(&pos);
//	if (popupMenu.m_hMenu) popupMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this);
//}
//
//void TracerTreeCtrl::OnExportChart()
//{
//	if (!g_pTracer->getDrawTrace())
//		return;
//
//	PTR(TracerTreeItem) pItem = getIfItemIsDrawable(GetSelectedItem());
//	if (!pItem)
//		return;
//
//	PTR(TracerSerie) pSerie = static_cast<PTR(TracerSerie)>(pItem);
//	ASSERT(pSerie);
//	TracerSerie::ExportData exportData = pSerie->exportData();
//	if (exportData.empty())
//		return;
//
//	CString filter("csv-файл (*.csv)|*.csv|Все файлы (*.*)|*.*||");
//	CFileDialog dlg(false, _T("csv"), pSerie->getTitle().c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, AfxGetMainWnd());
//	if (dlg.DoModal() != IDOK)
//		return;
//
//	CString fileName = dlg.GetPathName();
//	std::ofstream stream(fileName);
//	if (!stream.is_open())
//		return;
//
//	BOOST_FOREACH(CREF(TracerSerie::ExportData::value_type) exportItem, exportData)
//	{
//		stream << exportItem << std::endl;
//	}
//
//	stream.close();
//}
//
//void TracerTreeCtrl::OnUpdateExportChart(CCmdUI* pCmdUI)
//{
//	pCmdUI->Enable(g_pTracer->getDrawTrace() && getIfItemIsDrawable(GetSelectedItem()) != NULL);
//}

void ChartTree::OnUpdateChartFindincharts(CCmdUI* pCmdUI)
{
	rbool enable = false;
	if (g_pTracer->getDrawTrace())
	{
		LPTracerSerie pSerie = getIfItemIsDrawable(getSelected()).object_dynamic_cast<TracerSerie>();
		if (pSerie)
		{
			enable = pSerie->isInOneOrMoreDocs();
		}
	}
	pCmdUI->Enable(enable);
}

void ChartTree::OnChartFindincharts()
{
	findInCharts(getSelected());
}

void ChartTree::focusInEvent(QFocusEvent* pEvent)
{
	parent_type::focusInEvent(pEvent);
	activate(pEvent);
}

void ChartTree::focusOutEvent(QFocusEvent* pEvent)
{
	parent_type::focusOutEvent(pEvent);
	deactivate(pEvent);
}

void ChartTree::onUpdateActions(rbool activated)
{
	RDOStudioMainFrame* pMainWindow = studioApp.getMainWndUI();
	ASSERT(pMainWindow);

	updateAction(
		pMainWindow->actHelpContext,
		activated,
		this, &ChartTree::onHelpContext
	);
}

void ChartTree::onHelpContext()
{
	QByteArray ba;
	ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_model/work_model_chart.htm\n");
	studioApp.callQtAssistant(ba);
}
