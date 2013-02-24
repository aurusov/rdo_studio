/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_tree.h
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _APP_RDO_STUDIO_CHART_TREE_H_
#define _APP_RDO_STUDIO_CHART_TREE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtWidgets/qtreewidget.h>
#include <QtGui/qicon.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/chart/chart_tree_item.h"
#include "app/rdo_studio/src/action_activator/action_activator.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- ChartTree
// --------------------------------------------------------------------------------
class RDODropSource: public COleDropSource
{
public:
	RDODropSource()
		: COleDropSource()
	{}
	virtual ~RDODropSource()
	{}

private:
	virtual SCODE GiveFeedback(DROPEFFECT dropEffect);
};

PREDECLARE_POINTER(TracerResourceType);
PREDECLARE_POINTER(TracerResource);
PREDECLARE_POINTER(TracerPattern);
PREDECLARE_POINTER(TracerOperationBase);
PREDECLARE_POINTER(TracerResult);

class ChartTree
	: public QTreeWidget
	, public ActionActivator
{
Q_OBJECT

public:
	ChartTree(PTR(QWidget) pParent);
	virtual ~ChartTree();

	void setModelName   (CREF(QString) modelName);
	void addResourceType(CREF(LPTracerResourceType)       pRTP);
	void addResource    (CREF(LPTracerResource)      pRSS);
	void updateResource (CREF(LPTracerResource)      pRSS);
	void addPattern     (CREF(LPTracerPattern)       pPAT);
	void addOperation   (CREF(LPTracerOperationBase) pOPR);
	void addResult      (CREF(LPTracerResult)        pPMV);
	void deleteChildren (CREF(LPChartTreeItem)       pParent);
	void clear();
	//void addIrregularEvent( TracerOperation* opr );

private:
	typedef  QTreeWidget  parent_type;

	enum IconType
	{
		IT_ROOT       = 0,
		IT_SUB_ROOT_1,
		IT_SUB_ROOT_2,
		IT_SUB_ROOT_3,
		IT_VALUE,
		IT_ERASED,
		IT_COUNT
	};
	typedef  std::vector<QIcon>  IconList;

	LPChartTreeItem  m_root;
	LPChartTreeItem  m_rootRTP;
	LPChartTreeItem  m_rootPAT;
	LPChartTreeItem  m_rootPMV;
	IconList         m_iconList;

	COleDataSource m_source;
	RDODropSource  m_dropsource;
	CMenu          m_popupMenu;

	void createItem(CREF(LPChartTreeItem) parent, CREF(LPChartTreeItem) item, CREF(QString) name, IconType iconType);

	void  addToNewChart(PTR(QTreeWidgetItem) pCtrlItem) const;
	rbool findInCharts (PTR(QTreeWidgetItem) pCtrlItem) const;

	LPChartTreeItem      getIfItemIsDrawable(CPTR(QTreeWidgetItem) pCtrlItem) const;
	PTR(QTreeWidgetItem) getSelected        () const;

	virtual void focusInEvent   (QFocusEvent* pEvent);
	virtual void focusOutEvent  (QFocusEvent* pEvent);
	virtual void onUpdateActions(rbool activated);

	//! @todo qt
	//void doDragDrop( ChartTreeItem* item, CPoint point );
	//afx_msg void OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu );
	//afx_msg void OnDragDrop ( NMHDR * pNotifyStruct, LRESULT* result );
	//afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnExportChart();
	//afx_msg void OnUpdateExportChart( CCmdUI* pCmdUI );
	afx_msg void OnAddToNewChart();
	afx_msg void OnUpdateAddToNewChart( CCmdUI* pCmdUI );
	afx_msg void OnUpdateChartFindincharts(CCmdUI* pCmdUI);
	afx_msg void OnChartFindincharts();

private slots:
	void onTreeWidgetItemDoubleClicked(QTreeWidgetItem* pCtrlItem, int);
	void onHelpContext();
};

#endif // _APP_RDO_STUDIO_CHART_TREE_H_
