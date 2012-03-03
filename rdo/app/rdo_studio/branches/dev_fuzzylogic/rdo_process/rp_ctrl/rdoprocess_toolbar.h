#ifndef RDO_PROCESS_CTRL_TOOLBAR_H
#define RDO_PROCESS_CTRL_TOOLBAR_H

#include "utils/rdomacros.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_string.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// --------------------------------------------------------------------------------
// -------------------- RPCtrlToolbar
// --------------------------------------------------------------------------------
namespace rpMethod {
class RPMethod;
}

class RPCtrlToolbar
{
protected:
	RPCtrlToolbar();
	~RPCtrlToolbar();
public:
	virtual int  insertButton(PTR(rpMethod::RPMethod) method, PTR(char) xpm[], CREF(rp::string) tool_tip = _T("")) = 0;
	virtual void setCaption  (CREF(rp::string) caption) = 0;
	
	struct ButtonUpdate {
		int  id;
		bool enable;
		ButtonUpdate( int _id, bool _enable = true ): id(_id), enable( _enable ) {}
	};
};

#endif // RDO_PROCESS_CTRL_TOOLBAR_H
