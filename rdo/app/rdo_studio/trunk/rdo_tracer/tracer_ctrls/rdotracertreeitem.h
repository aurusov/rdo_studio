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
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOTracerTreeItem
// --------------------------------------------------------------------------------
class RDOTracerTreeItem
{
protected:
	HTREEITEM treeItem;
	bool drawable; // set to true ONLY for RDOTracerSerie and descendants
public:
	RDOTracerTreeItem( const bool _drawable = false ) : treeItem ( NULL ), drawable( _drawable ) {};
	~RDOTracerTreeItem() {};
	void setTreeItem( HTREEITEM item ) { treeItem = item; };
	const HTREEITEM getTreeItem() const { return treeItem; };
	const bool isDrawable() const { return drawable; };
};

#endif // _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERTREEITEM_H_