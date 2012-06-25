#if !defined(AFX_RDOPROCESS_SHAPE_TERMINATE_DLG1_MJ_H__0B17943D_987C_48BF_AEDD_79F2B8DB6275__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_TERMINATE_DLG1_MJ_H__0B17943D_987C_48BF_AEDD_79F2B8DB6275__INCLUDED_

#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_terminate.h"

class RPProcessShapeTerminateDlg1: public CDialog
{
public:
	RPProcessShapeTerminateDlg1(PTR(CWnd) pWndParent = NULL, PTR(RPProcessShapeTerminate) pShape = NULL);
	virtual BOOL OnInitDialog();

	PTR(RPProcessShapeTerminate) m_pShape;

	enum { IDD = IDD_DIALOG_PROCESS_SHAPE_TERMINATE };
	CString    m_name;
	int        m_terminateCounter;

private:
	virtual void DoDataExchange(PTR(CDataExchange) pDX);
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_TERMINATE_DLG1_MJ_H__0B17943D_987C_48BF_AEDD_79F2B8DB6275__INCLUDED_)
