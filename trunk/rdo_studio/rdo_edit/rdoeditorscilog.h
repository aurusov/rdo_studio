#ifndef RDOEDITORSCILOGEDIT_H
#define RDOEDITORSCILOGEDIT_H
#pragma once

#include "rdoeditorsciedit.h"
#include "rdoeditorscilogstyle.h"

#include <rdosimwin.h>

#include <list>

using namespace std;

namespace rdoEditor {

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciLogLineInfo
// ----------------------------------------------------------------------------
class RDOEditorSciLog;

class RDOEditorSciLogLineInfo
{
friend class RDOEditorSciLog;
protected:
	rdoModelObjects::RDOFileType fileType;
	int                          lineNumber;
	string                       message;

public:
	RDOEditorSciLogLineInfo( const string& _message, const rdoModelObjects::RDOFileType _fileType = rdoModelObjects::PAT, const int _lineNumber = -1 );
	~RDOEditorSciLogLineInfo();

	virtual string getMessage() const;
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciBuildLineInfo
// ----------------------------------------------------------------------------
class RDOEditorSciBuildLineInfo: public RDOEditorSciLogLineInfo
{
protected:
	bool error;

public:
	RDOEditorSciBuildLineInfo( const string& _message, const rdoModelObjects::RDOFileType _fileType = rdoModelObjects::PAT, const int _lineNumber = -1, bool _error = true );
	~RDOEditorSciBuildLineInfo();

	virtual string getMessage() const;
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciLog
// ----------------------------------------------------------------------------
class RDOEditorSciLog: public RDOEditorSciEdit
{
private:
	int sci_MARKER_LINE;

protected:
	list< RDOEditorSciLogLineInfo* > lines;

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

	void appendLine( RDOEditorSciLogLineInfo* line );
};

}; // namespace rdoEditor

//{{AFX_INSERT_LOCATION}}

#endif // RDOEDITORSCILOGEDIT_H
