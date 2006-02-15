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
	rpoparent( parent ),
	name( _name ),
	selected( false )
{
	if ( rpoparent ) {
		rpoparent->child.push_back( this );
		rpoparent->setCorrectChildName( this );
	}
}

RPObject::~RPObject()
{
	rpapp.sendMessage( this, rp::msg::RP_OBJ_BEFOREDELETE );
	clear();
	if ( isSelected() ) {
		setSelected( false );
	}
	if ( rpoparent ) {
		std::vector< RPObject* >::iterator it = std::find( rpoparent->child.begin(), rpoparent->child.end(), this );
		if ( it != rpoparent->child.end() ) {
			rpoparent->child.erase( it );
		}
	}
}

void RPObject::clear()
{
	std::vector< RPObject* >::iterator it = child.begin();
	while ( it != child.end() ) {
		RPObject* obj = *it;
		delete obj;
		it = child.begin();
	}
	child.clear();
}

void RPObject::setName( const rp::string& value )
{
	if ( !value.empty() ) {
		rp::string prev_name = name;
		name = value;
		if ( rpoparent && !rpoparent->isChildNameCorrect( this ) ) {
			name = prev_name;
			::MessageBox( NULL, _T("Name alredy used"), "Error", MB_OK | MB_ICONERROR );
		}
	} else {
		::MessageBox( NULL, _T("Name can't be empty"), "Error", MB_OK | MB_ICONERROR );
	}
	rpapp.sendMessage( this, rp::msg::RP_OBJ_NAMECHANGED );
}

bool RPObject::isChildNameCorrect( const RPObject* obj ) const
{
	std::vector< RPObject* >::const_iterator it = begin();
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
	std::vector< RPObject* >::iterator it = child.begin();
	while ( it != child.end() ) {
		RPObject* obj = *it;
		if ( (*it) != withoutObj ) {
			(*it)->setSelected( false );
		}
		(*it)->selectChildOff( withoutObj );
		it++;
	}
}
