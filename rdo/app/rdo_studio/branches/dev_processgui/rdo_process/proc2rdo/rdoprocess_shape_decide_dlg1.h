#if !defined(AFX_RDOPROCESS_SHAPE_DECIDE_DLG1_MJ_H__0B17943D_987C_48BF_AEDD_79F2B8DB6275__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_DECIDE_DLG1_MJ_H__0B17943D_987C_48BF_AEDD_79F2B8DB6275__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_decide.h"
/////////////////////////////////////////////////////////////////////////////
// RPShapeDecideDlg1 dialog

class RPShapeDecideDlg1 : public CDialog
{
// Construction
public:
	RPShapeDecideDlg1(CWnd* pParent = NULL, RPShapeDecide* ppParent=NULL);   // standard constructor
    virtual BOOL OnInitDialog();

	RPShapeDecide* pParentMJ; // ссылка на родителя
// Dialog Data
	//{{AFX_DATA(RPShapeDecideDlg1)
	enum { IDD = IDD_DIALOG_DECIDE_1 };
	CString	m_name;
	double	mp_true;
	double	mp_false;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RPShapeDecideDlg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RPShapeDecideDlg1)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RDOPROCESS_SHAPE_TERMINATE_DLG1_MJ_H__0B17943D_987C_48BF_AEDD_79F2B8DB6275__INCLUDED_)
