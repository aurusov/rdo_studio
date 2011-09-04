#ifndef RDO_PROCESS_PROJECT_H
#define RDO_PROCESS_PROJECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream>
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_messages.h"

// --------------------------------------------------------------------------------
// -------------------- RPProject
// --------------------------------------------------------------------------------
class RPObject;
class RPObjectFlowChart;
class RPPixmap;
class RPCtrlToolbar;

class RPProject: public RPObject
{
public:
	enum FlowState { flow_none = 0, flow_select, flow_connector, flow_rotate };
	enum Cursor {
		cursor_flow_select = 0,
		cursor_flow_move,
		cursor_flow_connector,
		cursor_flow_rotate,
		cursor_flow_rotate_center,
		cursor_flow_rotate_tl,
		cursor_flow_rotate_tr,
		cursor_flow_rotate_bl,
		cursor_flow_rotate_br,
		cursor_flow_scale_lr,
		cursor_flow_scale_tb,
		cursor_flow_scale_tlbr,
		cursor_flow_scale_trbl,
		cursor_flow_dock_in,
		cursor_flow_dock_out,
		cursor_flow_dock_inout,
		cursor_flow_dock_fly,
		cursor_flow_dock_not,
		cursor_flow_trash
	};
	std::map< Cursor, HCURSOR > cursors;

private:
	int                                  last_button_id;
	std::map< int, rpMethod::RPMethod* > button_reflection;

protected:
	FlowState flow_state;
	virtual void notify( RPObject* from, UINT message, void* param = NULL );

	rp::msg*           message;
	RPObject*          activeObject;
	RPObjectFlowChart* activeFlowChart;

public:
	RPProject();
	virtual ~RPProject();

	virtual std::ofstream& log() const = 0;
	virtual RPPixmap*      createBitmap( char* xpm[] ) = 0;
	virtual void makeFlowChartWnd( RPObjectFlowChart* flowobj ) = 0;

	rp::msg& msg() const { return *message; }
	void sendMessage( RPObject* from, UINT message_id, void* param = NULL ) {
		if ( message ) message->sendMessage( from, message_id, param );
	}

	virtual rp::string getClassName() const { return "RPProject"; }

	FlowState getFlowState() const                { return flow_state;      }
	void setFlowState( FlowState value );

	RPObject* getActiveObject() const             { return activeObject;    }

	RPObjectFlowChart* getActiveFlowChart() const { return activeFlowChart; }
	void setActiveFlowChart( RPObjectFlowChart* flowChart );

	int getNewButtonID( rpMethod::RPMethod* method ) {
		button_reflection[ last_button_id ] = method;
		return last_button_id++;
	}
	rpMethod::RPMethod* getMethodByButton( int button_id ) const {
		std::map< int, rpMethod::RPMethod* >::const_iterator it = button_reflection.find( button_id );
		return it != button_reflection.end() ? it->second : NULL;
	}
	RPObject* findObject( rp::string fullname );
};

namespace rpMethod {
extern RPProject* project;
}

#endif // RDO_PROCESS_CHART_OBJECT_H
