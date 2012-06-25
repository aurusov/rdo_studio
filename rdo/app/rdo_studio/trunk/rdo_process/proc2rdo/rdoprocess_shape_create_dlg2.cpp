#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create_dlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RPProcessShapeCreateDlg2::RPProcessShapeCreateDlg2(CWnd* pParent,RPProcessShapeCreate* ppParent)
	: CDialog(RPProcessShapeCreateDlg2::IDD, pParent)
{
	m_dlginf = 0;
	m_dlgid = 0;
	m_dlgpar1 = 0.0;
	m_dlgpar2 = 0.0;
	m_dlgpar3 = 0.0;
	m_pParent = ppParent;
}

void RPProcessShapeCreateDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_dlginf);
	DDX_Text(pDX, IDC_EDIT2, m_dlgid);
	DDX_Text(pDX, IDC_EDIT3, m_dlgpar1);
	DDX_Text(pDX, IDC_EDIT4, m_dlgpar2);
	DDX_Text(pDX, IDC_EDIT5, m_dlgpar3);
}

BOOL RPProcessShapeCreateDlg2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// инициализация окна значениями объекта

	m_dlginf  = m_pParent->inf; // бесконечноть
	m_dlgid   = m_pParent->gID; // ID группы
	m_dlgpar1 = m_pParent->gpar1;
	m_dlgpar2 = m_pParent->gpar2;
	m_dlgpar3 = m_pParent->gpar3;

	UpdateData(FALSE);
	return TRUE;
}

BEGIN_MESSAGE_MAP(RPProcessShapeCreateDlg2, CDialog)
END_MESSAGE_MAP()

void RPProcessShapeCreateDlg2::OnOK() 
{
	// инициализация объекта значениями окна

	UpdateData(TRUE);
	
	m_pParent->inf   = m_dlginf; // бесконечноть
	m_pParent->gID   = m_dlgid; // ID группы
	m_pParent->gpar1 = m_dlgpar1;
	m_pParent->gpar2 = m_dlgpar2;
	m_pParent->gpar3 = m_dlgpar3;

	CDialog::OnOK();
}
