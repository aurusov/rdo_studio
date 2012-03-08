#if !defined(AFX_RDOPROCESS_SHAPE_RESOURCE_DLG1_MJ_H__08926353_FF43_421C_8D32_681628748E52__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_RESOURCE_DLG1_MJ_H__08926353_FF43_421C_8D32_681628748E52__INCLUDED_

// rdoprocess_shape_resource_DLG1_MJ.h : header file
//
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_resource.h"
/////////////////////////////////////////////////////////////////////////////
// RPShapeResourceDLG1_MJ dialog

class RPShapeResourceDLG1_MJ : public CDialog
{
// Construction
public:
	RPShapeResourceDLG1_MJ(CWnd* pParent = NULL, RPShapeResource_MJ* ppParent = NULL); // standard constructor
	virtual BOOL OnInitDialog();


	RPShapeResource_MJ* pParentMJ;
// Dialog Data
	enum { IDD = IDD_DIALOG_RESOURCE_DLG1 };
	CString	m_name;
	int		m_dlgamount;

private:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_RESOURCE_DLG1_MJ_H__08926353_FF43_421C_8D32_681628748E52__INCLUDED_)
