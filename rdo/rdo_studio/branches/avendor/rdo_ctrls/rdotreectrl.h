#ifndef RDOTREECTRL_H
#define RDOTREECTRL_H
#pragma once

// ----------------------------------------------------------------------------
// ---------- RDOTreeCtrl
// ----------------------------------------------------------------------------
class RDOTreeCtrl: public CTreeCtrl
{
DECLARE_DYNAMIC( RDOTreeCtrl )
protected:

	//{{AFX_MSG(RDOTreeCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOTreeCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

public:
	RDOTreeCtrl();
	virtual ~RDOTreeCtrl();

	void deleteChildren( const HTREEITEM parent );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDOTREECTRL_H
