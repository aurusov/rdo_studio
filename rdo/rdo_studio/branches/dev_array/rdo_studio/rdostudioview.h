#ifndef RDOSTUDIOVIEW_H
#define RDOSTUDIOVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioView
// ----------------------------------------------------------------------------
class RDOStudioView: public CView
{
protected:
	RDOStudioView();
	DECLARE_DYNAMIC(RDOStudioView)

public:
	virtual ~RDOStudioView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	//{{AFX_VIRTUAL(RDOStudioView)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOVIEW_H
