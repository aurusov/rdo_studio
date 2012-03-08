#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create_dlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RPShapeCreateDlg2_MJ::RPShapeCreateDlg2_MJ(CWnd* pParent,RPShapeCreateMJ* ppParent)
	: CDialog(RPShapeCreateDlg2_MJ::IDD, pParent)
{
	m_dlginf = 0;
	m_dlgid = 0;
	m_dlgpar1 = 0.0;
	m_dlgpar2 = 0.0;
	m_dlgpar3 = 0.0;
	pParentMJ = ppParent;
}


void RPShapeCreateDlg2_MJ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_dlginf);
	DDX_Text(pDX, IDC_EDIT2, m_dlgid);
	DDX_Text(pDX, IDC_EDIT3, m_dlgpar1);
	DDX_Text(pDX, IDC_EDIT4, m_dlgpar2);
	DDX_Text(pDX, IDC_EDIT5, m_dlgpar3);
}


BOOL RPShapeCreateDlg2_MJ::OnInitDialog()
{
	CDialog::OnInitDialog();

	// инициализация окна значениями объекта
    
	m_dlginf= pParentMJ->inf; // бесконечноть
	m_dlgid=  pParentMJ->gID; // ID группы
	m_dlgpar1=pParentMJ->gpar1;
	m_dlgpar2=pParentMJ->gpar2;
	m_dlgpar3=pParentMJ->gpar3;

UpdateData(FALSE);
	return TRUE;
}



BEGIN_MESSAGE_MAP(RPShapeCreateDlg2_MJ, CDialog)
END_MESSAGE_MAP()

void RPShapeCreateDlg2_MJ::OnOK() 
{
	// инициализация объекта значениями окна

    UpdateData(TRUE);
	
	pParentMJ->inf   = m_dlginf; // бесконечноть
	pParentMJ->gID   = m_dlgid; // ID группы
	pParentMJ->gpar1 = m_dlgpar1;
	pParentMJ->gpar2 = m_dlgpar2;
	pParentMJ->gpar3 = m_dlgpar3;
  
		
	CDialog::OnOK();
}
