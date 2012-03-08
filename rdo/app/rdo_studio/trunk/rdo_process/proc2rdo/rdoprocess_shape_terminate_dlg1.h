#if !defined(AFX_RDOPROCESS_SHAPE_TERMINATE_DLG1_MJ_H__0B17943D_987C_48BF_AEDD_79F2B8DB6275__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_TERMINATE_DLG1_MJ_H__0B17943D_987C_48BF_AEDD_79F2B8DB6275__INCLUDED_

// rdoprocess_shape_terminate_dlg1_MJ.h : header file
//
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_terminate.h"
/////////////////////////////////////////////////////////////////////////////
// RPShapeTerminateDlg1_MJ dialog

class RPShapeTerminateDlg1_MJ: public CDialog
{
public:
	RPShapeTerminateDlg1_MJ(PTR(CWnd) pWndParent = NULL, PTR(RPShapeTerminateMJ) pShape = NULL);
	virtual BOOL OnInitDialog();

	PTR(RPShapeTerminateMJ) m_pShape;

	enum { IDD = IDD_DIALOG_TERMINATE_1_MJ };
	CString    m_name;
	int        m_terminateCounter;

private:
	virtual void DoDataExchange(PTR(CDataExchange) pDX); // DDX/DDV support
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_TERMINATE_DLG1_MJ_H__0B17943D_987C_48BF_AEDD_79F2B8DB6275__INCLUDED_)
