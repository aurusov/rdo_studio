#if !defined(AFX_RDOPROCESS_SHAPE_TERMINATE_DLG1_MJ_H__0B17943D_987C_48BF_AEDD_79F2B8DB6275__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_TERMINATE_DLG1_MJ_H__0B17943D_987C_48BF_AEDD_79F2B8DB6275__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// rdoprocess_shape_terminate_dlg1_MJ.h : header file
//
#include"resource.h"
/////////////////////////////////////////////////////////////////////////////
// RPShapeTerminateDlg1_MJ dialog

class RPShapeTerminateDlg1_MJ : public CDialog
{
// Construction
public:
	RPShapeTerminateDlg1_MJ(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(RPShapeTerminateDlg1_MJ)
	enum { IDD = IDD_DIALOG_TERMINATE_1_MJ };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RPShapeTerminateDlg1_MJ)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RPShapeTerminateDlg1_MJ)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RDOPROCESS_SHAPE_TERMINATE_DLG1_MJ_H__0B17943D_987C_48BF_AEDD_79F2B8DB6275__INCLUDED_)
