#ifndef RDOLOGEDIT_H
#define RDOLOGEDIT_H
#pragma once

#include "rdobaseedit.h"
#include "rdologeditstyle.h"
#include <rdosimwin.h>

namespace rdoEditCtrl {

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
	int                          posInLine;
	std::string                  message;

public:
	RDOLogEditLineInfo( const std::string& _message, const rdoModelObjects::RDOFileType _fileType = rdoModelObjects::PAT, const int _lineNumber = -1, const int _posInLine = 0 );
	~RDOLogEditLineInfo();

	virtual std::string getMessage() const;
};

// ----------------------------------------------------------------------------
// ---------- RDOLogEdit
// ----------------------------------------------------------------------------
class RDOLogEdit: public RDOBaseEdit
{
private:
	int sci_MARKER_LINE;

protected:
	int current_line;
	list< RDOLogEditLineInfo* > lines;
	void clearLines();

	void setSelectLine();
	void setSelectLine( const int line, const RDOLogEditLineInfo* lineInfo, const bool useScroll = false );
	void clearSelectLine();
	bool hasSelectLine() const;

	//{{AFX_MSG(RDOLogEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGotoNext();
	afx_msg void OnGotoPrev();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOLogEdit)
	protected:
	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );
	//}}AFX_VIRTUAL

public:
	RDOLogEdit();
	virtual ~RDOLogEdit();

	void setEditorStyle( RDOLogEditStyle* _style );

	void gotoNext();
	void gotoPrev();

	virtual void clearAll();
	void appendLine( RDOLogEditLineInfo* line );
};

}; // namespace rdoEditCtrl

//{{AFX_INSERT_LOCATION}}

#endif // RDOLOGEDIT_H
