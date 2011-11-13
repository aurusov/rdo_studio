// rdoprocess_shape_create_dlg1_MJ.cpp : implementation file
//

#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create_dlg1.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create_dlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RPShapeCreateDlg1_MJ dialog


RPShapeCreateDlg1_MJ::RPShapeCreateDlg1_MJ(CWnd* pParent /*=NULL*/,RPShapeCreateMJ* ppParent)
	: CDialog(RPShapeCreateDlg1_MJ::IDD, pParent)
{
	//{{AFX_DATA_INIT(RPShapeCreateDlg1_MJ)
	m_name = _T("");
	m_dlgfirst = 0.0;
	m_dlgamount = 0;
	m_dlgexp = 0.0;
	m_dlgdisp = 0.0;
	m_dlgmax = 0.0;
	m_dlgbase_gen = 0;
	//}}AFX_DATA_INIT
    pParentMJ = ppParent;
}

void RPShapeCreateDlg1_MJ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RPShapeCreateDlg1_MJ)
	DDX_Control(pDX, IDC_EDIT4, m_create_dlg1_exp_control_MJ);
	DDX_Control(pDX, IDC_EDIT5, m_create_dlg1_disp_control_MJ);
	DDX_Control(pDX, IDC_EDIT6, m_create_dlg1_max_control_MJ);
	DDX_Control(pDX, IDC_STATIC7, m_create_dlgl_exp_text_MJ);
	DDX_Control(pDX, IDC_STATIC8, m_create_dlgl_disp_text_MJ);
	DDX_Control(pDX, IDC_STATIC9, m_create_dlgl_max_text_MJ);
	DDX_Control(pDX, IDC_COMBO1, m_create_dlg1_combo1_MJ);
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_Text(pDX, IDC_EDIT2, m_dlgfirst);
	DDX_Text(pDX, IDC_EDIT3, m_dlgamount);
	DDX_Text(pDX, IDC_EDIT4, m_dlgexp);
	DDX_Text(pDX, IDC_EDIT5, m_dlgdisp);
	DDX_Text(pDX, IDC_EDIT6, m_dlgmax);
	DDX_Text(pDX, IDC_EDIT8, m_dlgbase_gen);
	DDV_MinMaxInt(pDX, m_dlgbase_gen, -2147483647, 2147483647);
	//}}AFX_DATA_MAP
}

BOOL RPShapeCreateDlg1_MJ::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	
	 //отображение имени блока в окне
	CString str( pParentMJ->getName().c_str() );
    m_name = str;
	UpdateData(FALSE);
	
// инициализация из вызвавшего объекта


	m_create_dlg1_combo1_MJ.SetCurSel(pParentMJ->gtype); // закон прибытия
   

	
	m_dlgfirst=pParentMJ->gfirst; // время первого
	m_dlgamount= static_cast<int>(pParentMJ->gamount); // кол-во создаваемых
	m_dlgbase_gen=pParentMJ->base_gen;
	//атрибуты законов
	m_dlgexp=pParentMJ->gexp;
	m_dlgdisp=pParentMJ->gdisp;
	m_dlgmax=pParentMJ->gmax;
	//второе окно
	pParentMJ->inf; // бесконечноть
	pParentMJ->gID; // ID группы
	pParentMJ->gpar1;
	pParentMJ->gpar2;
	pParentMJ->gpar3;

UpdateData(FALSE);

 RPShapeCreateDlg1_MJ::OnCbnSelchange1();
	return TRUE;
}



BEGIN_MESSAGE_MAP(RPShapeCreateDlg1_MJ, CDialog)
	//{{AFX_MSG_MAP(RPShapeCreateDlg1_MJ)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchange1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RPShapeCreateDlg1_MJ message handlers

