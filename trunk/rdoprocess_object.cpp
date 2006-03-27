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

void RPObject::list_name()
{
	TRACE( "%s\n", getName().c_str() );
	std::list< RPObject* >::const_iterator it = child.begin();
	while( it != child.end() ) {
		(*it)->list_name();
		it++;
	}
}

/*
void RPObject::list_name()
{
	std::list< RPObject* >::const_iterator it = child.begin();
	if ( it != child.end() ) {
		TRACE( "%s\n", (*it)->getName().c_str() );
	}
	while(it != child.end())
	{
		TRACE( "%s\n", (*it)->getName().c_str() );
	      
		if(!(*it)->child.empty())
		{
		RPObject::child_name((*it));
		}
		
	it++;
	}
}

void RPObject::child_name(RPObject* obj)
{
std::list< RPObject* >::const_iterator it = obj->child.begin();
	
	while(it!= obj->child.end())
	{
		TRACE( "%s\n", (*it)->getName().c_str() );
	      
		if(!(*it)->child.empty())
		{
		RPObject::child_name((*it));
		}
		
	it++;
	}

	

}
*/