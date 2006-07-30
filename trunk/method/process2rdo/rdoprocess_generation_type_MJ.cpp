// rdoprocess_generation_type_MJ.cpp : implementation file
//

#include "stdafx.h"
//#include "rdo_process.h"
#include "rdoprocess_generation_type_MJ.h"
#include "../../rdoprocess_app.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RP_GENERATION_TYPE_MJ dialog


RP_GENERATION_TYPE_MJ::RP_GENERATION_TYPE_MJ(CWnd* pParent /*=NULL*/)
	: CDialog(RP_GENERATION_TYPE_MJ::IDD, pParent)
{
	//{{AFX_DATA_INIT(RP_GENERATION_TYPE_MJ)
	m_time_value = 0.0;
	//}}AFX_DATA_INIT
}


void RP_GENERATION_TYPE_MJ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RP_GENERATION_TYPE_MJ)
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Control(pDX, IDC_EDIT1, m_time);
	DDX_Text(pDX, IDC_EDIT1, m_time_value);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RP_GENERATION_TYPE_MJ, CDialog)
	//{{AFX_MSG_MAP(RP_GENERATION_TYPE_MJ)
	ON_CBN_CLOSEUP(IDC_COMBO1, OnCloseupCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RP_GENERATION_TYPE_MJ message handlers

void RP_GENERATION_TYPE_MJ::OnCloseupCombo1() 
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





BOOL RP_GENERATION_TYPE_MJ::OnInitDialog()
{
	CDialog::OnInitDialog();

	
// инициализация из вызвавшего объекта
	m_combo.SetCurSel(0); // закон прибытия
OnCloseupCombo1();

	m_time_value = rpapp.project()->generate_time_MJ;
	UpdateData(FALSE);
	return TRUE;
}

void RP_GENERATION_TYPE_MJ::OnOK() 
{
	UpdateData(TRUE);
	rpapp.project()->generate_time_MJ = m_time_value;

	CDialog::OnOK();
}
