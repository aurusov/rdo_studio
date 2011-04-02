#include "stdafx.h"
#include "rdoprocess_shape_MJ.h"
#include "rdoprocess_method_proc2rdo_MJ.h"
#include "rdoprocess_shape_process_MJ.h" 
#include "rdoprocess_shape_create_MJ.h"
#include "rdoprocess_shape_decide.h"
#include "rdoprocess_shape_terminate_MJ.h"
#include "rdoprocess_shape_resource.h"

// ----------------------------------------------------------------------------
// ---------- RPObjectFlowChart_MJ
// ----------------------------------------------------------------------------
RPObjectFlowChart_MJ::RPObjectFlowChart_MJ( RPObject* parent ):
	RPObjectFlowChart( parent ),
	RPObject_MJ( get_this() )
{}

RPObject* RPObjectFlowChart_MJ::newObject( RPObject* _parent )
{
	return new RPObjectFlowChart_MJ( _parent );
}

rpMethod::RPMethod* RPObjectFlowChart_MJ::getMethod()
{
	return proc2rdo;
}
