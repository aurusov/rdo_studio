#ifndef RDOSTUDIOMODEL_NEW_H
#define RDOSTUDIOMODEL_NEW_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "resource.h"

// ----------------------------------------------------------------------------
// ---------- RDOStudioOptionsGeneral
// ----------------------------------------------------------------------------
class RDOStudioModelNew: public CDialog
{
friend class RDOStudioApp;

protected:
	//{{AFX_DATA(RDOStudioModelNew)
	enum { IDD = IDD_MODEL_NEW };
	CButton	m_ok;
	CString	m_modelName;
	CString	m_modelPath;
	CString	m_info;
	BOOL	m_comment;
	int		m_model_template;
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
	afx_msg void OnChangeModelName();
	virtual BOOL OnInitDialog();
	afx_msg void OnModelEmpty();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG

	CString getMyDocFolder() const;
	void updateInfo();

	CFont    font_normal;
	CFont    font_red;
	COLORREF color_normal;
	COLORREF color_red;
	bool     need_red;

	DECLARE_MESSAGE_MAP()

public:
	RDOStudioModelNew();
	virtual ~RDOStudioModelNew();
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOMODEL_NEW_H
