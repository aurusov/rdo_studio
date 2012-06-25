#ifndef _PROC2RDO_RDOPROCESS_SHAPE_RESOURCE_DLG_H_
#define _PROC2RDO_RDOPROCESS_SHAPE_RESOURCE_DLG_H_

#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_resource.h"

class RPProcessShapeResourceDlg1 : public CDialog
{
public:
	RPProcessShapeResourceDlg1(CWnd* pParent = NULL, RPProcessShapeResource* ppParent = NULL);
	virtual BOOL OnInitDialog();


	RPProcessShapeResource* m_pParent;

	enum { IDD = IDD_DIALOG_RESOURCE_DLG1 };
	CString	m_name;
	int		m_dlgamount;

private:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

#endif // _PROC2RDO_RDOPROCESS_SHAPE_RESOURCE_DLG_H_
