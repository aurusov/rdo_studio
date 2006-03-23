// rdoprocess_shape_create_dlg1_MJ.cpp : implementation file
//

#include "stdafx.h"
//#include "rdo_process.h"
#include "rdoprocess_shape_create_dlg1_MJ.h"
#include "rdoprocess_shape_create_dlg2_MJ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RPShapeCreateDlg1_MJ dialog


RPShapeCreateDlg1_MJ::RPShapeCreateDlg1_MJ(CWnd* pParent /*=NULL*/)
	: CDialog(RPShapeCreateDlg1_MJ::IDD, pParent)
{
	//{{AFX_DATA_INIT(RPShapeCreateDlg1_MJ)
	m_create_dlg1_edit_expectation_MJ = _T("");
	m_create_dlg1_dispersion_MJ = _T("");
	m_create_dlg1_min_MJ = _T("");
	m_create_dlg1_max_MJ = _T("");
	//}}AFX_DATA_INIT

}


void RPShapeCreateDlg1_MJ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RPShapeCreateDlg1_MJ)
	DDX_Control(pDX, IDC_EDIT7, m_create_dlg1_max_control_MJ);
	DDX_Control(pDX, IDC_EDIT6, m_create_dlg1_min_control_MJ);
	DDX_Control(pDX, IDC_EDIT5, m_create_dlg1_disp_control_MJ);
	DDX_Control(pDX, IDC_EDIT4, m_create_dlg1_exp_control_MJ);
	DDX_Control(pDX, IDC_COMBO1, m_create_dlg1_combo1_MJ);
	DDX_Text(pDX, IDC_EDIT4, m_create_dlg1_edit_expectation_MJ);
	DDX_Text(pDX, IDC_EDIT5, m_create_dlg1_dispersion_MJ);
	DDX_Text(pDX, IDC_EDIT6, m_create_dlg1_min_MJ);
	DDX_Text(pDX, IDC_EDIT7, m_create_dlg1_max_MJ);
	//}}AFX_DATA_MAP
}


BOOL RPShapeCreateDlg1_MJ::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_create_dlg1_combo1_MJ.SetCurSel(0);
	
	return TRUE;
}



BEGIN_MESSAGE_MAP(RPShapeCreateDlg1_MJ, CDialog)
	//{{AFX_MSG_MAP(RPShapeCreateDlg1_MJ)
	ON_CBN_CLOSEUP(IDC_COMBO1, OnCloseupCombo1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RPShapeCreateDlg1_MJ message handlers

void RPShapeCreateDlg1_MJ::OnCloseupCombo1() 
{

int cur = m_create_dlg1_combo1_MJ.GetCurSel();

switch(cur) // определяем активные окна исходя из закона
{
case 0: // константа
	m_create_dlg1_exp_control_MJ.EnableWindow(TRUE);
	m_create_dlg1_disp_control_MJ.EnableWindow(FALSE);
	m_create_dlg1_max_control_MJ.EnableWindow(FALSE);
	m_create_dlg1_min_control_MJ.EnableWindow(FALSE);
					break;	
case 1: // нормальный
	m_create_dlg1_exp_control_MJ.EnableWindow(TRUE);
	m_create_dlg1_disp_control_MJ.EnableWindow(TRUE);
	m_create_dlg1_max_control_MJ.EnableWindow(FALSE);
	m_create_dlg1_min_control_MJ.EnableWindow(FALSE);
					break;
case 2: // равномерный закон
	m_create_dlg1_exp_control_MJ.EnableWindow(FALSE);
	m_create_dlg1_disp_control_MJ.EnableWindow(FALSE);
	m_create_dlg1_max_control_MJ.EnableWindow(TRUE);
	m_create_dlg1_min_control_MJ.EnableWindow(TRUE);
					break;
case 3: // экспоненциальный
	m_create_dlg1_exp_control_MJ.EnableWindow(TRUE);
	m_create_dlg1_disp_control_MJ.EnableWindow(FALSE);
	m_create_dlg1_max_control_MJ.EnableWindow(FALSE);
	m_create_dlg1_min_control_MJ.EnableWindow(FALSE);
					break;

}

UpdateData(FALSE);	
}

void RPShapeCreateDlg1_MJ::OnButton1() 
{
	RPShapeCreateDlg2_MJ dlg2;
	dlg2.DoModal();
	
}
