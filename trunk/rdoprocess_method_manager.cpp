#include "stdafx.h"
#include "rdoprocess_method_manager.h"
#include "rdoprocess_app.h"
#include <rdoprocess_method.h>
#include <rdoprocess_factory.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPMethodPlugin
// ----------------------------------------------------------------------------
RPMethodPlugin::RPMethodPlugin( const rp::string& _file_name ):
	file_name( _file_name ),
	lib( NULL ),
	method( NULL )
{
	lib = ::LoadLibrary( file_name.c_str() );
	rpMethod::project->log() << "load method " << file_name << " = " << lib << std::endl;
	if ( lib ) {
		rpMethod::PFunRegisterMethod registerMethod = reinterpret_cast<rpMethod::PFunRegisterMethod>(::GetProcAddress( lib, "registerMethod" ));
		if ( registerMethod ) {
//			HINSTANCE hinst = AfxGetResourceHandle();
//			AfxSetResourceHandle( lib );
			method = registerMethod( rpMethod::factory, rpMethod::project );
//			AfxSetResourceHandle( hinst );
			rpMethod::RPMethod::Info info = method->getInfo();
		}
	}
}

RPMethodPlugin::~RPMethodPlugin()
{
	if ( method ) {
		rpMethod::factory->removeMethod( method );
		delete method;
		method = NULL;
	}
	if ( lib ) {
		rpMethod::project->log() << "unload method " << file_name << " = " << lib << std::endl;
		::FreeLibrary( lib );
		lib = NULL;
	}
}

bool RPMethodPlugin::isMethod( const std::string& file_name )
{
	bool res = false;
	HMODULE local_lib = ::LoadLibrary( file_name.c_str() );
	if ( local_lib ) {
		rpMethod::PFunRegisterMethod registerMethod = reinterpret_cast<rpMethod::PFunRegisterMethod>(::GetProcAddress( local_lib, "registerMethod" ));
		if ( registerMethod ) {
			res = true;
		}
		::FreeLibrary( local_lib );
	}
	return res;
}

// ----------------------------------------------------------------------------
// ---------- RPMethodManager
// ----------------------------------------------------------------------------
RPMethodManager::RPMethodManager()
{
}

RPMethodManager::~RPMethodManager()
{
//	close();
}

void RPMethodManager::init()
{
	std::string path = "";
	TCHAR szExeName[ MAX_PATH + 1 ];
	if ( ::GetModuleFileName( NULL, szExeName, MAX_PATH ) ) {
		path = rp::extractFilePath( szExeName );
	}
	if ( !path.empty() ) {
		enumPlugins( path + "\\method\\*.*" );
	}
}

void RPMethodManager::enumPlugins( const std::string& mask )
{
	CFileFind finder;
	if ( finder.FindFile( mask.c_str() ) ) {
		BOOL flag = TRUE;
		while ( flag ) {
			flag = finder.FindNextFile();
			if ( finder.IsDirectory() && !finder.IsDots() ) {
				std::string subDir = finder.GetFilePath();
				subDir += "\\*.*";
				enumPlugins( subDir );
			} else if ( !finder.IsDots() ) {
				std::string fullname = finder.GetFilePath();
				if ( fullname.rfind( ".dll" ) == fullname.length() - 4 ) {
					std::string modulName = finder.GetFilePath();
					if ( RPMethodPlugin::isMethod( modulName ) ) {
						RPMethodPlugin* plugin = new RPMethodPlugin( modulName );
						methods.push_back( plugin );
						CListCtrl* listctrl = new CListCtrl();
						listctrl->Create( LVS_LIST | LVS_SINGLESEL, CRect(0,0,1,1), rpapp.getProjectBar().prepareNewPage(), -1 );
						::SetWindowLong( listctrl->m_hWnd, GWL_EXSTYLE, ::GetWindowLong( listctrl->m_hWnd, GWL_EXSTYLE ) | WS_EX_CLIENTEDGE );
						std::list< RPObjectClassInfo* > classes = rpMethod::factory->getMethodClasses( plugin->getMethod() );
						int index = 0;
						std::list< RPObjectClassInfo* >::const_iterator it = classes.begin();
						while ( it != classes.end() ) {
							if ( (*it)->isKindOf( "RPShape" ) ) {
								listctrl->InsertItem( index++, (*it)->getLabel().c_str() );
							}
							it++;
						}
						rpapp.getProjectBar().insertPage( listctrl, plugin->getMethod()->getInfo().name );
					}
				}
			}
		}
	}
	finder.Close();
}

void RPMethodManager::close()
{
	std::vector< RPMethodPlugin* >::iterator it = methods.begin();
	while ( it != methods.end() ) {
		delete *it;
		it = methods.erase( it );
	}
}

// ----------------------------------------------------------------------------
// ---------- RPMethodNewDlg
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RPMethodNewDlg, CDialog )
	//{{AFX_MSG_MAP(RPMethodNewDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_METHOD_LIST, &RPMethodNewDlg::OnMethodListItemÑhanged)
END_MESSAGE_MAP()

RPMethodNewDlg::RPMethodNewDlg():
	CDialog( IDD_METHOD_NEW )
{
	//{{AFX_DATA_INIT(RPMethodNewDlg)
	//}}AFX_DATA_INIT

}

RPMethodNewDlg::~RPMethodNewDlg()
{
}

void RPMethodNewDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RPMethodNewDlg)
	DDX_Control(pDX, IDC_METHOD_LIST, methods);
	DDX_Control(pDX, IDC_METHOD_DESC, desc);
	//}}AFX_DATA_MAP
}

BOOL RPMethodNewDlg::OnInitDialog()
{
	BOOL res = CDialog::OnInitDialog();
	CRect rect;
	methods.GetClientRect( rect );
	methods.InsertColumn( 0, "Íàçâàíèå", LVCFMT_LEFT, 120 );
	methods.InsertColumn( 1, "Âåðñèÿ", LVCFMT_LEFT, rect.Width() - methods.GetColumnWidth(0), 1 );
	methods.SetExtendedStyle( methods.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	std::vector< RPMethodPlugin* >::const_iterator it = rpapp.getMethodManager().getList().begin();
	int index_prev = 0;
	while ( it != rpapp.getMethodManager().getList().end() ) {
		int index = methods.InsertItem( index_prev, (*it)->getMethod()->getInfo().name.c_str() );
		if ( index != LB_ERR ) {
			rpMethod::RPMethod::Info info = (*it)->getMethod()->getInfo();
			methods.SetItemText( index, 1, rp::format( "ver %d.%d (build %d) %s", info.version_major, info.version_minor, info.version_build, info.version_info.c_str() ).c_str() );
			methods.SetItemData( index, reinterpret_cast<DWORD_PTR>((*it)->getMethod()) );
		}
		index_prev = index;
		it++;
	}
	if ( methods.GetItemCount() ) {
		methods.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED );
	} else {
		GetDlgItem( IDOK )->EnableWindow( false );
	}
	return res;
}

void RPMethodNewDlg::OnMethodListItemÑhanged( NMHDR *pNMHDR, LRESULT *pResult )
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	rpMethod::RPMethod* method = reinterpret_cast<rpMethod::RPMethod*>(pNMLV->lParam);
	desc.SetWindowText( method->getInfo().description.c_str() );
	*pResult = 0;
}
