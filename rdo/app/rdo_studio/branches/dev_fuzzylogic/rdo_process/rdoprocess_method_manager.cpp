#include "app/rdo_studio_mfc/pch/stdpch.h"
#include "rdoprocess_method_manager.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_pixmap.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_method.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_factory.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_shape.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_method_proc2rdo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef RDO_METHOD_DLL
// --------------------------------------------------------------------------------
// -------------------- RPMethodPlugin
// --------------------------------------------------------------------------------
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
			method = registerMethod( rpMethod::factory, rpMethod::project );
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
#endif

// --------------------------------------------------------------------------------
// -------------------- RPMethodManager
// --------------------------------------------------------------------------------
RPMethodManager::RPMethodManager()
{
}

RPMethodManager::~RPMethodManager()
{
	std::list< CImageList* >::iterator it = im_lists.begin();
	while ( it != im_lists.end() ) {
		delete *it;
		it++;
	}
}

void RPMethodManager::init()
{
#ifdef RDO_METHOD_DLL
	std::string path = "";
	TCHAR szExeName[ MAX_PATH + 1 ];
	if ( ::GetModuleFileName( NULL, szExeName, MAX_PATH ) ) {
		path = rp::extractFilePath( szExeName );
	}
	if ( !path.empty() ) {
		enumPlugins( path + "\\method\\*.*" );
	}
#else
	//insertMethod( RPMethodAlgorithm::registerMethod() );
	insertMethod( RPMethodProc2RDO_MJ::registerMethod() );
#endif
}

static int CALLBACK BlocksCompareProc( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
	UNUSED(lParamSort);

	int order1 = reinterpret_cast<RPObjectClassInfo*>(lParam1)->getOrder();
	int order2 = reinterpret_cast<RPObjectClassInfo*>(lParam2)->getOrder();
	if ( order1 != -1 && order2 != -1 ) {
		return order1 >= order2;
	} else if ( order1 == -1 && order2 == -1 ) {
		return 0;
	} else if ( order1 == -1 ) {
		return 1;
	} else {
		return -1;
	}
}

#ifdef RDO_METHOD_DLL
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
						if ( !plugin->getMethod()->hasPixmap() ) {
							plugin->method->setPixmap( new RPPixmapMFC( IDB_FLOWCHART_DEFAULT, RGB(0xFF,0xFF,0xFF) ) );
						}
						CListCtrl* listctrl = new CListCtrl();
						listctrl->Create( WS_CHILD | LVS_SORTASCENDING | LVS_AUTOARRANGE | LVS_ICON | LVS_SINGLESEL | LVS_NOLABELWRAP, CRect(0,0,1,1), studioApp.mainFrame->workspace.pagectrl->prepareNewPage(), 1 );
						CImageList* im_list = new CImageList();
						im_lists.push_back( im_list );
						im_list->Create( 32, 32, ILC_MASK | ILC_COLOR32, 0, 1 );
						listctrl->SetImageList( im_list, LVSIL_NORMAL );
						::SetWindowLong( listctrl->m_hWnd, GWL_EXSTYLE, ::GetWindowLong( listctrl->m_hWnd, GWL_EXSTYLE ) | WS_EX_CLIENTEDGE );
						std::list< RPObjectClassInfo* > classes = rpMethod::factory->getMethodClasses( plugin->getMethod() );
						int index = 0;
						std::list< RPObjectClassInfo* >::const_iterator it = classes.begin();
						while ( it != classes.end() ) {
							if ( (*it)->isKindOf( "RPShape" ) ) {
								RPObjectClassInfo* class_info = *it;
								im_list->Add( &static_cast<RPPixmapMFC*>(class_info->getPreview())->getCBitmap(), static_cast<RPPixmapMFC*>(class_info->getPreview())->getTransparent() );
								int id = listctrl->InsertItem( index, class_info->getLabel().c_str(), index );
								listctrl->SetItemData( id, reinterpret_cast<DWORD_PTR>(class_info) );
								index++;
							}
							it++;
						}
						RPPageCtrlItem* page = studioApp.mainFrame->workspace.pagectrl->insertPage( listctrl, plugin->getMethod()->getName() );
						page->setPixmap( *static_cast<RPPixmapMFC*>(plugin->getMethod()->getPixmap()) );
						listctrl->SortItems( BlocksCompareProc, NULL );
					}
				}
			}
		}
	}
	finder.Close();
}
#else
void RPMethodManager::insertMethod( rpMethod::RPMethod* method )
{
	methods.push_back( method );
	if ( !method->hasPixmap() ) {
		method->setPixmap( new RPPixmap( IDB_FLOWCHART_DEFAULT, RGB(0xFF,0xFF,0xFF) ) );
	}
	CListCtrl* listctrl = new CListCtrl();
	listctrl->Create( WS_CHILD | LVS_SORTASCENDING | LVS_AUTOARRANGE | LVS_ICON | LVS_SINGLESEL | LVS_NOLABELWRAP, CRect(0,0,1,1), studioApp.mainFrame->workspace.pagectrl->prepareNewPage(), 1 );
	CImageList* im_list = new CImageList();
	im_lists.push_back( im_list );
	im_list->Create( 32, 32, ILC_MASK | ILC_COLOR32, 0, 1 );
	listctrl->SetImageList( im_list, LVSIL_NORMAL );
	::SetWindowLong( listctrl->m_hWnd, GWL_EXSTYLE, ::GetWindowLong( listctrl->m_hWnd, GWL_EXSTYLE ) | WS_EX_CLIENTEDGE );
	std::list< RPObjectClassInfo* > classes = rpMethod::factory->getMethodClasses( method );
	int index = 0;
	std::list< RPObjectClassInfo* >::const_iterator it = classes.begin();
	while ( it != classes.end() ) {
		if ( (*it)->isKindOf( "RPShape" ) ) {
			RPObjectClassInfo* class_info = *it;
			im_list->Add( &class_info->getPreview()->getCBitmap(), class_info->getPreview()->getTransparent() );
			int id = listctrl->InsertItem( index, class_info->getLabel().c_str(), index );
#if _MSC_VER > 1200
			listctrl->SetItemData( id, reinterpret_cast<DWORD_PTR>(class_info) );
#else
			listctrl->SetItemData( id, reinterpret_cast<DWORD>(class_info) );
#endif
			index++;
		}
		it++;
	}
	RPPageCtrlItem* page = studioApp.mainFrame->workspace.pagectrl->insertPage( listctrl, method->getName() );
	page->setPixmap( *method->getPixmap() );
	listctrl->SortItems( BlocksCompareProc, NULL );
}
#endif

