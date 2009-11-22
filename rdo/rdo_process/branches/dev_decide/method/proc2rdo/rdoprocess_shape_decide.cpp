// rdoprocess_shape_create_MJ.cpp: implementation of the RPShapeDecide class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "rdoprocess_shape_decide.h"
//#include "rdoprocess_shape_create_dlg1_MJ.h"
#include "rdoprocess_method_proc2rdo_MJ.h"
#include <rdoprocess_shape.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class RPConnectorDockTwo: public RPConnectorDock
{
public:
	RPConnectorDockTwo( RPShape* _parent, Type _type, const rp::point& _point, double _norm, const rp::string& type = "" ): RPConnectorDock( _parent, _type, _point, _norm, type ) {};
	virtual ~RPConnectorDockTwo() {};

	virtual bool can_connect( RPConnectorDock* dock = NULL ) const {
		if ( !RPConnectorDock::can_connect( dock ) ) return false;
		return connectors.empty();
	}
};
class RPConnectorDockThree: public RPConnectorDock
{
public:
	RPConnectorDockThree( RPShape* _parent, Type _type, const rp::point& _point, double _norm, const rp::string& type = "" ): RPConnectorDock( _parent, _type, _point, _norm, type ) {};
	virtual ~RPConnectorDockThree() {};

	virtual bool can_connect( RPConnectorDock* dock = NULL ) const {
		if ( !RPConnectorDock::can_connect( dock ) ) return false;
		return connectors.empty();
	}
};
RPShapeDecide::RPShapeDecide( RPObject* _parent ):
	RPShape_MJ( _parent, _T("_Decide") )
{
	pa_src.push_back( rp::point(55, 0) );
	pa_src.push_back( rp::point(0, 55) );
	pa_src.push_back( rp::point(-55, 0) );
	pa_src.push_back( rp::point(0, -55) );
	pa_src.push_back( rp::point(55, 0) );

	docks.push_back( new RPConnectorDockOne( this, RPConnectorDock::in, rp::point(  -55, 0 ), 180, "transact" ) );
	docks.push_back( new RPConnectorDockTwo( this, RPConnectorDock::out, rp::point(  0, 55 ), 90, "transact" ) );
	docks.push_back( new RPConnectorDockThree( this, RPConnectorDock::out, rp::point(  0, -55 ), 270, "transact" ) );
}

RPShapeDecide::~RPShapeDecide()
{
}

rp::RPXMLNode* RPShapeDecide::save( rp::RPXMLNode* parent_node )
{
	rp::RPXMLNode* obj_node = RPShape_MJ::save( parent_node );
	return obj_node;
}

RPObject* RPShapeDecide::newObject( RPObject* parent )
{
	return new RPShapeDecide( parent );
}
/*
void RPShapeDecide::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	RPShapeCreateDlg1_MJ dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}
*/
void RPShapeDecide::generate()
{
	RPShape_MJ::generate();

	RPCreationRDOFilesMJ* RDOfiles = proc2rdo->RDOfiles;

	RDOfiles->resourse<<std::endl<<std::endl<<"{-ÃÅÍÅÐÈÐÓÅÌ ÄÅÑÀÉÄ-}" <<std::endl;
}