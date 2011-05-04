// rdoprocess_shape_process_dlg1_MJ.cpp : implementation file
//

#include "rdo_studio/rdo_process/proc2rdo/stdafx.h"
#include "rdoprocess_shape_process_dlg1.h"
#include "rdoprocess_shape_process_dlg2.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include<list>

BEGIN_MESSAGE_MAP(RPListBox, CListBox)
	//{{AFX_MSG_MAP(RPListBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RPShapeProcessDlg1_MJ dialog


RPShapeProcessDlg1_MJ::RPShapeProcessDlg1_MJ(CWnd* pParent /*=NULL*/,RPShapeProcessMJ* ppParent)
	: CDialog(RPShapeProcessDlg1_MJ::IDD, pParent),
	brush1( RGB(0xFF, 0x00, 0x00) ),
	brush2( RGB(0x00, 0xFF, 0x00) )
{
	//{{AFX_DATA_INIT(RPShapeProcessDlg1_MJ)
	m_name = _T("");
	m_list_text = _T("");
	m_gprior = 0;
	m_gexp = 0.0;
	m_gdisp = 0.0;
	m_gmin = 0.0;
	m_gmax = 0.0;
	m_gbase_gen = 0;
	//}}AFX_DATA_INIT

    pParentMJ = ppParent;
}


void RPShapeProcessDlg1_MJ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RPShapeProcessDlg1_MJ)
	DDX_Control(pDX, IDC_COMBO1, m_gtype);
	DDX_Control(pDX, IDC_COMBO3, m_gqueque);
	DDX_Control(pDX, IDC_LIST1, m_ResList);
	DDX_Control(pDX, IDC_BUTTON2, m_DelRes);
	DDX_Control(pDX, IDC_BUTTON1, m_AddRes);
	DDX_Control(pDX, IDC_EDIT7, m_proc_dlg1_max_control_MJ);
	DDX_Control(pDX, IDC_EDIT6, m_proc_dlg1_min_control_MJ);
	DDX_Control(pDX, IDC_EDIT5, m_proc_dlg1_disp_control_MJ);
	DDX_Control(pDX, IDC_EDIT4, m_proc_dlg1_exp_control_MJ);
	DDX_Control(pDX, IDC_COMBO4, m_parameter);
	DDX_Control(pDX, IDC_COMBO2, m_action);
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_LBString(pDX, IDC_LIST1, m_list_text);
	DDX_Text(pDX, IDC_EDIT2, m_gprior);
	DDX_Text(pDX, IDC_EDIT4, m_gexp);
	DDX_Text(pDX, IDC_EDIT5, m_gdisp);
	DDX_Text(pDX, IDC_EDIT6, m_gmin);
	DDX_Text(pDX, IDC_EDIT7, m_gmax);
	DDX_Text(pDX, IDC_EDIT8, m_gbase_gen);
	DDV_MinMaxInt(pDX, m_gbase_gen, -2147483647, 2147483647);
	//}}AFX_DATA_MAP
}


BOOL RPShapeProcessDlg1_MJ::OnInitDialog()
{
	CDialog::OnInitDialog();
		 //отображение имени блока в окне
	CString str( pParentMJ->getName().c_str() );
    m_name = str;
	UpdateData(FALSE);
	
// инициализация из вызвавшего объекта
	m_gtype.SetCurSel(pParentMJ->gtype); // закон прибытия
	m_gbase_gen=pParentMJ->base_gen;
	//атрибуты законов
	m_gexp=pParentMJ->gexp;
	m_gdisp=pParentMJ->gdisp;
	m_gmax=pParentMJ->gmax;
	m_gmin=pParentMJ->gmin;


m_action.SetCurSel(pParentMJ->action);
m_gprior = pParentMJ->prior;
m_gqueque.SetCurSel(pParentMJ->queue);
//m_parameter.SetCurSel(pParentMJ->parameter);


//бегает по списку ресурсовlist_resource_procMJ
std::list<CString>::iterator it = pParentMJ->list_resource_procMJ.begin();
	while( it != pParentMJ->list_resource_procMJ.end() ) 
	{
	m_ResList.AddString(*it);
	it++;
	}	




UpdateData(FALSE);
OnCloseupCombo2(); 
OnCloseupCombo1();
	return TRUE;
}











