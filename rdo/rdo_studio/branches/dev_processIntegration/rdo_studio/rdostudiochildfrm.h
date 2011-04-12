#ifndef RDOSTUDIOCHILDFRM_H
#define RDOSTUDIOCHILDFRM_H

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioChildFrame
// ----------------------------------------------------------------------------
class RDOStudioChildFrame: public CMDIChildWnd
{
	DECLARE_DYNCREATE(RDOStudioChildFrame)

public:
	RDOStudioChildFrame();
	virtual ~RDOStudioChildFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	//{{AFX_VIRTUAL(RDOStudioChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioChildFrame)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOCHILDFRM_H