void RPShapeCreateDlg1_MJ::OnCbnSelchange1() 
{

int cur = m_create_dlg1_combo1_MJ.GetCurSel();
UpdateData(TRUE);
	switch(cur) // определяем активные окна исходя из закона
	{
case 0: // константа
	m_create_dlgl_exp_text_MJ.SetWindowText(_T("Константа"));
	m_create_dlgl_disp_text_MJ.ShowWindow(SW_HIDE);
	m_create_dlg1_disp_control_MJ.ShowWindow(SW_HIDE);
	m_create_dlgl_max_text_MJ.ShowWindow(SW_HIDE);
	m_create_dlg1_max_control_MJ.ShowWindow(SW_HIDE);
	break;
case 1: // нормальный
	m_create_dlgl_exp_text_MJ.SetWindowText(_T("Мат. ожидание"));
	m_create_dlgl_disp_text_MJ.ShowWindow(SW_SHOW);
	m_create_dlgl_disp_text_MJ.SetWindowText(_T("Дисперсия"));
	m_create_dlg1_disp_control_MJ.ShowWindow(SW_SHOW);
	m_create_dlgl_max_text_MJ.ShowWindow(SW_HIDE);
	m_create_dlg1_max_control_MJ.ShowWindow(SW_HIDE);
	break;
case 2: // равномерный закон
	m_create_dlgl_exp_text_MJ.SetWindowText(_T("Левая граница"));
	m_create_dlgl_disp_text_MJ.ShowWindow(SW_SHOW);
	m_create_dlgl_disp_text_MJ.SetWindowText(_T("Правая граница"));
	m_create_dlg1_disp_control_MJ.ShowWindow(SW_SHOW);
	m_create_dlgl_max_text_MJ.ShowWindow(SW_HIDE);
	m_create_dlg1_max_control_MJ.ShowWindow(SW_HIDE);
	break;
case 3: // труегольный
	m_create_dlgl_exp_text_MJ.SetWindowText(_T("Левая граница"));
	m_create_dlgl_disp_text_MJ.ShowWindow(SW_SHOW);
	m_create_dlgl_disp_text_MJ.SetWindowText(_T("Высота"));
	m_create_dlg1_disp_control_MJ.ShowWindow(SW_SHOW);
	m_create_dlgl_max_text_MJ.ShowWindow(SW_SHOW);
	m_create_dlgl_max_text_MJ.SetWindowText(_T("Правая граница"));
	m_create_dlg1_max_control_MJ.ShowWindow(SW_SHOW);
	break;
case 4: //экспоненциальный
	m_create_dlgl_exp_text_MJ.SetWindowText(_T("Мат. ожидание"));
	m_create_dlgl_disp_text_MJ.ShowWindow(SW_HIDE);
	m_create_dlg1_disp_control_MJ.ShowWindow(SW_HIDE);
	m_create_dlgl_max_text_MJ.ShowWindow(SW_HIDE);
	m_create_dlg1_max_control_MJ.ShowWindow(SW_HIDE);
	break;
	}
	UpdateData(FALSE);	
}

void RPShapeCreateDlg1_MJ::OnButton1() 
{
	RPShapeCreateDlg2_MJ dlg2( this, pParentMJ );
	dlg2.DoModal();
}

void RPShapeCreateDlg1_MJ::OnOK() 
{
	UpdateData(TRUE);
	pParentMJ->setName( std::string(m_name) );
//	pParentMJ->update_modify();	
	CDialog::OnOK();

	// запись параметров в объект
	// инициализация из вызвавшего объекта
UpdateData(TRUE);
	pParentMJ->gname=m_name;
	pParentMJ->gfirst=m_dlgfirst; // время первого
	pParentMJ->gamount=m_dlgamount; // кол-во создаваемых
	pParentMJ->gtype=m_create_dlg1_combo1_MJ.GetCurSel(); // закон прибытия
	pParentMJ->base_gen=m_dlgbase_gen;
	//атрибуты законов
	pParentMJ->gexp=m_dlgexp;
	pParentMJ->gdisp=m_dlgdisp;
	pParentMJ->gmax=m_dlgmax;

}
