#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create_dlg1.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create_dlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RPProcessShapeCreateDlg1::RPProcessShapeCreateDlg1(CWnd* pParent /*=NULL*/,RPProcessShapeCreate* ppParent)
	: CDialog(RPProcessShapeCreateDlg1::IDD, pParent)
{
	m_name = _T("");
	m_dlgfirst = 0.0;
	m_dlgamount = 0;
	m_dlgexp = 0.0;
	m_dlgdisp = 0.0;
	m_dlgmax = 0.0;
	m_dlgbase_gen = 0;
    pParentMJ = ppParent;
}

void RPProcessShapeCreateDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT4, m_expCtrl);
	DDX_Control(pDX, IDC_EDIT5, m_dispCtrl);
	DDX_Control(pDX, IDC_EDIT6, m_maxCtrl);
	DDX_Control(pDX, IDC_STATIC7, m_expText);
	DDX_Control(pDX, IDC_STATIC8, m_dispText);
	DDX_Control(pDX, IDC_STATIC9, m_maxText);
	DDX_Control(pDX, IDC_COMBO1, m_comboCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_Text(pDX, IDC_EDIT2, m_dlgfirst);
	DDX_Text(pDX, IDC_EDIT3, m_dlgamount);
	DDX_Text(pDX, IDC_EDIT4, m_dlgexp);
	DDX_Text(pDX, IDC_EDIT5, m_dlgdisp);
	DDX_Text(pDX, IDC_EDIT6, m_dlgmax);
	DDX_Text(pDX, IDC_EDIT8, m_dlgbase_gen);
	DDV_MinMaxInt(pDX, m_dlgbase_gen, -2147483647, 2147483647);
}

BOOL RPProcessShapeCreateDlg1::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	
	 //отображение имени блока в окне
	CString str( pParentMJ->getName().c_str() );
    m_name = str;
	UpdateData(FALSE);
	
// инициализация из вызвавшего объекта


	m_comboCtrl.SetCurSel(pParentMJ->gtype); // закон прибытия
   

	
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

 RPProcessShapeCreateDlg1::OnCbnSelchange1();
	return TRUE;
}



BEGIN_MESSAGE_MAP(RPProcessShapeCreateDlg1, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchange1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
END_MESSAGE_MAP()

void RPProcessShapeCreateDlg1::OnCbnSelchange1() 
{

int cur = m_comboCtrl.GetCurSel();
UpdateData(TRUE);
	switch(cur) // определяем активные окна исходя из закона
	{
case 0: // константа
	m_expText.SetWindowText(_T("Константа"));
	m_dispText.ShowWindow(SW_HIDE);
	m_dispCtrl.ShowWindow(SW_HIDE);
	m_maxText.ShowWindow(SW_HIDE);
	m_maxCtrl.ShowWindow(SW_HIDE);
	break;
case 1: // нормальный
	m_expText.SetWindowText(_T("Мат. ожидание"));
	m_dispText.ShowWindow(SW_SHOW);
	m_dispText.SetWindowText(_T("Дисперсия"));
	m_dispCtrl.ShowWindow(SW_SHOW);
	m_maxText.ShowWindow(SW_HIDE);
	m_maxCtrl.ShowWindow(SW_HIDE);
	break;
case 2: // равномерный закон
	m_expText.SetWindowText(_T("Левая граница"));
	m_dispText.ShowWindow(SW_SHOW);
	m_dispText.SetWindowText(_T("Правая граница"));
	m_dispCtrl.ShowWindow(SW_SHOW);
	m_maxText.ShowWindow(SW_HIDE);
	m_maxCtrl.ShowWindow(SW_HIDE);
	break;
case 3: // труегольный
	m_expText.SetWindowText(_T("Левая граница"));
	m_dispText.ShowWindow(SW_SHOW);
	m_dispText.SetWindowText(_T("Высота"));
	m_dispCtrl.ShowWindow(SW_SHOW);
	m_maxText.ShowWindow(SW_SHOW);
	m_maxText.SetWindowText(_T("Правая граница"));
	m_maxCtrl.ShowWindow(SW_SHOW);
	break;
case 4: //экспоненциальный
	m_expText.SetWindowText(_T("Мат. ожидание"));
	m_dispText.ShowWindow(SW_HIDE);
	m_dispCtrl.ShowWindow(SW_HIDE);
	m_maxText.ShowWindow(SW_HIDE);
	m_maxCtrl.ShowWindow(SW_HIDE);
	break;
	}
	UpdateData(FALSE);	
}

void RPProcessShapeCreateDlg1::OnButton1() 
{
	RPProcessShapeCreateDlg2 dlg2( this, pParentMJ );
	dlg2.DoModal();
}

void RPProcessShapeCreateDlg1::OnOK() 
{
	UpdateData(TRUE);
	pParentMJ->setName( tstring(m_name) );
//	pParentMJ->update_modify();	
	CDialog::OnOK();

	// запись параметров в объект
	// инициализация из вызвавшего объекта
UpdateData(TRUE);
	pParentMJ->gname=m_name;
	pParentMJ->gfirst=m_dlgfirst; // время первого
	pParentMJ->gamount=m_dlgamount; // кол-во создаваемых
	pParentMJ->gtype=m_comboCtrl.GetCurSel(); // закон прибытия
	pParentMJ->base_gen=m_dlgbase_gen;
	//атрибуты законов
	pParentMJ->gexp=m_dlgexp;
	pParentMJ->gdisp=m_dlgdisp;
	pParentMJ->gmax=m_dlgmax;

}
