#include "stdafx.h"
#include "rdoeditorresults.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace rdoEditor;
using namespace rdoEditCtrl;

// ----------------------------------------------------------------------------
// ---------- RDOEditorResults
// ---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOEditorResults, RDOEditorBase )
	//{{AFX_MSG_MAP(RDOEditorResults)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOEditorResults::RDOEditorResults(): RDOEditorBase()
{
}

RDOEditorResults::~RDOEditorResults()
{
}

void RDOEditorResults::setEditorStyle( RDOEditorResultsStyle* _style )
{
	RDOEditorBase::setEditorStyle( _style );
	if ( !style ) return;
}
