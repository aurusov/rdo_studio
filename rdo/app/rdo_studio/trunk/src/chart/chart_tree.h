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
#include <QtGui/qtreewidget.h>
#include <QtGui/qicon.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/chart/chart_tree_item.h"
#include "app/rdo_studio_mfc/src/action_activator/action_activator.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- ChartTree
// --------------------------------------------------------------------------------
class RDODropSource: public COleDropSource
{
private:
	virtual SCODE GiveFeedback(DROPEFFECT dropEffect);

public:
	RDODropSource()
		: COleDropSource()
	{}
	virtual ~RDODropSource()
	{}
};

class TracerResType;
class TracerResource;
class TracerPattern;
class TracerOperationBase;
class TracerResult;

class ChartTree
	: public QTreeWidget
	, public ActionActivator
{
Q_OBJECT

public:
	ChartTree(PTR(QWidget) pParent);
	virtual ~ChartTree();

	void setModelName   (CREF(tstring) modelName);
	void addResourceType(PTR(TracerResType)       pRTP);
	void addResource    (PTR(TracerResource)      pRSS);
	void updateResource (PTR(TracerResource)      pRSS);
	void addPattern     (PTR(TracerPattern)       pPAT);
	void addOperation   (PTR(TracerOperationBase) pOPR);
	void addResult      (PTR(TracerResult)        pPMV);
	void deleteChildren (REF(ChartTreeItem)       parent);
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

	ChartTreeItem  m_root;
	ChartTreeItem  m_rootRTP;
	ChartTreeItem  m_rootPAT;
	ChartTreeItem  m_rootPMV;
	IconList       m_iconList;

	COleDataSource m_source;
	RDODropSource  m_dropsource;
	CMenu          m_popupMenu;

	void createItem(REF(ChartTreeItem) parent, REF(ChartTreeItem) item, CREF(QString) name, IconType iconType);

	void  addToNewChart(PTR(QTreeWidgetItem) pCtrlItem) const;
	rbool findInCharts (PTR(QTreeWidgetItem) pCtrlItem) const;

	PTR(ChartTreeItem)   getIfItemIsDrawable(CPTR(QTreeWidgetItem) pCtrlItem) const;
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
