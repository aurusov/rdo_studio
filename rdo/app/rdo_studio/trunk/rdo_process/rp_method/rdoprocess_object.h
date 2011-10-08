#ifndef RDO_PROCESS_OBJECT_H
#define RDO_PROCESS_OBJECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_factory.h"

// --------------------------------------------------------------------------------
// -------------------- RPObject
// --------------------------------------------------------------------------------
namespace rp {
class msg;
class RPXMLNode;
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

	void save_child( rp::RPXMLNode* node ) const;
	// <-- manager

	rp::string name;
	bool       selected;
	bool       can_modify;

	virtual void notify( RPObject* from, UINT message, void* param = NULL )
	{
		UNUSED(from   );
		UNUSED(message);
		UNUSED(param  );
	}
	virtual void modify()                        {};
	RPObject* get_this()                         { return this;     }
	virtual rp::string get_xml_node_name() const { return "object"; }

public:
	RPObject( RPObject* parent = NULL, const rp::string& name = "object" );
	virtual ~RPObject();

	virtual rp::string getClassName() const = 0;
	const RPObjectClassInfo* getClassInfo() const { return rpMethod::factory->getClassInfo( getClassName() ); }
	virtual void load( rp::RPXMLNode* node );
	virtual rp::RPXMLNode* save( rp::RPXMLNode* parent_node );

	bool hasChild() const { return !child.empty(); }
	std::list< RPObject* >::const_iterator find_child( const RPObject* object ) {
		return std::find( child.begin(), child.end(), object );
	};
	RPObject* find_child( const rp::string& name ) const;

	const std::list< RPObject* >& getChild() const { return child; }
	void getAllChild( std::list< RPObject* >& all_child, bool recursive = false ) const;
	void getAllChildByClass( std::list< RPObject* >& all_child, const rp::string& class_name, bool recursive = false ) const;
	void getAllChildByClasses( std::list< RPObject* >& all_child, const std::list< rp::string >& class_names, bool recursive = false ) const;
	void getAllChildNotByClass( std::list< RPObject* >& all_child, const rp::string& class_name, bool recursive = false ) const;

	rp::string getName() const { return name; }
	rp::string getFullName() const;
	virtual bool setName( const rp::string& value );
	void setCorrectName( const rp::string& value );

	bool isSelected() const { return selected; }
	virtual void setSelected( bool value );

	virtual bool isChartObject() const { return false; }
};

#endif // RDO_PROCESS_OBJECT_H
