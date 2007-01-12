#include "stdafx.h"
#include "rdostudiomodelnew.h"
#include "resource.h"
#include <shlobj.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioModelNew
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioModelNew, CDialog)
	//{{AFX_MSG_MAP(RDOStudioModelNew)
	ON_BN_CLICKED(IDC_MODEL_PATH_BUTTON, OnModelPathButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioModelNew::RDOStudioModelNew():
	CDialog( IDD_MODEL_NEW )
{
	//{{AFX_DATA_INIT(RDOStudioModelNew)
	m_modelName = _T("mymodel");
	m_modelPath = _T("");
	//}}AFX_DATA_INIT
}

RDOStudioModelNew::~RDOStudioModelNew()
{
}

void RDOStudioModelNew::DoDataExchange(CDataExchange* pDX) 
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RDOStudioModelNew)
	DDX_Text(pDX, IDC_MODEL_NAME, m_modelName);
	DDX_Text(pDX, IDC_MODEL_PATH, m_modelPath);
	//}}AFX_DATA_MAP
}

void RDOStudioModelNew::OnOK() 
{
	if ( m_modelName.IsEmpty() ) {
		MessageBox( "Необходимо указать имя модели", "Создание модели", MB_ICONHAND );
		return;
	}
	CDialog::OnOK();
}

BOOL RDOStudioModelNew::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void RDOStudioModelNew::OnModelPathButton()
{
// http://www.rsdn.ru/Forum/?mid=77166
	LPITEMIDLIST pidl = NULL;
	HRESULT hr = SHGetSpecialFolderLocation( m_hWnd, CSIDL_PERSONAL, &pidl );
	if ( hr == NOERROR ) {
		TRACE( "ok\n" );
	}
}