BEGIN_MESSAGE_MAP(RPShapeProcessDlg1_MJ, CDialog)
	//{{AFX_MSG_MAP(RPShapeProcessDlg1_MJ)
	ON_CBN_CLOSEUP(IDC_COMBO1, OnCloseupCombo1)
	ON_CBN_CLOSEUP(IDC_COMBO3, OnCloseupCombo3)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_CBN_CLOSEUP(IDC_COMBO2, OnCloseupCombo2)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RPShapeProcessDlg1_MJ message handlers

void RPShapeProcessDlg1_MJ::OnCloseupCombo1() 
{
int cur = m_gtype.GetCurSel();

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
int cur = m_gqueque.GetCurSel();

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
	RPShapeProcessDlg2_MJ dlg( this, pParentMJ, this );
	dlg.DoModal();
}

void RPShapeProcessDlg1_MJ::OnOK() 
{
	// вывод и ввод имени блока
	UpdateData(TRUE);
	pParentMJ->setName(std::string(m_name));

	
// инициализация из вызвавшего объекта
	pParentMJ->gtype= m_gtype.GetCurSel(); // закон прибытия
	pParentMJ->base_gen=m_gbase_gen;
	//атрибуты законов
	pParentMJ->gexp=m_gexp;
	pParentMJ->gdisp=m_gdisp;
	pParentMJ->gmax=m_gmax;
	pParentMJ->gmin=m_gmin;


pParentMJ->action=m_action.GetCurSel();
pParentMJ->prior = m_gprior ;
pParentMJ->queue=m_gqueque.GetCurSel();




	CDialog::OnOK();	
}

void RPShapeProcessDlg1_MJ::OnCloseupCombo2() 
{
	int cur = m_action.GetCurSel();

	switch(cur) // определяем активные окна исходя из закона
	{
	case 0: // задержать
			m_gqueque.EnableWindow(FALSE); // невидимая очередь
			m_parameter.EnableWindow(FALSE);// невидимая очередь
			m_AddRes.EnableWindow(FALSE); 
			m_DelRes.EnableWindow(FALSE); 
			m_ResList.EnableWindow(FALSE); 
			
						break;	
	case 1: //занять задержать освободить
			m_gqueque.EnableWindow(TRUE);// невидимая очередь
				 if(m_gqueque.GetCurSel() == 0 || m_gqueque.GetCurSel() == 1)
					   m_parameter.EnableWindow(FALSE);
				 else
					   m_parameter.EnableWindow(TRUE);

			m_AddRes.EnableWindow(TRUE); 
			m_DelRes.EnableWindow(TRUE); 
			m_ResList.EnableWindow(TRUE); 
		
					break;

	case 2: // занять задержать
		m_gqueque.EnableWindow(TRUE);// невидимая очередь
				if(m_gqueque.GetCurSel() == 0 || m_gqueque.GetCurSel() == 1)
					m_parameter.EnableWindow(FALSE);
				else
					m_parameter.EnableWindow(TRUE);

			m_AddRes.EnableWindow(TRUE); 
			m_DelRes.EnableWindow(TRUE); 
			m_ResList.EnableWindow(TRUE); 
						break;



	case 3: // задержать освободить
		m_gqueque.EnableWindow(TRUE);// невидимая очередь
				 if(m_gqueque.GetCurSel() == 0 || m_gqueque.GetCurSel() == 1)
					   m_parameter.EnableWindow(FALSE);
				 else
					   m_parameter.EnableWindow(TRUE);

			m_AddRes.EnableWindow(FALSE); 
			m_DelRes.EnableWindow(FALSE); 
			m_ResList.EnableWindow(FALSE); 
						break;
	}	
}

HBRUSH RPShapeProcessDlg1_MJ::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	return CDialog::OnCtlColor( pDC, pWnd, nCtlColor );
	if ( nCtlColor == CTLCOLOR_LISTBOX && pWnd == &m_ResList ) {
		return m_ResList.IsWindowEnabled() ? brush1 : brush2;
	} else {
		return CDialog::OnCtlColor( pDC, pWnd, nCtlColor );
	}
}

void RPShapeProcessDlg1_MJ::OnButton2() 
{


}