void RPMethodManager::close()
{
#ifdef RDO_METHOD_DLL
	std::vector< RPMethodPlugin* >::iterator it = methods.begin();
#else
	std::vector< rpMethod::RPMethod* >::iterator it = methods.begin();
#endif
	while ( it != methods.end() ) {
		delete *it;
		it = methods.erase( it );
	}
}

// --------------------------------------------------------------------------------
// -------------------- RPMethodNewDlg
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RPMethodNewDlg, CDialog )
	//{{AFX_MSG_MAP(RPMethodNewDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_METHOD_LIST, RPMethodNewDlg::OnMethodListItemChanged)
	ON_NOTIFY(NM_DBLCLK, IDC_METHOD_LIST, RPMethodNewDlg::OnMethodListDblClick)
	ON_NOTIFY(NM_CLICK, IDC_METHOD_LIST, RPMethodNewDlg::OnMethodListClick)
END_MESSAGE_MAP()

RPMethodNewDlg::RPMethodNewDlg():
	CDialog( IDD_METHOD_NEW ),
	method_last( NULL )
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

static int CALLBACK MethodsCompareProc( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
	UNUSED(lParamSort);

	return reinterpret_cast<rpMethod::RPMethod*>(lParam1)->getName() > reinterpret_cast<rpMethod::RPMethod*>(lParam2)->getName();
}

BOOL RPMethodNewDlg::OnInitDialog()
{
	BOOL res = CDialog::OnInitDialog();
	if ( !res ) {
		rpMethod::project->log() << "RPMethodNewDlg::OnInitDialog() = FAILD" << std::endl;
		return FALSE;
	}
	rpMethod::project->log() << "RPMethodNewDlg::OnInitDialog().." << std::endl;
	im_list.Create( 16, 16, ILC_MASK, 0, 1 );
	methods.SetImageList( &im_list, LVSIL_SMALL );
	rpMethod::project->log() << "methods.SetImageList" << std::endl;
	CRect rect;
	methods.GetClientRect( rect );
	methods.InsertColumn( 1, "Ќазвание", LVCFMT_LEFT, 120 );
	methods.InsertColumn( 2, "¬ерси€", LVCFMT_LEFT, rect.Width() - methods.GetColumnWidth(0) );
	rpMethod::project->log() << "methods.InsertColumns" << std::endl;
	methods.SetExtendedStyle( methods.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
#ifdef RDO_METHOD_DLL
	std::vector< RPMethodPlugin* >::const_iterator it = studioApp.getMethodManager().getList().begin();
#else
	std::vector< rpMethod::RPMethod* >::const_iterator it = studioApp.getMethodManager().getList().begin();
#endif
	int index = 0;
	rpMethod::project->log() << "перед вставкой методов" << std::endl;
	while ( it != studioApp.getMethodManager().getList().end() ) {
#ifdef RDO_METHOD_DLL
		rpMethod::RPMethod* method = (*it)->getMethod();
#else
		rpMethod::RPMethod* method = *it;
#endif
		rpMethod::project->log() << "  добавление метода " << method->getName() << std::endl;
		index = methods.InsertItem( index, method->getName().c_str() );
		if ( index != LB_ERR ) {
			methods.SetItemText( index, 1, rp::format( "ver %d.%d (build %d) %s", method->getVersionMajor(), method->getVersionMinor(), method->getVersionBuild(),  method->getVersionDesc().c_str() ).c_str() );
			rpMethod::project->log() << "    index = " << index << std::endl;
#if _MSC_VER > 1200
			methods.SetItemData( index, reinterpret_cast<DWORD_PTR>(method) );
#else
			methods.SetItemData( index, reinterpret_cast<DWORD>(method) );
#endif
		} else {
			rpMethod::project->log() << "  ошибка добавлени€" << std::endl;
			return FALSE;
		}
		rpMethod::project->log() << "  метод добавлен " << method->getName() << std::endl;
		index++;
		it++;
	}
	if ( methods.GetItemCount() ) {
		rpMethod::project->log() << "сортировка.." << std::endl;
		methods.SortItems( MethodsCompareProc, NULL );
		rpMethod::project->log() << "сортировка.. ok" << std::endl;
		methods.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED );
	} else {
		GetDlgItem( IDOK )->EnableWindow( false );
	}
	rpMethod::project->log() << "RPMethodNewDlg::OnInitDialog().. ok" << std::endl;
	return res;
}

