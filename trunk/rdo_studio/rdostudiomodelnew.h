#ifndef RDOSTUDIOMODEL_NEW_H
#define RDOSTUDIOMODEL_NEW_H

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptionsGeneral
// ----------------------------------------------------------------------------
class RDOStudioModelNew: public CDialog
{
protected:
	//{{AFX_DATA(RDOStudioModelNew)
	CString	m_modelName;
	CString	m_modelPath;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(RDOStudioModelNew)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioModelNew)
	afx_msg void OnModelPathButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioModelNew();
	virtual ~RDOStudioModelNew();
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOMODEL_NEW_H
