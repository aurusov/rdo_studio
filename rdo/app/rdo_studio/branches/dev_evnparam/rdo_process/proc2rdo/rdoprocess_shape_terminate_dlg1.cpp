// rdoprocess_shape_terminate_dlg1_MJ.cpp : implementation file
//

#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "rdoprocess_shape_terminate_dlg1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RPShapeTerminateDlg1_MJ dialog


RPShapeTerminateDlg1_MJ::RPShapeTerminateDlg1_MJ(CWnd* pParent /*=NULL*/,RPShapeTerminateMJ* ppParent)
	: CDialog(RPShapeTerminateDlg1_MJ::IDD, pParent)
{
	//{{AFX_DATA_INIT(RPShapeTerminateDlg1_MJ)
	m_name = _T("");
	//}}AFX_DATA_INIT
	pParentMJ = ppParent;
}



BOOL RPShapeTerminateDlg1_MJ::OnInitDialog()
{
	CDialog::OnInitDialog();
// отображение имени блока
	CString str( pParentMJ->getName().c_str() );
    m_name = str;
	m_term_inc=1;
	UpdateData(FALSE);

return TRUE;
}



void RPShapeTerminateDlg1_MJ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RPShapeTerminateDlg1_MJ)
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_Text(pDX, IDC_EDIT3, m_term_inc);
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
	UpdateData( true );
	if ( pParentMJ->setName( rp::string(m_name) ) ) {
		pParentMJ->m_term_inc=m_term_inc;
		pParentMJ->m_name=m_name;
		CDialog::OnOK();
	}
}
