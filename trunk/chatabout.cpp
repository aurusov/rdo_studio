#include "stdafx.h"
#include "chatabout.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatAbout
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CChatAbout, CDialog)
	//{{AFX_MSG_MAP(CChatAbout)
	//}}AFX_MSG_MAP
	ON_NOTIFY( NM_RDOLINK_CLICK, IDC_EMAIL_STATIC, OnEMailClick )
END_MESSAGE_MAP()

CChatAbout::CChatAbout( CWnd* pParent ): CDialog( IDD_ABOUT, pParent )
{
	//{{AFX_DATA_INIT(CChatAbout)
	//}}AFX_DATA_INIT
}

void CChatAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatAbout)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_EMAIL_STATIC, email );
}

BOOL CChatAbout::OnInitDialog() 
{
	CDialog::OnInitDialog();


	TCHAR szExeName[ MAX_PATH + 1 ];
	if ( ::GetModuleFileName( NULL, szExeName, MAX_PATH ) ) {
		DWORD dwHnd;
		DWORD size = ::GetFileVersionInfoSize( szExeName, &dwHnd );
		if ( size ) {
			void* pBuffer = malloc( size );
			if ( pBuffer != NULL ) {
				if ( ::GetFileVersionInfo( szExeName, dwHnd, size, pBuffer ) ) {
					DWORD* pTranslation;
					UINT   length;
					if ( ::VerQueryValue( pBuffer, _T("\\VarFileInfo\\Translation"), (void**)&pTranslation, &length ) ) {
						DWORD translation = *pTranslation;
						char key[2000];
						wsprintf( key, _T("\\StringFileInfo\\%04x%04x\\ProductName"), LOWORD( translation ), HIWORD( translation ) );
						char* productName;
						if ( ::VerQueryValue( pBuffer, key, (void**)&productName, &length ) ) {
							VS_FIXEDFILEINFO* fixedInfo;
							if ( ::VerQueryValue( pBuffer, _T("\\"), (void**)&fixedInfo, &length ) ) {
								GetDlgItem( IDC_BUILD_STATIC )->SetWindowText( format( "%s     version %u.%u (build %u)", productName, HIWORD( fixedInfo->dwProductVersionMS ), LOWORD( fixedInfo->dwProductVersionMS ), LOWORD( fixedInfo->dwProductVersionLS ) ).c_str() );
							}
						}
					}
				}
				free( pBuffer );
			}
		}
	}

	email.setTextColor( RGB(0x00, 0x40, 0x80) );

	return TRUE;
}

void CChatAbout::OnEMailClick( NMHDR* /*pNotifyStruct*/, LRESULT* result )
{
	::ShellExecute( m_hWnd, "open", "mailto:andrewu@mail.ru", 0, 0, SW_SHOWNORMAL );
	*result = 0;
}
