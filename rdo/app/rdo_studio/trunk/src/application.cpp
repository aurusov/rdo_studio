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
class Convertor
{
public:
	Convertor()
		: settingsTo  ()
		, settingsFrom("HKEY_CURRENT_USER\\Software\\RAO-studio", QSettings::NativeFormat)
	{
		childGroupsFrom = settingsFrom.childGroups();
	}

	rbool contains(const QString& name) const
	{
		return childGroupsFrom.contains(name);
	}

	template <class T>
	void convert(const QString& from, const QString& to)
	{
		if (!settingsFrom.contains(from))
			return;

		setValue(to, value<T>(from));
	}

	template <class T>
	T value(const QString& from)
	{
		return convert<T>(settingsFrom.value(from, qt_to_mfc_type_convertor<T>::type())).value<T>();
	}

	template <class T>
	void setValue(const QString& to, const T& value)
	{
		settingsTo.setValue(to, value);
	}

	void remove(const QString& name)
	{
		return settingsFrom.remove(name);
	}

private:
	QSettings   settingsTo;
	QSettings   settingsFrom;
	QStringList childGroupsFrom;

	template <class T>
	struct qt_to_mfc_type_convertor
	{
		typedef  T  type;
	};

	template <>
	struct qt_to_mfc_type_convertor<QColor>
	{
		typedef  __int32  type;
	};

	template <class T>
	static QVariant convert(const QVariant& value)
	{
		return value;
	}

	template <>
	static QVariant convert<QColor>(const QVariant& value)
	{
		__int32 colorRef = value.value<__int32>();
		return QColor(GetRValue(colorRef), GetGValue(colorRef), GetBValue(colorRef));
	}
};

