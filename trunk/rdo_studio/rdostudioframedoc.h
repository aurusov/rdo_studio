#ifndef RDOSTUDIOFRAMEDOC_H
#define RDOSTUDIOFRAMEDOC_H

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioFrameDoc
// ----------------------------------------------------------------------------
class RDOStudioFrameDoc: public CDocument
{
protected:
	RDOStudioFrameDoc();
	DECLARE_DYNCREATE(RDOStudioFrameDoc)

public:
	//{{AFX_VIRTUAL(RDOStudioFrameDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioFrameDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual ~RDOStudioFrameDoc();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOFRAMEDOC_H
