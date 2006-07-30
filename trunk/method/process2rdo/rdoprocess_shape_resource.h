// rdoprocess_shape_resource.h: interface for the RPShapeResource_MJ class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDOPROCESS_SHAPE_RESOURCE_H__108AB1F9_8742_446F_9CB2_ABD461A09C12__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_RESOURCE_H__108AB1F9_8742_446F_9CB2_ABD461A09C12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_shape_MJ.h"
#include "../../rdoprocess_app.h"

class RPShapeResource_MJ : public RPShape_MJ
{
public:
	RPShapeResource_MJ( RPObject* parent );
	virtual ~RPShapeResource_MJ();

	virtual rp::string getType() { return "RPShapeResource_MJ"; }

	virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos );
	virtual void generate();

	int gamount;
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_RESOURCE_H__108AB1F9_8742_446F_9CB2_ABD461A09C12__INCLUDED_)
