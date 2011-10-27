#include "app/rdo_studio_mfc/rdo_process/rp_method/stdafx.h"
#include <fstream>
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_factory.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_connector.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_method.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/block_default.xpm"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_pixmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rpMethod {
RPObjectFactory* factory = NULL;
}

// --------------------------------------------------------------------------------
// -------------------- RPObjectClassInfo
// --------------------------------------------------------------------------------
RPObjectClassInfo::RPObjectClassInfo( const rp::string& _class_name, RPObjectClassInfo* _parent, MakeObject _makeObject ):
	class_name( _class_name ),
	makeObject( _makeObject ),
	method( NULL ),
	label( "" ),
	preview( NULL ),
	order( -1 )
{
	multiParent( _parent );
}

RPObjectClassInfo::RPObjectClassInfo( const rp::string& _class_name, const rp::string& _parent, MakeObject _makeObject, rpMethod::RPMethod* _method, const rp::string& _label, char* _xpm[], int _order ):
	class_name( _class_name ),
	makeObject( _makeObject ),
	method( _method ),
	label( _label ),
	preview( NULL ),
	order( _order )
{
	multiParent( _parent );
	if ( _xpm ) preview = rpMethod::project->createBitmap( _xpm );
}

RPObjectClassInfo::RPObjectClassInfo( const RPObjectClassInfo& copy ):
	class_name( copy.class_name ),
	makeObject( copy.makeObject ),
	method( copy.method ),
	label( copy.label ),
	preview( copy.preview ),
	order( copy.order )
{
	parent.clear();
	parent.assign( copy.parent.begin(), copy.parent.end() );
	child.clear();
	child.assign( copy.child.begin(), copy.child.end() );
}

RPObjectClassInfo::~RPObjectClassInfo()
{
	rpMethod::project->log() << "delete factory " << class_name << " .." << std::endl;
	rpMethod::project->log() << "  remove info from parents..." << std::endl;
	std::list< RPObjectClassInfo* >::iterator parent_it = parent.begin();
	while ( parent_it != parent.end() ) {
		std::list< RPObjectClassInfo* >::iterator it = (*parent_it)->child.begin();
		while ( it != (*parent_it)->child.end() ) {
			if ( *it == this ) {
				rpMethod::project->log() << "  --remove from " << (*parent_it)->class_name << std::endl;
				it = (*parent_it)->child.erase( it );
			} else {
				it++;
			}
		}
		parent_it++;
	}
	rpMethod::project->log() << "  remove info from child..." << std::endl;
	std::list< RPObjectClassInfo* >::iterator child_it = child.begin();
	while ( child_it != child.end() ) {
		std::list< RPObjectClassInfo* >::iterator it = (*child_it)->parent.begin();
		while ( it != (*child_it)->parent.end() ) {
			if ( *it == this ) {
				rpMethod::project->log() << "  --remove from " << (*child_it)->class_name << std::endl;
				it = (*child_it)->parent.erase( it );
			} else {
				it++;
			}
		}
		child_it++;
	}
	if ( preview ) delete preview;
	rpMethod::project->log() << "delete factory " << class_name << " .. ok" << std::endl;
}

void RPObjectClassInfo::multiParent( RPObjectClassInfo* _parent )
{
	if ( _parent ) {
		_parent->child.push_back( this );
		parent.push_back( _parent );
	}
}

void RPObjectClassInfo::multiParent( const rp::string& _parent )
{
	multiParent( rpMethod::factory->getClassInfo( _parent ) );
}

RPPixmap* RPObjectClassInfo::getPreview()
{
	if ( !preview ) {
		preview = rpMethod::project->createBitmap( block_default_xpm );
	}
	return preview;
}

// --------------------------------------------------------------------------------
// -------------------- RPObjectFactory
// --------------------------------------------------------------------------------
RPObjectFactory::RPObjectFactory()
{
	rpMethod::project->log() << "RPObjectFactory::RPObjectFactory()" << std::endl;
	rpMethod::factory = this;
}

RPObjectFactory::~RPObjectFactory()
{
	rpMethod::project->log() << "RPObjectFactory::~RPObjectFactory().." << std::endl;
	std::map< rp::string, RPObjectClassInfo* >::iterator it = factories.begin();
	while ( it != factories.end() ) {
		delete it->second;
		it = factories.erase( it );
	}
	rpMethod::factory = NULL;
	rpMethod::project->log() << "RPObjectFactory::~RPObjectFactory().. ok" << std::endl;
}

