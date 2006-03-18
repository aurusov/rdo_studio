#if !defined(AFX_RDOPROCESS_SHAPE_CREATE_DLG1_MJ_H__CBBEC246_A9B1_4006_BBD0_AD6A6ED903F2__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_CREATE_DLG1_MJ_H__CBBEC246_A9B1_4006_BBD0_AD6A6ED903F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// rdoprocess_shape_create_dlg1_MJ.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// RPShapeCreateDlg1_MJ dialog

class RPShapeCreateDlg1_MJ : public CDialog
{
// Construction
public:
	RPShapeCreateDlg1_MJ(CWnd* pParent = NULL);   // standard constructor
virtual BOOL OnInitDialog();
// Dialog Data
	//{{AFX_DATA(RPShapeCreateDlg1_MJ)
	enum { IDD = IDD_DIALOG_CREATE_1_MJ };
	CEdit	m_create_dlg1_max_control_MJ;
	CEdit	m_create_dlg1_min_control_MJ;
	CEdit	m_create_dlg1_disp_control_MJ;
	CEdit	m_create_dlg1_exp_control_MJ;
	CComboBox	m_create_dlg1_combo1_MJ;
	CString	m_create_dlg1_edit_expectation_MJ;
	CString	m_create_dlg1_dispersion_MJ;
	CString	m_create_dlg1_min_MJ;
	CString	m_create_dlg1_max_MJ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RPShapeCreateDlg1_MJ)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RPShapeCreateDlg1_MJ)
	afx_msg void OnCloseupCombo1();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RDOPROCESS_SHAPE_CREATE_DLG1_MJ_H__CBBEC246_A9B1_4006_BBD0_AD6A6ED903F2__INCLUDED_)
