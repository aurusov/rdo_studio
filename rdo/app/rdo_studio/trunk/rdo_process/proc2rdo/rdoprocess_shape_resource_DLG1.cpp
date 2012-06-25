#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_resource_DLG1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RPProcessShapeResourceDlg1::RPProcessShapeResourceDlg1 (CWnd* pParent, RPProcessShapeResource* ppParent)
	: CDialog(RPProcessShapeResourceDlg1::IDD, pParent)
{
	m_name = _T("");
	m_dlgamount = 0;
	m_pParent = ppParent;
}

void RPProcessShapeResourceDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_Text(pDX, IDC_EDIT2, m_dlgamount);
}

BOOL RPProcessShapeResourceDlg1::OnInitDialog()
{
	// отображение имени блока
	CString str( m_pParent->getName().c_str() );
	m_name = str;

	m_dlgamount = m_pParent->gamount;
	UpdateData(FALSE);

	return TRUE;
}

BEGIN_MESSAGE_MAP(RPProcessShapeResourceDlg1, CDialog)
END_MESSAGE_MAP()

void RPProcessShapeResourceDlg1::OnOK()
{
	UpdateData(TRUE);
	m_pParent->setName(tstring(m_name));

	m_pParent->gamount = m_dlgamount;
	CDialog::OnOK();
}
