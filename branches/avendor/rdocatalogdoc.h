#ifndef RDOCATALOGDOC_H
#define RDOCATALOGDOC_H

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- RDOCatalogDoc
// ----------------------------------------------------------------------------
class RDOCatalogDoc: public CDocument
{
protected:
	RDOCatalogDoc();
	DECLARE_DYNCREATE(RDOCatalogDoc)

public:
	virtual ~RDOCatalogDoc();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext& dc ) const;
#endif

	//{{AFX_VIRTUAL(RDOCatalogDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOCatalogDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOCATALOGDOC_H
