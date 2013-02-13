/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      application.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtCore/qprocess.h>
#include <QtCore/qtextcodec.h>
#include <QtCore/qsettings.h>
#include <QtWidgets/qmessagebox.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdofile.h"
#include "kernel/rdothread.h"
#include "repository/rdorepository.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_frm.h"
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/thread.h"
#include "app/rdo_studio/src/dialog/file_association_dialog.h"
#include "app/rdo_studio/rdo_tracer/rdotracer.h"
#include "thirdparty/qt-solutions/qtwinmigrate/src/qmfcapp.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -----------------------------------------------------------------
// -------------------- RDOStudioCommandLineInfo
// -----------------------------------------------------------------
class RDOStudioCommandLineInfo: public CCommandLineInfo
{
public:
	RDOStudioCommandLineInfo()
		: CCommandLineInfo()
	{}

protected:
	virtual void ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast);
};

void RDOStudioCommandLineInfo::ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
{
	CCommandLineInfo::ParseParam(lpszParam, bFlag, bLast);
	if (bFlag)
	{
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
	}
	else
	{
		if (studioApp.m_openModelName.empty())
		{
			studioApp.m_openModelName = lpszParam;
		}
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioApp
// --------------------------------------------------------------------------------
RDOStudioApp studioApp;

#ifdef _DEBUG
void g_messageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
	if (msg.contains("requested for null window or window without handle"))
		return;

	if (msg.contains("QBackingStore::flush() called with non-exposed window, behavior is undefined"))
		return;

	if (msg.contains("Cannot create accessible interface for object"))
		return;

	qInstallMessageHandler(NULL);

	QString message = QString("%1\n\nfile: %2: %3\nat function: %4")
		.arg(msg)
		.arg(context.file)
		.arg(context.line)
		.arg(context.function);

	switch (type)
	{
	case QtDebugMsg:
		TRACE1("Debug: %s\n", msg.toLocal8Bit().constData());
		break;

	case QtWarningMsg:
		TRACE1("Warning: %s\n", message.toLocal8Bit().constData());
		QMessageBox::warning(studioApp.getMainWnd(), "QtWarning", message);
		break;

	case QtCriticalMsg:
		QMessageBox::critical(studioApp.getMainWnd(), "QtCritical", message);
		break;

	case QtFatalMsg:
		QMessageBox::critical(studioApp.getMainWnd(), "QtFatal", message);
		break;
	}

	qInstallMessageHandler(g_messageOutput);
}
#endif

RDOStudioApp::RDOStudioApp()
	: CWinApp()
	, m_pStudioGUI                  (NULL  )
#ifdef RDO_MT
	, m_pStudioMT                   (NULL  )
#endif
	, m_fileAssociationSetup        (false )
	, m_fileAssociationCheckInFuture(false )
	, m_openLastProject             (false )
	, m_showCaptionFullName         (false )
	, m_autoRun                     (false )
	, m_autoExitByModel             (false )
	, m_dontCloseIfError            (false )
	, m_exitCode                    (rdo::simulation::report::EC_OK)
	, m_openModelName               (_T(""))
	, m_pMainFrame                  (NULL  )
{
#ifdef _DEBUG
	qInstallMessageHandler(g_messageOutput);
#endif

	setlocale(LC_ALL,     _T("rus"));
	setlocale(LC_NUMERIC, _T("eng"));

	m_log.open(_T("log.txt"));
}

RDOStudioApp::~RDOStudioApp()
{}

BOOL RDOStudioApp::Run()
{
	int result = QMfcApp::run(this);
	delete qApp;
	return result;
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

	QMfcApp::instance(this);
	qApp->setQuitOnLastWindowClosed(true);

	QApplication::setApplicationName("RAO-studio");
	QApplication::setOrganizationName("RAO-studio");

#ifdef Q_OS_WIN
	convertSettings();
#endif

	QSettings settings;

	m_fileAssociationSetup         = settings.value("general/file_association_setup", true).toBool();
	m_fileAssociationCheckInFuture = settings.value("general/file_association_check_in_future", true).toBool();

	m_lastProjectName     = settings.value("general/last_project_full_name", QString()).toString().toLocal8Bit().constData();
	m_openLastProject     = settings.value("general/last_project_auto_open", true).toBool();
	m_showCaptionFullName = settings.value("general/show_caption_full_name", false).toBool();

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

	g_pTracer = new rdo::gui::tracer::Tracer();

	m_pEditorEditStyle = rdo::Factory<rdoEditor::RDOEditorEditStyle>::create();

	// Внутри создается объект модели
	m_pMainFrame = new RDOStudioMainFrame();
	m_pMainFrame->init();
	m_pMainWnd = m_pMainFrame->c_wnd();
	m_pMainFrame->show();

#ifdef RDO_MT
	kernel->thread_studio = m_pStudioGUI;
#else
	kernel->thread_studio = model;
#endif

	if (getFileAssociationCheckInFuture())
	{
		setupFileAssociation();
	}

	RDOStudioCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

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
			if (model->openModel(QString::fromLocal8Bit(m_openModelName.c_str())))
			{
				autoModel = true;
			}
		}
		else
		{
			m_openModelName = rdo::extractFilePath(RDOStudioApp::getFullExtName()) + m_openModelName;
			if (rdo::File::exist(m_openModelName) && model->openModel(QString::fromLocal8Bit(m_openModelName.c_str())))
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
		if (getOpenLastProject() && !getLastProjectName().isEmpty() && QFile::exists(getLastProjectName()))
		{
			model->openModel(getLastProjectName().toLocal8Bit().constData());
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

	if (m_autoRun)
	{
		model->runModel();
	}

	return TRUE;
}

int RDOStudioApp::ExitInstance()
{
	m_pMainFrame = NULL;

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

	// Роняем кернел и закрываем все треды
	RDOKernel::close();

	if (m_autoExitByModel)
	{
		CWinApp::ExitInstance();
		return m_exitCode;
	}
	else
	{
		return CWinApp::ExitInstance();
	}
}

PTR(RDOStudioMainFrame) RDOStudioApp::getMainWndUI()
{
	return m_pMainFrame;
}

PTR(QMainWindow) RDOStudioApp::getMainWnd()
{
	return m_pMainFrame;
}

PTR(MainWindowBase) RDOStudioApp::getStyle()
{
	return m_pMainFrame;
}

PTR(MainWindowBase) RDOStudioApp::getIMainWnd()
{
	return m_pMainFrame;
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

tstring RDOStudioApp::getFullExtName()
{
	return qApp->applicationFilePath().toLocal8Bit().constData();
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
		QMessageBox::warning(studioApp.getMainWnd(), "RAO-Studio", QString::fromStdWString(L"Невозможно найти файл справки '%1'.\r\nОн должен быть расположен в директории с RAO-studio.").arg(QString::fromLocal8Bit(fileName.c_str())));
		return tstring();
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
	args << QString("-collectionFile")
		<< QString(getFullHelpFileName().c_str())
		<< QString("-enableRemoteControl")
		<< QString("-quiet");
	pProcess->start(QString("assistant"), args);
	return pProcess;
}

void RDOStudioApp::callQtAssistant(CREF(QByteArray) ba)
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
	QSettings settings;
	settings.setValue("general/file_association_setup", m_fileAssociationSetup);
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
		QSettings settings;
		settings.setValue("general/file_association_check_in_future", m_fileAssociationCheckInFuture);
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
		QSettings settings;
		settings.setValue("general/last_project_auto_open", m_openLastProject);
	}
}

CREF(QString) RDOStudioApp::getLastProjectName() const
{
	return m_lastProjectName;
}

void RDOStudioApp::setLastProjectName(CREF(QString) projectName)
{
	m_pMainFrame->insertMenuFileReopenItem(projectName);
	if (m_lastProjectName != projectName)
	{
		m_lastProjectName = projectName;
		QSettings settings;
		settings.setValue("general/last_project_full_name", getOpenLastProject()
			? m_lastProjectName
			: QString()
		);
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
		QSettings settings;
		settings.setValue("general/show_caption_full_name", m_showCaptionFullName);
	}
}

void RDOStudioApp::setupFileAssociation()
{
#ifdef Q_OS_WIN
	QString strFileTypeId("RAO.Project");
	QString strParam(" \"%1\"");
	QString strPathName(QString::fromLocal8Bit(RDOStudioApp::getFullExtName().c_str()));
	strPathName.replace("/", "\\");

	QSettings settings("HKEY_CURRENT_USER\\Software\\Classes", QSettings::NativeFormat);

	rbool mustBeRegistered = true;
	if (settings.childGroups().contains(strFileTypeId))
	{
		QString openCommand = settings.value(strFileTypeId + "/shell/open/command/Default").toString();
		int pos = openCommand.indexOf(strParam);
		if (pos != -1)
		{
			openCommand.remove(pos, strParam.length());
			if (openCommand != strPathName)
			{
				FileAssociationDialog dlg(studioApp.getMainWndUI());
				mustBeRegistered = dlg.exec() == QDialog::Accepted;
				setFileAssociationCheckInFuture(dlg.checkBox->isChecked());
			}
			else
			{
				mustBeRegistered = false;
			}
		}
	}

	if (mustBeRegistered)
	{
		{
			QString strFileTypeIdOld("RAO.FileInfo");
			QString strRAOExtOld(".smr");
			settings.remove(strFileTypeIdOld);
			settings.remove(strRAOExtOld);
		}

		settings.setValue(strFileTypeId + "/Default", "RAO Project");
		settings.setValue(strFileTypeId + "/DefaultIcon/Default", strPathName + ",0");
		settings.setValue(strFileTypeId + "/shell/open/command/Default", strPathName + strParam);

		QString strRAOExt(".rdox");
		settings.setValue(strRAOExt + "/Default", strFileTypeId);
		settings.setValue(strRAOExt + "/ShellNew/NullFile", "");
	}
#endif
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

BOOL RDOStudioApp::PreTranslateMessage(PTR(MSG) pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		//! @todo qt
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

CREF(rdoEditor::LPRDOEditorEditStyle) RDOStudioApp::getEditorEditStyle() const
{
	ASSERT(m_pEditorEditStyle);
	return m_pEditorEditStyle;
}

#ifdef Q_OS_WIN
void RDOStudioApp::convertSettings() const
{
	class ColorConvertor
	{
	public:
		static QColor convert(QColor color)
		{
			return QColor(color.blue(), color.green(), color.red());
		}
	};

	QSettings settingsTo;
	QSettings settingsFrom("HKEY_CURRENT_USER\\Software\\RAO-studio", QSettings::NativeFormat);
	QStringList childGroupsFrom = settingsFrom.childGroups();

	if (childGroupsFrom.contains("fileAssociation"))
	{
		settingsTo.setValue("general/file_association_setup",           settingsFrom.value("fileAssociation/setup",         true).toBool());
		settingsTo.setValue("general/file_association_check_in_future", settingsFrom.value("fileAssociation/checkInFuture", true).toBool());
		settingsFrom.remove("fileAssociation");
	}

	if (childGroupsFrom.contains("general"))
	{
		settingsTo.setValue("general/last_project_full_name", settingsFrom.value("general/lastProject", QString()).toString());
		settingsTo.setValue("general/last_project_auto_open", settingsFrom.value("general/openLastProject",     true).toBool());
		settingsTo.setValue("general/show_caption_full_name", settingsFrom.value("general/showCaptionFullName", false).toBool());
		settingsFrom.remove("general");
	}

	if (childGroupsFrom.contains("plugins"))
	{
		settingsFrom.remove("plugins");
	}

	if (childGroupsFrom.contains("reopen"))
	{
		for (ruint i = 0; i < 10; i++)
		{
			QString value = settingsFrom.value(QString("reopen/%1%2").arg(i+1 < 10 ? "0" : "").arg(i+1), QString()).toString();
			if (value.isEmpty())
				break;

			settingsTo.setValue(QString("reopen/%1").arg(i+1), value);
		}
		settingsFrom.remove("reopen");
	}
	if(childGroupsFrom.contains("style"))
	{
		settingsTo.setValue("style/build/build/warning", settingsFrom.value("style/build/build/warning", bool()).toBool());
		
		settingsTo.setValue("style/build/font/character_set", settingsFrom.value("style/build/font/characterSet", int()).toInt());
		settingsTo.setValue("style/build/font/codepage", settingsFrom.value("style/build/font/codepage", int()).toInt());
		settingsTo.setValue("style/build/font/name", settingsFrom.value("style/build/font/name", QString()).toString());
		settingsTo.setValue("style/build/font/size", settingsFrom.value("style/build/font/size", int()).toInt());

		settingsTo.setValue("style/build/tab/auto_indent", settingsFrom.value("style/build/tab/autoIndent", bool()).toBool());
		settingsTo.setValue("style/build/tab/backspace_untabs", settingsFrom.value("style/build/tab/backspaceUntabs", bool()).toBool());
		settingsTo.setValue("style/build/tab/indent_size", settingsFrom.value("style/build/tab/indentSize", int()).toInt());
		settingsTo.setValue("style/build/tab/tab_indents", settingsFrom.value("style/build/tab/tabIndents", bool()).toBool());
		settingsTo.setValue("style/build/tab/tab_size", settingsFrom.value("style/build/tab/tabSize", int()).toInt());
		settingsTo.setValue("style/build/tab/use_tabs", settingsFrom.value("style/build/tab/useTabs", bool()).toBool());

		settingsTo.setValue("style/build/theme/background_color", ColorConvertor::convert(settingsFrom.value("style/build/tab/backgroundColor", __int32()).value<QColor>()));
		settingsTo.setValue("style/build/theme/bookmark_bg_color", ColorConvertor::convert(settingsFrom.value("style/build/tab/bookmarkBgColor", __int32()).value<QColor>()));
		settingsTo.setValue("style/build/theme/bookmark_fg_color", ColorConvertor::convert(settingsFrom.value("style/build/tab/bookmarkFgColor", __int32()).value<QColor>()));

	}
	
}
#endif
