#ifndef RDO_PROCESS_FACTORY_H
#define RDO_PROCESS_FACTORY_H

#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_string.h"
#include <list>
#include <map>

// --------------------------------------------------------------------------------
// -------------------- RPObjectClassInfo
// --------------------------------------------------------------------------------
class RPObject;
class RPPixmap;

namespace rpMethod {
class RPMethod;
}
typedef RPObject* (*MakeObject)( RPObject* _parent );

class RPObjectClassInfo {
protected:
	rp::string                      class_name;
	std::list< RPObjectClassInfo* > parent;
	std::list< RPObjectClassInfo* > child;
	MakeObject                      makeObject;
	rpMethod::RPMethod*             method;
	rp::string                      label;
	RPPixmap*                       preview;
	int                             order;

public:
	RPObjectClassInfo( const rp::string& _class_name, RPObjectClassInfo* _parent = NULL, MakeObject _makeObject = NULL );
	RPObjectClassInfo( const rp::string& _class_name, const rp::string& _parent, MakeObject _makeObject = NULL, rpMethod::RPMethod* _method = NULL, const rp::string& _label = "", char* _xpm[] = NULL, int _order = -1 );
	RPObjectClassInfo( const RPObjectClassInfo& copy );
	~RPObjectClassInfo();

	void multiParent( RPObjectClassInfo* _parent );
	void multiParent( const rp::string& _parent );
	rp::string getClassName() const                    { return class_name;          }
	bool isAbstract() const                            { return makeObject == NULL;  }
	bool haveMethod() const                            { return method     != NULL;  }
	const rpMethod::RPMethod* getMethod() const        { return method;              }
	rp::string getLabel() const                        { return label;               }
	bool isKindOf( const rp::string& _class_name ) const {
		if ( class_name == _class_name ) return true;
		std::list< RPObjectClassInfo* >::const_iterator it = parent.begin();
		while ( it != parent.end() ) {
			if ( (*it)->isKindOf( _class_name ) ) return true;
			it++;
		}
		return false;
	}
	RPObject* getNewObject( RPObject* parent ) const {
		return !isAbstract() ? makeObject( parent ) : NULL;
	}
	RPPixmap* getPreview();
	int getOrder() const { return order; }
};

// --------------------------------------------------------------------------------
// -------------------- RPObjectFactory
// --------------------------------------------------------------------------------
class RPFlowChart;

class RPObjectFactory {
protected:
	std::map< rp::string, RPObjectClassInfo* > factories;
	RPFlowChart* flowchart;

public:
	RPObjectFactory();
	~RPObjectFactory();
	void registerDefaultObject();
	void insertFactory( RPObjectClassInfo* _factory );
	RPObjectClassInfo* getClassInfo( const rp::string& class_name ) const {
		std::map< rp::string, RPObjectClassInfo* >::const_iterator it = factories.find( class_name );
		return it != factories.end() ? it->second : NULL;
	}
	RPObject* getNewObject( const rp::string& class_name, RPObject* parent ) const;
	std::list< RPObjectClassInfo* > getMethodClasses( const rpMethod::RPMethod* const method ) const;
	void removeMethod( const rpMethod::RPMethod* const method );

	void setCurrentFlowchart( RPFlowChart* value ) { flowchart = value; }
	RPFlowChart* getCurrentFlowchart() const       { return flowchart;  }
};

namespace rpMethod {
extern RPObjectFactory* factory;
}

#endif // RDO_PROCESS_FACTORY_H
