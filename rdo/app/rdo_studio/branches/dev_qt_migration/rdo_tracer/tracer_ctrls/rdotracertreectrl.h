/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracertreectrl.h
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERTREECTRL_H_
#define _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERTREECTRL_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qtreewidget.h>
#include <QtGui/qicon.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdotracertreeitem.h"
#include "app/rdo_studio_mfc/src/help_context_i.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOTracerTreeCtrl
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

class RDOTracerResType;
class RDOTracerResource;
class RDOTracerPattern;
class RDOTracerOperationBase;
class RDOTracerResult;

class RDOTracerTreeCtrl
	: public QTreeWidget
	, public IHelpContext
{
Q_OBJECT

public:
	RDOTracerTreeCtrl(PTR(QWidget) pParent);
	virtual ~RDOTracerTreeCtrl();

	void setModelName   (CREF(tstring) modelName);
	void addResourceType(PTR(RDOTracerResType)       pRTP);
	void addResource    (PTR(RDOTracerResource)      pRSS);
	void updateResource (PTR(RDOTracerResource)      pRSS);
	void addPattern     (PTR(RDOTracerPattern)       pPAT);
	void addOperation   (PTR(RDOTracerOperationBase) pOPR);
	void addResult      (PTR(RDOTracerResult)        pPMV);
	void deleteChildren (REF(RDOTracerTreeItem)      parent);
	void clear();
	//void addIrregularEvent( RDOTracerOperation* opr );

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

	RDOTracerTreeItem  m_root;
	RDOTracerTreeItem  m_rootRTP;
	RDOTracerTreeItem  m_rootPAT;
	RDOTracerTreeItem  m_rootPMV;
	IconList           m_iconList;

	COleDataSource     m_source;
	RDODropSource      m_dropsource;
	CMenu              m_popupMenu;

	void createItem(REF(RDOTracerTreeItem) parent, REF(RDOTracerTreeItem) item, CREF(QString) name, IconType iconType);

	void  addToNewChart(PTR(QTreeWidgetItem) pCtrlItem) const;
	rbool findInCharts (PTR(QTreeWidgetItem) pCtrlItem) const;

	PTR(RDOTracerTreeItem) getIfItemIsDrawable(CPTR(QTreeWidgetItem) pCtrlItem) const;
	PTR(QTreeWidgetItem)   getSelected        () const;

	DECLARE_IHelpContext;

	//! @todo qt
	//void doDragDrop( RDOTracerTreeItem* item, CPoint point );
	//afx_msg void OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu );
	//afx_msg void OnDragDrop ( NMHDR * pNotifyStruct, LRESULT* result );
	//afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnAddToNewChart();
	afx_msg void OnUpdateAddToNewChart( CCmdUI* pCmdUI );
	afx_msg void OnUpdateChartFindincharts(CCmdUI* pCmdUI);
	afx_msg void OnChartFindincharts();

private slots:
	void onTreeWidgetItemDoubleClicked(QTreeWidgetItem* pCtrlItem, int);
};

#endif // _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERTREECTRL_H_
