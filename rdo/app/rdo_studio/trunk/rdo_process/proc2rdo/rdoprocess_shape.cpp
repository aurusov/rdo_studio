#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_method_proc2rdo.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_process.h" 
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_decide.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_terminate.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_resource.h"

// --------------------------------------------------------------------------------
// -------------------- RPProcessFlowChart
// --------------------------------------------------------------------------------
RPProcessFlowChart::RPProcessFlowChart( RPObject* parent ):
	RPObjectFlowChart( parent ),
	RPProcessObject( get_this() )
{
	after_constructor();
/*
	RPProcessShapeCreate* shape_create = new RPProcessShapeCreate( this );
	shape_create->setPosition( 100, 100 );
	shape_create->gamount = 1000;
	shape_create->gname   = "Входной_склад";
	shape_create->setName( tstring("Входной_склад") );
	shape_create->gtype = 2;
	shape_create->gmin  = 10;
	shape_create->gmax  = 30;
	
	RPProcessShapeProcess* shape_process = new RPProcessShapeProcess( this );
	shape_process->setPosition( 100, 150 );
	shape_process->setName( tstring("Перевозка_станок") );
	shape_process->gtype = 3;
	shape_process->gexp  = 7;

	shape_process = new RPProcessShapeProcess( this );
	shape_process->setPosition( 100, 200 );
	shape_process->gtype = 1;
	shape_process->gdisp = 2;
	shape_process->gexp  = 15;

	shape_process = new RPProcessShapeProcess( this );
	shape_process->setPosition( 100, 250 );
	shape_process->setName( tstring("Перевозка_склад") );
	shape_process->gtype = 3;
	shape_process->gexp  = 5;

	RPProcessShapeTerminate* shape_terminate = new RPProcessShapeTerminate( this );
	shape_terminate->setPosition( 100, 200 );
	shape_terminate->setName( tstring("Склад") );

	RPProcessShapeResource* shape_resource = new RPProcessShapeResource( this );
	shape_resource->setPosition( 100, 250 );

	shape_resource = new RPProcessShapeResource( this );
	shape_resource->setPosition( 100, 300 );
*/
}

RPObject* RPProcessFlowChart::newObject( RPObject* _parent )
{
	return new RPProcessFlowChart( _parent );
}

rpMethod::RPMethod* RPProcessFlowChart::getMethod()
{
	return proc2rdo;
}

void RPProcessFlowChart::saveToXML(REF(pugi::xml_node) parentNode) const
{
	pugi::xml_node node = parentNode.append_child(getClassName().c_str());
	node.append_attribute(_T("name")).set_value(getName().c_str());

	std::list<PTR(RPObject)> all_child;
	getAllChild(all_child);

	STL_FOR_ALL_CONST(all_child, it)
	{
		(*it)->saveToXML(node);
	}
}

void RPProcessFlowChart::loadFromXML(CREF(pugi::xml_node) node)
{
	setName(node.first_attribute().value());
	for(pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
	{
		if (strcmp(child.name(), "RPObjectPixmap") != 0)
		{
			PTR(RPObject) pFlowObject = static_cast<PTR(RPObject)>(rpMethod::factory->getNewObject(child.name(), this));
			ASSERT(pFlowObject);
			pFlowObject->loadFromXML(child);
		}
	}
}
