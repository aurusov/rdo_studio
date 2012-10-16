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
public:
	RDOTracerTreeItem(rbool drawable = false)
		: m_pCtrlItem(NULL    )
		, m_drawable (drawable)
	{}
	~RDOTracerTreeItem()
	{}

	void setCtrlItem(PTR(QTreeWidgetItem) pCtrlItem)
	{
		m_pCtrlItem = pCtrlItem;
	}
	REF(QTreeWidgetItem) getCtrlItem()
	{
		ASSERT(m_pCtrlItem)
		return *m_pCtrlItem;
	}

	rbool isDrawable() const
	{
		return m_drawable;
	}

private:
	PTR(QTreeWidgetItem) m_pCtrlItem;
	rbool                m_drawable; // set to true ONLY for RDOTracerSerie and descendants
};

#endif // _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERTREEITEM_H_
