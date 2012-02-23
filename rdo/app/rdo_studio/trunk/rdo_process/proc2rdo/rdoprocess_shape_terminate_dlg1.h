#if !defined(AFX_RDOPROCESS_SHAPE_TERMINATE_DLG1_MJ_H__0B17943D_987C_48BF_AEDD_79F2B8DB6275__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_TERMINATE_DLG1_MJ_H__0B17943D_987C_48BF_AEDD_79F2B8DB6275__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
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

	//{{AFX_DATA(RPShapeTerminateDlg1_MJ)
	enum { IDD = IDD_DIALOG_TERMINATE_1_MJ };
	CString    m_name;
	int        m_terminateCounter;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(RPShapeTerminateDlg1_MJ)
protected:
	virtual void DoDataExchange(PTR(CDataExchange) pDX); // DDX/DDV support
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RPShapeTerminateDlg1_MJ)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RDOPROCESS_SHAPE_TERMINATE_DLG1_MJ_H__0B17943D_987C_48BF_AEDD_79F2B8DB6275__INCLUDED_)
