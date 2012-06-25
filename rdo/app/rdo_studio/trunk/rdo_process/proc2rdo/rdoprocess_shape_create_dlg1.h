#if !defined(AFX_RDOPROCESS_SHAPE_CREATE_DLG1_MJ_H__CBBEC246_A9B1_4006_BBD0_AD6A6ED903F2__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_CREATE_DLG1_MJ_H__CBBEC246_A9B1_4006_BBD0_AD6A6ED903F2__INCLUDED_

#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create.h"
#include "app/rdo_studio_mfc/resource.h"

class RPProcessShapeCreateDlg1 : public CDialog
{
protected:
	RPProcessShapeCreate* pParentMJ; // ссылка на объект, который вызвал диалог
	
public:
	RPProcessShapeCreateDlg1(CWnd* pParent = NULL,RPProcessShapeCreate* ppParent=NULL);
	virtual BOOL OnInitDialog();

	enum { IDD = IDD_DIALOG_CREATE_1_MJ };
	CEdit		m_expCtrl;
	CEdit		m_dispCtrl;
	CEdit		m_maxCtrl;
	CStatic		m_expText;
	CStatic		m_dispText;
	CStatic		m_maxText;
	CComboBox	m_comboCtrl;
	CString		m_name;
	double		m_dlgfirst;
	int			m_dlgamount;
	double		m_dlgexp;
	double		m_dlgdisp;
	double		m_dlgmax;
	int			m_dlgbase_gen;

private:
	virtual void DoDataExchange(CDataExchange* pDX);

	afx_msg void OnCbnSelchange1();
	afx_msg void OnButton1();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_CREATE_DLG1_MJ_H__CBBEC246_A9B1_4006_BBD0_AD6A6ED903F2__INCLUDED_)
