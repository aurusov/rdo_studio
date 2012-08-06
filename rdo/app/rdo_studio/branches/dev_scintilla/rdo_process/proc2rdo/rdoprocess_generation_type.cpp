#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_generation_type.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_method_proc2rdo.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RPProcessGenerationType::RPProcessGenerationType(CWnd* pParent /*=NULL*/)
	: CDialog(RPProcessGenerationType::IDD, pParent)
{
	m_time_value = 0.0;
}

void RPProcessGenerationType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Control(pDX, IDC_EDIT1, m_time);
	DDX_Text(pDX, IDC_EDIT1, m_time_value);
}

BEGIN_MESSAGE_MAP(RPProcessGenerationType, CDialog)
	ON_CBN_CLOSEUP(IDC_COMBO1, OnCloseupCombo1)
END_MESSAGE_MAP()

void RPProcessGenerationType::OnCloseupCombo1() 
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

BOOL RPProcessGenerationType::OnInitDialog()
{
	CDialog::OnInitDialog();

	
// инициализация из вызвавшего объекта
	m_combo.SetCurSel(0); // закон прибытия
OnCloseupCombo1();

	m_time_value = proc2rdo->m_generateTime;
	UpdateData(FALSE);
	return TRUE;
}

void RPProcessGenerationType::OnOK() 
{
	UpdateData(TRUE);
	proc2rdo->m_generateTime = m_time_value;

	CDialog::OnOK();
}
