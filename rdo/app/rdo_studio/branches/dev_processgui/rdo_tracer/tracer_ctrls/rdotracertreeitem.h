#ifndef RDOTRACERTREEITEM_H
#define RDOTRACERTREEITEM_H
#pragma once

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

#endif // RDOTRACERTREEITEM_H