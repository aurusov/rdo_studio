#ifndef RDOTREECTRL_H
#define RDOTREECTRL_H
#pragma once

// ----------------------------------------------------------------------------
// ---------- RDOTree
// ----------------------------------------------------------------------------
class RDOTree : public CTreeCtrl
{
friend class RDOTreeCtrl;

protected:
	RDOTree();
	virtual ~RDOTree();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	DECLARE_MESSAGE_MAP()
};

// ----------------------------------------------------------------------------
// ---------- RDOTreeCtrl
// ----------------------------------------------------------------------------
class RDOTreeCtrl: public CWnd
{
DECLARE_DYNAMIC( RDOTreeCtrl )
protected:
	RDOTree tree;

	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	DECLARE_MESSAGE_MAP()

public:
	RDOTreeCtrl();
	virtual ~RDOTreeCtrl();

	virtual BOOL OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );

	const RDOTree& getTree() const;
	void setFont( CFont* font );
};

#endif // RDOTREECTRL_H
