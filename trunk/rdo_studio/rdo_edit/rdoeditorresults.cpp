#include "stdafx.h"
#include "rdoeditorresults.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace rdoEditor;

// ----------------------------------------------------------------------------
// ---------- RDOEditorResults
// ---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOEditorResults, RDOEditorBaseEdit )
	//{{AFX_MSG_MAP(RDOEditorResults)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOEditorResults::RDOEditorResults(): RDOEditorBaseEdit()
{
}

RDOEditorResults::~RDOEditorResults()
{
}

int RDOEditorResults::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( RDOEditorBaseEdit ::OnCreate(lpCreateStruct) == -1 ) return -1;

	setReadOnly( true );

	return 0;
}

void RDOEditorResults::setEditorStyle( RDOEditorResultsStyle* _style )
{
	RDOEditorBaseEdit::setEditorStyle( _style );
	if ( !style ) return;
}
