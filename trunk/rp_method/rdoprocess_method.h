#ifndef RDO_PROCESS_METHOD_H
#define RDO_PROCESS_METHOD_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_object.h"
#include <rdoprocess_toolbar.h>

class RPObjectFlowChart;
class RPObjectFactory;
class RPProject;
class RPPixmap;

namespace rpMethod {

// ----------------------------------------------------------------------------
// ---------- RPMethod
// ----------------------------------------------------------------------------
class RPMethod: public RPObject
{
public:
	struct Info {
		char* name;
		char* description;
		int version_major;
		int version_minor;
		int version_build;
		char* version_info;
		Info(): name( NULL ), description( NULL ), version_major( 0 ), version_minor( 0 ), version_build( 0 ), version_info( NULL ) {}
	};
	enum PMSize {
		pm_big,
		pm_small
	};

protected:
	RPPixmap* pixmap_big;
	RPPixmap* pixmap_small;

public:
	RPMethod( RPObject* parent, const rp::string name );
	virtual ~RPMethod();

	virtual void getInfo( rpMethod::RPMethod::Info& info ) const = 0;

	virtual void buttonCommand( int button_id ) {};
	virtual void buttonUpdate( RPCtrlToolbar::ButtonUpdate& button_update ) {};
	virtual RPObjectFlowChart* makeFlowChart( RPObject* parent ) = 0;

	RPPixmap* getPixmap( PMSize size = pm_small ) const {
		switch ( size ) {
			case pm_big  : return pixmap_big;
			case pm_small: return pixmap_small;
		}
		return pixmap_big;
	}
	void setPixmap( RPPixmap* pixmap, PMSize size = pm_small ) {
		switch ( size ) {
			case pm_big  : pixmap_big   = pixmap; break;
			case pm_small: pixmap_small = pixmap; break;
		}
	}
	bool hasPixmap( PMSize size = pm_small ) const {
		switch ( size ) {
			case pm_big  : return pixmap_big   ? true : false;
			case pm_small: return pixmap_small ? true : false;
		}
		return false;
	}
};

typedef RPMethod* (*PFunRegisterMethod)( RPObjectFactory* _factory, RPProject* _project );

} // namespace rpMethod

#ifdef RP_METHOD_EXPORTS
#define RP_METHOD_DLL __declspec(dllexport)
#else
#define RP_METHOD_DLL __declspec(dllimport)
#endif

#endif // RDO_PROCESS_METHOD_H
