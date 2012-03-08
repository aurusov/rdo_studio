#ifndef RDO_PROCESS_DOCKWND_H
#define RDO_PROCESS_DOCKWND_H

#include "thirdparty/sizecbar/scbarg.h"

// --------------------------------------------------------------------------------
// -------------------- RPDockWnd
// --------------------------------------------------------------------------------
class RPDockWnd: public CSizingControlBarG
{
public:
	RPDockWnd();
	virtual ~RPDockWnd();

private:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};

#endif // RDO_PROCESS_DOCKWND_H
