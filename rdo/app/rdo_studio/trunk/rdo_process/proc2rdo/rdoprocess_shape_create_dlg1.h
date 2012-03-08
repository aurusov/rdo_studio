#if !defined(AFX_RDOPROCESS_SHAPE_CREATE_DLG1_MJ_H__CBBEC246_A9B1_4006_BBD0_AD6A6ED903F2__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_CREATE_DLG1_MJ_H__CBBEC246_A9B1_4006_BBD0_AD6A6ED903F2__INCLUDED_

// rdoprocess_shape_create_dlg1_MJ.h : header file
//
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create.h"
#include "app/rdo_studio_mfc/resource.h"

/////////////////////////////////////////////////////////////////////////////
// RPShapeCreateDlg1_MJ dialog

class RPShapeCreateDlg1_MJ : public CDialog
{
protected:
	RPShapeCreateMJ* pParentMJ; // ссылка на объект, который вызвал диалог
	
	// Construction
public:
	RPShapeCreateDlg1_MJ(CWnd* pParent = NULL,RPShapeCreateMJ* ppParent=NULL);   // standard constructor
virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DIALOG_CREATE_1_MJ };
	CEdit		m_create_dlg1_exp_control_MJ;
	CEdit		m_create_dlg1_disp_control_MJ;
	CEdit		m_create_dlg1_max_control_MJ;
	CStatic		m_create_dlgl_exp_text_MJ;
	CStatic		m_create_dlgl_disp_text_MJ;
	CStatic		m_create_dlgl_max_text_MJ;
	CComboBox	m_create_dlg1_combo1_MJ;
	CString		m_name;
	double		m_dlgfirst;
	int			m_dlgamount;
	double		m_dlgexp;
	double		m_dlgdisp;
	double		m_dlgmax;
	int			m_dlgbase_gen;

private:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnCbnSelchange1();
	afx_msg void OnButton1();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_CREATE_DLG1_MJ_H__CBBEC246_A9B1_4006_BBD0_AD6A6ED903F2__INCLUDED_)
