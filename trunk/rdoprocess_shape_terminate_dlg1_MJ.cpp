// rdoprocess_shape_terminate_dlg1_MJ.cpp : implementation file
//

#include "stdafx.h"
//#include "rdo_process.h"
#include "rdoprocess_shape_terminate_dlg1_MJ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RPShapeTerminateDlg1_MJ dialog


RPShapeTerminateDlg1_MJ::RPShapeTerminateDlg1_MJ(CWnd* pParent /*=NULL*/)
	: CDialog(RPShapeTerminateDlg1_MJ::IDD, pParent)
{
	//{{AFX_DATA_INIT(RPShapeTerminateDlg1_MJ)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void RPShapeTerminateDlg1_MJ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RPShapeTerminateDlg1_MJ)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RPShapeTerminateDlg1_MJ, CDialog)
	//{{AFX_MSG_MAP(RPShapeTerminateDlg1_MJ)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RPShapeTerminateDlg1_MJ message handlers

void RPShapeTerminateDlg1_MJ::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
