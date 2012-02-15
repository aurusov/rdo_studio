// rdoprocess_shape_terminate_MJ.h: interface for the RPShapeTerminate_MJ class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDOPROCESS_SHAPE_TERMINATE_MJ_H__86CC09DB_DB23_4B8C_9EC4_080002103CDF__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_TERMINATE_MJ_H__86CC09DB_DB23_4B8C_9EC4_080002103CDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape.h"
#include "simulator/compiler/procgui/procgui_datablock.h"

class RPShapeTerminateMJ : public RPShape_MJ
{
friend class RPMethodProc2RDO_MJ;

private:
	static RPObject* newObject( RPObject* parent );

public:
	tstring m_name;
	int     m_term_inc;

	LPRPShapeDataBlockTerminate m_pParams;

	RPShapeTerminateMJ( RPObject* parent );
	virtual ~RPShapeTerminateMJ();

	virtual rp::string getClassName() const { return "RPShapeTerminateMJ"; }

    virtual void onLButtonDblClk( UINT nFlags, CPoint global_chart_pos );
	virtual void generate();
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_TERMINATE_MJ_H__86CC09DB_DB23_4B8C_9EC4_080002103CDF__INCLUDED_)
