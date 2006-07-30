#ifndef RDO_PROCESS_OBJECT_H
#define RDO_PROCESS_OBJECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "misc/rdoprocess_string.h"
#include <list>

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
	RPObject*              parent;
	std::list< RPObject* > child;
	bool isChildNameCorrect( const RPObject* obj ) const;
	void setCorrectChildName( RPObject* obj );

	std::list< RPObject* >::const_iterator begin() const { return child.begin(); }
	std::list< RPObject* >::const_iterator end()   const { return child.end();   }
	void clear();

	void selectChildOff( RPObject* withoutObj = NULL );
	// <-- manager

	rp::string name;
	bool       selected;

	virtual void notify( RPObject* from, UINT message, WPARAM wParam, LPARAM lParam ) {};

	virtual void modify() {};

	RPObject* get_this() { return this; }

public:
	RPObject( RPObject* parent = NULL, const rp::string& name = "object" );
	virtual ~RPObject();

	virtual rp::string getType() = 0;
	bool isParent( const rp::string& type );

	const std::list< RPObject* >& getChild() const { return child; }

	virtual void generate() {};

	rp::string getName() const { return name; }
	virtual bool setName( const rp::string& value );

	bool isSelected() const { return selected; }
	virtual void setSelected( bool value );

	virtual bool isChartObject() const { return false; }

	bool hasChild() const { return !child.empty(); }
	std::list< RPObject* >::const_iterator find_child( const RPObject* object ) {
		return std::find( child.begin(), child.end(), object );
	};

};

#endif // RDO_PROCESS_OBJECT_H
