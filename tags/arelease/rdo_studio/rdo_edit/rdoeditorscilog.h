#ifndef RDOEDITORSCILOGEDIT_H
#define RDOEDITORSCILOGEDIT_H
#pragma once

#include "rdoeditorsciedit.h"
#include "rdoeditorscilogstyle.h"

namespace rdoEditor {

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciLog
// ----------------------------------------------------------------------------
class RDOEditorSciLog: public RDOEditorSciEdit
{
private:
	int sci_MARKER_LINE;

protected:
	void setSelectLine();
	void clearSelectLine();
	bool hasSelectLine() const;

	//{{AFX_MSG(RDOEditorSciLog)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOEditorSciLog)
	protected:
	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );
	//}}AFX_VIRTUAL

public:
	RDOEditorSciLog();
	virtual ~RDOEditorSciLog();

	virtual void setEditorStyle( RDOEditorSciEditStyle* style );
};

}; // namespace rdoEditor

//{{AFX_INSERT_LOCATION}}

#endif // RDOEDITORSCILOGEDIT_H
