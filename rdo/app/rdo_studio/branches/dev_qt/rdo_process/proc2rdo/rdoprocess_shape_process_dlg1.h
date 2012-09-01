#ifndef _PROC2RDO_RDOPROCESS_SHAPE_PROCESS_DLG1_H_
#define _PROC2RDO_RDOPROCESS_SHAPE_PROCESS_DLG1_H_

#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_process.h"

class RPListBox: public CListBox
{
private:
	DECLARE_MESSAGE_MAP()
};

class RPProcessShapeProcessDlg1 : public CDialog
{
public:
	RPProcessShapeProcessDlg1(CWnd* pParent = NULL,RPProcessShapeProcess* ppParent=NULL);
    virtual BOOL OnInitDialog();

	RPProcessShapeProcess* m_pParent; // ссылка на родителя

	enum { IDD = IDD_DIALOG_PROCESS_SHAPE_PROCESS_1 };
	CComboBox	m_gtype;
	CComboBox	m_gqueque;
	RPListBox	m_ResList;
	CButton		m_DelRes;
	CButton		m_AddRes;
	CEdit		m_maxCtrl;
	CEdit		m_dispCtrl;
	CEdit		m_expCtrl;
	CStatic		m_expText;
	CStatic		m_dispText;
	CStatic		m_maxText;
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
	virtual void DoDataExchange(CDataExchange* pDX);

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

#endif // _PROC2RDO_RDOPROCESS_SHAPE_PROCESS_DLG1_H_
