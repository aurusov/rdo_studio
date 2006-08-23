#include "stdafx.h"

#define RP_METHOD_EXPORTS
#include "rdoprocess_method_algorithm.h"

#include "rdoprocess_shape_action.h"
#include "rdoprocess_shape_if.h"
#include <rdoprocess_factory.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPMethodAlgorithm
// ----------------------------------------------------------------------------
RPMethodAlgorithm::RPMethodAlgorithm( RPObject* _parent ):
	RPMethod( _parent )
{
}

RPMethodAlgorithm::~RPMethodAlgorithm()
{
}

rpMethod::RPMethod::Info RPMethodAlgorithm::getInfo() const
{
	rpMethod::RPMethod::Info info;
	info.name = "Алгоритм";
	info.version_major = 0;
	info.version_minor = 1;
	info.version_build = 1;
	info.version_info  = "Глубокая альфа";
	info.description   = "Тестовый метод";
	return info;
}

void RPMethodAlgorithm::registerObject()
{
	rpMethod::factory->insertFactory( new RPObjectClassInfo( getClassName(), "RPMethod" ) );
	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPAlgorithmFlowChart", "RPObjectFlowChart", RPAlgorithmFlowChart::newObject, this, _T("Алгоритм") ) );
	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPShapeAction", "RPShape", RPShapeAction::newObject, this, _T("Операция") ) );
	rpMethod::factory->insertFactory( new RPObjectClassInfo( "RPShapeIf", "RPShape", RPShapeIf::newObject, this, _T("Условие") ) );
}

RP_METHOD_DLL rpMethod::RPMethod* registerMethod( RPObjectFactory* _factory, RPProject* _project )
{
	rpMethod::factory = _factory;
	rpMethod::project = _project;

	RPMethodAlgorithm* method = new RPMethodAlgorithm( NULL );
	method->registerObject();

	return method;
}

// ----------------------------------------------------------------------------
// ---------- RPAlgorithmFlowChart
// ----------------------------------------------------------------------------
RPAlgorithmFlowChart::RPAlgorithmFlowChart( RPObject* _parent ):
	RPObjectFlowChart( _parent )
{
	RPShape* shape_action = static_cast<RPShape*>(rpMethod::factory->getNewObject( "RPShapeAction", this ));
	shape_action->setPosition( 100, 45 );
	shape_action = static_cast<RPShape*>(rpMethod::factory->getNewObject( "RPShapeAction", this ));
	shape_action->setPosition( 250, 145 );
	RPShape* shape_if = static_cast<RPShape*>(rpMethod::factory->getNewObject( "RPShapeIf", this ));
	shape_if->setPosition( 300, 100 );
}

RPAlgorithmFlowChart::~RPAlgorithmFlowChart()
{
}

RPObject* RPAlgorithmFlowChart::newObject( RPObject* parent )
{
	return new RPAlgorithmFlowChart( parent );
}
