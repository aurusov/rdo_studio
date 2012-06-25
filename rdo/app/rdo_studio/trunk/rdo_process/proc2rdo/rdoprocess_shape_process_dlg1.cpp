#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_process_dlg1.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_process_dlg2.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include<list>

BEGIN_MESSAGE_MAP(RPListBox, CListBox)
END_MESSAGE_MAP()

RPProcessShapeProcessDlg1::RPProcessShapeProcessDlg1(CWnd* pParent /*=NULL*/,RPProcessShapeProcess* ppParent)
	: CDialog(RPProcessShapeProcessDlg1::IDD, pParent),
	brush1( RGB(0xFF, 0x00, 0x00) ),
	brush2( RGB(0x00, 0xFF, 0x00) )
{
	m_name = _T("");
	m_list_text = _T("");
	m_gprior = 0;
	m_gexp = 0.0;
	m_gdisp = 0.0;
	m_gmin = 0.0;
	m_gmax = 0.0;
	m_gbase_gen = 0;

	m_pParent = ppParent;
}

void RPProcessShapeProcessDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_gtype);
	DDX_Control(pDX, IDC_COMBO3, m_gqueque);
	DDX_Control(pDX, IDC_LIST1, m_ResList);
	DDX_Control(pDX, IDC_BUTTON2, m_DelRes);
	DDX_Control(pDX, IDC_BUTTON1, m_AddRes);
	DDX_Control(pDX, IDC_EDIT4, m_expCtrl);
	DDX_Control(pDX, IDC_EDIT5, m_dispCtrl);
	DDX_Control(pDX, IDC_EDIT7, m_maxCtrl);
	DDX_Control(pDX, IDC_STATIC4, m_expText);
	DDX_Control(pDX, IDC_STATIC5, m_dispText);
	DDX_Control(pDX, IDC_STATIC7, m_maxText);
	DDX_Control(pDX, IDC_COMBO4, m_parameter);
	DDX_Control(pDX, IDC_COMBO2, m_action);
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_LBString(pDX, IDC_LIST1, m_list_text);
	DDX_Text(pDX, IDC_EDIT2, m_gprior);
	DDX_Text(pDX, IDC_EDIT4, m_gexp);
	DDX_Text(pDX, IDC_EDIT5, m_gdisp);
	DDX_Text(pDX, IDC_EDIT7, m_gmax);
	DDX_Text(pDX, IDC_EDIT8, m_gbase_gen);
	DDV_MinMaxInt(pDX, m_gbase_gen, -2147483647, 2147483647);
}

BOOL RPProcessShapeProcessDlg1::OnInitDialog()
{
	CDialog::OnInitDialog();
	//отображение имени блока в окне
	CString str( m_pParent->getName().c_str() );
	m_name = str;
	UpdateData(FALSE);
	
	// инициализаци€ из вызвавшего объекта
	m_gtype.SetCurSel(m_pParent->gtype); // закон прибыти€
	m_gbase_gen=m_pParent->base_gen;
	//атрибуты законов
	m_gexp  = m_pParent->gexp;
	m_gdisp = m_pParent->gdisp;
	m_gmax  = m_pParent->gmax;
	m_gmin  = m_pParent->gmin;

	m_action.SetCurSel(m_pParent->action);
	m_gprior = m_pParent->prior;
	m_gqueque.SetCurSel(m_pParent->queue);

	//бегает по списку ресурсовm_resourceList
	std::list<CString>::iterator it = m_pParent->m_resourceList.begin();
	while( it != m_pParent->m_resourceList.end() )
	{
		m_ResList.AddString(*it);
		++it;
	}

	UpdateData(FALSE);
	OnCbnSelchange2();
	OnCbnSelchange1();
	return TRUE;
}

BEGIN_MESSAGE_MAP(RPProcessShapeProcessDlg1, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchange1)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnCbnSelchange3)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnCbnSelchange2)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
END_MESSAGE_MAP()

