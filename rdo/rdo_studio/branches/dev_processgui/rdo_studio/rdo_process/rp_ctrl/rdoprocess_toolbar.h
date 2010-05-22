#ifndef RDO_PROCESS_CTRL_TOOLBAR_H
#define RDO_PROCESS_CTRL_TOOLBAR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------
// ---------- RPCtrlToolbar
// ----------------------------------------------------------------------------
namespace rpMethod {
class RPMethod;
}

class RPCtrlToolbar
{
protected:
	RPCtrlToolbar();
	~RPCtrlToolbar();
public:
//	virtual int insertButton(rpMethod::RPMethod* method, char* xpm[], const rp::string& tool_tip = "")=0;

	virtual int insertButton( rpMethod::RPMethod* method, char* xpm[], const rp::string& tool_tip = "" ) = 0;
	virtual void setCaption( const rp::string& caption ) = 0;
	
	struct ButtonUpdate {
		int  id;
		bool enable;
		ButtonUpdate( int _id, bool _enable = true ): id(_id), enable( _enable ) {}
	};
};

#endif // RDO_PROCESS_CTRL_TOOLBAR_H