void RPMethodNewDlg::OnMethodListItemChanged( NMHDR *pNMHDR, LRESULT *pResult )
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	rpMethod::RPMethod* method = reinterpret_cast<rpMethod::RPMethod*>(pNMLV->lParam); // method_map[pNMLV->iItem];
	desc.SetWindowText( method->getDescription().c_str() );
	*pResult = 0;
}

void RPMethodNewDlg::OnOK()
{
	if ( methods.GetItemCount() ) {
		int index = methods.GetNextItem( -1, LVNI_SELECTED );
		if ( index != -1 ) {
			method_last = reinterpret_cast<rpMethod::RPMethod*>(methods.GetItemData(index));
			rpMethod::project->log() << "method_last = " << method_last->getName() << std::endl;
		}
	}
	CDialog::OnOK();
}

void RPMethodNewDlg::OnMethodListDblClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	UNUSED(pNMHDR);

	if ( methods.GetItemCount() ) {
		int index = methods.GetNextItem( -1, LVNI_SELECTED );
		if ( index != -1 ) {
			method_last = reinterpret_cast<rpMethod::RPMethod*>(methods.GetItemData(index));
			CDialog::OnOK();
		}
	}
	*pResult = 0;
}

void RPMethodNewDlg::OnMethodListClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	UNUSED(pNMHDR);

	method_last = NULL;
	if ( methods.GetItemCount() ) {
		int index = methods.GetNextItem( -1, LVNI_SELECTED );
		if ( index != -1 ) {
			method_last = reinterpret_cast<rpMethod::RPMethod*>(methods.GetItemData(index));
		}
	}
	if ( method_last ) {
		desc.SetWindowText( method_last->getDescription().c_str() );
		GetDlgItem( IDOK )->EnableWindow( true );
	} else {
		desc.SetWindowText( "" );
		GetDlgItem( IDOK )->EnableWindow( false );
	}
	*pResult = 0;
}

// --------------------------------------------------------------------------------
// -------------------- CListCtrlDraw
// --------------------------------------------------------------------------------
void CListCtrlDraw::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	CDC* pDC = CDC::FromHandle( lpDrawItemStruct->hDC );

	CRect  rect( lpDrawItemStruct->rcItem );
	CRect  rect_col0;
	CRect  rect_col1;
	GetHeaderCtrl()->GetItemRect( 0, rect_col0 );
	GetHeaderCtrl()->GetItemRect( 1, rect_col1 );
	rect_col0.left  += rect.left + 22;
	rect_col0.right += rect.left;
	rect_col0.top    = rect.top;
	rect_col0.bottom = rect.bottom;
	rect_col1.left   = rect_col0.right + 4;
	rect_col1.right += rect.left;
	rect_col1.top    = rect.top;
	rect_col1.bottom = rect.bottom;
	CPoint imagePoint( rect_col0.left - 20, rect_col0.top );

	if ( lpDrawItemStruct->itemState & ODS_SELECTED ) {
		CBrush br( ::GetSysColor(COLOR_HIGHLIGHT) );
		pDC->FillRect( &rect, &br );
	} else {
		CBrush br( ::GetSysColor(COLOR_WINDOW) );
		pDC->FillRect( &rect, &br );
	}

	rpMethod::RPMethod* method = reinterpret_cast<rpMethod::RPMethod*>(lpDrawItemStruct->itemData);
	method->getPixmap()->Draw( pDC->m_hDC, imagePoint.x, imagePoint.y, GetColumnWidth(0) - imagePoint.x - 1 );

	int prev_bkMode = pDC->SetBkMode( TRANSPARENT );
	COLORREF prev_textColor;
	if ( lpDrawItemStruct->itemState & ODS_SELECTED ) {
		prev_textColor = pDC->SetTextColor( ::GetSysColor(COLOR_HIGHLIGHTTEXT) );
	} else {
		prev_textColor = pDC->SetTextColor( ::GetSysColor(COLOR_WINDOWTEXT) );
	}

	CString col0 = GetItemText( lpDrawItemStruct->itemID, 0 );
	CString col1 = GetItemText( lpDrawItemStruct->itemID, 1 );

	UINT nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	pDC->DrawText( col0, -1, &rect_col0, nFormat );
	pDC->DrawText( col1, -1, &rect_col1, nFormat );

	pDC->SetTextColor( prev_textColor );
	pDC->SetBkMode( prev_bkMode );
}
