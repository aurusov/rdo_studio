#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "rdoprocess_shape_terminate_dlg1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(RPProcessShapeTerminateDlg1, CDialog)
END_MESSAGE_MAP()

RPProcessShapeTerminateDlg1::RPProcessShapeTerminateDlg1(PTR(CWnd) pWndParent, PTR(RPProcessShapeTerminate) pShape)
	: CDialog(RPProcessShapeTerminateDlg1::IDD, pWndParent)
{
	m_name = _T("");
	m_pShape = pShape;
	ASSERT(m_pShape);
}

void RPProcessShapeTerminateDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_name            );
	DDX_Text(pDX, IDC_EDIT3, m_terminateCounter);
}

BOOL RPProcessShapeTerminateDlg1::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_name             = CString(m_pShape->getName().c_str());
	m_terminateCounter = m_pShape->m_term_inc;
	UpdateData(FALSE);

	return TRUE;
}

void RPProcessShapeTerminateDlg1::OnOK()
{
	UpdateData(true);

	if (m_pShape->setName(rp::string(m_name)))
	{
		m_pShape->m_term_inc = m_terminateCounter;
		m_pShape->m_name     = m_name;
		CDialog::OnOK();
	}
}
