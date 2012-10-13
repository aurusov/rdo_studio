#ifndef _PROC2RDO_RDOPROCESS_SHAPE_PROCESS_DLG2_H_
#define _PROC2RDO_RDOPROCESS_SHAPE_PROCESS_DLG2_H_

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
	RPProcessShapeProcessDlg1* m_pParentDlg;

	std::list< RPObject* > m_resourceList;

	enum { IDD = IDD_DIALOG_PROCESS_SHAPE_PROCESS_2 };
	CComboBox	m_combo_res_control;
	CString	m_combo_res_value;

private:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnCloseupCombo1();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

#endif // _PROC2RDO_RDOPROCESS_SHAPE_PROCESS_DLG2_H_
