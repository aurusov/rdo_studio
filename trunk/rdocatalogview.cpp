#include "stdafx.h"
#include "rdocatalogdoc.h"
#include "rdocatalogview.h"
#include "rdocatalogapp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOCatalogView
// ----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOCatalogView, CHtmlView)

BEGIN_MESSAGE_MAP(RDOCatalogView, CHtmlView)
	//{{AFX_MSG_MAP(RDOCatalogView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOCatalogView::RDOCatalogView(): CHtmlView()
{
}

RDOCatalogView::~RDOCatalogView()
{
}

BOOL RDOCatalogView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CHtmlView::PreCreateWindow(cs);
}

void RDOCatalogView::OnDraw(CDC* pDC)
{
	RDOCatalogDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

#ifdef _DEBUG
void RDOCatalogView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void RDOCatalogView::Dump( CDumpContext& dc ) const
{
	CHtmlView::Dump( dc );
}

RDOCatalogDoc* RDOCatalogView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(RDOCatalogDoc)));
	return static_cast<RDOCatalogDoc*>(m_pDocument);
}
#endif

void RDOCatalogView::OnBeforeNavigate2( LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel )
{
	CString url = lpszURL;
	url.MakeLower();
	CString ext = url.Right( 4 );
	ext.MakeLower();
	if ( ext.Find( ".smr" ) == 0 ) {
		TCHAR szExeName[ MAX_PATH + 1 ];
		if ( ::GetModuleFileName( NULL, szExeName, MAX_PATH ) ) {
			int pos = url.ReverseFind( ' ' );
			if ( pos != -1 ) {
				CString model = url.Mid( pos + 1, url.GetLength() - pos - 1 - 4 );
				url.Delete( pos, url.GetLength() - pos );
				CString cmd = url;
				pos = cmd.Find( "rao-studio\\rao-studio.exe" );
				if ( pos == -1 ) {
					pos = cmd.Find( "rao-studio/rao-studio.exe" );
				}
				if ( pos != -1 ) {
					cmd.Delete( pos, url.GetLength() - pos );
					cmd = "\"" + cmd + "rao-explorer\\data\\russian\\models\\" + model + "\\source\\" + model + ".smr\" -autorun";
					::ShellExecute( GetSafeHwnd(), "open", url, cmd, NULL, SW_SHOWNORMAL );
					*pbCancel = TRUE;
				}
			}
		}
	} else if ( ext.Find( "htm" ) == -1 ) {
		::ShellExecute( GetSafeHwnd(), "open", lpszURL, NULL, NULL, SW_SHOWNORMAL );
		*pbCancel = TRUE;
	}
	CHtmlView::OnBeforeNavigate2( lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel );
}
