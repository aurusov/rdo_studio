#if !defined(AFX_RDOPROCESS_SHAPE_PROCESS_DLG2_MJ_H__CDF27E31_27E6_4234_B896_4E8D70947E9F__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_PROCESS_DLG2_MJ_H__CDF27E31_27E6_4234_B896_4E8D70947E9F__INCLUDED_

#include "app/rdo_studio_mfc/resource.h"
#include <list>
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_process_dlg1.h"

class RPProcessShapeProcessDlg2 : public CDialog
{
public:
	void list_name_resource();
	RPProcessShapeProcessDlg2(CWnd* pParent = NULL,RPProcessShapeProcess* ppParent=NULL,RPProcessShapeProcessDlg1* pParentDlg = NULL );
	virtual BOOL OnInitDialog();
	
	RPProcessShapeProcess* m_pParent;
	RPProcessShapeProcessDlg1* pParentDlgMJ;

	std::list< RPObject* > m_resourceList;

	enum { IDD = IDD_DIALOG_PROCESS_2_MJ };
	CComboBox	m_combo_res_control;
	CString	m_combo_res_value;

private:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnCloseupCombo1();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_PROCESS_DLG2_MJ_H__CDF27E31_27E6_4234_B896_4E8D70947E9F__INCLUDED_)