void RDOStudioApp::convertSettings() const
{
	Convertor convertor;

	if (convertor.contains("fileAssociation"))
	{
		convertor.convert<rbool>("fileAssociation/setup",         "general/file_association_setup");
		convertor.convert<rbool>("fileAssociation/checkInFuture", "general/file_association_check_in_future");
		convertor.remove("fileAssociation");
	}

	if (convertor.contains("general"))
	{
		convertor.convert<QString>("general/lastProject",         "general/last_project_full_name");
		convertor.convert<rbool>  ("general/openLastProject",     "general/last_project_auto_open");
		convertor.convert<rbool>  ("general/showCaptionFullName", "general/show_caption_full_name");
		convertor.remove("general");
	}

	if (convertor.contains("plugins"))
	{
		convertor.remove("plugins");
	}

	if (convertor.contains("reopen"))
	{
		for (ruint i = 0; i < 10; i++)
		{
			QString value = convertor.value<QString>(QString("reopen/%1%2").arg(i+1 < 10 ? "0" : "").arg(i+1));
			if (value.isEmpty())
				break;

			convertor.setValue(QString("reopen/%1").arg(i+1), value);
		}
		convertor.remove("reopen");
	}

	if (convertor.contains("style"))
	{
		convertor.convert<rbool>("style/build/build/warning", "style/build/build/warning");

		convertor.convert<int>    ("style/build/font/characterSet", "style/build/font/character_set");
		convertor.convert<int>    ("style/build/font/codepage",     "style/build/font/codepage");
		convertor.convert<QString>("style/build/font/name",         "style/build/font/name");
		convertor.convert<int>    ("style/build/font/size",         "style/build/font/size");

		convertor.convert<bool>("style/build/tab/autoIndent",      "style/build/tab/auto_indent");
		convertor.convert<bool>("style/build/tab/backspaceUntabs", "style/build/tab/backspace_untabs");
		convertor.convert<int >("style/build/tab/indentSize",      "style/build/tab/indent_size");
		convertor.convert<bool>("style/build/tab/tabIndents",      "style/build/tab/tab_indents");
		convertor.convert<int >("style/build/tab/tabSize",         "style/build/tab/tab_size");
		convertor.convert<bool>("style/build/tab/useTabs",         "style/build/tab/use_tabs");

		convertor.convert<QColor>("style/build/theme/backgroundColor", "style/build/theme/background_color");

		//convertor.convert("style/build/theme/bookmark_bg_color",    ColorConvertor::convert(settingsFrom.value("style/build/theme/bookmarkBgColor", __int32()).value<__int32>()));
		//convertor.convert("style/build/theme/bookmark_fg_color",    ColorConvertor::convert(settingsFrom.value("style/build/theme/bookmarkFgColor", __int32()).value<__int32>()));
		//convertor.convert("style/build/theme/bookmark_style",       settingsFrom.value("style/build/theme/bookmarkStyle", int()).toInt());
		//convertor.convert("style/build/theme/caret_color",          ColorConvertor::convert(settingsFrom.value("style/build/theme/caretColor", __int32()).value<__int32>()));
		//convertor.convert("style/build/theme/default_color",        ColorConvertor::convert(settingsFrom.value("style/build/theme/defaultColor", __int32()).value<__int32>()));
		//convertor.convert("style/build/theme/default_style",        settingsFrom.value("style/build/theme/defaultStyle", int()).toInt());
		//convertor.convert("style/build/theme/selection_bg_color",   ColorConvertor::convert(settingsFrom.value("style/build/theme/selectionBgColor", __int32()).value<__int32>()));
		//convertor.convert("style/build/theme/select_line_bg_color", ColorConvertor::convert(settingsFrom.value("style/build/theme/selectLineBgColor", __int32()).value<__int32>()));
		//convertor.convert("style/build/window/show_horz_scroll_bar", settingsFrom.value("style/build/window/showHorzScrollBar", bool()).toBool());
		//convertor.convert("style/build/window/word_wrap",            settingsFrom.value("style/build/window/wordWrap", bool()).toBool());

		//convertor.convert("style/chart/font/character_set", settingsFrom.value("style/chart/font/characterSet", int()).toInt());
		//convertor.convert("style/chart/font/codepage",      settingsFrom.value("style/chart/font/codepage", int()).toInt());
		//convertor.convert("style/chart/font/name",          settingsFrom.value("style/chart/font/name", QString()).toString());
		//convertor.convert("style/chart/font/size",          settingsFrom.value("style/chart/font/size", int()).toInt());

		//convertor.convert("style/chart/fonts_ticks/legend_font_size", settingsFrom.value("style/chart/fonts_ticks/legendFontSize", int()).toInt());
		//convertor.convert("style/chart/fonts_ticks/tick_width",       settingsFrom.value("style/chart/fonts_ticks/tickWidth", int()).toInt());
		//convertor.convert("style/chart/fonts_ticks/title_font_size",  settingsFrom.value("style/chart/fonts_ticks/titleFontSize", int()).toInt());

		//convertor.convert("style/chart/theme/axis_fg_color",    ColorConvertor::convert(settingsFrom.value("style/chart/theme/axisFgColor", __int32()).value<__int32>()));
		//convertor.convert("style/chart/theme/background_color", ColorConvertor::convert(settingsFrom.value("style/chart/theme/backgroundColor", __int32()).value<__int32>()));
		//convertor.convert("style/chart/theme/chart_bg_color",   ColorConvertor::convert(settingsFrom.value("style/chart/theme/chartBgColor", __int32()).value<__int32>()));
		//convertor.convert("style/chart/theme/default_color",    ColorConvertor::convert(settingsFrom.value("style/chart/theme/defaultColor", __int32()).value<__int32>()));
		//convertor.convert("style/chart/theme/default_style",    settingsFrom.value("style/chart/theme/defaultStyle", int()).toInt());
		//convertor.convert("style/chart/theme/legend_fg_color",  ColorConvertor::convert(settingsFrom.value("style/chart/theme/legendFgColor", __int32()).value<__int32>()));
		//convertor.convert("style/chart/theme/legend_style",     settingsFrom.value("style/chart/theme/legendStyle", int()).toInt());
		//convertor.convert("style/chart/theme/time_bg_color",    ColorConvertor::convert(settingsFrom.value("style/chart/theme/timeBgColor", __int32()).value<__int32>()));
		//convertor.convert("style/chart/theme/title_fg_color",   ColorConvertor::convert(settingsFrom.value("style/chart/theme/titleFGColor", __int32()).value<__int32>()));
		//convertor.convert("style/chart/theme/title_style",     settingsFrom.value("style/chart/theme/titleStyle", int()).toInt());

		//convertor.convert("style/debug/font/character_set", settingsFrom.value("style/debug/font/characterSet", int()).toInt());
		//convertor.convert("style/debug/font/codepage",      settingsFrom.value("style/debug/font/codepage", int()).toInt());
		//convertor.convert("style/debug/font/name",          settingsFrom.value("style/debug/font/name", QString()).toString());
		//convertor.convert("style/debug/font/size",          settingsFrom.value("style/debug/font/size", int()).toInt());

		//convertor.convert("style/debug/tab/auto_indent",      settingsFrom.value("style/debug/tab/autoIndent", bool()).toBool());
		//convertor.convert("style/debug/tab/backspace_untabs", settingsFrom.value("style/debug/tab/backspaceUntabs", bool()).toBool());
		//convertor.convert("style/debug/tab/indent_size",      settingsFrom.value("style/debug/tab/indentSize", int()).toInt());
		//convertor.convert("style/debug/tab/tab_indents",      settingsFrom.value("style/debug/tab/tabIndents", bool()).toBool());
		//convertor.convert("style/debug/tab/tab_size",         settingsFrom.value("style/debug/tab/tabSize", int()).toInt());
		//convertor.convert("style/debug/tab/use_tabs",         settingsFrom.value("style/debug/tab/useTabs", bool()).toBool());

		//convertor.convert("style/debug/theme/background_color",     ColorConvertor::convert(settingsFrom.value("style/debug/theme/backgroundColor", __int32()).value<__int32>()));
		//convertor.convert("style/debug/theme/bookmark_bg_color",    ColorConvertor::convert(settingsFrom.value("style/debug/theme/bookmarkBgColor", __int32()).value<__int32>()));
		//convertor.convert("style/debug/theme/bookmark_fg_color",    ColorConvertor::convert(settingsFrom.value("style/debug/theme/bookmarkFgColor", __int32()).value<__int32>()));
		//convertor.convert("style/debug/theme/bookmark_style",       settingsFrom.value("style/debug/theme/bookmarkStyle", int()).toInt());
		//convertor.convert("style/debug/theme/caret_color",          ColorConvertor::convert(settingsFrom.value("style/debug/theme/caretColor", __int32()).value<__int32>()));
		//convertor.convert("style/debug/theme/default_color",        ColorConvertor::convert(settingsFrom.value("style/debug/theme/defaultColor", __int32()).value<__int32>()));
		//convertor.convert("style/debug/theme/default_style",        settingsFrom.value("style/debug/theme/defaultStyle", int()).toInt());
		//convertor.convert("style/debug/theme/selection_bg_color",   ColorConvertor::convert(settingsFrom.value("style/debug/theme/selectionBgColor", __int32()).value<__int32>()));

		//convertor.convert("style/debug/window/show_horz_scroll_bar", settingsFrom.value("style/debug/window/showHorzScrollBar", bool()).toBool());
		//convertor.convert("style/debug/window/word_wrap",            settingsFrom.value("style/debug/window/wordWrap", bool()).toBool());

		//convertor.convert("style/editor/autoComplete/show_full_list",    settingsFrom.value("style/editor/autoComplete/showFullList", bool()).toBool());
		//convertor.convert("style/editor/autoComplete/use_auto_complete", settingsFrom.value("style/editor/autoComplete/useAutoComplete", bool()).toBool());

		//settingsFrom.remove("style/editor/buffer");

		//convertor.convert("style/editor/font/character_set", settingsFrom.value("style/editor/font/characterSet", int()).toInt());
		//convertor.convert("style/editor/font/codepage",      settingsFrom.value("style/editor/font/codepage", int()).toInt());
		//convertor.convert("style/editor/font/name",          settingsFrom.value("style/editor/font/name", QString()).toString());
		//convertor.convert("style/editor/font/size",          settingsFrom.value("style/editor/font/size", int()).toInt());

		//convertor.convert("style/editor/margin/bookmark",    settingsFrom.value("style/editor/margin/bookmark", bool()).toBool());
		//convertor.convert("style/editor/margin/fold",        settingsFrom.value("style/editor/margin/fold", bool()).toBool());
		//convertor.convert("style/editor/margin/line_number", settingsFrom.value("style/editor/margin/lineNumber", bool()).toBool());

		//convertor.convert("style/editor/tab/auto_indent",      settingsFrom.value("style/editor/tab/autoIndent", bool()).toBool());
		//convertor.convert("style/editor/tab/backspace_untabs", settingsFrom.value("style/editor/tab/backspaceUntabs", bool()).toBool());
		//convertor.convert("style/editor/tab/indent_size",      settingsFrom.value("style/editor/tab/indentSize", int()).toInt());
		//convertor.convert("style/editor/tab/tab_indents",      settingsFrom.value("style/editor/tab/tabIndents", bool()).toBool());
		//convertor.convert("style/editor/tab/tab_size",         settingsFrom.value("style/editor/tab/tabSize", int()).toInt());
		//convertor.convert("style/editor/tab/use_tabs",         settingsFrom.value("style/editor/tab/useTabs", bool()).toBool());

		//convertor.convert("style/editor/theme/background_color",   ColorConvertor::convert(settingsFrom.value("style/editor/theme/backgroundColor", __int32()).value<__int32>()));
		//convertor.convert("style/editor/theme/bookmark_bg_color",  ColorConvertor::convert(settingsFrom.value("style/editor/theme/bookmarkBgColor", __int32()).value<__int32>()));
		//convertor.convert("style/editor/theme/bookmark_fg_color",  ColorConvertor::convert(settingsFrom.value("style/editor/theme/bookmarkFgColor", __int32()).value<__int32>()));
		//convertor.convert("style/editor/theme/bookmark_style",     settingsFrom.value("style/editor/theme/bookmarkStyle", int()).toInt());
		//convertor.convert("style/editor/theme/caret_color",        ColorConvertor::convert(settingsFrom.value("style/editor/theme/caretColor", __int32()).value<__int32>()));
		//convertor.convert("style/editor/theme/color_color",        ColorConvertor::convert(settingsFrom.value("style/editor/theme/colorColor", __int32()).value<__int32>()));
		//convertor.convert("style/editor/theme/comment_color",      ColorConvertor::convert(settingsFrom.value("style/editor/theme/commentColor", __int32()).value<__int32>()));
		//convertor.convert("style/editor/theme/comment_fold",       settingsFrom.value("style/editor/theme/commentFold", bool()).toBool());
		//convertor.convert("style/editor/theme/comment_style",      settingsFrom.value("style/editor/theme/commentStyle", int()).toInt());
		//convertor.convert("style/editor/theme/default_color",      ColorConvertor::convert(settingsFrom.value("style/editor/theme/defaultColor", __int32()).value<__int32>()));
		//convertor.convert("style/editor/theme/default_style",      settingsFrom.value("style/editor/theme/defaultStyle", int()).toInt());
		//convertor.convert("style/editor/theme/error_bg_color",     ColorConvertor::convert(settingsFrom.value("style/editor/theme/errorBgColor", __int32()).value<__int32>()));
		//convertor.convert("style/editor/theme/fold_bg_color",      ColorConvertor::convert(settingsFrom.value("style/editor/theme/foldBgColor", __int32()).value<__int32>()));
		//convertor.convert("style/editor/theme/fold_fg_color",      ColorConvertor::convert(settingsFrom.value("style/editor/theme/foldFgColor", __int32()).value<__int32>()));
		//convertor.convert("style/editor/theme/fold_style",         settingsFrom.value("style/editor/theme/foldStyle", int()).toInt());
		//convertor.convert("style/editor/theme/functions_color",    ColorConvertor::convert(settingsFrom.value("style/editor/theme/functionsColor", __int32()).value<__int32>()));
		//convertor.convert("style/editor/theme/functions_style",    settingsFrom.value("style/editor/theme/functionsStyle", int()).toInt());
		//convertor.convert("style/editor/theme/identifier_color",   ColorConvertor::convert(settingsFrom.value("style/editor/theme/identifierColor", __int32()).value<__int32>()));
		//convertor.convert("style/editor/theme/identifier_style",   settingsFrom.value("style/editor/theme/identifierStyle", int()).toInt());
		//convertor.convert("style/editor/theme/keyword_color",      ColorConvertor::convert(settingsFrom.value("style/editor/theme/keywordColor", __int32()).value<__int32>()));
		//convertor.convert("style/editor/theme/keyword_style",      settingsFrom.value("style/editor/theme/keywordStyle", int()).toInt());
		//convertor.convert("style/editor/theme/number_color",       ColorConvertor::convert(settingsFrom.value("style/editor/theme/numberColor", __int32()).value<__int32>()));
		//convertor.convert("style/editor/theme/number_style",       settingsFrom.value("style/editor/theme/numberStyle", int()).toInt());
		//convertor.convert("style/editor/theme/operator_color",     ColorConvertor::convert(settingsFrom.value("style/editor/theme/operatorColor", __int32()).value<__int32>()));
		//convertor.convert("style/editor/theme/operator_style",     settingsFrom.value("style/editor/theme/operatorStyle", int()).toInt());
		//convertor.convert("style/editor/theme/selection_bg_color", ColorConvertor::convert(settingsFrom.value("style/editor/theme/selectionBgColor", __int32()).value<__int32>()));
		//convertor.convert("style/editor/theme/string_color",       ColorConvertor::convert(settingsFrom.value("style/editor/theme/stringColor", __int32()).value<__int32>()));
		//convertor.convert("style/editor/theme/string_style",       settingsFrom.value("style/editor/theme/stringStyle", int()).toInt());
		//convertor.convert("style/editor/theme/trace_color",        ColorConvertor::convert(settingsFrom.value("style/editor/theme/traceColor", __int32()).value<__int32>()));

		//convertor.convert("style/editor/window/show_horz_scroll_bar", settingsFrom.value("style/editor/window/showHorzScrollBar", bool()).toBool());
		//convertor.convert("style/editor/window/word_wrap",            settingsFrom.value("style/editor/window/wordWrap", bool()).toBool());
	}
}
#endif
