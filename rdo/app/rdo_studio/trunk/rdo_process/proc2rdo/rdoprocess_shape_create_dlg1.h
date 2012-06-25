#ifndef _PROC2RDO_RDOPROCESS_SHAPE_CREATE_DLG1_H_
#define _PROC2RDO_RDOPROCESS_SHAPE_CREATE_DLG1_H_

#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create.h"
#include "app/rdo_studio_mfc/resource.h"

class RPProcessShapeCreateDlg1 : public CDialog
{
protected:
	RPProcessShapeCreate* m_pParent; // ссылка на объект, который вызвал диалог
	
public:
	RPProcessShapeCreateDlg1(CWnd* pParent = NULL,RPProcessShapeCreate* ppParent=NULL);
	virtual BOOL OnInitDialog();

	enum { IDD = IDD_DIALOG_PROCESS_SHAPE_CREATE_1 };
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

#endif // _PROC2RDO_RDOPROCESS_SHAPE_CREATE_DLG1_H_
