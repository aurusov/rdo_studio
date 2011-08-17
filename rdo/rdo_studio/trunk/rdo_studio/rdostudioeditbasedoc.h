#ifndef RDOSTUDIOEDITBASEDOC_H
#define RDOSTUDIOEDITBASEDOC_H

#if _MSC_VER > 1000
#pragma once
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioEditBaseDoc
// --------------------------------------------------------------------------------
class RDOStudioEditBaseDoc: public CDocument
{
protected:
	RDOStudioEditBaseDoc();
	DECLARE_DYNCREATE(RDOStudioEditBaseDoc)

public:
	virtual void updateModify() {};

	//{{AFX_VIRTUAL(RDOStudioEditBaseDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioEditBaseDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual ~RDOStudioEditBaseDoc();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOEDITBASEDOC_H
