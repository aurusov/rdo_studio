/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      frame_tree_ctrl.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.03.2003
  \brief     Дерево кадров анимации
  \indent    4T
*/

#ifndef _RDO_STUDIO_FRAME_TREE_CTRL_H_
#define _RDO_STUDIO_FRAME_TREE_CTRL_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QTreeWidget>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/action_activator/action_activator.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace frame {

class TreeCtrl
	: public QTreeWidget
	, public ActionActivator
{
Q_OBJECT

public:
	TreeCtrl(QWidget* pParent);
	virtual ~TreeCtrl();

	QTreeWidgetItem* insertFrame(const QString& name);

	void clear();

private:
	typedef  QTreeWidget  parent_type;

	QTreeWidgetItem* m_pRootItem;

	virtual void focusInEvent   (QFocusEvent* pEvent);
	virtual void focusOutEvent  (QFocusEvent* pEvent);
	virtual void onUpdateActions(bool activated);

private slots:
	void onHelpContext();
};

}}} // namespace rdo::gui::frame

#endif // _RDO_STUDIO_FRAME_TREE_CTRL_H_
