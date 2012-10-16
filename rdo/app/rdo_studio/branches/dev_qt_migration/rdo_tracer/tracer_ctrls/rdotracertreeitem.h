/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracertreeitem.h
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERTREEITEM_H_
#define _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERTREEITEM_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qtreewidget.h>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class RDOTracerTreeItem
{
protected:
	PTR(QTreeWidgetItem) m_pTreeItem;
	rbool                m_drawable; // set to true ONLY for RDOTracerSerie and descendants

public:
	RDOTracerTreeItem(const rbool drawable = false)
		: m_pTreeItem(NULL)
		, m_drawable (drawable)
	{}
	~RDOTracerTreeItem()
	{}

	void setTreeItem(PTR(QTreeWidgetItem) pTreeItem)
	{
		m_pTreeItem = pTreeItem;
	}
	REF(QTreeWidgetItem) getTreeItem()
	{
		ASSERT(m_pTreeItem)
		return *m_pTreeItem;
	}

	const rbool isDrawable() const
	{
		return m_drawable;
	}
};

#endif // _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERTREEITEM_H_