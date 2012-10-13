#ifndef _PROC2RDO_RDOPROCESS_SHAPE_CREATE_DLG2_H_
#define _PROC2RDO_RDOPROCESS_SHAPE_CREATE_DLG2_H_

#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create.h"

class RPProcessShapeCreateDlg2 : public CDialog
{
public:
	RPProcessShapeCreateDlg2(CWnd* pParent = NULL,RPProcessShapeCreate* ppParent=NULL);
	
	RPProcessShapeCreate* m_pParent; // ссылка на вызывающий объект

	virtual BOOL OnInitDialog();

	enum { IDD = IDD_DIALOG_PROCESS_SHAPE_CREATE_2 };
	int		m_dlginf;
	int		m_dlgid;
	double	m_dlgpar1;
	double	m_dlgpar2;
	double	m_dlgpar3;

private:
	virtual void DoDataExchange(CDataExchange* pDX);

	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

#endif // _PROC2RDO_RDOPROCESS_SHAPE_CREATE_DLG2_H_
