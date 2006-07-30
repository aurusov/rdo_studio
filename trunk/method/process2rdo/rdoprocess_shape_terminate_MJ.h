// rdoprocess_shape_terminate_MJ.h: interface for the RPShapeTerminate_MJ class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDOPROCESS_SHAPE_TERMINATE_MJ_H__86CC09DB_DB23_4B8C_9EC4_080002103CDF__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_TERMINATE_MJ_H__86CC09DB_DB23_4B8C_9EC4_080002103CDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_shape_MJ.h"
#include "../../rdoprocess_app.h"

class RPShapeTerminateMJ : public RPShape_MJ
{
public:
	RPShapeTerminateMJ( RPObject* parent );
	virtual ~RPShapeTerminateMJ();

	virtual rp::string getType() { return "RPShapeTerminateMJ"; }

    virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos );
	virtual void generate();
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_TERMINATE_MJ_H__86CC09DB_DB23_4B8C_9EC4_080002103CDF__INCLUDED_)
