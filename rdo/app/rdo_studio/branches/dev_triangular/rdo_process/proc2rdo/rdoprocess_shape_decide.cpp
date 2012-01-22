// rdoprocess_shape_create_MJ.cpp: implementation of the RPShapeDecide class.
//
//////////////////////////////////////////////////////////////////////

#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_decide.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_decide_dlg1.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_method_proc2rdo.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_shape.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class RPConnectorDockTrue: public RPConnectorDock
{
public:
	RPConnectorDockTrue( RPShape* _parent, Type _type, const rp::point& _point, double _norm, const rp::string& type = "" ): RPConnectorDock( _parent, _type, _point, _norm, type ) {};
	virtual ~RPConnectorDockTrue() {};

	virtual bool can_connect( RPConnectorDock* dock = NULL ) const {
		if ( !RPConnectorDock::can_connect( dock ) ) return false;
		return connectors.empty();
	}
};
class RPConnectorDockFalse: public RPConnectorDock
{
public:
	RPConnectorDockFalse( RPShape* _parent, Type _type, const rp::point& _point, double _norm, const rp::string& type = "" ): RPConnectorDock( _parent, _type, _point, _norm, type ) {};
	virtual ~RPConnectorDockFalse() {};

	virtual bool can_connect( RPConnectorDock* dock = NULL ) const {
		if ( !RPConnectorDock::can_connect( dock ) ) return false;
		return connectors.empty();
	}
};
RPShapeDecide::RPShapeDecide( RPObject* _parent ):
	RPShape_MJ( _parent, _T("Decide") )
{

	pa_src.push_back(rp::point(-55, 0));
	pa_src.push_back(rp::point(0, -55));
	pa_src.push_back( rp::point(55, 0) );
	pa_src.push_back( rp::point(0, 55) );
	pa_src.push_back( rp::point(-55, 0) );

	docks.push_back( new RPConnectorDock( this, RPConnectorDock::in, rp::point(  -55, 0 ), 180, "transact" ) );
	docks.push_back( new RPConnectorDockOne( this, RPConnectorDock::out, rp::point( 55, 0 ), 0, "transact" ) );
	docks.push_back( new RPConnectorDockOne( this, RPConnectorDock::out, rp::point(  0, 55 ), 270, "transact" ) );
// инициализация параметров для генерирования
	 ptrue  = 0.9; 
	 pfalse = 0.1;
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

void RPShapeDecide::onLButtonDblClk( UINT nFlags, CPoint global_chart_pos )
{
	UNUSED(nFlags          );
	UNUSED(global_chart_pos);

	RPShapeDecideDlg1 dlg( AfxGetMainWnd(), this );
	dlg.DoModal();
}

void RPShapeDecide::generate()
{
	RPCreationRDOFilesMJ* RDOfiles = proc2rdo->RDOfiles;
	//заполняем лист названиями паттернов для *.opr
		CString name_value1("Блок_десайд_");
		CString name_value2(getName().c_str());
		CString name_value(name_value1 + name_value2);
		proc2rdo->list_pattern_names.push_back(name_value);
// ГЕНЕРАЦИЯ паттерное РДО ФАЙЛ *.pat

RDOfiles->pattern <<std::endl
<<std::endl<<"{-------блок десайд------------"<<getName().c_str()<<"---------------------------------}"
<<std::endl
<<std::endl
<<std::endl<<"$Pattern Блок_десайд_"<<getName().c_str()<<" : rule  {срабатывание закона}trace"
<<std::endl<<"	$Relevant_resources"
<<std::endl
<<std::endl<<"		_transact_X : Group_of_transacts_X keep"
<<std::endl
<<std::endl<<"$Body"
<<std::endl 
<<std::endl<<"  _transact_X"
<<std::endl<<"		 Choice from _transact_X.Состояние_транспортировки = ожидает and"
<<std::endl<<"		_transact_X.Место_нахождения_будущее = "<<getName().c_str()
<<std::endl<<"			first"
<<std::endl	 
<<std::endl
<<std::endl<<"	Convert_rule"
<<std::endl<<"		  Место_нахождения            set "<<getName().c_str()
<<std::endl<<"			Место_нахождения_будущее    set ГИСТ_"<<getName().c_str()
<<std::endl	 
<<std::endl<<"$End";

RDOfiles->function<<std::endl<<"{-------блок десайд ------" <<getName().c_str()<<"--------------}" <<std::endl
<<std::endl<<"$Sequence ГИСТ_"<<getName().c_str()<<": such_as Group_of_transacts_X.Место_нахождения"
	<<std::endl<<"$Type = by_hist"
	<<std::endl<<"$Body"
<<std::endl<<id_next.c_str()<<"  "<<ptrue
<<std::endl<<id_next2.c_str()<<"  "<<pfalse

	<<std::endl<<"	$End";
}