#ifndef RDOCATALOGMAINFRM_H
#define RDOCATALOGMAINFRM_H

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- RDOCatalogMainFrame
// ----------------------------------------------------------------------------
class RDOCatalogMainFrame: public CFrameWnd
{
	
protected:
	RDOCatalogMainFrame();
	DECLARE_DYNCREATE(RDOCatalogMainFrame)

public:
	virtual ~RDOCatalogMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//{{AFX_VIRTUAL(RDOCatalogMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOCatalogMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOCATALOGMAINFRM_H
