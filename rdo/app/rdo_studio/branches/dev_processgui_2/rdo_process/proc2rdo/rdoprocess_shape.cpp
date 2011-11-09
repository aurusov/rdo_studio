#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_method_proc2rdo.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_process.h" 
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_decide.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_terminate.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_resource.h"

// --------------------------------------------------------------------------------
// -------------------- RPObjectFlowChart_MJ
// --------------------------------------------------------------------------------
RPObjectFlowChart_MJ::RPObjectFlowChart_MJ( RPObject* parent ):
	RPObjectFlowChart( parent ),
	RPObject_MJ( get_this() )
{
	after_constructor();
/*
	RPShapeCreateMJ* shape_create = new RPShapeCreateMJ( this );
	shape_create->setPosition( 100, 100 );
	shape_create->gamount = 1000;
	shape_create->gname   = "Входной_склад";
	shape_create->setName( std::string("Входной_склад") );
	shape_create->gtype = 2;
	shape_create->gmin  = 10;
	shape_create->gmax  = 30;
	
	RPShapeProcessMJ* shape_process = new RPShapeProcessMJ( this );
	shape_process->setPosition( 100, 150 );
	shape_process->setName( std::string("Перевозка_станок") );
	shape_process->gtype = 3;
	shape_process->gexp  = 7;

	shape_process = new RPShapeProcessMJ( this );
	shape_process->setPosition( 100, 200 );
	shape_process->gtype = 1;
	shape_process->gdisp = 2;
	shape_process->gexp  = 15;

	shape_process = new RPShapeProcessMJ( this );
	shape_process->setPosition( 100, 250 );
	shape_process->setName( std::string("Перевозка_склад") );
	shape_process->gtype = 3;
	shape_process->gexp  = 5;

	RPShapeTerminateMJ* shape_terminate = new RPShapeTerminateMJ( this );
	shape_terminate->setPosition( 100, 200 );
	shape_terminate->setName( std::string("Склад") );

	RPShapeResource_MJ* shape_resource = new RPShapeResource_MJ( this );
	shape_resource->setPosition( 100, 250 );

	shape_resource = new RPShapeResource_MJ( this );
	shape_resource->setPosition( 100, 300 );
*/
}

RPObject* RPObjectFlowChart_MJ::newObject( RPObject* _parent )
{
	return new RPObjectFlowChart_MJ( _parent );
}

rpMethod::RPMethod* RPObjectFlowChart_MJ::getMethod()
{
	return proc2rdo;
}

void RPObjectFlowChart_MJ::Save_To_XML(pugi::xml_node &node)
{
	// Заносим в список потомков FlowChart'а:
	std::list< RPObject* > all_child;
	getAllChild(all_child);
	 
	// Пробегая по листу, вызываем для каждого его элемента соответствующую функцию-потомок Save_To_XML(node):
	for(std::list< RPObject* >::const_iterator list_iterator = all_child.begin(); list_iterator != all_child.end(); ++list_iterator)
		(*list_iterator)->Save_To_XML(node);
}