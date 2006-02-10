#ifndef RDO_PROCESS_OBJECT_H
#define RDO_PROCESS_OBJECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_string.h"
#include <vector>

// ----------------------------------------------------------------------------
// ---------- RDOPROCObject
// ----------------------------------------------------------------------------
static const int RP_OBJ_BEFOREDELETE = ::RegisterWindowMessage( "RP_OBJ_BEFOREDELETE" );
static const int RP_OBJ_NAMECHANGED  = ::RegisterWindowMessage( "RP_OBJ_NAMECHANGED" );
static const int RP_OBJ_SELCHANGED   = ::RegisterWindowMessage( "RP_OBJ_SELCHANGED" );

class RDOPROCObject
{
protected:
	// --> manager
	RDOPROCObject*                rpoparent;
	std::vector< RDOPROCObject* > child;
	bool isChildNameCorrect( const RDOPROCObject* obj ) const;
	void setCorrectChildName( RDOPROCObject* obj );

	std::vector< RDOPROCObject* >::const_iterator begin() const { return child.begin(); }
	std::vector< RDOPROCObject* >::const_iterator end()   const { return child.end();   }
	void clear();

	void selectChildObject( RDOPROCObject* obj, const bool value );
	void selectChildOff( RDOPROCObject* withoutObj = NULL );
	// <-- manager

	rp::string name;

	bool selected;
	virtual void set_selected( const bool value = true );

public:
	RDOPROCObject( RDOPROCObject* parent = NULL );
	virtual ~RDOPROCObject();

	// --> manager
	// <-- manager

	rp::string getName() const { return name; }
	void setName( const rp::string& value );

	bool isSelected() const { return selected; }
	void setSelected( const bool value );

	bool hasChild() const { return !child.empty(); }
};

#endif // RDO_PROCESS_OBJECT_H
