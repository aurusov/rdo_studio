#if !defined(AFX_RDOPROCESS_SHAPE_PROCESS_DLG2_MJ_H__CDF27E31_27E6_4234_B896_4E8D70947E9F__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_PROCESS_DLG2_MJ_H__CDF27E31_27E6_4234_B896_4E8D70947E9F__INCLUDED_
#include "app/rdo_studio_mfc/resource.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// rdoprocess_shape_process_dlg2_MJ.h : header file
//
#include <list>
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_process_dlg1.h"
/////////////////////////////////////////////////////////////////////////////
// RPShapeProcessDlg2_MJ dialog

class RPShapeProcessDlg2_MJ : public CDialog
{
// Construction
public:
	void list_name_resource();
	RPShapeProcessDlg2_MJ(CWnd* pParent = NULL,RPShapeProcessMJ* ppParent=NULL,RPShapeProcessDlg1_MJ* pParentDlg = NULL );   // standard constructor
	virtual BOOL OnInitDialog();
	
	RPShapeProcessMJ* pParentMJ;
	RPShapeProcessDlg1_MJ* pParentDlgMJ;

	std::list< RPObject* > list_resource_MJ;

// Dialog Data
	//{{AFX_DATA(RPShapeProcessDlg2_MJ)
	enum { IDD = IDD_DIALOG_PROCESS_2_MJ };
	CComboBox	m_combo_res_control;
	CString	m_combo_res_value;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RPShapeProcessDlg2_MJ)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RPShapeProcessDlg2_MJ)
	afx_msg void OnCloseupCombo1();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RDOPROCESS_SHAPE_PROCESS_DLG2_MJ_H__CDF27E31_27E6_4234_B896_4E8D70947E9F__INCLUDED_)
