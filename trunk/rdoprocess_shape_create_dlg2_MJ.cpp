// rdoprocess_shape_create_dlg2_MJ.cpp : implementation file
//

#include "stdafx.h"
//#include "rdo_process.h"
#include "rdoprocess_shape_create_dlg2_MJ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RPShapeCreateDlg2_MJ dialog


RPShapeCreateDlg2_MJ::RPShapeCreateDlg2_MJ(CWnd* pParent /*=NULL*/)
	: CDialog(RPShapeCreateDlg2_MJ::IDD, pParent)
{
	//{{AFX_DATA_INIT(RPShapeCreateDlg2_MJ)
	m_create_dlg2_edit_inf = 0;
	//}}AFX_DATA_INIT
}


void RPShapeCreateDlg2_MJ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RPShapeCreateDlg2_MJ)
	DDX_Text(pDX, IDC_EDIT1, m_create_dlg2_edit_inf);
	//}}AFX_DATA_MAP
}


BOOL RPShapeCreateDlg2_MJ::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}



BEGIN_MESSAGE_MAP(RPShapeCreateDlg2_MJ, CDialog)
	//{{AFX_MSG_MAP(RPShapeCreateDlg2_MJ)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RPShapeCreateDlg2_MJ message handlers
