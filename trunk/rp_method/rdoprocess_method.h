#ifndef RDO_PROCESS_METHOD_H
#define RDO_PROCESS_METHOD_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_object.h"
#include <rdoprocess_toolbar.h>

class RPObjectFactory;
class RPProject;

namespace rpMethod {

// ----------------------------------------------------------------------------
// ---------- RPMethod
// ----------------------------------------------------------------------------
class RPMethod: public RPObject
{
public:
	struct Info {
		rp::string name;
		int version_major;
		int version_minor;
		int version_build;
		rp::string version_info;
		rp::string description;
		Info(): name( "" ), version_major( 0 ), version_minor( 0 ), version_build( 0 ), version_info( "" ), description( "" ) {}
	};

public:
	RPMethod( RPObject* parent );
	virtual ~RPMethod();
	virtual Info getInfo() const = 0;
	virtual void buttonCommand( int button_id ) {};
	virtual void buttonUpdate( RPCtrlToolbar::ButtonUpdate& button_update ) {};
};

typedef RPMethod* (*PFunRegisterMethod)( RPObjectFactory* _factory, RPProject* _project );

} // namespace rpMethod

#ifdef RP_METHOD_EXPORTS
#define RP_METHOD_DLL __declspec(dllexport)
#else
#define RP_METHOD_DLL __declspec(dllimport)
#endif

#endif // RDO_PROCESS_METHOD_H
