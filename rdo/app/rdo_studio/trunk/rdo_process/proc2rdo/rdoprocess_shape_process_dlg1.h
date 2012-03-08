#if !defined(AFX_RDOPROCESS_SHAPE_PROCESS_DLG1_MJ_H__0DF5DDB9_3E18_409E_A23C_A0A0F476FFFB__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_PROCESS_DLG1_MJ_H__0DF5DDB9_3E18_409E_A23C_A0A0F476FFFB__INCLUDED_

// rdoprocess_shape_process_dlg1_MJ.h : header file
//
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_process.h"
/////////////////////////////////////////////////////////////////////////////
// RPShapeProcessDlg1_MJ dialog

class RPListBox: public CListBox
{
private:
	DECLARE_MESSAGE_MAP()
};

class RPShapeProcessDlg1_MJ : public CDialog
{
// Construction
public:
	RPShapeProcessDlg1_MJ(CWnd* pParent = NULL,RPShapeProcessMJ* ppParent=NULL);   // standard constructor
    virtual BOOL OnInitDialog();

	RPShapeProcessMJ* pParentMJ; // ссылка на родителя
// Dialog Data

	enum { IDD = IDD_DIALOG_PROCESS_1_MJ };
	CComboBox	m_gtype;
	CComboBox	m_gqueque;
	RPListBox	m_ResList;
	CButton		m_DelRes;
	CButton		m_AddRes;
	CEdit		m_proc_dlg1_max_control_MJ;
	CEdit		m_proc_dlg1_disp_control_MJ;
	CEdit		m_proc_dlg1_exp_control_MJ;
	CStatic		m_proc_dlgl_exp_text_MJ;
	CStatic		m_proc_dlgl_disp_text_MJ;
	CStatic		m_proc_dlgl_max_text_MJ;
	CComboBox	m_parameter;
	CComboBox	m_action;
	CString	m_name;
	CString	m_list_text;
	int		m_gprior;
	double	m_gexp;
	double	m_gdisp;
	double	m_gmin;
	double	m_gmax;
	int		m_gbase_gen;

private:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	CBrush brush1;
	CBrush brush2;

private:
	afx_msg void OnCbnSelchange1();
	afx_msg void OnCbnSelchange3();
	afx_msg void OnButton1();
	virtual void OnOK();
	afx_msg void OnCbnSelchange2();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButton2();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_PROCESS_DLG1_MJ_H__0DF5DDB9_3E18_409E_A23C_A0A0F476FFFB__INCLUDED_)