void RPObjectFactory::registerDefaultObject()
{
	// Базовый класс
	RPObjectClassInfo* class_info_RPObject = new RPObjectClassInfo( "RPObject", NULL );
	insertFactory( class_info_RPObject );
	// Потомки RPObject
	RPObjectClassInfo* class_info_msg            = new RPObjectClassInfo( "msg", class_info_RPObject );
	RPObjectClassInfo* class_info_RPMainFrameMsg = new RPObjectClassInfo( "RPMainFrameMsg", class_info_RPObject );
	RPObjectClassInfo* class_info_RPObjectChart  = new RPObjectClassInfo( "RPObjectChart", class_info_RPObject );
	RPObjectClassInfo* class_info_RPProject      = new RPObjectClassInfo( "RPProject", class_info_RPObject );
	RPObjectClassInfo* class_info_method         = new RPObjectClassInfo( "RPMethod", class_info_RPObject );
	insertFactory( class_info_msg );
	insertFactory( class_info_RPMainFrameMsg );
	insertFactory( class_info_RPObjectChart );
	insertFactory( class_info_RPProject );
	insertFactory( class_info_method );
	// Потомки RPObjectChart
	RPConnector::registerObject();
	RPObjectClassInfo* class_info_RPObjectMatrix = new RPObjectClassInfo( "RPObjectMatrix", class_info_RPObjectChart );
	insertFactory( class_info_RPObjectMatrix );
	// Потомки RPObjectMatrix
	RPObjectClassInfo* class_info_RPObjectFlowChart = new RPObjectClassInfo( "RPObjectFlowChart", class_info_RPObjectMatrix );
	RPObjectClassInfo* class_info_RPShape           = new RPObjectClassInfo( "RPShape", class_info_RPObjectMatrix );
	insertFactory( class_info_RPObjectFlowChart );
	insertFactory( class_info_RPShape );
	// Потомки RPShape
	RPObjectClassInfo* class_info_RPObjectPixmap    = new RPObjectClassInfo( "RPObjectPixmap", class_info_RPShape );
	insertFactory( class_info_RPObjectPixmap );
}

void RPObjectFactory::insertFactory( RPObjectClassInfo* _factory )
{
	if ( _factory ) {
		if ( factories.find( _factory->getClassName() ) == factories.end() ) {
			factories[_factory->getClassName()] = _factory;
			rpMethod::project->log() << "new factory " << _factory->getClassName() << std::endl;
		} else {
			AfxGetMainWnd()->MessageBox( rp::format("Класс %s уже зарегистрирован", _factory->getClassName().c_str()).c_str(), NULL, MB_ICONERROR );
		}
	} else {
		AfxGetMainWnd()->MessageBox( "Невозможно зарегистрировать пустую фабрику", NULL, MB_ICONERROR );
	}
}

RPObject* RPObjectFactory::getNewObject( const rp::string& class_name, RPObject* parent ) const
{
	std::map< rp::string, RPObjectClassInfo* >::const_iterator it = factories.find( class_name );
	if ( it != factories.end() ) {
		RPObject* obj = it->second->getNewObject( parent );
		if ( obj ) {
			return obj;
		} else {
			AfxGetMainWnd()->MessageBox( rp::format("Класс %s является абстрактным", class_name.c_str()).c_str(), NULL, MB_ICONERROR );
			return NULL;
		}
	} else {
		AfxGetMainWnd()->MessageBox( rp::format("Класс %s не найден", class_name.c_str()).c_str(), NULL, MB_ICONERROR );
		return NULL;
	}
}

std::list< RPObjectClassInfo* > RPObjectFactory::getMethodClasses( const rpMethod::RPMethod* const method ) const
{
	std::list< RPObjectClassInfo* > objects;
	std::map< rp::string, RPObjectClassInfo* >::const_iterator it = factories.begin();
	while ( it != factories.end() ) {
		if ( it->second->getMethod() == method ) objects.push_back( it->second );
		it++;
	}
	return objects;
}

void RPObjectFactory::removeMethod( const rpMethod::RPMethod* const method )
{
	std::map< rp::string, RPObjectClassInfo* >::iterator it = factories.begin();
	while ( it != factories.end() ) {
		if ( it->second->getMethod() == method || it->second->getClassName() == method->getClassName() ) {
			delete it->second;
			it = factories.erase( it );
		} else {
			it++;
		}
	}
}
