#ifndef RDO_PROCESS_METHOD_H
#define RDO_PROCESS_METHOD_H

#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object.h"
#include "app/rdo_studio_mfc/rdo_process/rp_ctrl/rdoprocess_toolbar.h"

class RPObjectFlowChart;
class RPObjectFactory;
class RPProject;
class RPPixmap;

namespace rpMethod {

// --------------------------------------------------------------------------------
// -------------------- RPMethod
// --------------------------------------------------------------------------------
class RPMethod: public RPObject
{
public:
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

	virtual int getVersionMajor() const       { return 0;  }
	virtual int getVersionMinor() const       { return 1;  }
	virtual int getVersionBuild() const       { return 1;  }
	virtual rp::string getVersionDesc() const { return ""; }
	virtual rp::string getDescription() const { return ""; }

	virtual void buttonCommand( int button_id )
	{
		UNUSED(button_id);
	}
	virtual void buttonUpdate( RPCtrlToolbar::ButtonUpdate& button_update )
	{
		UNUSED(button_update);
	}
	virtual RPObjectFlowChart* makeFlowChart( RPObject* parent ) = 0;
	virtual void generate() = 0;

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

#endif // RDO_PROCESS_METHOD_H
