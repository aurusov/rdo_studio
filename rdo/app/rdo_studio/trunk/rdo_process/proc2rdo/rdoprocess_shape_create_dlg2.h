#if !defined(AFX_RDOPROCESS_SHAPE_CREATE_DLG2_MJ_H__0B67F21A_9090_4CE0_B47B_AD518FC2589D__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_CREATE_DLG2_MJ_H__0B67F21A_9090_4CE0_B47B_AD518FC2589D__INCLUDED_

#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create.h"

class RPProcessShapeCreateDlg2 : public CDialog
{
public:
	RPProcessShapeCreateDlg2(CWnd* pParent = NULL,RPProcessShapeCreate* ppParent=NULL);
	
	RPProcessShapeCreate* m_pParent; // ссылка на вызывающий объект

	virtual BOOL OnInitDialog();

	enum { IDD = IDD_DIALOG_CREATE_2_MJ };
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

#endif // !defined(AFX_RDOPROCESS_SHAPE_CREATE_DLG2_MJ_H__0B67F21A_9090_4CE0_B47B_AD518FC2589D__INCLUDED_)
