#ifndef RDOCATALOGVIEW_H
#define RDOCATALOGVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- RDOCatalogView
// ----------------------------------------------------------------------------
class RDOCatalogView: public CHtmlView
{
protected:
	RDOCatalogView();
	DECLARE_DYNCREATE(RDOCatalogView)

public:
	virtual ~RDOCatalogView();

	RDOCatalogDoc* GetDocument();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext& dc ) const;
#endif

	//{{AFX_VIRTUAL(RDOCatalogView)
	public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	virtual void OnStatusTextChange(LPCTSTR lpszText);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOCatalogView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline RDOCatalogDoc* RDOCatalogView::GetDocument()
{ return static_cast<RDOCatalogDoc*>(m_pDocument); }
#endif

//{{AFX_INSERT_LOCATION}}

#endif // RDOCATALOGVIEW_H
