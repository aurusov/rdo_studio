/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      application.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtCore/qprocess.h>
#include <QtCore/qtextcodec.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdofile.h"
#include "kernel/rdothread.h"
#include "repository/rdorepository.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/src/child_frm.h"
#include "app/rdo_studio_mfc/src/about.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/src/edit/document.h"
#include "app/rdo_studio_mfc/src/edit/view.h"
#include "app/rdo_studio_mfc/src/plugins.h"
#include "app/rdo_studio_mfc/src/thread.h"
#include "app/rdo_studio_mfc/src/model/new.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracer.h"
#include "app/rdo_studio_mfc/htmlhelp.h"
#include "app/rdo_studio_mfc/plugins/common/rdoplugin.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_factory.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_method.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_project.h"
#include "thirdparty/win_registry/registry.h"
#include "thirdparty/qt-solutions/qtwinmigrate/src/qmfcapp.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOFileAssociationDlg
// --------------------------------------------------------------------------------
class RDOFileAssociationDlg: public CDialog
{
protected:
	virtual void DoDataExchange(PTR(CDataExchange) pDX);
	virtual void OnCancel      ();

public:
	RDOFileAssociationDlg(PTR(CWnd) pParentWnd = NULL);
	virtual ~RDOFileAssociationDlg();

	int m_checkInFuture;
};

RDOFileAssociationDlg::RDOFileAssociationDlg(PTR(CWnd) pParentWnd)
	: CDialog(IDD_FILEASSOCIATION, pParentWnd)
	, m_checkInFuture(true)
{}

RDOFileAssociationDlg::~RDOFileAssociationDlg()
{}

void RDOFileAssociationDlg::DoDataExchange(PTR(CDataExchange) pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_FILEASSOCIATION_CHECK, m_checkInFuture);
}

void RDOFileAssociationDlg::OnCancel()
{
	CDialog::UpdateData(true);
	CDialog::OnCancel  ();
}

// -----------------------------------------------------------------
// -------------------- RDOStudioCommandLineInfo
// -----------------------------------------------------------------
class RDOStudioCommandLineInfo: public CCommandLineInfo
{
public:
	RDOStudioCommandLineInfo()
		: CCommandLineInfo()
		, m_pluginStartFound(false)
		, m_pluginExitFound (false)
	{}

protected:
	rbool m_pluginStartFound;
	rbool m_pluginExitFound;

	virtual void ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast);
};

