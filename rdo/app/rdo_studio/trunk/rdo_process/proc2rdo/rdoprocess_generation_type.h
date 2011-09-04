#if !defined(AFX_RDOPROCESS_GENERATION_TYPE_MJ_H__6E103D6A_16C3_4ADD_869A_70505FA8090B__INCLUDED_)
#define AFX_RDOPROCESS_GENERATION_TYPE_MJ_H__6E103D6A_16C3_4ADD_869A_70505FA8090B__INCLUDED_
#include "app/rdo_studio_mfc/resource.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// rdoprocess_generation_type_MJ.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RP_GENERATION_TYPE_MJ dialog

class RP_GENERATION_TYPE_MJ : public CDialog
{
// Construction
public:
	RP_GENERATION_TYPE_MJ(CWnd* pParent = NULL);   // standard constructor
	virtual BOOL OnInitDialog();
// Dialog Data
	//{{AFX_DATA(RP_GENERATION_TYPE_MJ)
	enum { IDD = IDD_DIALOG_GEN_TYPE };
	CComboBox	m_combo;
	CEdit	m_time;
	double	m_time_value;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RP_GENERATION_TYPE_MJ)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RP_GENERATION_TYPE_MJ)
	afx_msg void OnCloseupCombo1();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RDOPROCESS_GENERATION_TYPE_MJ_H__6E103D6A_16C3_4ADD_869A_70505FA8090B__INCLUDED_)
