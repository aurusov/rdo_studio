#if !defined(AFX_RDOPROCESS_SHAPE_PROCESS_DLG2_MJ_H__CDF27E31_27E6_4234_B896_4E8D70947E9F__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_PROCESS_DLG2_MJ_H__CDF27E31_27E6_4234_B896_4E8D70947E9F__INCLUDED_

#include "app/rdo_studio_mfc/resource.h"
#include <list>
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_process_dlg1.h"

class RPShapeProcessDlg2_MJ : public CDialog
{
public:
	void list_name_resource();
	RPShapeProcessDlg2_MJ(CWnd* pParent = NULL,RPShapeProcessMJ* ppParent=NULL,RPShapeProcessDlg1_MJ* pParentDlg = NULL );
	virtual BOOL OnInitDialog();
	
	RPShapeProcessMJ* pParentMJ;
	RPShapeProcessDlg1_MJ* pParentDlgMJ;

	std::list< RPObject* > list_resource_MJ;

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
