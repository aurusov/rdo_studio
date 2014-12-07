#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QTreeWidget>
#include <QMenu>
#include <QIcon>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/chart/chart_tree_item.h"
#include "app/rdo_studio/src/action_activator/action_activator.h"
#include "app/rdo_studio/src/tracer/chart/chart_serie.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

PREDECLARE_POINTER(ResourceType);
PREDECLARE_POINTER(Resource);
PREDECLARE_POINTER(Pattern);
PREDECLARE_POINTER(OperationBase);
PREDECLARE_POINTER(Result);

class ChartTree
	: public QTreeWidget
	, public ActionActivator
{
Q_OBJECT

public:
	ChartTree(QWidget* pParent);
	virtual ~ChartTree();

	void setModelName   (const QString&    modelName);
	void addResourceType(const LPResourceType&  pRTP);
	void addResource    (const LPResource&      pRSS);
	void updateResource (const LPResource&      pRSS);
	void addPattern     (const LPPattern&       pPAT);
	void addOperation   (const LPOperationBase& pOPR);
	void addResult      (const LPResult&        pPMV);
	void deleteChildren (const LPChartTreeItem& pParent);
	void clear();

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
	QMenu*           m_pPopupMenu;

	void createItem(const LPChartTreeItem& parent, const LPChartTreeItem& item, const QString& name, IconType iconType);

	void  createChart          (QTreeWidgetItem* pCtrlItem) const;
	bool  activateExistingChart(QTreeWidgetItem* pCtrlItem) const;

	LPChartTreeItem getIfItemIsDrawable(const QTreeWidgetItem* pCtrlItem) const;
	QTreeWidgetItem* getSelected() const;

	virtual void focusInEvent    (QFocusEvent* pEvent);
	virtual void focusOutEvent   (QFocusEvent* pEvent);
	virtual void mousePressEvent (QMouseEvent* pEvent);
	virtual void contextMenuEvent(QContextMenuEvent* pEvent);
	virtual void onUpdateActions (bool activated);

	void doDragDrop(const LPSerie& pSerie);

private slots:
	void onTreeWidgetItemDoubleClicked(QTreeWidgetItem* pCtrlItem, int);
	void onChartCreate();
	void onChartExport();
	void onHelpContext();
};

}}} // namespace rdo::gui::tracer
