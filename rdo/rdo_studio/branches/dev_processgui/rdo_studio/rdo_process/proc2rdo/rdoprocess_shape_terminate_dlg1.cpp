// rdoprocess_shape_terminate_dlg1_MJ.cpp : implementation file
//

#include "rdo_studio/rdo_process/proc2rdo/stdafx.h"
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
	m_time_value=0.0;
	//}}AFX_DATA_INIT

	pParentMJ = ppParent;
}



BOOL RPShapeTerminateDlg1_MJ::OnInitDialog()
{
	CDialog::OnInitDialog();
// отображение имени блока
	CString str( pParentMJ->getName().c_str() );
    m_name = str;
	m_combo.SetCurSel(0);
	OnCloseupCombo1();
	m_time_value=100;
	m_term_counter=1;
	UpdateData(FALSE);

return TRUE;
}



void RPShapeTerminateDlg1_MJ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RPShapeTerminateDlg1_MJ)
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Control(pDX, IDC_EDIT2, m_time);
	DDX_Text(pDX, IDC_EDIT2, m_time_value);
	DDX_Text(pDX, IDC_EDIT3, m_term_counter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RPShapeTerminateDlg1_MJ, CDialog)
	//{{AFX_MSG_MAP(RPShapeTerminateDlg1_MJ)
	ON_CBN_CLOSEUP(IDC_COMBO1, OnCloseupCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RPShapeTerminateDlg1_MJ message handlers

void RPShapeTerminateDlg1_MJ::OnCloseupCombo1() 
{
int cur = m_combo.GetCurSel();

	switch(cur) // определяем активные окна исходя из закона
	{
	case 0: // константа
	m_time.EnableWindow(TRUE);
						break;	
	case 1: // нормальный
	m_time.EnableWindow(FALSE);
						break;
	}	
}

void RPShapeTerminateDlg1_MJ::OnOK() 
{
	UpdateData( true );
	if ( pParentMJ->setName( rp::string(m_name) ) ) {
		int cur = m_combo.GetCurSel();
		pParentMJ->m_term_state=cur;
		pParentMJ->m_term_counter=m_term_counter;
		pParentMJ->m_time=m_time_value;
		CDialog::OnOK();
	}
}
