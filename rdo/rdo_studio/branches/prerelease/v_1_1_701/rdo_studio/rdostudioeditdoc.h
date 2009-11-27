#ifndef RDOSTUDIOEDITDOC_H
#define RDOSTUDIOEDITDOC_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudioeditbasedoc.h"

// ----------------------------------------------------------------------------
// ---------- RDOStudioEditDoc
// ----------------------------------------------------------------------------
class RDOStudioEditDoc: public RDOStudioEditBaseDoc
{
protected:
	RDOStudioEditDoc();
	virtual void updateModify();

	DECLARE_DYNCREATE(RDOStudioEditDoc)

public:
	//{{AFX_VIRTUAL(RDOStudioEditDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioEditDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual ~RDOStudioEditDoc();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOEDITDOC_H
