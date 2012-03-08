#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_decide_dlg1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RPShapeDecideDlg1::RPShapeDecideDlg1(CWnd* pParent,RPShapeDecide* ppParent)
	: CDialog(RPShapeDecideDlg1::IDD, pParent)
{
	m_name = _T("");
	mp_true = 0.9;
	mp_false = 0.1;

	pParentMJ = ppParent;
}



BOOL RPShapeDecideDlg1::OnInitDialog()
{
// отображение имени блока
	CString str( pParentMJ->getName().c_str() );
    m_name = str;
	mp_true=pParentMJ->ptrue;
	mp_false=pParentMJ->pfalse;
	UpdateData(FALSE);
	 
return TRUE;
}



void RPShapeDecideDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_Text(pDX, IDC_EDIT2, mp_true);
	DDX_Text(pDX, IDC_EDIT3, mp_false);
}


BEGIN_MESSAGE_MAP(RPShapeDecideDlg1, CDialog)
END_MESSAGE_MAP()

void RPShapeDecideDlg1::OnOK() 
{
	UpdateData( true );
	if ( pParentMJ->setName( rp::string(m_name) ) ) {
		CDialog::OnOK();
		pParentMJ->ptrue=mp_true;
		pParentMJ->pfalse=mp_false;
	}
	
}
