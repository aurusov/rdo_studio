#include "app/rdo_studio_mfc/rdo_process/rp_method/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_project.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_xml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RPObject
// --------------------------------------------------------------------------------
RPObject::RPObject( RPObject* _parent, const rp::string& _name ):
	parent( _parent ),
	name( _name ),
	selected( false ),
	can_modify( true )
{
	if ( parent ) {
		parent->child.push_back( this );
		parent->setCorrectChildName( this );
	}
}

RPObject::~RPObject()
{
	if ( rpMethod::project ) rpMethod::project->sendMessage( this, rp::msg::RP_OBJ_BEFOREDELETE );
	clear();
	if ( isSelected() ) {
		setSelected( false );
	}
	if ( parent ) {
		std::list< RPObject* >::iterator it = std::find( parent->child.begin(), parent->child.end(), this );
		if ( it != parent->child.end() ) {
			parent->child.erase( it );
		}
	}
}

void RPObject::clear()
{
	std::list< RPObject* >::iterator it = child.begin();
	while ( it != child.end() ) {
		RPObject* obj = *it;
		delete obj;
		it = child.begin();
	}
	child.clear();
}

void RPObject::save_child( rp::RPXMLNode* node ) const
{
	std::list< RPObject* >::const_iterator it = child.begin();
	while ( it != child.end() ) {
		(*it)->save( node );
		it++;
	}
}

RPObject* RPObject::find_child( const rp::string& name ) const
{
	std::list< RPObject* >::const_iterator it = child.begin();
	while ( it != child.end() ) {
		if ( (*it)->getName() == name ) return *it;
		it++;
	}
	return NULL;
}

void RPObject::load( rp::RPXMLNode* node )
{
	setName( node->getAttribute("name") );
}

rp::RPXMLNode* RPObject::save( rp::RPXMLNode* parent_node )
{
	rp::RPXMLNode* obj_node = parent_node->makeChild( get_xml_node_name() );
	obj_node->insertAttribute( "name", name );
	obj_node->insertAttribute( "class", getClassName() );
	return obj_node;
}

rp::string RPObject::getFullName() const
{
	rp::string str = "";
	if ( parent ) str = parent->getFullName();
	str += "/" + getName();
	return str;
}

rbool RPObject::setName( const rp::string& value )
{
	if ( !value.empty() ) {
		rp::string prev_name = name;
		name = value;
		if ( parent && !parent->isChildNameCorrect( this ) ) {
			name = prev_name;
			::MessageBox( NULL, _T("Name alredy used"), "Error", MB_OK | MB_ICONERROR );
			return false;
		}
	} else {
		::MessageBox( NULL, _T("Name can't be empty"), "Error", MB_OK | MB_ICONERROR );
		return false;
	}
	modify();
	if ( rpMethod::project ) rpMethod::project->sendMessage( this, rp::msg::RP_OBJ_NAMECHANGED );
	return true;
}

rbool RPObject::isChildNameCorrect( const RPObject* obj ) const
{
	std::list< RPObject* >::const_iterator it = begin();
	while ( it != end() ) {
		if ( *it != obj ) {
			if ( (*it)->getName().tolower() == obj->getName().tolower() ) {
				return false;
			}
		}
		it++;
	}
	return true;
}

void RPObject::setCorrectChildName( RPObject* obj )
{
	rp::string name = obj->getName();
	int i = 2;
	while ( !isChildNameCorrect( obj ) ) {
		obj->name = name + rp::string::fromint( i++ );
	}
}

void RPObject::setCorrectName( const rp::string& value )
{
	name = value;
	if ( parent ) {
		parent->setCorrectChildName( this );
		setName( name );
	}
	modify();
	if ( rpMethod::project ) rpMethod::project->sendMessage( this, rp::msg::RP_OBJ_NAMECHANGED );
}

void RPObject::setSelected( rbool value )
{
	if ( selected != value ) {
		selected = value;
		rpMethod::project->sendMessage( this, rp::msg::RP_OBJ_SELCHANGED, &selected );
	}
}

void RPObject::selectChildOff( RPObject* withoutObj )
{
	std::list< RPObject* >::iterator it = child.begin();
	while ( it != child.end() ) {
		if ( (*it) != withoutObj ) {
			(*it)->setSelected( false );
		}
		(*it)->selectChildOff( withoutObj );
		it++;
	}
}

void RPObject::getAllChild( std::list< RPObject* >& all_child, rbool recursive ) const
{
	all_child.insert( all_child.end(), child.begin(), child.end() );
	if ( recursive ) {
		std::list< RPObject* >::const_iterator it = child.begin();
		while ( it != child.end() ) {
			(*it)->getAllChild( all_child, true );
			it++;
		}
	}
}

void RPObject::getAllChildByClass( std::list< RPObject* >& all_child, const rp::string& class_name, rbool recursive ) const
{
	std::list< RPObject* >::const_iterator it = child.begin();
	while ( it != child.end() ) {
		if ( (*it)->getClassInfo()->isKindOf( class_name ) ) {
			all_child.push_back( *it );
		}
		it++;
	}
	if ( recursive ) {
		it = child.begin();
		while ( it != child.end() ) {
			(*it)->getAllChildByClass( all_child, class_name, true );
			it++;
		}
	}
}

void RPObject::getAllChildByClasses( std::list< RPObject* >& all_child, const std::list< rp::string >& class_names, rbool recursive ) const
{
	std::list< RPObject* >::const_iterator it = child.begin();
	while ( it != child.end() ) {
		std::list< rp::string >::const_iterator class_name_it = class_names.begin();
		while ( class_name_it != class_names.end() ) {
			if ( (*it)->getClassInfo()->isKindOf( *class_name_it ) ) {
				all_child.push_back( *it );
				break;
			}
			class_name_it++;
		}
		it++;
	}
	if ( recursive ) {
		it = child.begin();
		while ( it != child.end() ) {
			(*it)->getAllChildByClasses( all_child, class_names, true );
			it++;
		}
	}
}

void RPObject::getAllChildNotByClass( std::list< RPObject* >& all_child, const rp::string& class_name, rbool recursive ) const
{
	std::list< RPObject* >::const_iterator it = child.begin();
	while ( it != child.end() ) {
		if ( !(*it)->getClassInfo()->isKindOf( class_name ) ) {
			all_child.push_back( *it );
		}
		it++;
	}
	if ( recursive ) {
		it = child.begin();
		while ( it != child.end() ) {
			(*it)->getAllChildNotByClass( all_child, class_name, true );
			it++;
		}
	}
}