void RDOStudioCommandLineInfo::ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
{
	CCommandLineInfo::ParseParam(lpszParam, bFlag, bLast);
	if (bFlag)
	{
		m_pluginStartFound = false;
		m_pluginExitFound  = false;

		if (CString(lpszParam).CompareNoCase(_T("autorun")) == 0)
		{
			studioApp.m_autoRun = true;
		}
		else if (CString(lpszParam).CompareNoCase(_T("autoexit")) == 0)
		{
			studioApp.m_autoExitByModel = true;
		}
		else if (CString(lpszParam).CompareNoCase(_T("dont_close_if_error")) == 0)
		{
			studioApp.m_dontCloseIfError = true;
		}
		else if (CString(lpszParam).CompareNoCase(_T("pluginstart")) == 0)
		{
			m_pluginStartFound = true;
		}
		else if (CString(lpszParam).CompareNoCase(_T("pluginautoexit")) == 0)
		{
			m_pluginExitFound = true;
		}
	}
	else
	{
		if (m_pluginStartFound)
		{
			studioApp.m_pluginStartNameList.push_back(lpszParam);
		}
		else if (m_pluginExitFound)
		{
			studioApp.m_pluginExitNameList.push_back(lpszParam);
		}
		else if (studioApp.m_openModelName.empty())
		{
			studioApp.m_openModelName = lpszParam;
		}
		m_pluginStartFound = false;
		m_pluginExitFound  = false;
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioApp
// --------------------------------------------------------------------------------
RDOStudioApp studioApp;

BEGIN_MESSAGE_MAP(RDOStudioApp, CWinApp)
	ON_COMMAND          (ID_FILE_NEW,           OnFileNew          )
	ON_COMMAND          (ID_FILE_OPEN,          OnFileOpen         )
	ON_COMMAND          (ID_FILE_MODEL_SAVE,    OnFileSave         )
	ON_UPDATE_COMMAND_UI(ID_FILE_MODEL_SAVE,    OnUpdateFileSave   )
	ON_COMMAND          (ID_FILE_SAVE_ALL,      OnFileSaveAll      )
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_ALL,      OnUpdateFileSaveAll)
	ON_COMMAND          (ID_FILE_MODEL_CLOSE,   OnFileClose        )
	ON_COMMAND          (ID_FILE_MODEL_SAVE_AS, OnFileSaveAs       )
	ON_UPDATE_COMMAND_UI(ID_FILE_MODEL_CLOSE,   OnUpdateFileClose  )
	ON_UPDATE_COMMAND_UI(ID_FILE_MODEL_SAVE_AS, OnUpdateFileSaveAs )
	ON_COMMAND          (ID_MODEL_BUILD,        OnModelBuild       )
	ON_COMMAND          (ID_MODEL_RUN,          OnModelRun         )
	ON_COMMAND          (ID_MODEL_STOP,         OnModelStop        )
	ON_UPDATE_COMMAND_UI(ID_MODEL_BUILD,        OnUpdateModelBuild )
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUN,          OnUpdateModelRun   )
	ON_UPDATE_COMMAND_UI(ID_MODEL_STOP,         OnUpdateModelStop  )
	ON_COMMAND          (ID_APP_ABOUT,          OnAppAbout         )
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW,           OnUpdateFileNew    )
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN,          OnUpdateFileOpen   )
	ON_COMMAND_RANGE    (ID_FILE_REOPEN_1, ID_FILE_REOPEN_10, OnProjectReopen)
	ON_COMMAND          (ID_FILE_PRINT_SETUP,   CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

RDOStudioApp::RDOStudioApp()
	: CWinApp()
	, m_pStudioGUI                  (NULL  )
#ifdef RDO_MT
	, m_pStudioMT                   (NULL  )
#endif
	, m_editDocTemplate             (NULL  )
	, m_fileAssociationSetup        (false )
	, m_fileAssociationCheckInFuture(false )
	, m_openLastProject             (false )
	, m_lastProjectName             (_T(""))
	, m_showCaptionFullName         (false )
	, m_autoRun                     (false )
	, m_autoExitByModel             (false )
	, m_dontCloseIfError            (false )
	, m_exitCode                    (rdo::simulation::report::EC_OK)
	, m_openModelName               (_T(""))
	, m_pMainFrame                  (NULL  )
{
	setlocale(LC_ALL,     _T("rus"));
	setlocale(LC_NUMERIC, _T("eng"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

	m_log.open(_T("log.txt"));
#ifdef PROCGUI_ENABLE
	new RPProjectMFC();
	new RPObjectFactory();
	rpMethod::factory->registerDefaultObject();
#endif
}

RDOStudioApp::~RDOStudioApp()
{}

BOOL RDOStudioApp::Run()
{
	return QMfcApp::run(this);
}

BOOL RDOStudioApp::InitInstance()
{
	CWinApp::InitInstance();

	if (::OleInitialize(NULL) != S_OK)
		return FALSE;

	free((PTR(void))m_pszProfileName);
	m_pszProfileName = _tcsdup(_T(""));
	free((PTR(void))m_pszRegistryKey);
	m_pszRegistryKey = _tcsdup(_T("RAO-studio"));

	m_fileAssociationSetup         = GetProfileInt   (_T("fileAssociation"), _T("setup"),                true) ? true : false;
	m_fileAssociationCheckInFuture = GetProfileInt   (_T("fileAssociation"), _T("checkInFuture"),        true) ? true : false;
	m_openLastProject              = GetProfileInt   (_T("general"),         _T("openLastProject"),      true) ? true : false;
	m_lastProjectName              = GetProfileString(_T("general"),         _T("lastProject"),        _T(""));
	m_showCaptionFullName          = GetProfileInt   (_T("general"),         _T("showCaptionFullName"), false) ? true : false;

	m_editDocTemplate = new CMultiDocTemplate(IDR_EDIT_TYPE, RUNTIME_CLASS(RDOStudioEditDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioEditView));
	AddDocTemplate(m_editDocTemplate);

	// Кто-то должен поднять кернел и треды
	RDOKernel::init();
#ifdef RDO_MT
	m_pStudioGUI = new RDOThreadStudioGUI();
#else
	m_pStudioGUI = kernel;
#endif
	new rdo::service::simulation::RDOThreadSimulator();
	new rdo::service::simulation::RDOThreadCodeComp();
	new rdo::repository::RDOThreadRepository();

//#define CORBA_ENABLE
#ifdef CORBA_ENABLE
	new rdoCorba::RDOThreadCorba();
#endif

#ifdef RDO_MT
	m_pStudioMT = new RDOThreadStudio();
#endif
//	new RDOThreadStudio1();
//	new RDOThreadStudio2();

	tracer = new rdoTracer::RDOTracer();
	AddDocTemplate(tracer->createDocTemplate());
#ifdef PROCGUI_ENABLE
	rpMethod::project->cursors[ RPProject::cursor_flow_select ]        = AfxGetApp()->LoadCursor(IDC_FLOW_SELECT);
	rpMethod::project->cursors[ RPProject::cursor_flow_move ]          = AfxGetApp()->LoadCursor(IDC_FLOW_MOVE);
	rpMethod::project->cursors[ RPProject::cursor_flow_connector ]     = AfxGetApp()->LoadCursor(IDC_FLOW_CONNECTOR);
	rpMethod::project->cursors[ RPProject::cursor_flow_rotate ]        = AfxGetApp()->LoadCursor(IDC_FLOW_ROTATE);
	rpMethod::project->cursors[ RPProject::cursor_flow_rotate_center ] = AfxGetApp()->LoadCursor(IDC_FLOW_ROTATE_CENTER);
	rpMethod::project->cursors[ RPProject::cursor_flow_rotate_tl ]     = AfxGetApp()->LoadCursor(IDC_FLOW_ROTATE_TL);
	rpMethod::project->cursors[ RPProject::cursor_flow_rotate_tr ]     = AfxGetApp()->LoadCursor(IDC_FLOW_ROTATE_TL);
	rpMethod::project->cursors[ RPProject::cursor_flow_rotate_bl ]     = AfxGetApp()->LoadCursor(IDC_FLOW_ROTATE_TL);
	rpMethod::project->cursors[ RPProject::cursor_flow_rotate_br ]     = AfxGetApp()->LoadCursor(IDC_FLOW_ROTATE_TL);
	rpMethod::project->cursors[ RPProject::cursor_flow_scale_lr ]      = AfxGetApp()->LoadCursor(IDC_FLOW_SCALE_LR);
	rpMethod::project->cursors[ RPProject::cursor_flow_scale_tb ]      = AfxGetApp()->LoadCursor(IDC_FLOW_SCALE_TB);
	rpMethod::project->cursors[ RPProject::cursor_flow_scale_tlbr ]    = AfxGetApp()->LoadCursor(IDC_FLOW_SCALE_TLBR);
	rpMethod::project->cursors[ RPProject::cursor_flow_scale_trbl ]    = AfxGetApp()->LoadCursor(IDC_FLOW_SCALE_TRBL);
	rpMethod::project->cursors[ RPProject::cursor_flow_dock_in ]       = AfxGetApp()->LoadCursor(IDC_FLOW_DOCK_IN);
	rpMethod::project->cursors[ RPProject::cursor_flow_dock_out ]      = AfxGetApp()->LoadCursor(IDC_FLOW_DOCK_IN);
	rpMethod::project->cursors[ RPProject::cursor_flow_dock_inout ]    = AfxGetApp()->LoadCursor(IDC_FLOW_DOCK_IN);
	rpMethod::project->cursors[ RPProject::cursor_flow_dock_fly ]      = AfxGetApp()->LoadCursor(IDC_FLOW_DOCK_IN);
	rpMethod::project->cursors[ RPProject::cursor_flow_dock_not ]      = AfxGetApp()->LoadCursor(IDC_FLOW_DOCK_NOT);
	rpMethod::project->cursors[ RPProject::cursor_flow_trash ]         = AfxGetApp()->LoadCursor(IDC_FLOW_TRASH);
#endif
	QMfcApp::instance(this);
	qApp->setQuitOnLastWindowClosed(true);

	// Внутри создается объект модели
	m_pMainFrame = new RDOStudioMainFrame;
	m_pMainFrame->init();
	m_pMainWnd = m_pMainFrame->c_wnd();
	m_pMainFrame->show();

#ifdef RDO_MT
	kernel->thread_studio = m_pStudioGUI;
#else
	kernel->thread_studio = model;
#endif

	loadReopen();
//	updateReopenSubMenu();

	if (getFileAssociationCheckInFuture())
	{
		setupFileAssociation();
	}

#ifdef PROCGUI_ENABLE
	m_methodManager.init();
	m_pMainFrame->workspace.pagectrl->selectFirst();
#endif
	RDOStudioCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Создаем менеджер плагинов после создания главного окна и парсера командной строки
	plugins = new RDOStudioPlugins;

	rbool newModel  = true;
	rbool autoModel = false;
	if (!m_openModelName.empty())
	{
		if (!rdo::extractFilePath(m_openModelName).empty())
		{
			tstring longFileName;
			if (shortToLongPath(m_openModelName, longFileName))
			{
				m_openModelName = longFileName;
			}
			if (model->openModel(m_openModelName))
			{
				autoModel = true;
			}
		}
		else
		{
			m_openModelName = rdo::extractFilePath(RDOStudioApp::getFullExtName()) + m_openModelName;
			if (rdo::File::exist(m_openModelName) && model->openModel(m_openModelName))
			{
				m_autoRun          = true;
				m_autoExitByModel  = true;
				m_dontCloseIfError = true;
				autoModel          = true;
			}
			else
			{
				m_exitCode = rdo::simulation::report::EC_ModelNotFound;
				return false;
			}
		}
	}
	else
	{
		if (getOpenLastProject() && !getLastProjectName().empty() && rdo::File::exist(getLastProjectName()))
		{
			model->openModel(getLastProjectName());
			newModel = false;
		}
	}

	if (autoModel)
	{
		newModel = false;
	}
	else
	{
		m_autoRun          = false;
		m_autoExitByModel  = false;
		m_dontCloseIfError = false;
	}

	if (newModel)
	{
//		OnFileNew();
	}

	if (m_autoRun)
	{
		OnModelRun();
	}

	return TRUE;
}

int RDOStudioApp::ExitInstance()
{
#ifdef PROCGUI_ENABLE
	m_methodManager.close();

	if (rpMethod::factory)
	{
		delete rpMethod::factory;
		rpMethod::factory = NULL;
	}

	if (rpMethod::project)
	{
		delete rpMethod::project;
		rpMethod::project = NULL;
	}
#endif
	if (m_exitCode != rdo::simulation::report::EC_ModelNotFound)
	{
		m_exitCode = model->getExitCode();
	}
#ifdef RDO_MT
	if (m_pStudioGUI)
	{
		m_pStudioGUI->sendMessage(m_pStudioGUI, RDOThread::RT_THREAD_CLOSE);
		delete static_cast<PTR(RDOThreadStudioGUI)>(m_pStudioGUI);
		m_pStudioGUI = NULL;
	}
#endif

	if (plugins)
	{
		delete plugins;
		plugins = NULL;
	}

	// Роняем кернел и закрываем все треды
	RDOKernel::close();

	::HtmlHelp(NULL, NULL, HH_CLOSE_ALL, 0);

	if (m_autoExitByModel || !m_pluginExitNameList.empty())
	{
		CWinApp::ExitInstance();
		return m_exitCode;
	}
	else
	{
		return CWinApp::ExitInstance();
	}
}

CREF(RPMethodManager) RDOStudioApp::getMethodManager() const
{
	return m_methodManager;
}

REF(std::ofstream) RDOStudioApp::log()
{
	return m_log;
}

rbool RDOStudioApp::shortToLongPath(CREF(tstring) shortPath, REF(tstring) longPath)
{
	USES_CONVERSION;

	LPSHELLFOLDER psfDesktop    = NULL;
	ULONG         chEaten       = 0;
	LPITEMIDLIST  pidlShellItem = NULL;
	WCHAR         szLongPath[_MAX_PATH] = { 0 };

	// Get the Desktop's shell folder interface
	HRESULT hr = ::SHGetDesktopFolder(&psfDesktop);

	LPWSTR  lpwShortPath = A2W(shortPath.c_str());

	// Request an ID list (relative to the desktop) for the short pathname 
	hr = psfDesktop->ParseDisplayName(NULL, NULL, lpwShortPath, &chEaten, &pidlShellItem, NULL);
	psfDesktop->Release(); // Release the desktop's IShellFolder

	if (FAILED(hr))
	{
		// If we couldn't get an ID list for short pathname, it must not exist.
		longPath.empty();
		return false;
	}
	else
	{
		// We did get an ID list, convert it to a long pathname
		::SHGetPathFromIDListW(pidlShellItem, szLongPath);
		// Free the ID list allocated by ParseDisplayName
		LPMALLOC pMalloc = NULL;
		::SHGetMalloc(&pMalloc);
		pMalloc->Free(pidlShellItem);
		pMalloc->Release();
		longPath = W2A(szLongPath);
		return true;
	}
}

void RDOStudioApp::OnFileNew() 
{
	RDOStudioModelNew dlg;
	if (dlg.DoModal() == IDOK)
	{
		model->newModel(dlg.getModelName(), dlg.getModelPath() + dlg.getModelName(), dlg.getModelTemplate());
	}
}

void RDOStudioApp::OnFileOpen() 
{
	model->openModel();
}

void RDOStudioApp::OnFileClose() 
{
	model->closeModel();
}

void RDOStudioApp::OnFileSave() 
{
	model->saveModel();
}

void RDOStudioApp::OnFileSaveAs() 
{
	model->saveAsModel();
}

void RDOStudioApp::OnFileSaveAll() 
{
	model->saveModel();
}

void RDOStudioApp::OnUpdateFileNew(PTR(CCmdUI) pCmdUI)
{
	pCmdUI->Enable(model->canNew());
}

void RDOStudioApp::OnUpdateFileOpen(PTR(CCmdUI) pCmdUI)
{
	pCmdUI->Enable(model->canOpen());
}

void RDOStudioApp::OnUpdateFileClose(PTR(CCmdUI) pCmdUI) 
{
	pCmdUI->Enable(model->canClose());
}

void RDOStudioApp::OnUpdateFileSave(PTR(CCmdUI) pCmdUI) 
{
	pCmdUI->Enable(model->canSave());
}

void RDOStudioApp::OnUpdateFileSaveAs(PTR(CCmdUI) pCmdUI) 
{
	pCmdUI->Enable(model->hasModel() && !model->isRunning());
}

void RDOStudioApp::OnUpdateFileSaveAll(PTR(CCmdUI) pCmdUI) 
{
	pCmdUI->Enable(model->canSave());
}

void RDOStudioApp::OnProjectReopen(UINT nID)
{
	ruint i = 0;
	switch (nID)
	{
	case ID_FILE_REOPEN_1 : i = 0; break;
	case ID_FILE_REOPEN_2 : i = 1; break;
	case ID_FILE_REOPEN_3 : i = 2; break;
	case ID_FILE_REOPEN_4 : i = 3; break;
	case ID_FILE_REOPEN_5 : i = 4; break;
	case ID_FILE_REOPEN_6 : i = 5; break;
	case ID_FILE_REOPEN_7 : i = 6; break;
	case ID_FILE_REOPEN_8 : i = 7; break;
	case ID_FILE_REOPEN_9 : i = 8; break;
	case ID_FILE_REOPEN_10: i = 9; break;
	}

	if (!model->openModel(m_reopenList[i]) && model->isPrevModelClosed())
	{
		m_reopenList.erase(m_reopenList.begin() + i);
		updateReopenSubMenu();
	}
}

void RDOStudioApp::insertReopenItem(CREF(tstring) item)
{
	if (!item.empty())
	{
		STL_FOR_ALL(m_reopenList, it)
		{
			if (*it == item)
			{
				m_reopenList.erase(it);
				break;
			}
		}

		m_reopenList.insert(m_reopenList.begin(), item);

		while (m_reopenList.size() > 10)
		{
			ReopenList::iterator it = m_reopenList.end();
			--it;
			m_reopenList.erase(it);
		}

		updateReopenSubMenu();
	}
}

void RDOStudioApp::updateReopenSubMenu() const
{
	if (!AfxGetMainWnd() || !AfxGetMainWnd()->GetMenu())
		return;

	rbool maximized = m_pMainFrame->isMDIMaximazed();
	int delta = maximized ? 1 : 0;

	PTR(CMenu) pReopenMenu = AfxGetMainWnd()->GetMenu()->GetSubMenu(delta)->GetSubMenu(2);

	while (pReopenMenu->GetMenuItemCount())
	{
		pReopenMenu->DeleteMenu(0, MF_BYPOSITION);
	}

	if (!m_reopenList.empty())
	{
		AfxGetMainWnd()->GetMenu()->GetSubMenu(delta)->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
		for (ReopenList::size_type i = 0; i < m_reopenList.size(); i++)
		{
			if (i == 4)
			{
				pReopenMenu->AppendMenu(MF_SEPARATOR);
			}
			ruint id = ID_FILE_MRU_FILE1;
			switch (i)
			{
			case 0: id = ID_FILE_REOPEN_1;  break;
			case 1: id = ID_FILE_REOPEN_2;  break;
			case 2: id = ID_FILE_REOPEN_3;  break;
			case 3: id = ID_FILE_REOPEN_4;  break;
			case 4: id = ID_FILE_REOPEN_5;  break;
			case 5: id = ID_FILE_REOPEN_6;  break;
			case 6: id = ID_FILE_REOPEN_7;  break;
			case 7: id = ID_FILE_REOPEN_8;  break;
			case 8: id = ID_FILE_REOPEN_9;  break;
			case 9: id = ID_FILE_REOPEN_10; break;
			}
			pReopenMenu->AppendMenu(MF_STRING, id, rdo::format(_T("%d. %s"), i+1, m_reopenList[i].c_str()).c_str());
		}
	}
	else
	{
		AfxGetMainWnd()->GetMenu()->GetSubMenu(delta)->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	}

	saveReopen();
}

void RDOStudioApp::loadReopen()
{
	m_reopenList.clear();
	for (ruint i = 0; i < 10; i++)
	{
		tstring sec;
		if (i+1 < 10)
		{
			sec = rdo::format(_T("0%d"), i+1);
		}
		else
		{
			sec = rdo::format(_T("%d"), i+1);
		}
		TRY
		{
			tstring s = AfxGetApp()->GetProfileString(_T("reopen"), sec.c_str(), _T(""));
			if (!s.empty())
			{
				m_reopenList.insert(m_reopenList.end(), s);
			}
		}
		CATCH(CException, e)
		{}
		END_CATCH
	}
}

void RDOStudioApp::saveReopen() const
{
	for (ReopenList::size_type i = 0; i < 10; i++)
	{
		tstring sec;
		if (i+1 < 10)
		{
			sec = rdo::format(_T("0%d"), i+1);
		}
		else
		{
			sec = rdo::format(_T("%d"), i+1);
		}
		tstring s;
		if (i < m_reopenList.size())
		{
			s = m_reopenList[i];
		}
		else
		{
			s = _T("");
		}
		TRY
		{
			AfxGetApp()->WriteProfileString(_T("reopen"), sec.c_str(), s.c_str());
		}
		CATCH(CException, e)
		{}
		END_CATCH
	}
}

void RDOStudioApp::OnModelBuild() 
{
	model->buildModel();
}

void RDOStudioApp::OnModelRun() 
{
	model->runModel();
}

void RDOStudioApp::OnModelStop() 
{
	model->stopModel();
}

void RDOStudioApp::OnUpdateModelBuild(PTR(CCmdUI) pCmdUI) 
{
	pCmdUI->Enable(model->canBuild());
}

void RDOStudioApp::OnUpdateModelRun(PTR(CCmdUI) pCmdUI) 
{
	pCmdUI->Enable(model->canRun());
}

void RDOStudioApp::OnUpdateModelStop(PTR(CCmdUI) pCmdUI) 
{
	pCmdUI->Enable(model->isRunning());
}

tstring RDOStudioApp::getFullExtName()
{
	tstring fileName = _T("");
	TCHAR szExeName[ MAX_PATH + 1 ];
	if (::GetModuleFileName(NULL, szExeName, MAX_PATH))
	{
		fileName = szExeName;
	}
	return fileName;
}

tstring RDOStudioApp::getFullHelpFileName(tstring str) const
{
	tstring strTemp = chkHelpExist(str);
	if (                       strTemp.size() < 3) return _T("");
	if (chkHelpExist ("assistant.exe").size() < 3) return _T("");

	return strTemp;
}

tstring RDOStudioApp::chkHelpExist(tstring fileName) const
{
	fileName.insert(0, rdo::extractFilePath(RDOStudioApp::getFullExtName()));
	if (!rdo::File::exist(fileName))
	{
		::MessageBox(NULL, rdo::format(ID_MSG_NO_HELP_FILE, fileName.c_str()).c_str(), NULL, MB_ICONEXCLAMATION | MB_OK);
		return _T("");
	}
	return fileName;
}

void RDOStudioApp::chkAndRunQtAssistant()
{
	if (!m_pAssistant)
	{
		m_pAssistant = runQtAssistant();
	}
	else if (m_pAssistant->state() == m_pAssistant->Running)
		return;
	else
		m_pAssistant = runQtAssistant();
}

PTR(QProcess) RDOStudioApp::runQtAssistant() const
{
	PTR(QProcess) pProcess = new QProcess;
	QStringList args;
	args << QLatin1String("-collectionFile")
		<< QLatin1String(getFullHelpFileName().c_str())
		<< QLatin1String("-enableRemoteControl")
		<< QLatin1String("-quiet");
	pProcess->start(QLatin1String("assistant"), args);
	return pProcess;
}

void RDOStudioApp::callQtAssistant(QByteArray ba)
{
	chkAndRunQtAssistant();
	if (m_pAssistant->state() != m_pAssistant->Running)
		return;

	m_pAssistant->write(ba);
}

rbool RDOStudioApp::getFileAssociationSetup() const
{
	return m_fileAssociationSetup;
}

void RDOStudioApp::setFileAssociationSetup(rbool value)
{
	m_fileAssociationSetup = value;
	WriteProfileInt(_T("fileAssociation"), _T("setup"), m_fileAssociationSetup);
	if (m_fileAssociationSetup)
	{
		setupFileAssociation();
	}
}

rbool RDOStudioApp::getFileAssociationCheckInFuture() const
{
	return m_fileAssociationCheckInFuture;
}

void RDOStudioApp::setFileAssociationCheckInFuture(rbool value)
{
	if (m_fileAssociationCheckInFuture != value)
	{
		m_fileAssociationCheckInFuture = value;
		WriteProfileInt(_T("fileAssociation"), _T("checkInFuture"), m_fileAssociationCheckInFuture);
	}
}

rbool RDOStudioApp::getOpenLastProject() const
{
	return m_openLastProject;
}

void RDOStudioApp::setOpenLastProject(rbool value)
{
	if (m_openLastProject != value)
	{
		m_openLastProject = value;
		WriteProfileInt(_T("general"), _T("openLastProject"), m_openLastProject);
	}
}

CREF(tstring) RDOStudioApp::getLastProjectName() const
{
	return m_lastProjectName;
}

void RDOStudioApp::setLastProjectName(CREF(tstring) projectName)
{
	insertReopenItem(projectName);
	if (m_lastProjectName != projectName)
	{
		m_lastProjectName = projectName;
		WriteProfileString(_T("general"), _T("lastProject"), getOpenLastProject() ? m_lastProjectName.c_str() : _T(""));
	}
}

rbool RDOStudioApp::getShowCaptionFullName() const
{
	return m_showCaptionFullName;
}

void RDOStudioApp::setShowCaptionFullName(rbool value)
{
	if (m_showCaptionFullName != value)
	{
		m_showCaptionFullName = value;
		model->setName(model->getName());
		WriteProfileInt(_T("general"), _T("showCaptionFullName"), m_showCaptionFullName);
	}
}

void RDOStudioApp::setupFileAssociation()
{
	tstring strFileTypeId    = _T("RAO.Project");
	tstring strFileTypeName  = _T("RAO Project");
	tstring strParam         = _T(" \"%1\"");
	tstring strPathName      = RDOStudioApp::getFullExtName();
	tstring strRAOExt        = _T(".rdox");

	tstring strFileTypeIdOld = _T("RAO.FileInfo");
	tstring strRAOExtOld     = _T(".smr");

	rbool win2000 = false;
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (::GetVersionEx(&osvi))
	{
		win2000 = osvi.dwMajorVersion >= 5;
	}

	HKEY hCurUserSoftClasses = 0;
	HKEY hKey;
	DWORD result;
	if (win2000)
	{
		if (::RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Classes"), 0, KEY_ALL_ACCESS, &hCurUserSoftClasses) != ERROR_SUCCESS)
		{
			hCurUserSoftClasses = 0;
		}
	}
	else
	{
		hCurUserSoftClasses = HKEY_CLASSES_ROOT;
	}
	if (hCurUserSoftClasses)
	{
		if (::RegCreateKeyEx(hCurUserSoftClasses, strFileTypeId.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &result) == ERROR_SUCCESS)
		{
			rbool mustBeRegistered = true;
			DWORD size;
			if (::RegQueryValueEx(hKey, _T(""), NULL, NULL, NULL, &size) == ERROR_SUCCESS)
			{
				if (size > 0)
				{
					HKEY hKeyPath;
					if (::RegOpenKeyEx(hKey, _T("shell\\open\\command"), 0, KEY_READ, &hKeyPath) == ERROR_SUCCESS)
					{
						CString s;
						DWORD length = MAX_PATH + strParam.length() * sizeof(TCHAR) + 1;
						::RegQueryValueEx(hKeyPath, _T(""), NULL, NULL, (LPBYTE)s.GetBuffer(length), &length);
						s.ReleaseBuffer();
						::RegCloseKey(hKeyPath);

						int pos = s.Find(strParam.c_str());
						if (pos != -1)
						{
							s.Delete(pos, strParam.length());
							if (s != strPathName.c_str())
							{
								if (!getFileAssociationSetup())
								{
									RDOFileAssociationDlg dlg;
									mustBeRegistered = dlg.DoModal() == IDOK;
									setFileAssociationCheckInFuture(dlg.m_checkInFuture ? true : false);
								}
							}
							else
							{
								mustBeRegistered = false;
							}
						}
					}
				}
			}
			if (mustBeRegistered)
			{
				DeleteRegistryKey(hCurUserSoftClasses, strFileTypeIdOld.c_str());
				DeleteRegistryKey(hCurUserSoftClasses, tstring(strFileTypeIdOld + _T("\\DefaultIcon")).c_str());
				DeleteRegistryKey(hCurUserSoftClasses, tstring(strFileTypeIdOld + _T("\\shell\\open\\command")).c_str());
				DeleteRegistryKey(hCurUserSoftClasses, tstring(strRAOExtOld + _T("\\ShellNew")).c_str());
				DeleteRegistryKey(hCurUserSoftClasses, strRAOExtOld.c_str());

				HKEY hKey_tmp;
				if (::RegCreateKeyEx(hCurUserSoftClasses, strFileTypeId.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result) == ERROR_SUCCESS)
				{
					tstring s = strFileTypeName;
					::RegSetValueEx(hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length());
					::RegCloseKey(hKey_tmp);
				}
				if (::RegCreateKeyEx(hCurUserSoftClasses, tstring(strFileTypeId + _T("\\DefaultIcon")).c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result) == ERROR_SUCCESS)
				{
					tstring s = strPathName + _T(",0");
					::RegSetValueEx(hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length());
					::RegCloseKey(hKey_tmp);
				}
				if (::RegCreateKeyEx(hCurUserSoftClasses, tstring(strFileTypeId + _T("\\shell\\open\\command")).c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result) == ERROR_SUCCESS)
				{
					tstring s = strPathName + strParam;
					::RegSetValueEx(hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length());
					::RegCloseKey(hKey_tmp);
				}
				if (::RegCreateKeyEx(hCurUserSoftClasses, strRAOExt.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result) == ERROR_SUCCESS)
				{
					tstring s = strFileTypeId;
					::RegSetValueEx(hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length());
					::RegCloseKey(hKey_tmp);
				}
				if (::RegCreateKeyEx(hCurUserSoftClasses, tstring(strRAOExt + _T("\\ShellNew")).c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result) == ERROR_SUCCESS)
				{
					tstring s = _T("");
					::RegSetValueEx(hKey_tmp, _T("NullFile"), 0, REG_SZ, (LPBYTE)s.c_str(), s.length());
					::RegCloseKey(hKey_tmp);
				}
			}
			::RegCloseKey(hKey);
		}
		if (hCurUserSoftClasses != HKEY_CLASSES_ROOT)
		{
			::RegCloseKey(hCurUserSoftClasses);
		}
	}
}

void RDOStudioApp::OnAppAbout()
{
	About dlg(m_pMainFrame);
	dlg.exec();
}

void RDOStudioApp::autoCloseByModel()
{
	if (m_autoExitByModel)
	{
		if (!m_dontCloseIfError || !model || (m_dontCloseIfError && (model->getExitCode() == rdo::simulation::report::EC_OK || model->getExitCode() == rdo::simulation::report::EC_NoMoreEvents)))
		{
			m_pMainFrame->close();
		}
	}
}

void RDOStudioApp::autoCloseByPlugin(PTR(RDOStudioPlugin) plugin)
{
	if (std::find(m_pluginExitNameList.begin(), m_pluginExitNameList.end(), plugin->getFileName()) != m_pluginExitNameList.end())
	{
		m_pMainFrame->close();
	}
}

rbool RDOStudioApp::isPluginAutoStart(PTR(RDOStudioPlugin) plugin) const
{
	return std::find(m_pluginStartNameList.begin(), m_pluginStartNameList.end(), plugin->getFileName()) != m_pluginStartNameList.end();
}

BOOL RDOStudioApp::PreTranslateMessage(PTR(MSG) pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		//PTR(CMDIChildWnd) pChild = m_pMainFrame->MDIGetActive();
		//if (pChild)
		//{
		//	PTR(CView) pView = pChild->GetActiveView();
		//	if (dynamic_cast<PTR(RDOStudioFrameView)>(pView) && pView == pChild->GetFocus())
		//	{
		//		pView->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		//		return true;
		//	}
		//}
	}
	if (pMsg->message == PLUGIN_MUSTEXIT_MESSAGE)
	{
		plugins->stopPluginByStudio(reinterpret_cast<HMODULE>(pMsg->wParam));
	}
	return CWinApp::PreTranslateMessage(pMsg);
}

BOOL RDOStudioApp::ProcessMessageFilter(int code, LPMSG lpMsg)
{
	return CWinApp::ProcessMessageFilter(code, lpMsg);
}

void RDOStudioApp::broadcastMessage(RDOThread::RDOTreadMessage message, PTR(void) pParam)
{
#ifdef RDO_MT
	PTR(CEvent) pEvent = m_pStudioMT->manualMessageFrom(message, pParam);
	while (::WaitForSingleObject(pEvent->m_hObject, 0) == WAIT_TIMEOUT) {
		static_cast<PTR(RDOThreadStudioGUI)>(m_pStudioGUI)->processMessages();
		if (m_pMainFrame) {
			m_pMainFrame->UpdateWindow();
		} else {
			break;
		}
	}
	delete pEvent;
#else
	m_pStudioGUI->broadcastMessage(message, pParam);
#endif
}

BOOL RDOStudioApp::OnIdle(LONG lCount)
{
#ifdef RDO_MT
	static_cast<PTR(RDOThreadStudioGUI)>(m_pStudioGUI)->processMessages();
	CWinApp::OnIdle(lCount);
	return true;
#else
	kernel->idle();
	if (lCount > 10000)
	{
		return CWinApp::OnIdle(lCount);
	}
	else
	{
		CWinApp::OnIdle(lCount);
		return true;
	}
#endif
}
