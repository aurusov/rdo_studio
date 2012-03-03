#if !defined(AFX_RDOPROCESS_SHAPE_CREATE_DLG2_MJ_H__0B67F21A_9090_4CE0_B47B_AD518FC2589D__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_CREATE_DLG2_MJ_H__0B67F21A_9090_4CE0_B47B_AD518FC2589D__INCLUDED_
#include "app/rdo_studio_mfc/resource.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// rdoprocess_shape_create_dlg2_MJ.h : header file
//
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create.h"
/////////////////////////////////////////////////////////////////////////////
// RPShapeCreateDlg2_MJ dialog

class RPShapeCreateDlg2_MJ : public CDialog
{
// Construction
public:
	RPShapeCreateDlg2_MJ(CWnd* pParent = NULL,RPShapeCreateMJ* ppParent=NULL);    // standard constructor
	
	RPShapeCreateMJ* pParentMJ; // ссылка на вызывающий объект

virtual BOOL OnInitDialog();
// Dialog Data
	//{{AFX_DATA(RPShapeCreateDlg2_MJ)
	enum { IDD = IDD_DIALOG_CREATE_2_MJ };
	int		m_dlginf;
	int		m_dlgid;
	double	m_dlgpar1;
	double	m_dlgpar2;
	double	m_dlgpar3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RPShapeCreateDlg2_MJ)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RPShapeCreateDlg2_MJ)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RDOPROCESS_SHAPE_CREATE_DLG2_MJ_H__0B67F21A_9090_4CE0_B47B_AD518FC2589D__INCLUDED_)
