#ifndef RDOLOGEDIT_H
#define RDOLOGEDIT_H
#pragma once

#include "rdobaseedit.h"
#include "rdologeditstyle.h"

#include <rdosimwin.h>

#include <list>

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOLogEditLineInfo
// ----------------------------------------------------------------------------
class RDOLogEdit;

class RDOLogEditLineInfo
{
friend class RDOLogEdit;
protected:
	rdoModelObjects::RDOFileType fileType;
	int                          lineNumber;
	string                       message;

public:
	RDOLogEditLineInfo( const string& _message, const rdoModelObjects::RDOFileType _fileType = rdoModelObjects::PAT, const int _lineNumber = -1 );
	~RDOLogEditLineInfo();

	virtual string getMessage() const;
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciBuildLineInfo
// ----------------------------------------------------------------------------
class RDOEditorSciBuildLineInfo: public RDOLogEditLineInfo
{
protected:
	bool error;

public:
	RDOEditorSciBuildLineInfo( const string& _message, const rdoModelObjects::RDOFileType _fileType = rdoModelObjects::PAT, const int _lineNumber = -1, bool _error = true );
	~RDOEditorSciBuildLineInfo();

	virtual string getMessage() const;
};

// ----------------------------------------------------------------------------
// ---------- RDOLogEdit
// ----------------------------------------------------------------------------
class RDOLogEdit: public RDOBaseEdit
{
private:
	int sci_MARKER_LINE;

protected:
	list< RDOLogEditLineInfo* > lines;
	void clearLines();

	void setSelectLine();
	void clearSelectLine();
	bool hasSelectLine() const;

	//{{AFX_MSG(RDOLogEdit)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOLogEdit)
	protected:
	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );
	//}}AFX_VIRTUAL

public:
	RDOLogEdit();
	virtual ~RDOLogEdit();

	virtual void setEditorStyle( rdoStyle::RDOBaseEditStyle* style );

	virtual void clearAll();
	void appendLine( RDOLogEditLineInfo* line );
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOLOGEDIT_H
