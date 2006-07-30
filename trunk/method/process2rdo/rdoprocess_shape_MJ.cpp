#include "stdafx.h"
#include "rdoprocess_shape_MJ.h"
#include "rdo_process_project_RDO_proc_MJ.h"
#include "../../rdoprocess_app.h"

// ----------------------------------------------------------------------------
// ---------- RPObject_MJ
// ----------------------------------------------------------------------------
void RPObject_MJ::list_name_for_resource_MJ( std::list< RPObject* >* list_resource )
{
	std::list< RPObject* >::const_iterator it = obj->getChild().begin();
	while ( it != obj->getChild().end() ) {
		RPObject* obj_it = *it;
	    if ( (*it)->getType() == "RPShapeResource_MJ" ) {
			TRACE( "%s\n", (*it)->getName().c_str() );
			list_resource->push_back(*it);
		}
		if ( (*it)->isParent( "RPShape_MJ" ) || (*it)->isParent( "RPObjectFlowChart_MJ" ) ) {
			dynamic_cast<RPObject_MJ*>(*it)->list_name_for_resource_MJ( list_resource );
		}
		it++;
	}
}

// MJ start 30.03.06  бегает по list объектов и вызывает генерирование
void RPObject_MJ::list_name()
{
	std::list< RPObject* >::const_iterator it = obj->getChild().begin();
	while( it != obj->getChild().end() ) {
		if ( (*it)->isChartObject() ) {
			rp::string type = (*it)->getType();
			if ( type == "RPShapeCreateMJ" || type == "RPShapeProcessMJ" || type == "RPShapeTerminateMJ" ) {
				std::list< RPShape* > list = static_cast<RPShape*>(*it)->getNextBlock();
				if ( !list.empty() ) {
					dynamic_cast<RPObject_MJ*>(*it)->id_next = list.front()->getName();
				}
			}
		}
		if ( (*it)->isParent( "RPShape_MJ" ) || (*it)->isParent( "RPObjectFlowChart_MJ" ) ) {
			dynamic_cast<RPObject_MJ*>(*it)->list_name();
		}
		(*it)->generate();
		it++;
	}
}

void RPObject_MJ::list_name_for_type_res()
{
	RPCreationRDOFilesMJ* RDOfiles = static_cast<RPProject_RDO_Proc_MJ*>(rpapp.project())->RDOfiles;
	std::list< RPObject* >::const_iterator it = obj->getChild().begin();
	while ( it != obj->getChild().end() ) {
		RDOfiles->typeres << (*it)->getName().c_str() << ", ";
		if ( (*it)->isParent( "RPShape_MJ" ) || (*it)->isParent( "RPObjectFlowChart_MJ" ) ) {
			dynamic_cast<RPObject_MJ*>(*it)->list_name_for_type_res();
		}
		it++;
	}
}
