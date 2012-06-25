#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_process_dlg2.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_method_proc2rdo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RPProcessShapeProcessDlg2::RPProcessShapeProcessDlg2(CWnd* pParent,RPProcessShapeProcess* ppParent,RPProcessShapeProcessDlg1* pParentDlg)
	: CDialog(RPProcessShapeProcessDlg2::IDD, pParent)
{
	m_combo_res_value = _T("");
	pParentMJ = ppParent;
	pParentDlgMJ = pParentDlg;
}

BOOL RPProcessShapeProcessDlg2::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_resourceList.clear();
	rpMethod::project->getAllChildByClass( m_resourceList, "RPProcessShapeResource", true );
	list_name_resource();

	return true;
}

void RPProcessShapeProcessDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo_res_control);
	DDX_CBString(pDX, IDC_COMBO1, m_combo_res_value);
}

BEGIN_MESSAGE_MAP(RPProcessShapeProcessDlg2, CDialog)
	ON_CBN_CLOSEUP(IDC_COMBO1, OnCloseupCombo1)
END_MESSAGE_MAP()

void RPProcessShapeProcessDlg2::OnCloseupCombo1() 
{
//pParentMJ->list_name_for_resource_MJ();	
}

void RPProcessShapeProcessDlg2::list_name_resource()
{

	std::list< RPObject* >::const_iterator it = m_resourceList.begin();
	while( it != m_resourceList.end() )
	{
		TRACE1( "%s\n", (*it)->getName().c_str() );
		m_combo_res_control.AddString((*it)->getName().c_str());
		it++;
	}
}


void RPProcessShapeProcessDlg2::OnOK() 
{
	/// @todo Add extra validation here
	CString data_combo;
	if(	m_combo_res_control.GetCurSel() != -1)
	{
		m_combo_res_control.GetLBText(m_combo_res_control.GetCurSel(),data_combo);
		pParentDlgMJ->m_ResList.AddString(data_combo);
        pParentMJ->m_resourceList.push_back(data_combo);
	}
	CDialog::OnOK();
}
