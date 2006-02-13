#ifndef RDO_PROCESS_OBJECT_H
#define RDO_PROCESS_OBJECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_string.h"
#include <vector>

// ----------------------------------------------------------------------------
// ---------- RPObject
// ----------------------------------------------------------------------------
namespace rp {
class msg;
}

class RPObject
{
friend class rp::msg;

protected:
	// --> manager
	RPObject*                rpoparent;
	std::vector< RPObject* > child;
	bool isChildNameCorrect( const RPObject* obj ) const;
	void setCorrectChildName( RPObject* obj );

	std::vector< RPObject* >::const_iterator begin() const { return child.begin(); }
	std::vector< RPObject* >::const_iterator end()   const { return child.end();   }
	void clear();

	void selectChildOff( RPObject* withoutObj = NULL );
	// <-- manager

	rp::string name;
	bool       selected;

	virtual void notify( RPObject* from, UINT message, WPARAM wParam, LPARAM lParam ) {};

public:
	RPObject( RPObject* parent = NULL, const rp::string& name = "object" );
	virtual ~RPObject();

	rp::string getName() const { return name; }
	void setName( const rp::string& value );

	bool isSelected() const { return selected; }
	virtual void setSelected( bool value );

	bool hasChild() const { return !child.empty(); }
};

#endif // RDO_PROCESS_OBJECT_H