void RPProcessShapeProcessDlg1::OnCbnSelchange1() 
{
	int cur = m_gtype.GetCurSel();

	switch(cur) // определ€ем активные окна исход€ из закона
	{
	case 0: // константа
		m_expCtrl.EnableWindow(TRUE);
		m_expText.SetWindowText(_T(" онстанта"));
		m_dispText.ShowWindow(SW_HIDE);
		m_dispCtrl.ShowWindow(SW_HIDE);
		m_maxText.ShowWindow(SW_HIDE);
		m_maxCtrl.ShowWindow(SW_HIDE);
		break;
	case 1: // нормальный
		m_expText.SetWindowText(_T("ћат. ожидание"));
		m_dispText.ShowWindow(SW_SHOW);
		m_dispText.SetWindowText(_T("ƒисперси€"));
		m_dispCtrl.ShowWindow(SW_SHOW);
		m_maxText.ShowWindow(SW_HIDE);
		m_maxCtrl.ShowWindow(SW_HIDE);
		break;
	case 2: // равномерный закон
		m_expText.SetWindowText(_T("Ћева€ граница"));
		m_dispText.ShowWindow(SW_SHOW);
		m_dispText.SetWindowText(_T("ѕрава€ граница"));
		m_dispCtrl.ShowWindow(SW_SHOW);
		m_maxText.ShowWindow(SW_HIDE);
		m_maxCtrl.ShowWindow(SW_HIDE);
		break;
	case 3: // труегольный
		m_expText.SetWindowText(_T("Ћева€ граница"));
		m_dispText.ShowWindow(SW_SHOW);
		m_dispText.SetWindowText(_T("¬ысота"));
		m_dispCtrl.ShowWindow(SW_SHOW);
		m_maxText.ShowWindow(SW_SHOW);
		m_maxText.SetWindowText(_T("ѕрава€ граница"));
		m_maxCtrl.ShowWindow(SW_SHOW);
		break;
	case 4: //экспоненциальный
		m_expText.SetWindowText(_T("ћат. ожидание"));
		m_dispText.ShowWindow(SW_HIDE);
		m_dispCtrl.ShowWindow(SW_HIDE);
		m_maxText.ShowWindow(SW_HIDE);
		m_maxCtrl.ShowWindow(SW_HIDE);
		break;
	}
}

void RPProcessShapeProcessDlg1::OnCbnSelchange3() 
{
	int cur = m_gqueque.GetCurSel();

	switch(cur) // определ€ем активные окна исход€ из закона
	{
	case 0: // fifo
		m_parameter.EnableWindow(FALSE);
		break;
	case 1: // lifo
		m_parameter.EnableWindow(FALSE);
		break;
	case 2: // минимизаци€
		m_parameter.EnableWindow(TRUE);
		break;
	case 3: // максимизаци€
		m_parameter.EnableWindow(TRUE);
		break;
	}
}

void RPProcessShapeProcessDlg1::OnButton1()
{
	RPProcessShapeProcessDlg2 dlg( this, m_pParent, this );
	dlg.DoModal();
}

void RPProcessShapeProcessDlg1::OnOK()
{
	// вывод и ввод имени блока
	UpdateData(TRUE);
	m_pParent->setName(tstring(m_name));

	// инициализаци€ из вызвавшего объекта
	m_pParent->gtype    = m_gtype.GetCurSel(); // закон прибыти€
	m_pParent->base_gen = m_gbase_gen;
	//атрибуты законов
	m_pParent->gexp   = m_gexp;
	m_pParent->gdisp  = m_gdisp;
	m_pParent->gmax   = m_gmax;
	m_pParent->gmin   = m_gmin;
	m_pParent->gname  = m_name;

	m_pParent->action = m_action.GetCurSel();
	m_pParent->prior  = m_gprior ;
	m_pParent->queue  = m_gqueque.GetCurSel();

	CDialog::OnOK();
}

void RPProcessShapeProcessDlg1::OnCbnSelchange2() 
{
	int cur = m_action.GetCurSel();

	switch(cur) // определ€ем активные окна исход€ из закона
	{
	case 0: // задержать
		m_gqueque.EnableWindow(FALSE); // невидима€ очередь
		m_parameter.EnableWindow(FALSE);// невидима€ очередь
		m_AddRes.EnableWindow(FALSE);
		m_DelRes.EnableWindow(FALSE);
		m_ResList.EnableWindow(FALSE);
		break;

	case 1: //зан€ть задержать освободить
		m_gqueque.EnableWindow(TRUE);// невидима€ очередь
		if(m_gqueque.GetCurSel() == 0 || m_gqueque.GetCurSel() == 1)
			m_parameter.EnableWindow(FALSE);
		else
			m_parameter.EnableWindow(TRUE);

		m_AddRes.EnableWindow(TRUE);
		m_DelRes.EnableWindow(TRUE);
		m_ResList.EnableWindow(TRUE);
		break;

	case 2: // зан€ть задержать
		m_gqueque.EnableWindow(TRUE);// невидима€ очередь
		if(m_gqueque.GetCurSel() == 0 || m_gqueque.GetCurSel() == 1)
			m_parameter.EnableWindow(FALSE);
		else
			m_parameter.EnableWindow(TRUE);

		m_AddRes.EnableWindow(TRUE);
		m_DelRes.EnableWindow(TRUE);
		m_ResList.EnableWindow(TRUE);
		break;

	case 3: // задержать освободить
		m_gqueque.EnableWindow(TRUE);// невидима€ очередь
		if(m_gqueque.GetCurSel() == 0 || m_gqueque.GetCurSel() == 1)
			m_parameter.EnableWindow(FALSE);
		else
			m_parameter.EnableWindow(TRUE);

		m_AddRes.EnableWindow(TRUE);
		m_DelRes.EnableWindow(TRUE);
		m_ResList.EnableWindow(TRUE);
		break;
	}
}

HBRUSH RPProcessShapeProcessDlg1::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	return CDialog::OnCtlColor( pDC, pWnd, nCtlColor );
}

void RPProcessShapeProcessDlg1::OnButton2()
{}
