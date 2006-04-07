#include "stdafx.h"
#include "rdoprocess_object.h"
#include "rdoprocess_app.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPObject
// ----------------------------------------------------------------------------


RPObject::RPObject( RPObject* parent, const rp::string& _name ):
	parent( parent ),
	name( _name ),
	selected( false )
{
	if ( parent ) {
		parent->child.push_back( this );
		parent->setCorrectChildName( this );
	}
	
	type ="block"; // MJ инициализация типа блоков, по умолчанию
}

RPObject::~RPObject()
{
	rpapp.sendMessage( this, rp::msg::RP_OBJ_BEFOREDELETE );
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

bool RPObject::setName( const rp::string& value )
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
	rpapp.sendMessage( this, rp::msg::RP_OBJ_NAMECHANGED );
	return true;
}

bool RPObject::isChildNameCorrect( const RPObject* obj ) const
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

void RPObject::setSelected( bool value )
{
	if ( selected != value ) {
		selected = value;
		rpapp.sendMessage( this, rp::msg::RP_OBJ_SELCHANGED, value );
	}
}

void RPObject::selectChildOff( RPObject* withoutObj )
{
	std::list< RPObject* >::iterator it = child.begin();
	while ( it != child.end() ) {
		RPObject* obj = *it;
		if ( (*it) != withoutObj ) {
			(*it)->setSelected( false );
		}
		(*it)->selectChildOff( withoutObj );
		it++;
	}
}
	// MJ start 30.03.06  бегает по list объектов и вызывает генерирование
void RPObject::list_name()
{
//	TRACE( "%s\n", getName().c_str() );
	std::list< RPObject* >::const_iterator it = child.begin();
	while( it != child.end() ) {
	
		TRACE( "%s\n", (*it)->getName().c_str() );
	    (*it)->find_next_block_MJ();
		(*it)->list_name();
		(*it)->generate_MJ();
		it++;
	}
}

void RPObject::list_name_for_type_res()
{
//	TRACE( "%s\n", getName().c_str() );
	std::list< RPObject* >::const_iterator it = child.begin();
	while( it != child.end() ) 
	{
	

		rpapp.RDOfiles->typeres<<(*it)->getName().c_str()<<", ";
		(*it)->list_name_for_type_res();	

	it++;
	
	}
}


	



// MJ stop

void RPObject::list_name_for_resource_MJ(std::list< RPObject* >* list_resource)
{

	std::list< RPObject* >::const_iterator it = child.begin();
	while( it != child.end() ) {
	
		
	    if((*it)->type == "resource")
		{
		TRACE( "%s\n", (*it)->getName().c_str() );
         list_resource->push_back(*it);
		}
			
		(*it)->list_name_for_resource_MJ(list_resource);
		it++;
	}
}

 