#include "stdafx.h"
#include "rdoprocess_object.h"
#include "rdoprocess_app.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPROCObject
// ----------------------------------------------------------------------------
RDOPROCObject::RDOPROCObject( RDOPROCObject* parent ):
	rpoparent( parent ),
	name( "object" ),
	selected( false )
{
	if ( rpoparent ) {
		rpoparent->child.push_back( this );
		rpoparent->setCorrectChildName( this );
	}
}

RDOPROCObject::~RDOPROCObject()
{
	rpapp.sendMessage( this, RP_OBJ_BEFOREDELETE );
	if ( isSelected() ) {
		setSelected( false );
	}
	clear();
	if ( rpoparent ) {
		std::vector< RDOPROCObject* >::iterator it = std::find( rpoparent->child.begin(), rpoparent->child.end(), this );
		if ( it != rpoparent->child.end() ) {
			rpoparent->child.erase( it );
		}
	}
}

void RDOPROCObject::clear()
{
	std::vector< RDOPROCObject* >::iterator it = child.begin();
	while ( it != child.end() ) {
		RDOPROCObject* obj = *it;
		delete obj;
		it = child.begin();
	}
	child.clear();
}

void RDOPROCObject::setName( const rp::string& value )
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
	rpapp.sendMessage( this, RP_OBJ_NAMECHANGED );
}

bool RDOPROCObject::isChildNameCorrect( const RDOPROCObject* obj ) const
{
	std::vector< RDOPROCObject* >::const_iterator it = begin();
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

void RDOPROCObject::setCorrectChildName( RDOPROCObject* obj )
{
	rp::string name = obj->getName();
	int i = 2;
	while ( !isChildNameCorrect( obj ) ) {
		obj->name = name + rp::string::fromint( i++ );
	}
}

void RDOPROCObject::setSelected( const bool value )
{
	if ( selected != value ) {
		selected = value;
		rpapp.sendMessage( this, RP_OBJ_SELCHANGED, value );
	}
}

/*
void RDOPROCObject::selectChildObject( RDOPROCObject* obj, const bool value )
{
	RDOPROCObject* root;
	if ( rpoparent ) {
		root = rpoparent;
		while ( root->rpoparent ) {
			root = root->rpoparent;
		}
	} else {
		root = this;
	}
	root->selectChildOff( value ? obj : NULL );
	std::vector< RDOPROCObject* >::iterator it = child.begin();
	while ( it != child.end() ) {
		RDOPROCObject* object = *it;
		if ( object == obj ) {
			object->set_selected( value );
			break;
		}
		it++;
	}
	rpapp.sendMessage( this, RP_OBJ_SELCHANGED );
}

void RDOPROCObject::selectChildOff( RDOPROCObject* withoutObj )
{
	std::vector< RDOPROCObject* >::iterator it = child.begin();
	while ( it != child.end() ) {
		if ( (*it) != withoutObj ) {
			(*it)->set_selected( false );
		}
		(*it)->selectChildOff( withoutObj );
		it++;
	}
}
*/

void RDOPROCObject::notify( RDOPROCObject* from, UINT message, WPARAM wParam, LPARAM lParam )
{
}
