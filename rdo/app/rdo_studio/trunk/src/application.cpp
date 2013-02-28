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
#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(disable: 4100)
#endif

#include <boost/program_options.hpp>

#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(default: 4100)
#endif

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
#include "app/rdo_studio/src/tracer/tracer.h"
#include "thirdparty/qt-solutions/qtwinmigrate/src/qmfcapp.h"
// --------------------------------------------------------------------------------

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
	, m_autoExitByModel             (false )
	, m_dontCloseIfError            (false )
	, m_exitCode                    (rdo::simulation::report::EC_OK)
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

	m_lastProjectName     = settings.value("general/last_project_full_name", QString()).toString();
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
	kernel->thread_studio = g_pModel;
#endif

	if (getFileAssociationCheckInFuture())
	{
		setupFileAssociation();
	}

	namespace po = boost::program_options;

	po::options_description desc("RAO-studio");
	desc.add_options()
		("input,i", po::value<tstring>(), "model file name")
		("autorun", "auto run model")
		("autoexit", "auto exit after simulation stoped")
		("dont_close_if_error", "don't close application if model error detected")
	;

	std::vector<tstring> args = po::split_winmain(m_lpCmdLine);
	po::variables_map vm;
	try
	{
		po::store(po::command_line_parser(args).options(desc).run(), vm);
		po::notify(vm);
	}
	catch (const std::exception&)
	{}

	tstring openModelName;
	if (vm.count("input"))
	{
		openModelName = vm["input"].as<tstring>();
	}

	rbool autoRun = false;
	if (vm.count("autorun"))
	{
		autoRun = true;
	}

	if (vm.count("autoexit"))
	{
		m_autoExitByModel = true;
	}

	if (vm.count("dont_close_if_error"))
	{
		m_dontCloseIfError = true;
	}

	rbool newModel  = true;
	rbool autoModel = false;
	if (!openModelName.empty())
	{
		openModelName = rdo::File::extractFilePath(qApp->applicationFilePath().toLocal8Bit().constData()) + openModelName;
		if (rdo::File::exist(openModelName) && g_pModel->openModel(QString::fromLocal8Bit(openModelName.c_str())))
		{
			autoRun            = true;
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
	else
	{
		if (getOpenLastProject() && !getLastProjectName().isEmpty() && QFile::exists(getLastProjectName()))
		{
			g_pModel->openModel(getLastProjectName());
			newModel = false;
		}
	}

	if (autoModel)
	{
		newModel = false;
	}
	else
	{
		autoRun            = false;
		m_autoExitByModel  = false;
		m_dontCloseIfError = false;
	}

	if (autoRun)
	{
		g_pModel->runModel();
	}

	return TRUE;
}

int RDOStudioApp::ExitInstance()
{
	m_pMainFrame = NULL;

	if (m_exitCode != rdo::simulation::report::EC_ModelNotFound)
	{
		m_exitCode = g_pModel->getExitCode();
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

QString RDOStudioApp::getFullHelpFileName(CREF(QString) helpFileName) const
{
	QString result = chkHelpExist(helpFileName);
	if (result.size() < 3)
	{
		result = QString();
	}
	if (chkHelpExist("assistant.exe").size() < 3)
	{
		result = QString();
	}
	return result;
}

QString RDOStudioApp::chkHelpExist(CREF(QString) helpFileName) const
{
	QString fullHelpFileName = QString("%1%2")
		.arg(QString::fromLocal8Bit(rdo::File::extractFilePath(qApp->applicationFilePath().toLocal8Bit().constData()).c_str()))
		.arg(helpFileName);

	if (!QFile::exists(fullHelpFileName))
	{
		QMessageBox::warning(studioApp.getMainWnd(), "RAO-Studio", QString::fromStdWString(L"Невозможно найти файл справки '%1'.\r\nОн должен быть расположен в директории с RAO-studio.").arg(helpFileName));
		fullHelpFileName = QString();
	}

	return fullHelpFileName;
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
		<< getFullHelpFileName()
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
		g_pModel->setName(g_pModel->getName());
		QSettings settings;
		settings.setValue("general/show_caption_full_name", m_showCaptionFullName);
	}
}

void RDOStudioApp::setupFileAssociation()
{
#ifdef Q_OS_WIN
	QString fileTypeID("RAO.Project");
	QString appParam(" -i \"%1\"");
	QString appFullName = qApp->applicationFilePath();
	appFullName.replace("/", "\\");

	QSettings settings("HKEY_CURRENT_USER\\Software\\Classes", QSettings::NativeFormat);

	rbool mustBeRegistered = true;
	if (settings.childGroups().contains(fileTypeID))
	{
		QString openCommand = settings.value(fileTypeID + "/shell/open/command/Default").toString();
		int pos = openCommand.indexOf(appParam);
		if (pos != -1)
		{
			openCommand.remove(pos, appParam.length());
			if (openCommand != appFullName)
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
			QString fileTypeIDPrev("RAO.FileInfo");
			QString appExtPrev(".smr");
			settings.remove(fileTypeIDPrev);
			settings.remove(appExtPrev);
		}

		settings.setValue(fileTypeID + "/Default", "RAO Project");
		settings.setValue(fileTypeID + "/DefaultIcon/Default", appFullName + ",0");
		settings.setValue(fileTypeID + "/shell/open/command/Default", appFullName + appParam);

		QString appExt(".rdox");
		settings.setValue(appExt + "/Default", fileTypeID);
		settings.setValue(appExt + "/ShellNew/NullFile", "");
	}
#endif
}

void RDOStudioApp::autoCloseByModel()
{
	if (m_autoExitByModel)
	{
		if (!m_dontCloseIfError || !g_pModel || (m_dontCloseIfError && (g_pModel->getExitCode() == rdo::simulation::report::EC_OK || g_pModel->getExitCode() == rdo::simulation::report::EC_NoMoreEvents)))
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

		setValue(to, convertTo<T>(value<T>(from)));
	}

	template <class T>
	T value(const QString& from)
	{
		return convertFrom<T>(settingsFrom.value(from, qt_to_mfc_type_convertor<T>::type())).value<T>();
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
	static QVariant convertFrom(const QVariant& value)
	{
		return value;
	}

	template <>
	static QVariant convertFrom<QColor>(const QVariant& value)
	{
		__int32 colorRef = value.value<__int32>();
		return QColor(GetRValue(colorRef), GetGValue(colorRef), GetBValue(colorRef));
	}

	template <class T>
	static QVariant convertTo(const QVariant& value)
	{
		return value;
	}

	template <>
	static QVariant convertTo<QColor>(const QVariant& value)
	{
		return value.value<QColor>().name();
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
		convertor.convert<int> ("style/build/tab/indentSize",      "style/build/tab/indent_size");
		convertor.convert<bool>("style/build/tab/tabIndents",      "style/build/tab/tab_indents");
		convertor.convert<int> ("style/build/tab/tabSize",         "style/build/tab/tab_size");
		convertor.convert<bool>("style/build/tab/useTabs",         "style/build/tab/use_tabs");

		convertor.convert<QColor>("style/build/theme/backgroundColor", "style/build/theme/background_color");
		convertor.convert<QColor>("style/build/theme/bookmarkBgColor",    "style/build/theme/bookmark_bg_color");
		convertor.convert<QColor>("style/build/theme/bookmarkFgColor",    "style/build/theme/bookmark_fg_color");
		convertor.convert<int>   ("style/build/theme/bookmarkStyle",      "style/build/theme/bookmark_style");
		convertor.convert<QColor>("style/build/theme/caretColor",         "style/build/theme/caret_color");
		convertor.convert<QColor>("style/build/theme/defaultColor",       "style/build/theme/default_color");
		convertor.convert<int>   ("style/build/theme/defaultStyle",       "style/build/theme/default_style");
		convertor.convert<QColor>("style/build/theme/selectionBgColor",   "style/build/theme/selection_bg_color");
		convertor.convert<QColor>("style/build/theme/selectLineBgColor",  "style/build/theme/select_line_bg_color");

		convertor.convert<bool>  ("style/build/window/showHorzScrollBar", "style/build/window/show_horz_scroll_bar");
		convertor.convert<bool>  ("style/build/window/wordWrap",          "style/build/window/word_wrap");

		convertor.convert<int>    ("style/chart/font/characterSet", "style/chart/font/character_set");
		convertor.convert<int>    ("style/chart/font/codepage",     "style/chart/font/codepage");
		convertor.convert<QString>("style/chart/font/name",         "style/chart/font/name");
		convertor.convert<int>    ("style/chart/font/size",         "style/chart/font/size");

		convertor.convert<int>("style/chart/fonts_ticks/legendFontSize", "style/chart/fonts_ticks/legend_font_size");
		convertor.convert<int>("style/chart/fonts_ticks/tickWidth",      "style/chart/fonts_ticks/tick_width");
		convertor.convert<int>("style/chart/fonts_ticks/titleFontSize",  "style/chart/fonts_ticks/title_font_size");

		convertor.convert<QColor>("style/chart/theme/axisFgColor",     "style/chart/theme/axis_fg_color");
		convertor.convert<QColor>("style/chart/theme/backgroundColor", "style/chart/theme/background_color");
		convertor.convert<QColor>("style/chart/theme/chartBgColor",    "style/chart/theme/chart_bg_color");
		convertor.convert<QColor>("style/chart/theme/defaultColor",    "style/chart/theme/default_color");
		convertor.convert<int>   ("style/chart/theme/defaultStyle",    "style/chart/theme/default_style");
		convertor.convert<QColor>("style/chart/theme/legendFgColor",   "style/chart/theme/legend_fg_color");
		convertor.convert<int>   ("style/chart/theme/legendStyle",     "style/chart/theme/legend_style");
		convertor.convert<QColor>("style/chart/theme/timeBgColor",     "style/chart/theme/time_bg_color");
		convertor.convert<QColor>("style/chart/theme/titleFGColor",    "style/chart/theme/title_fg_color");
		convertor.convert<int>   ("style/chart/theme/titleStyle",      "style/chart/theme/title_style");

		convertor.convert<int>    ("style/debug/font/characterSet", "style/debug/font/character_set");
		convertor.convert<int>    ("style/debug/font/codepage",     "style/debug/font/codepage");
		convertor.convert<QString>("style/debug/font/name",         "style/debug/font/name");
		convertor.convert<int>    ("style/debug/font/size",         "style/debug/font/size");

		convertor.convert<bool>("style/debug/tab/autoIndent",      "style/debug/tab/auto_indent");
		convertor.convert<bool>("style/debug/tab/backspaceUntabs", "style/debug/tab/backspace_untabs");
		convertor.convert<int> ("style/debug/tab/indentSize",      "style/debug/tab/indent_size");
		convertor.convert<bool>("style/debug/tab/tabIndents",      "style/debug/tab/tab_indents");
		convertor.convert<int> ("style/debug/tab/tabSize",         "style/debug/tab/tab_size");
		convertor.convert<bool>("style/debug/tab/useTabs",         "style/debug/tab/use_tabs");

		convertor.convert<QColor>("style/debug/theme/backgroundColor",  "style/debug/theme/background_color");
		convertor.convert<QColor>("style/debug/theme/bookmarkBgColor",  "style/debug/theme/bookmark_bg_color");
		convertor.convert<QColor>("style/debug/theme/bookmarkFgColor",  "style/debug/theme/bookmark_fg_color");
		convertor.convert<int>   ("style/debug/theme/bookmarkStyle",    "style/debug/theme/bookmark_style");
		convertor.convert<QColor>("style/debug/theme/caretColor",       "style/debug/theme/caret_color");
		convertor.convert<QColor>("style/debug/theme/defaultColor",     "style/debug/theme/default_color");
		convertor.convert<int>   ("style/debug/theme/defaultStyle",     "style/debug/theme/default_style");
		convertor.convert<QColor>("style/debug/theme/selectionBgColor", "style/debug/theme/selection_bg_color");

		convertor.convert<bool>("style/debug/window/showHorzScrollBar", "style/debug/window/show_horz_scroll_bar");
		convertor.convert<bool>("style/debug/window/wordWrap",          "style/debug/window/word_wrap");

		convertor.convert<bool>("style/editor/autoComplete/showFullList",    "style/editor/auto_complete/show_full_list");
		convertor.convert<bool>("style/editor/autoComplete/useAutoComplete", "style/editor/auto_complete/use_auto_complete");

		convertor.remove("style/editor/buffer");

		convertor.convert<int>    ("style/editor/font/characterSet", "style/editor/font/character_set");
		convertor.convert<int>    ("style/editor/font/codepage",     "style/editor/font/codepage");
		convertor.convert<QString>("style/editor/font/name",         "style/editor/font/name");
		convertor.convert<int>    ("style/editor/font/size",         "style/editor/font/size");

		convertor.convert<bool>("style/editor/margin/bookmark",   "style/editor/margin/bookmark");
		convertor.convert<bool>("style/editor/margin/fold",       "style/editor/margin/fold");
		convertor.convert<bool>("style/editor/margin/lineNumber", "style/editor/margin/line_number");

		convertor.convert<bool>("style/editor/tab/autoIndent",      "style/editor/tab/auto_indent");
		convertor.convert<bool>("style/editor/tab/backspaceUntabs", "style/editor/tab/backspace_untabs");
		convertor.convert<int> ("style/editor/tab/indentSize",      "style/editor/tab/indent_size");
		convertor.convert<bool>("style/editor/tab/tabIndents",      "style/editor/tab/tab_indents");
		convertor.convert<int> ("style/editor/tab/tabSize",         "style/editor/tab/tab_size");
		convertor.convert<bool>("style/editor/tab/useTabs",         "style/editor/tab/use_tabs");

		convertor.convert<QColor>("style/editor/theme/backgroundColor",  "style/editor/theme/background_color");
		convertor.convert<QColor>("style/editor/theme/bookmarkBgColor",  "style/editor/theme/bookmark_bg_color");
		convertor.convert<QColor>("style/editor/theme/bookmarkFgColor",  "style/editor/theme/bookmark_fg_color");
		convertor.convert<int>   ("style/editor/theme/bookmarkStyle",    "style/editor/theme/bookmark_style");
		convertor.convert<QColor>("style/editor/theme/caretColor",       "style/editor/theme/caret_color");
		convertor.convert<QColor>("style/editor/theme/colorColor",       "style/editor/theme/color_color");
		convertor.convert<QColor>("style/editor/theme/commentColor",     "style/editor/theme/comment_color");
		convertor.convert<bool>  ("style/editor/theme/commentFold",      "style/editor/theme/comment_fold");
		convertor.convert<int>   ("style/editor/theme/commentStyle",     "style/editor/theme/comment_style");
		convertor.convert<QColor>("style/editor/theme/defaultColor",     "style/editor/theme/default_color");
		convertor.convert<int>   ("style/editor/theme/defaultStyle",     "style/editor/theme/default_style");
		convertor.convert<QColor>("style/editor/theme/errorBgColor",     "style/editor/theme/error_bg_color");
		convertor.convert<QColor>("style/editor/theme/foldBgColor",      "style/editor/theme/fold_bg_color");
		convertor.convert<QColor>("style/editor/theme/foldFgColor",      "style/editor/theme/fold_fg_color");
		convertor.convert<int>   ("style/editor/theme/foldStyle",        "style/editor/theme/fold_style");
		convertor.convert<QColor>("style/editor/theme/functionsColor",   "style/editor/theme/functions_color");
		convertor.convert<int>   ("style/editor/theme/functionsStyle",   "style/editor/theme/functions_style");
		convertor.convert<QColor>("style/editor/theme/identifierColor",  "style/editor/theme/identifier_color");
		convertor.convert<int>   ("style/editor/theme/identifierStyle",  "style/editor/theme/identifier_style");
		convertor.convert<QColor>("style/editor/theme/keywordColor",     "style/editor/theme/keyword_color");
		convertor.convert<int>   ("style/editor/theme/keywordStyle",     "style/editor/theme/keyword_style");
		convertor.convert<QColor>("style/editor/theme/numberColor",      "style/editor/theme/number_color");
		convertor.convert<int>   ("style/editor/theme/numberStyle",      "style/editor/theme/number_style");
		convertor.convert<QColor>("style/editor/theme/operatorColor",    "style/editor/theme/operator_color");
		convertor.convert<int>   ("style/editor/theme/operatorStyle",    "style/editor/theme/operator_style");
		convertor.convert<QColor>("style/editor/theme/selectionBgColor", "style/editor/theme/selection_bg_color");
		convertor.convert<QColor>("style/editor/theme/stringColor",      "style/editor/theme/string_color");
		convertor.convert<int>   ("style/editor/theme/stringStyle",      "style/editor/theme/string_style");
		convertor.convert<QColor>("style/editor/theme/traceColor",       "style/editor/theme/trace_color");

		convertor.convert<bool>("style/editor/window/showHorzScrollBar", "style/editor/window/show_horz_scroll_bar");
		convertor.convert<bool>("style/editor/window/wordWrap",          "style/editor/window/word_wrap");

		convertor.convert<int>    ("style/find/font/characterSet", "style/find/font/character_set");
		convertor.convert<int>    ("style/find/font/codepage",     "style/find/font/codepage");
		convertor.convert<QString>("style/find/font/name",         "style/find/font/name");
		convertor.convert<int>    ("style/find/font/size",         "style/find/font/size");

		convertor.convert<bool>("style/find/tab/autoIndent",      "style/find/tab/auto_indent");
		convertor.convert<bool>("style/find/tab/backspaceUntabs", "style/find/tab/backspace_untabs");
		convertor.convert<int> ("style/find/tab/indentSize",      "style/find/tab/indent_size");
		convertor.convert<bool>("style/find/tab/tabIndents",      "style/find/tab/tab_indents");
		convertor.convert<int> ("style/find/tab/tabSize",         "style/find/tab/tab_size");
		convertor.convert<bool>("style/find/tab/useTabs",         "style/find/tab/use_tabs");

		convertor.convert<QColor>("style/find/theme/backgroundColor",   "style/find/theme/background_color");
		convertor.convert<QColor>("style/find/theme/bookmarkBgColor",   "style/find/theme/bookmark_bg_color");
		convertor.convert<QColor>("style/find/theme/bookmarkFgColor",   "style/find/theme/bookmark_fg_color");
		convertor.convert<int>   ("style/find/theme/bookmarkStyle",     "style/find/theme/bookmark_style");
		convertor.convert<QColor>("style/find/theme/caretColor",        "style/find/theme/caret_color");
		convertor.convert<QColor>("style/find/theme/defaultColor",      "style/find/theme/default_color");
		convertor.convert<int>   ("style/find/theme/defaultStyle",      "style/find/theme/default_style");
		convertor.convert<QColor>("style/find/theme/keywordColor",      "style/find/theme/keyword_сolor");
		convertor.convert<int>   ("style/find/theme/keywordStyle",      "style/find/theme/keyword_style");
		convertor.convert<QColor>("style/find/theme/selectionBgColor",  "style/find/theme/selection_bg_color");
		convertor.convert<QColor>("style/find/theme/selectLineBgColor", "style/find/theme/select_line_bg_color");

		convertor.convert<bool>("style/find/window/showHorzScrollBar", "style/find/window/show_horz_scroll_bar");
		convertor.convert<bool>("style/find/window/wordWrap",          "style/find/window/word_wrap");

		convertor.convert<int>    ("style/frame/font/characterSet", "style/frame/font/character_set");
		convertor.convert<int>    ("style/frame/font/codepage",     "style/frame/font/codepage");
		convertor.convert<QString>("style/frame/font/name",         "style/frame/font/name");
		convertor.convert<int>    ("style/frame/font/size",         "style/frame/font/size");

		convertor.convert<QColor>("style/frame/theme/backgroundColor",   "style/frame/theme/background_color");
		convertor.convert<QColor>("style/frame/theme/defaultColor",      "style/frame/theme/default_color");
		convertor.convert<int>   ("style/frame/theme/defaultStyle",      "style/frame/theme/default_style");

		convertor.convert<int>    ("style/results/font/characterSet", "style/results/font/character_set");
		convertor.convert<int>    ("style/results/font/codepage",     "style/results/font/codepage");
		convertor.convert<QString>("style/results/font/name",         "style/results/font/name");
		convertor.convert<int>    ("style/results/font/size",         "style/results/font/size");

		convertor.convert<bool>("style/results/tab/autoIndent",      "style/results/tab/auto_indent");
		convertor.convert<bool>("style/results/tab/backspaceUntabs", "style/results/tab/backspace_untabs");
		convertor.convert<int> ("style/results/tab/indentSize",      "style/results/tab/indent_size");
		convertor.convert<bool>("style/results/tab/tabIndents",      "style/results/tab/tab_indents");
		convertor.convert<int> ("style/results/tab/tabSize",         "style/results/tab/tab_size");
		convertor.convert<bool>("style/results/tab/useTabs",         "style/results/tab/use_tabs");

		convertor.convert<QColor>("style/results/theme/backgroundColor",  "style/results/theme/background_color");
		convertor.convert<QColor>("style/results/theme/bookmarkBgColor",  "style/results/theme/bookmark_bg_color");
		convertor.convert<QColor>("style/results/theme/bookmarkFgColor",  "style/results/theme/bookmark_fg_color");
		convertor.convert<int>   ("style/results/theme/bookmarkStyle",    "style/results/theme/bookmark_style");
		convertor.convert<QColor>("style/results/theme/caretColor",       "style/results/theme/caret_color");
		convertor.convert<QColor>("style/results/theme/colorColor",       "style/results/theme/color_color");
		convertor.convert<QColor>("style/results/theme/commentColor",     "style/results/theme/comment_color");
		convertor.convert<int>   ("style/results/theme/commentStyle",     "style/results/theme/comment_style");
		convertor.convert<QColor>("style/results/theme/defaultColor",     "style/results/theme/default_color");
		convertor.convert<int>   ("style/results/theme/defaultStyle",     "style/results/theme/default_style");
		convertor.convert<QColor>("style/results/theme/functionsColor",   "style/results/theme/functions_color");
		convertor.convert<int>   ("style/results/theme/functionsStyle",   "style/results/theme/functions_style");
		convertor.convert<QColor>("style/results/theme/identifierColor",  "style/results/theme/identifier_color");
		convertor.convert<int>   ("style/results/theme/identifierStyle",  "style/results/theme/identifier_style");
		convertor.convert<QColor>("style/results/theme/keywordColor",     "style/results/theme/keyword_color");
		convertor.convert<int>   ("style/results/theme/keywordStyle",     "style/results/theme/keyword_style");
		convertor.convert<QColor>("style/results/theme/numberColor",      "style/results/theme/number_color");
		convertor.convert<int>   ("style/results/theme/numberStyle",      "style/results/theme/number_style");
		convertor.convert<QColor>("style/results/theme/operatorColor",    "style/results/theme/operator_color");
		convertor.convert<int>   ("style/results/theme/operatorStyle",    "style/results/theme/operator_style");
		convertor.convert<QColor>("style/results/theme/selectionBgColor", "style/results/theme/selection_bg_color");
		convertor.convert<QColor>("style/results/theme/stringColor",      "style/results/theme/string_color");
		convertor.convert<int>   ("style/results/theme/stringStyle",      "style/results/theme/string_style");
		convertor.convert<QColor>("style/results/theme/traceColor",       "style/results/theme/trace_color");

		convertor.convert<bool>("style/results/window/showHorzScrollBar", "style/results/window/show_horz_scroll_bar");
		convertor.convert<bool>("style/results/window/wordWrap",          "style/results/window/word_wrap");

		convertor.convert<int>("style/trace/borders/horzBorder", "style/trace/borders/horz_border");
		convertor.convert<int>("style/trace/borders/vertBorder", "style/trace/borders/vert_border");

		convertor.convert<int>    ("style/trace/font/characterSet", "style/trace/font/character_set");
		convertor.convert<int>    ("style/trace/font/codepage",     "style/trace/font/codepage");
		convertor.convert<QString>("style/trace/font/name",         "style/trace/font/name");
		convertor.convert<int>    ("style/trace/font/size",         "style/trace/font/size");		

		convertor.convert<QColor>("style/trace/theme/defaultColor_backgroundColor",  "style/trace/theme/default_color_background_color");
		convertor.convert<QColor>("style/trace/theme/defaultColor_foregroundColor",  "style/trace/theme/default_color_foreground_color");
		convertor.convert<QColor>("style/trace/theme/dps_backgroundColor",           "style/trace/theme/dps_background_color");
		convertor.convert<QColor>("style/trace/theme/dps_foregroundColor",           "style/trace/theme/dps_foreground_color");
		convertor.convert<QColor>("style/trace/theme/eb_backgroundColor",            "style/trace/theme/eb_background_color");
		convertor.convert<QColor>("style/trace/theme/eb_foregroundColor",            "style/trace/theme/eb_foreground_color");
		convertor.convert<QColor>("style/trace/theme/ef_backgroundColor",            "style/trace/theme/ef_background_color");
		convertor.convert<QColor>("style/trace/theme/ef_foregroundColor",            "style/trace/theme/ef_foreground_color");
		convertor.convert<QColor>("style/trace/theme/ei_backgroundColor",            "style/trace/theme/ei_background_color");
		convertor.convert<QColor>("style/trace/theme/ei_foregroundColor",            "style/trace/theme/ei_foreground_color");
		convertor.convert<QColor>("style/trace/theme/er_backgroundColor",            "style/trace/theme/er_background_color");
		convertor.convert<QColor>("style/trace/theme/er_foregroundColor",            "style/trace/theme/er_foreground_color");
		convertor.convert<QColor>("style/trace/theme/es_backgroundColor",            "style/trace/theme/es_background_color");
		convertor.convert<QColor>("style/trace/theme/es_foregroundColor",            "style/trace/theme/es_foreground_color");
		convertor.convert<QColor>("style/trace/theme/rc_backgroundColor",            "style/trace/theme/rc_background_color");
		convertor.convert<QColor>("style/trace/theme/rc_foregroundColor",            "style/trace/theme/rc_foreground_color");
		convertor.convert<QColor>("style/trace/theme/re_backgroundColor",            "style/trace/theme/re_background_color");
		convertor.convert<QColor>("style/trace/theme/re_foregroundColor",            "style/trace/theme/re_foreground_color");
		convertor.convert<QColor>("style/trace/theme/rk_backgroundColor",            "style/trace/theme/rk_background_color");
		convertor.convert<QColor>("style/trace/theme/rk_foregroundColor",            "style/trace/theme/rk_foreground_color");
		convertor.convert<QColor>("style/trace/theme/s_backgroundColor",             "style/trace/theme/s_background_color");
		convertor.convert<QColor>("style/trace/theme/s_foregroundColor",             "style/trace/theme/s_foreground_color");
		convertor.convert<QColor>("style/trace/theme/sb_backgroundColor",            "style/trace/theme/sb_background_color");
		convertor.convert<QColor>("style/trace/theme/sb_foregroundColor",            "style/trace/theme/sb_foreground_color");
		convertor.convert<QColor>("style/trace/theme/sd_backgroundColor",            "style/trace/theme/sd_background_color");
		convertor.convert<QColor>("style/trace/theme/sd_foregroundColor",            "style/trace/theme/sd_foreground_color");
		convertor.convert<QColor>("style/trace/theme/sef_backgroundColor",           "style/trace/theme/sef_background_color");
		convertor.convert<QColor>("style/trace/theme/sef_foregroundColor",           "style/trace/theme/sef_foreground_color");
		convertor.convert<QColor>("style/trace/theme/sem_backgroundColor",           "style/trace/theme/sem_background_color");
		convertor.convert<QColor>("style/trace/theme/sem_foregroundColor",           "style/trace/theme/sem_foreground_color");
		convertor.convert<QColor>("style/trace/theme/sen_backgroundColor",           "style/trace/theme/sen_background_color");
		convertor.convert<QColor>("style/trace/theme/sen_foregroundColor",           "style/trace/theme/sen_foreground_color");
		convertor.convert<QColor>("style/trace/theme/ses_backgroundColor",           "style/trace/theme/ses_background_color");
		convertor.convert<QColor>("style/trace/theme/ses_foregroundColor",           "style/trace/theme/ses_foreground_color");
		convertor.convert<QColor>("style/trace/theme/seu_backgroundColor",           "style/trace/theme/seu_background_color");
		convertor.convert<QColor>("style/trace/theme/seu_foregroundColor",           "style/trace/theme/seu_foreground_color");
		convertor.convert<QColor>("style/trace/theme/so_backgroundColor",            "style/trace/theme/so_background_color");
		convertor.convert<QColor>("style/trace/theme/so_foregroundColor",            "style/trace/theme/so_foreground_color");
		convertor.convert<QColor>("style/trace/theme/src_backgroundColor",           "style/trace/theme/src_background_color");
		convertor.convert<QColor>("style/trace/theme/src_foregroundColor",           "style/trace/theme/src_foreground_color");
		convertor.convert<QColor>("style/trace/theme/sre_backgroundColor",           "style/trace/theme/sre_background_color");
		convertor.convert<QColor>("style/trace/theme/sre_foregroundColor",           "style/trace/theme/sre_foreground_color");
		convertor.convert<QColor>("style/trace/theme/srk_backgroundColor",           "style/trace/theme/srk_background_color");
		convertor.convert<QColor>("style/trace/theme/srk_foregroundColor",           "style/trace/theme/srk_foreground_color");
		convertor.convert<QColor>("style/trace/theme/std_backgroundColor",           "style/trace/theme/std_background_color");
		convertor.convert<QColor>("style/trace/theme/std_foregroundColor",           "style/trace/theme/std_foreground_color");
		convertor.convert<QColor>("style/trace/theme/stn_backgroundColor",           "style/trace/theme/stn_background_color");
		convertor.convert<QColor>("style/trace/theme/stn_foregroundColor",           "style/trace/theme/stn_foreground_color");
		convertor.convert<QColor>("style/trace/theme/str_backgroundColor",           "style/trace/theme/str_background_color");
		convertor.convert<QColor>("style/trace/theme/str_foregroundColor",           "style/trace/theme/str_foreground_color");
		convertor.convert<int>   ("style/trace/theme/style",                         "style/trace/theme/style");
		convertor.convert<QColor>("style/trace/theme/v_backgroundColor",             "style/trace/theme/v_background_color");
		convertor.convert<QColor>("style/trace/theme/v_foregroundColor",             "style/trace/theme/v_foreground_color");

		convertor.remove("style");
	}
}
#endif
