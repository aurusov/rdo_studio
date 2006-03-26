// rdoprocess_shape_process_dlg1_MJ.cpp : implementation file
//

#include "stdafx.h"
//#include "rdo_process.h"
#include "rdoprocess_shape_process_dlg1_MJ.h"
#include "rdoprocess_shape_process_dlg2_MJ.h"

//#include "resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// RPShapeProcessDlg1_MJ dialog


RPShapeProcessDlg1_MJ::RPShapeProcessDlg1_MJ(CWnd* pParent /*=NULL*/,RPShapeProcessMJ* ppParent)
	: CDialog(RPShapeProcessDlg1_MJ::IDD, pParent)
{
	//{{AFX_DATA_INIT(RPShapeProcessDlg1_MJ)
	m_name = _T("");
	//}}AFX_DATA_INIT

    pParentMJ = ppParent;
}


void RPShapeProcessDlg1_MJ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RPShapeProcessDlg1_MJ)
	DDX_Control(pDX, IDC_LIST1, m_ResList);
	DDX_Control(pDX, IDC_BUTTON2, m_DelRes);
	DDX_Control(pDX, IDC_BUTTON1, m_AddRes);
	DDX_Control(pDX, IDC_EDIT7, m_proc_dlg1_max_control_MJ);
	DDX_Control(pDX, IDC_EDIT6, m_proc_dlg1_min_control_MJ);
	DDX_Control(pDX, IDC_EDIT5, m_proc_dlg1_disp_control_MJ);
	DDX_Control(pDX, IDC_EDIT4, m_proc_dlg1_exp_control_MJ);
	DDX_Control(pDX, IDC_COMBO4, m_parameter);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Control(pDX, IDC_COMBO3, m_type);
	DDX_Control(pDX, IDC_COMBO2, m_action);
	DDX_Text(pDX, IDC_EDIT1, m_name);
	//}}AFX_DATA_MAP
}


BOOL RPShapeProcessDlg1_MJ::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_combo.SetCurSel(0);
	m_action.SetCurSel(0);

	m_type.SetCurSel(0); // по умолчанию закон fifo
	m_parameter.EnableWindow(FALSE);// параметр не видно

	// отображение имени в окне
	CString str( pParentMJ->getName().c_str() );
    m_name = str;
	UpdateData(FALSE);

	return TRUE;
}











BEGIN_MESSAGE_MAP(RPShapeProcessDlg1_MJ, CDialog)
	//{{AFX_MSG_MAP(RPShapeProcessDlg1_MJ)
	ON_CBN_CLOSEUP(IDC_COMBO1, OnCloseupCombo1)
	ON_CBN_CLOSEUP(IDC_COMBO3, OnCloseupCombo3)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_CBN_CLOSEUP(IDC_COMBO2, OnCloseupCombo2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RPShapeProcessDlg1_MJ message handlers

void RPShapeProcessDlg1_MJ::OnCloseupCombo1() 
{
int cur = m_combo.GetCurSel();

switch(cur) // определяем активные окна исходя из закона
{
case 0: // константа
	m_proc_dlg1_exp_control_MJ.EnableWindow(TRUE);
	m_proc_dlg1_disp_control_MJ.EnableWindow(FALSE);
	m_proc_dlg1_max_control_MJ.EnableWindow(FALSE);
	m_proc_dlg1_min_control_MJ.EnableWindow(FALSE);
					break;	
case 1: // нормальный
	m_proc_dlg1_exp_control_MJ.EnableWindow(TRUE);
	m_proc_dlg1_disp_control_MJ.EnableWindow(TRUE);
	m_proc_dlg1_max_control_MJ.EnableWindow(FALSE);
	m_proc_dlg1_min_control_MJ.EnableWindow(FALSE);
					break;
case 2: // равномерный закон
	m_proc_dlg1_exp_control_MJ.EnableWindow(FALSE);
	m_proc_dlg1_disp_control_MJ.EnableWindow(FALSE);
	m_proc_dlg1_max_control_MJ.EnableWindow(TRUE);
	m_proc_dlg1_min_control_MJ.EnableWindow(TRUE);
					break;
case 3: // экспоненциальный
	m_proc_dlg1_exp_control_MJ.EnableWindow(TRUE);
	m_proc_dlg1_disp_control_MJ.EnableWindow(FALSE);
	m_proc_dlg1_max_control_MJ.EnableWindow(FALSE);
	m_proc_dlg1_min_control_MJ.EnableWindow(FALSE);
					break;
}	
}

void RPShapeProcessDlg1_MJ::OnCloseupCombo3() 
{
int cur = m_type.GetCurSel();

switch(cur) // определяем активные окна исходя из закона
{
case 0: // fifo
	m_parameter.EnableWindow(FALSE);
					break;	
case 1: // lifo
	m_parameter.EnableWindow(FALSE);
				break;
case 2: // минимизация
	m_parameter.EnableWindow(TRUE);
					break;
case 3: // максимизация
	m_parameter.EnableWindow(TRUE);
					break;
}	
}

void RPShapeProcessDlg1_MJ::OnButton1() 
{
	RPShapeProcessDlg2_MJ dlg;
	dlg.DoModal();
	
}

void RPShapeProcessDlg1_MJ::OnOK() 
{
//вывод и ввод имени блока
UpdateData(TRUE);
pParentMJ->setName(std::string(m_name));
pParentMJ->update_modifyMJ();	
	CDialog::OnOK();	
	CDialog::OnOK();
}

void RPShapeProcessDlg1_MJ::OnCloseupCombo2() 
{
	int cur = m_action.GetCurSel();

switch(cur) // определяем активные окна исходя из закона
{
case 0: // задержать
		m_type.EnableWindow(FALSE); // невидимая очередь
		m_parameter.EnableWindow(FALSE);// невидимая очередь
		m_AddRes.EnableWindow(FALSE); 
		m_DelRes.EnableWindow(FALSE); 
		m_ResList.EnableWindow(FALSE); 
		
					break;	
case 1: //занять задержать освободить
	    m_type.EnableWindow(TRUE);// невидимая очередь
             if(m_type.GetCurSel() == 0 || m_type.GetCurSel() == 1)
                   m_parameter.EnableWindow(FALSE);
			 else
                   m_parameter.EnableWindow(TRUE);

		m_AddRes.EnableWindow(TRUE); 
		m_DelRes.EnableWindow(TRUE); 
		m_ResList.EnableWindow(TRUE); 
	
				break;

case 2: // занять задержать
	m_type.EnableWindow(TRUE);// невидимая очередь
             if(m_type.GetCurSel() == 0 || m_type.GetCurSel() == 1)
                   m_parameter.EnableWindow(FALSE);
			 else
                   m_parameter.EnableWindow(TRUE);

		m_AddRes.EnableWindow(TRUE); 
		m_DelRes.EnableWindow(TRUE); 
		m_ResList.EnableWindow(TRUE); 
					break;



case 3: // задержать освободить
	m_type.EnableWindow(TRUE);// невидимая очередь
             if(m_type.GetCurSel() == 0 || m_type.GetCurSel() == 1)
                   m_parameter.EnableWindow(FALSE);
			 else
                   m_parameter.EnableWindow(TRUE);

		m_AddRes.EnableWindow(FALSE); 
		m_DelRes.EnableWindow(FALSE); 
		m_ResList.EnableWindow(FALSE); 
					break;
}	
}
