#ifndef RDOEDITORRESULTS_H
#define RDOEDITORRESULTS_H
#pragma once

#include "rdoeditorbaseedit.h"
#include "rdoeditoreditstyle.h"

namespace rdoEditor {

// ----------------------------------------------------------------------------
// ---------- RDOEditorResults
// ----------------------------------------------------------------------------
class RDOEditorResults: public RDOEditorBase
{
protected:
	//{{AFX_MSG(RDOEditorResults)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOEditorResults)
	//}}AFX_VIRTUAL

public:
	RDOEditorResults();
	virtual ~RDOEditorResults();

	void setEditorStyle( RDOEditorResultsStyle* _style );
};

}; // namespace rdoEditor

//{{AFX_INSERT_LOCATION}}

#endif // RDOEDITORRESULTS_H
